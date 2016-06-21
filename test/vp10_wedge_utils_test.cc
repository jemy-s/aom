/*
 *  Copyright (c) 2016 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "third_party/googletest/src/include/gtest/gtest.h"

#include "./vpx_config.h"

#include "./vpx_dsp_rtcd.h"
#include "./vp10_rtcd.h"

#include "vpx_dsp/vpx_dsp_common.h"

#include "vp10/common/enums.h"

#include "test/acm_random.h"
#include "test/function_equivalence_test.h"

#define WEDGE_WEIGHT_BITS 6
#define MAX_MASK_VALUE  (1 << (WEDGE_WEIGHT_BITS))

using std::tr1::make_tuple;
using libvpx_test::ACMRandom;
using libvpx_test::FunctionEquivalenceTest;

namespace {

static const int16_t kInt13Max = (1 << 12) - 1;

//////////////////////////////////////////////////////////////////////////////
// vp10_wedge_sse_from_residuals - functionality
//////////////////////////////////////////////////////////////////////////////

class WedgeUtilsSSEFuncTest : public testing::Test {
 protected:
  WedgeUtilsSSEFuncTest() : rng_(ACMRandom::DeterministicSeed()) {}

  static const int kIterations = 1000;

  ACMRandom rng_;
};

static void equiv_blend_residuals(int16_t *r,
                                  const int16_t *r0,
                                  const int16_t *r1,
                                  const uint8_t *m,
                                  int N) {
  for (int i = 0 ; i < N ; i++) {
    const int32_t m0 = m[i];
    const int32_t m1 = MAX_MASK_VALUE - m0;
    const int16_t R = m0 * r0[i] + m1 * r1[i];
    // Note that this rounding is designed to match the result
    // you would get when actually blending the 2 predictors and computing
    // the residuals.
    r[i] = ROUND_POWER_OF_TWO(R - 1, WEDGE_WEIGHT_BITS);
  }
}

static uint64_t equiv_sse_from_residuals(const int16_t *r0,
                                         const int16_t *r1,
                                         const uint8_t *m,
                                         int N) {
  uint64_t acc = 0;
  for (int i = 0 ; i < N ; i++) {
    const int32_t m0 = m[i];
    const int32_t m1 = MAX_MASK_VALUE - m0;
    const int16_t R = m0 * r0[i] + m1 * r1[i];
    const int32_t r = ROUND_POWER_OF_TWO(R - 1, WEDGE_WEIGHT_BITS);
    acc += r * r;
  }
  return acc;
}

TEST_F(WedgeUtilsSSEFuncTest, ResidualBlendingEquiv) {
  DECLARE_ALIGNED(32, uint8_t, s[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, p0[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, p1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, p[MAX_SB_SQUARE]);

  DECLARE_ALIGNED(32, int16_t, r0[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, r1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, r_ref[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, r_tst[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, m[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
      s[i] = rng_.Rand8();
      m[i] = rng_(MAX_MASK_VALUE + 1);
    }

    const int w = 1 << (rng_(MAX_SB_SIZE_LOG2 + 1 - 3) + 3);
    const int h = 1 << (rng_(MAX_SB_SIZE_LOG2 + 1 - 3) + 3);
    const int N = w * h;

    for (int j = 0 ; j < N ; j++) {
      p0[j] = clamp(s[j] + rng_(33) - 16, 0, UINT8_MAX);
      p1[j] = clamp(s[j] + rng_(33) - 16, 0, UINT8_MAX);
    }

    vpx_blend_mask6(p, w, p0, w, p1, w, m, w, h, w, 0, 0);

    vpx_subtract_block(h, w, r0, w, s, w, p0, w);
    vpx_subtract_block(h, w, r1, w, s, w, p1, w);

    vpx_subtract_block(h, w, r_ref, w, s, w, p, w);
    equiv_blend_residuals(r_tst, r0, r1, m, N);

    for (int i = 0 ; i < N ; ++i)
      ASSERT_EQ(r_ref[i], r_tst[i]);

    uint64_t ref_sse = vpx_sum_squares_i16(r_ref, N);
    uint64_t tst_sse = equiv_sse_from_residuals(r0, r1, m, N);

    ASSERT_EQ(ref_sse, tst_sse);
  }
}

static uint64_t sse_from_residuals(const int16_t *r0,
                                   const int16_t *r1,
                                   const uint8_t *m,
                                   int N) {
  uint64_t acc = 0;
  for (int i = 0 ; i < N ; i++) {
    const int32_t m0 = m[i];
    const int32_t m1 = MAX_MASK_VALUE - m0;
    const int32_t r = m0 * r0[i] + m1 * r1[i];
    acc += r * r;
  }
  return ROUND_POWER_OF_TWO(acc, 2 * WEDGE_WEIGHT_BITS);
}

TEST_F(WedgeUtilsSSEFuncTest, ResidualBlendingMethod) {
  DECLARE_ALIGNED(32, int16_t, r0[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, r1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, d[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, m[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
      r1[i] = rng_(2 * INT8_MAX - 2 * INT8_MIN + 1) + 2 * INT8_MIN;
      d[i] = rng_(2 * INT8_MAX - 2 * INT8_MIN + 1) + 2 * INT8_MIN;
      m[i] = rng_(MAX_MASK_VALUE + 1);
    }

    const int N = 64 * (rng_(MAX_SB_SQUARE/64) + 1);

    for (int i = 0 ; i < N ; i++)
      r0[i] = r1[i] + d[i];

    uint64_t ref_res = sse_from_residuals(r0, r1, m, N);
    uint64_t tst_res = vp10_wedge_sse_from_residuals(r1, d, m, N);

    ASSERT_EQ(ref_res, tst_res);
  }
}

//////////////////////////////////////////////////////////////////////////////
// vp10_wedge_sse_from_residuals - optimizations
//////////////////////////////////////////////////////////////////////////////

typedef uint64_t (*FSSE)(const int16_t *r1,
                         const int16_t *d,
                         const uint8_t *m,
                         int N);

class WedgeUtilsSSEOptTest : public FunctionEquivalenceTest<FSSE> {
 protected:
  WedgeUtilsSSEOptTest() : rng_(ACMRandom::DeterministicSeed()) {}

  static const int kIterations = 10000;

  ACMRandom rng_;
};

TEST_P(WedgeUtilsSSEOptTest, RandomValues) {
  DECLARE_ALIGNED(32, int16_t, r1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, d[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, m[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
      r1[i] = rng_(2 * kInt13Max + 1) - kInt13Max;
      d[i] = rng_(2 * kInt13Max + 1) - kInt13Max;
      m[i] = rng_(MAX_MASK_VALUE + 1);
    }

    const int N = 64 * (rng_(MAX_SB_SQUARE/64) + 1);

    const uint64_t ref_res = ref_func_(r1, d, m, N);
    const uint64_t tst_res = tst_func_(r1, d, m, N);

    ASSERT_EQ(ref_res, tst_res);
  }
}

TEST_P(WedgeUtilsSSEOptTest, ExtremeValues) {
  DECLARE_ALIGNED(32, int16_t, r1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, d[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, m[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    if (rng_(2)) {
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
        r1[i] = kInt13Max;
    } else {
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
        r1[i] = -kInt13Max;
    }

    if (rng_(2)) {
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
        d[i] = kInt13Max;
    } else {
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
        d[i] = -kInt13Max;
    }

    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
      m[i] = MAX_MASK_VALUE;

    const int N = 64 * (rng_(MAX_SB_SQUARE/64) + 1);

    const uint64_t ref_res = ref_func_(r1, d, m, N);
    const uint64_t tst_res = tst_func_(r1, d, m, N);

    ASSERT_EQ(ref_res, tst_res);
  }
}

#if HAVE_SSE2
INSTANTIATE_TEST_CASE_P(
    SSE2, WedgeUtilsSSEOptTest,
    ::testing::Values(
        make_tuple(&vp10_wedge_sse_from_residuals_c,
                   &vp10_wedge_sse_from_residuals_sse2)
    )
);
#endif  // HAVE_SSE2

//////////////////////////////////////////////////////////////////////////////
// vp10_wedge_sign_from_residuals
//////////////////////////////////////////////////////////////////////////////

typedef int (*FSign)(const int16_t *ds,
                     const uint8_t *m,
                     int N,
                     int64_t limit);

class WedgeUtilsSignOptTest : public FunctionEquivalenceTest<FSign> {
 protected:
  WedgeUtilsSignOptTest() : rng_(ACMRandom::DeterministicSeed()) {}

  static const int kIterations = 10000;
  static const int kMaxSize = 8196;  // Size limited by SIMD implementation.

  ACMRandom rng_;
};

TEST_P(WedgeUtilsSignOptTest, RandomValues) {
  DECLARE_ALIGNED(32, int16_t, r0[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, r1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, ds[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, m[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
      r0[i] = rng_(2 * kInt13Max + 1) - kInt13Max;
      r1[i] = rng_(2 * kInt13Max + 1) - kInt13Max;
      m[i] = rng_(MAX_MASK_VALUE + 1);
    }

    const int maxN = VPXMIN(kMaxSize, MAX_SB_SQUARE);
    const int N = 64 * (rng_(maxN/64 - 1) + 1);

    int64_t limit;
    limit = (int64_t)vpx_sum_squares_i16(r0, N);
    limit -= (int64_t)vpx_sum_squares_i16(r1, N);
    limit *= (1 << WEDGE_WEIGHT_BITS) / 2;

    for (int i = 0 ; i < N ; i++)
      ds[i] = clamp(r0[i]*r0[i] - r1[i]*r1[i], INT16_MIN, INT16_MAX);

    const int ref_res = ref_func_(ds, m, N, limit);
    const int tst_res = tst_func_(ds, m, N, limit);

    ASSERT_EQ(ref_res, tst_res);
  }
}

TEST_P(WedgeUtilsSignOptTest, ExtremeValues) {
  DECLARE_ALIGNED(32, int16_t, r0[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, r1[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, ds[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, uint8_t, m[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    switch (rng_(4)) {
    case 0:
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
        r0[i] = 0;
        r1[i] = kInt13Max;
      }
      break;
    case 1:
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
        r0[i] = kInt13Max;
        r1[i] = 0;
      }
      break;
    case 2:
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
        r0[i] = 0;
        r1[i] = -kInt13Max;
      }
      break;
    default:
      for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
        r0[i] = -kInt13Max;
        r1[i] = 0;
      }
      break;
    }

    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
      m[i] = MAX_MASK_VALUE;

    const int maxN = VPXMIN(kMaxSize, MAX_SB_SQUARE);
    const int N = 64 * (rng_(maxN/64 - 1) + 1);

    int64_t limit;
    limit = (int64_t)vpx_sum_squares_i16(r0, N);
    limit -= (int64_t)vpx_sum_squares_i16(r1, N);
    limit *= (1 << WEDGE_WEIGHT_BITS) / 2;

    for (int i = 0 ; i < N ; i++)
      ds[i] = clamp(r0[i]*r0[i] - r1[i]*r1[i], INT16_MIN, INT16_MAX);

    const int ref_res = ref_func_(ds, m, N, limit);
    const int tst_res = tst_func_(ds, m, N, limit);

    ASSERT_EQ(ref_res, tst_res);
  }
}

#if HAVE_SSE2
INSTANTIATE_TEST_CASE_P(
    SSE2, WedgeUtilsSignOptTest,
    ::testing::Values(
        make_tuple(&vp10_wedge_sign_from_residuals_c,
                   &vp10_wedge_sign_from_residuals_sse2)
    )
);
#endif  // HAVE_SSE2

//////////////////////////////////////////////////////////////////////////////
// vp10_wedge_compute_delta_squares
//////////////////////////////////////////////////////////////////////////////

typedef void (*FDS)(int16_t *d,
                    const int16_t *a,
                    const int16_t *b,
                    int N);

class WedgeUtilsDeltaSquaresOptTest : public FunctionEquivalenceTest<FDS> {
 protected:
  WedgeUtilsDeltaSquaresOptTest() : rng_(ACMRandom::DeterministicSeed()) {}

  static const int kIterations = 10000;

  ACMRandom rng_;
};

TEST_P(WedgeUtilsDeltaSquaresOptTest, RandomValues) {
  DECLARE_ALIGNED(32, int16_t, a[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, b[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, d_ref[MAX_SB_SQUARE]);
  DECLARE_ALIGNED(32, int16_t, d_tst[MAX_SB_SQUARE]);

  for (int iter = 0 ; iter < kIterations && !HasFatalFailure(); ++iter) {
    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i) {
      a[i] = rng_.Rand16();
      b[i] = rng_(2 * INT16_MAX + 1) - INT16_MAX;
    }

    const int N = 64 * (rng_(MAX_SB_SQUARE/64) + 1);

    memset(&d_ref, INT16_MAX, sizeof(d_ref));
    memset(&d_tst, INT16_MAX, sizeof(d_tst));

    ref_func_(d_ref, a, b, N);
    tst_func_(d_tst, a, b, N);

    for (int i = 0 ; i < MAX_SB_SQUARE ; ++i)
      ASSERT_EQ(d_ref[i], d_tst[i]);
  }
}

#if HAVE_SSE2
INSTANTIATE_TEST_CASE_P(
    SSE2, WedgeUtilsDeltaSquaresOptTest,
    ::testing::Values(
        make_tuple(&vp10_wedge_compute_delta_squares_c,
                   &vp10_wedge_compute_delta_squares_sse2)
    )
);
#endif  // HAVE_SSE2

}  // namespace
