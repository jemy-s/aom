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

/*!\defgroup vp8 VP8
 * \ingroup codecs
 * VP8 is aom's newest video compression algorithm that uses motion
 * compensated prediction, Discrete Cosine Transform (DCT) coding of the
 * prediction error signal and context dependent entropy coding techniques
 * based on arithmetic principles. It features:
 *  - YUV 4:2:0 image format
 *  - Macro-block based coding (16x16 luma plus two 8x8 chroma)
 *  - 1/4 (1/8) pixel accuracy motion compensated prediction
 *  - 4x4 DCT transform
 *  - 128 level linear quantizer
 *  - In loop deblocking filter
 *  - Context-based entropy coding
 *
 * @{
 */
/*!\file
 * \brief Provides controls common to both the VP8 encoder and decoder.
 */
#ifndef AOM_VP8_H_
#define AOM_VP8_H_

#include "./aom_codec.h"
#include "./aom_image.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!\brief Control functions
 *
 * The set of macros define the control functions of VP8 interface
 */
enum vp8_com_control_id {
  VP8_SET_REFERENCE =
      1, /**< pass in an external frame into decoder to be used as reference
            frame */
  VP8_COPY_REFERENCE = 2, /**< get a copy of reference frame from the decoder */
  VP8_SET_POSTPROC = 3,   /**< set the decoder's post processing settings  */
  VP8_SET_DBG_COLOR_REF_FRAME =
      4, /**< set the reference frames to color for each macroblock */
  VP8_SET_DBG_COLOR_MB_MODES = 5, /**< set which macro block modes to color */
  VP8_SET_DBG_COLOR_B_MODES = 6,  /**< set which blocks modes to color */
  VP8_SET_DBG_DISPLAY_MV = 7,     /**< set which motion vector modes to draw */

  /* TODO(jkoleszar): The encoder incorrectly reuses some of these values (5+)
   * for its control ids. These should be migrated to something like the
   * VP8_DECODER_CTRL_ID_START range next time we're ready to break the ABI.
   */
  VP9_GET_REFERENCE = 128, /**< get a pointer to a reference frame */
  VP8_COMMON_CTRL_ID_MAX,
  VP8_DECODER_CTRL_ID_START = 256
};

/*!\brief post process flags
 *
 * The set of macros define VP8 decoder post processing flags
 */
enum vp8_postproc_level {
  VP8_NOFILTERING = 0,
  VP8_DEBLOCK = 1 << 0,
  VP8_DEMACROBLOCK = 1 << 1,
  VP8_ADDNOISE = 1 << 2,
  VP8_DEBUG_TXT_FRAME_INFO = 1 << 3, /**< print frame information */
  VP8_DEBUG_TXT_MBLK_MODES =
      1 << 4, /**< print macro block modes over each macro block */
  VP8_DEBUG_TXT_DC_DIFF = 1 << 5,   /**< print dc diff for each macro block */
  VP8_DEBUG_TXT_RATE_INFO = 1 << 6, /**< print video rate info (encoder only) */
  VP8_MFQE = 1 << 10
};

/*!\brief post process flags
 *
 * This define a structure that describe the post processing settings. For
 * the best objective measure (using the PSNR metric) set post_proc_flag
 * to VP8_DEBLOCK and deblocking_level to 1.
 */

typedef struct vp8_postproc_cfg {
  int post_proc_flag;   /**< the types of post processing to be done, should be
                           combination of "vp8_postproc_level" */
  int deblocking_level; /**< the strength of deblocking, valid range [0, 16] */
  int noise_level; /**< the strength of additive noise, valid range [0, 16] */
} vp8_postproc_cfg_t;

/*!\brief reference frame type
 *
 * The set of macros define the type of VP8 reference frames
 */
typedef enum aom_ref_frame_type {
  VP8_LAST_FRAME = 1,
  VP8_GOLD_FRAME = 2,
  VP8_ALTR_FRAME = 4
} aom_ref_frame_type_t;

/*!\brief reference frame data struct
 *
 * Define the data struct to access vp8 reference frames.
 */
typedef struct aom_ref_frame {
  aom_ref_frame_type_t frame_type; /**< which reference frame */
  aom_image_t img;                 /**< reference frame data in image format */
} aom_ref_frame_t;

/*!\brief VP9 specific reference frame data struct
 *
 * Define the data struct to access vp9 reference frames.
 */
typedef struct vp9_ref_frame {
  int idx;         /**< frame index to get (input) */
  aom_image_t img; /**< img structure to populate (output) */
} vp9_ref_frame_t;

/*!\cond */
/*!\brief vp8 decoder control function parameter type
 *
 * defines the data type for each of VP8 decoder control function requires
 */
AOM_CTRL_USE_TYPE(VP8_SET_REFERENCE, aom_ref_frame_t *)
#define AOM_CTRL_VP8_SET_REFERENCE
AOM_CTRL_USE_TYPE(VP8_COPY_REFERENCE, aom_ref_frame_t *)
#define AOM_CTRL_VP8_COPY_REFERENCE
AOM_CTRL_USE_TYPE(VP8_SET_POSTPROC, vp8_postproc_cfg_t *)
#define AOM_CTRL_VP8_SET_POSTPROC
AOM_CTRL_USE_TYPE(VP8_SET_DBG_COLOR_REF_FRAME, int)
#define AOM_CTRL_VP8_SET_DBG_COLOR_REF_FRAME
AOM_CTRL_USE_TYPE(VP8_SET_DBG_COLOR_MB_MODES, int)
#define AOM_CTRL_VP8_SET_DBG_COLOR_MB_MODES
AOM_CTRL_USE_TYPE(VP8_SET_DBG_COLOR_B_MODES, int)
#define AOM_CTRL_VP8_SET_DBG_COLOR_B_MODES
AOM_CTRL_USE_TYPE(VP8_SET_DBG_DISPLAY_MV, int)
#define AOM_CTRL_VP8_SET_DBG_DISPLAY_MV
AOM_CTRL_USE_TYPE(VP9_GET_REFERENCE, vp9_ref_frame_t *)
#define AOM_CTRL_VP9_GET_REFERENCE

/*!\endcond */
/*! @} - end defgroup vp8 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AOM_VP8_H_
