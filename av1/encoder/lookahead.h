/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AV1_ENCODER_LOOKAHEAD_H_
#define AV1_ENCODER_LOOKAHEAD_H_

#include "aom_scale/yv12config.h"
#include "aom/aom_integer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LAG_BUFFERS 25

struct lookahead_entry {
  YV12_BUFFER_CONFIG img;
  int64_t ts_start;
  int64_t ts_end;
  aom_enc_frame_flags_t flags;
};

// The max of past frames we want to keep in the queue.
#define MAX_PRE_FRAMES 1

struct lookahead_ctx {
  int max_sz;                  /* Absolute size of the queue */
  int sz;                      /* Number of buffers currently in the queue */
  int read_idx;                /* Read index */
  int write_idx;               /* Write index */
  struct lookahead_entry *buf; /* Buffer list */
};

/**\brief Initializes the lookahead stage
 *
 * The lookahead stage is a queue of frame buffers on which some analysis
 * may be done when buffers are enqueued.
 */
struct lookahead_ctx *av1_lookahead_init(unsigned int width,
                                         unsigned int height,
                                         unsigned int subsampling_x,
                                         unsigned int subsampling_y,
#if CONFIG_AOM_HIGHBITDEPTH
                                         int use_highbitdepth,
#endif
                                         unsigned int depth);

/**\brief Destroys the lookahead stage
 */
void av1_lookahead_destroy(struct lookahead_ctx *ctx);

/**\brief Enqueue a source buffer
 *
 * This function will copy the source image into a new framebuffer with
 * the expected stride/border.
 *
 * If active_map is non-NULL and there is only one frame in the queue, then copy
 * only active macroblocks.
 *
 * \param[in] ctx         Pointer to the lookahead context
 * \param[in] src         Pointer to the image to enqueue
 * \param[in] ts_start    Timestamp for the start of this frame
 * \param[in] ts_end      Timestamp for the end of this frame
 * \param[in] flags       Flags set on this frame
 * \param[in] active_map  Map that specifies which macroblock is active
 */
int av1_lookahead_push(struct lookahead_ctx *ctx, YV12_BUFFER_CONFIG *src,
                       int64_t ts_start, int64_t ts_end,
#if CONFIG_AOM_HIGHBITDEPTH
                       int use_highbitdepth,
#endif
                       aom_enc_frame_flags_t flags);

/**\brief Get the next source buffer to encode
 *
 *
 * \param[in] ctx       Pointer to the lookahead context
 * \param[in] drain     Flag indicating the buffer should be drained
 *                      (return a buffer regardless of the current queue depth)
 *
 * \retval NULL, if drain set and queue is empty
 * \retval NULL, if drain not set and queue not of the configured depth
 */
struct lookahead_entry *av1_lookahead_pop(struct lookahead_ctx *ctx, int drain);

/**\brief Get a future source buffer to encode
 *
 * \param[in] ctx       Pointer to the lookahead context
 * \param[in] index     Index of the frame to be returned, 0 == next frame
 *
 * \retval NULL, if no buffer exists at the specified index
 */
struct lookahead_entry *av1_lookahead_peek(struct lookahead_ctx *ctx,
                                           int index);

/**\brief Get the number of frames currently in the lookahead queue
 *
 * \param[in] ctx       Pointer to the lookahead context
 */
unsigned int av1_lookahead_depth(struct lookahead_ctx *ctx);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AV1_ENCODER_LOOKAHEAD_H_
