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

/*!\file
 * \brief Describes the decoder algorithm interface for algorithm
 *        implementations.
 *
 * This file defines the private structures and data types that are only
 * relevant to implementing an algorithm, as opposed to using it.
 *
 * To create a decoder algorithm class, an interface structure is put
 * into the global namespace:
 *     <pre>
 *     my_codec.c:
 *       aom_codec_iface_t my_codec = {
 *           "My Codec v1.0",
 *           AOM_CODEC_ALG_ABI_VERSION,
 *           ...
 *       };
 *     </pre>
 *
 * An application instantiates a specific decoder instance by using
 * aom_codec_init() and a pointer to the algorithm's interface structure:
 *     <pre>
 *     my_app.c:
 *       extern aom_codec_iface_t my_codec;
 *       {
 *           aom_codec_ctx_t algo;
 *           res = aom_codec_init(&algo, &my_codec);
 *       }
 *     </pre>
 *
 * Once initialized, the instance is manged using other functions from
 * the aom_codec_* family.
 */
#ifndef AOM_INTERNAL_AOM_CODEC_INTERNAL_H_
#define AOM_INTERNAL_AOM_CODEC_INTERNAL_H_
#include "../aom_decoder.h"
#include "../aom_encoder.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!\brief Current ABI version number
 *
 * \internal
 * If this file is altered in any way that changes the ABI, this value
 * must be bumped.  Examples include, but are not limited to, changing
 * types, removing or reassigning enums, adding/removing/rearranging
 * fields to structures
 */
#define AOM_CODEC_INTERNAL_ABI_VERSION (5) /**<\hideinitializer*/

typedef struct aom_codec_alg_priv aom_codec_alg_priv_t;
typedef struct aom_codec_priv_enc_mr_cfg aom_codec_priv_enc_mr_cfg_t;

/*!\brief init function pointer prototype
 *
 * Performs algorithm-specific initialization of the decoder context. This
 * function is called by the generic aom_codec_init() wrapper function, so
 * plugins implementing this interface may trust the input parameters to be
 * properly initialized.
 *
 * \param[in] ctx   Pointer to this instance's context
 * \retval #AOM_CODEC_OK
 *     The input stream was recognized and decoder initialized.
 * \retval #AOM_CODEC_MEM_ERROR
 *     Memory operation failed.
 */
typedef aom_codec_err_t (*aom_codec_init_fn_t)(
    aom_codec_ctx_t *ctx, aom_codec_priv_enc_mr_cfg_t *data);

/*!\brief destroy function pointer prototype
 *
 * Performs algorithm-specific destruction of the decoder context. This
 * function is called by the generic aom_codec_destroy() wrapper function,
 * so plugins implementing this interface may trust the input parameters
 * to be properly initialized.
 *
 * \param[in] ctx   Pointer to this instance's context
 * \retval #AOM_CODEC_OK
 *     The input stream was recognized and decoder initialized.
 * \retval #AOM_CODEC_MEM_ERROR
 *     Memory operation failed.
 */
typedef aom_codec_err_t (*aom_codec_destroy_fn_t)(aom_codec_alg_priv_t *ctx);

/*!\brief parse stream info function pointer prototype
 *
 * Performs high level parsing of the bitstream. This function is called by the
 * generic aom_codec_peek_stream_info() wrapper function, so plugins
 * implementing this interface may trust the input parameters to be properly
 * initialized.
 *
 * \param[in]      data    Pointer to a block of data to parse
 * \param[in]      data_sz Size of the data buffer
 * \param[in,out]  si      Pointer to stream info to update. The size member
 *                         \ref MUST be properly initialized, but \ref MAY be
 *                         clobbered by the algorithm. This parameter \ref MAY
 *                         be NULL.
 *
 * \retval #AOM_CODEC_OK
 *     Bitstream is parsable and stream information updated
 */
typedef aom_codec_err_t (*aom_codec_peek_si_fn_t)(const uint8_t *data,
                                                  unsigned int data_sz,
                                                  aom_codec_stream_info_t *si);

/*!\brief Return information about the current stream.
 *
 * Returns information about the stream that has been parsed during decoding.
 *
 * \param[in]      ctx     Pointer to this instance's context
 * \param[in,out]  si      Pointer to stream info to update. The size member
 *                         \ref MUST be properly initialized, but \ref MAY be
 *                         clobbered by the algorithm. This parameter \ref MAY
 *                         be NULL.
 *
 * \retval #AOM_CODEC_OK
 *     Bitstream is parsable and stream information updated
 */
typedef aom_codec_err_t (*aom_codec_get_si_fn_t)(aom_codec_alg_priv_t *ctx,
                                                 aom_codec_stream_info_t *si);

/*!\brief control function pointer prototype
 *
 * This function is used to exchange algorithm specific data with the decoder
 * instance. This can be used to implement features specific to a particular
 * algorithm.
 *
 * This function is called by the generic aom_codec_control() wrapper
 * function, so plugins implementing this interface may trust the input
 * parameters to be properly initialized. However,  this interface does not
 * provide type safety for the exchanged data or assign meanings to the
 * control codes. Those details should be specified in the algorithm's
 * header file. In particular, the ctrl_id parameter is guaranteed to exist
 * in the algorithm's control mapping table, and the data parameter may be NULL.
 *
 *
 * \param[in]     ctx              Pointer to this instance's context
 * \param[in]     ctrl_id          Algorithm specific control identifier
 * \param[in,out] data             Data to exchange with algorithm instance.
 *
 * \retval #AOM_CODEC_OK
 *     The internal state data was deserialized.
 */
typedef aom_codec_err_t (*aom_codec_control_fn_t)(aom_codec_alg_priv_t *ctx,
                                                  va_list ap);

/*!\brief control function pointer mapping
 *
 * This structure stores the mapping between control identifiers and
 * implementing functions. Each algorithm provides a list of these
 * mappings. This list is searched by the aom_codec_control() wrapper
 * function to determine which function to invoke. The special
 * value {0, NULL} is used to indicate end-of-list, and must be
 * present. The special value {0, <non-null>} can be used as a catch-all
 * mapping. This implies that ctrl_id values chosen by the algorithm
 * \ref MUST be non-zero.
 */
typedef const struct aom_codec_ctrl_fn_map {
  int ctrl_id;
  aom_codec_control_fn_t fn;
} aom_codec_ctrl_fn_map_t;

/*!\brief decode data function pointer prototype
 *
 * Processes a buffer of coded data. If the processing results in a new
 * decoded frame becoming available, #AOM_CODEC_CB_PUT_SLICE and
 * #AOM_CODEC_CB_PUT_FRAME events are generated as appropriate. This
 * function is called by the generic aom_codec_decode() wrapper function,
 * so plugins implementing this interface may trust the input parameters
 * to be properly initialized.
 *
 * \param[in] ctx          Pointer to this instance's context
 * \param[in] data         Pointer to this block of new coded data. If
 *                         NULL, a #AOM_CODEC_CB_PUT_FRAME event is posted
 *                         for the previously decoded frame.
 * \param[in] data_sz      Size of the coded data, in bytes.
 *
 * \return Returns #AOM_CODEC_OK if the coded data was processed completely
 *         and future pictures can be decoded without error. Otherwise,
 *         see the descriptions of the other error codes in ::aom_codec_err_t
 *         for recoverability capabilities.
 */
typedef aom_codec_err_t (*aom_codec_decode_fn_t)(aom_codec_alg_priv_t *ctx,
                                                 const uint8_t *data,
                                                 unsigned int data_sz,
                                                 void *user_priv,
                                                 long deadline);

/*!\brief Decoded frames iterator
 *
 * Iterates over a list of the frames available for display. The iterator
 * storage should be initialized to NULL to start the iteration. Iteration is
 * complete when this function returns NULL.
 *
 * The list of available frames becomes valid upon completion of the
 * aom_codec_decode call, and remains valid until the next call to
 * aom_codec_decode.
 *
 * \param[in]     ctx      Pointer to this instance's context
 * \param[in out] iter     Iterator storage, initialized to NULL
 *
 * \return Returns a pointer to an image, if one is ready for display. Frames
 *         produced will always be in PTS (presentation time stamp) order.
 */
typedef aom_image_t *(*aom_codec_get_frame_fn_t)(aom_codec_alg_priv_t *ctx,
                                                 aom_codec_iter_t *iter);

/*!\brief Pass in external frame buffers for the decoder to use.
 *
 * Registers functions to be called when libaom needs a frame buffer
 * to decode the current frame and a function to be called when libaom does
 * not internally reference the frame buffer. This set function must
 * be called before the first call to decode or libaom will assume the
 * default behavior of allocating frame buffers internally.
 *
 * \param[in] ctx          Pointer to this instance's context
 * \param[in] cb_get       Pointer to the get callback function
 * \param[in] cb_release   Pointer to the release callback function
 * \param[in] cb_priv      Callback's private data
 *
 * \retval #AOM_CODEC_OK
 *     External frame buffers will be used by libaom.
 * \retval #AOM_CODEC_INVALID_PARAM
 *     One or more of the callbacks were NULL.
 * \retval #AOM_CODEC_ERROR
 *     Decoder context not initialized, or algorithm not capable of
 *     using external frame buffers.
 *
 * \note
 * When decoding VP9, the application may be required to pass in at least
 * #AOM_MAXIMUM_WORK_BUFFERS external frame
 * buffers.
 */
typedef aom_codec_err_t (*aom_codec_set_fb_fn_t)(
    aom_codec_alg_priv_t *ctx, aom_get_frame_buffer_cb_fn_t cb_get,
    aom_release_frame_buffer_cb_fn_t cb_release, void *cb_priv);

typedef aom_codec_err_t (*aom_codec_encode_fn_t)(aom_codec_alg_priv_t *ctx,
                                                 const aom_image_t *img,
                                                 aom_codec_pts_t pts,
                                                 unsigned long duration,
                                                 aom_enc_frame_flags_t flags,
                                                 unsigned long deadline);
typedef const aom_codec_cx_pkt_t *(*aom_codec_get_cx_data_fn_t)(
    aom_codec_alg_priv_t *ctx, aom_codec_iter_t *iter);

typedef aom_codec_err_t (*aom_codec_enc_config_set_fn_t)(
    aom_codec_alg_priv_t *ctx, const aom_codec_enc_cfg_t *cfg);
typedef aom_fixed_buf_t *(*aom_codec_get_global_headers_fn_t)(
    aom_codec_alg_priv_t *ctx);

typedef aom_image_t *(*aom_codec_get_preview_frame_fn_t)(
    aom_codec_alg_priv_t *ctx);

typedef aom_codec_err_t (*aom_codec_enc_mr_get_mem_loc_fn_t)(
    const aom_codec_enc_cfg_t *cfg, void **mem_loc);

/*!\brief usage configuration mapping
 *
 * This structure stores the mapping between usage identifiers and
 * configuration structures. Each algorithm provides a list of these
 * mappings. This list is searched by the aom_codec_enc_config_default()
 * wrapper function to determine which config to return. The special value
 * {-1, {0}} is used to indicate end-of-list, and must be present. At least
 * one mapping must be present, in addition to the end-of-list.
 *
 */
typedef const struct aom_codec_enc_cfg_map {
  int usage;
  aom_codec_enc_cfg_t cfg;
} aom_codec_enc_cfg_map_t;

/*!\brief Decoder algorithm interface interface
 *
 * All decoders \ref MUST expose a variable of this type.
 */
struct aom_codec_iface {
  const char *name;                   /**< Identification String  */
  int abi_version;                    /**< Implemented ABI version */
  aom_codec_caps_t caps;              /**< Decoder capabilities */
  aom_codec_init_fn_t init;           /**< \copydoc ::aom_codec_init_fn_t */
  aom_codec_destroy_fn_t destroy;     /**< \copydoc ::aom_codec_destroy_fn_t */
  aom_codec_ctrl_fn_map_t *ctrl_maps; /**< \copydoc ::aom_codec_ctrl_fn_map_t */
  struct aom_codec_dec_iface {
    aom_codec_peek_si_fn_t peek_si; /**< \copydoc ::aom_codec_peek_si_fn_t */
    aom_codec_get_si_fn_t get_si;   /**< \copydoc ::aom_codec_get_si_fn_t */
    aom_codec_decode_fn_t decode;   /**< \copydoc ::aom_codec_decode_fn_t */
    aom_codec_get_frame_fn_t
        get_frame;                   /**< \copydoc ::aom_codec_get_frame_fn_t */
    aom_codec_set_fb_fn_t set_fb_fn; /**< \copydoc ::aom_codec_set_fb_fn_t */
  } dec;
  struct aom_codec_enc_iface {
    int cfg_map_count;
    aom_codec_enc_cfg_map_t
        *cfg_maps;                /**< \copydoc ::aom_codec_enc_cfg_map_t */
    aom_codec_encode_fn_t encode; /**< \copydoc ::aom_codec_encode_fn_t */
    aom_codec_get_cx_data_fn_t
        get_cx_data; /**< \copydoc ::aom_codec_get_cx_data_fn_t */
    aom_codec_enc_config_set_fn_t
        cfg_set; /**< \copydoc ::aom_codec_enc_config_set_fn_t */
    aom_codec_get_global_headers_fn_t
        get_glob_hdrs; /**< \copydoc ::aom_codec_get_global_headers_fn_t */
    aom_codec_get_preview_frame_fn_t
        get_preview; /**< \copydoc ::aom_codec_get_preview_frame_fn_t */
    aom_codec_enc_mr_get_mem_loc_fn_t
        mr_get_mem_loc; /**< \copydoc ::aom_codec_enc_mr_get_mem_loc_fn_t */
  } enc;
};

/*!\brief Callback function pointer / user data pair storage */
typedef struct aom_codec_priv_cb_pair {
  union {
    aom_codec_put_frame_cb_fn_t put_frame;
    aom_codec_put_slice_cb_fn_t put_slice;
  } u;
  void *user_priv;
} aom_codec_priv_cb_pair_t;

/*!\brief Instance private storage
 *
 * This structure is allocated by the algorithm's init function. It can be
 * extended in one of two ways. First, a second, algorithm specific structure
 * can be allocated and the priv member pointed to it. Alternatively, this
 * structure can be made the first member of the algorithm specific structure,
 * and the pointer cast to the proper type.
 */
struct aom_codec_priv {
  const char *err_detail;
  aom_codec_flags_t init_flags;
  struct {
    aom_codec_priv_cb_pair_t put_frame_cb;
    aom_codec_priv_cb_pair_t put_slice_cb;
  } dec;
  struct {
    aom_fixed_buf_t cx_data_dst_buf;
    unsigned int cx_data_pad_before;
    unsigned int cx_data_pad_after;
    aom_codec_cx_pkt_t cx_data_pkt;
    unsigned int total_encoders;
  } enc;
};

/*
 * Multi-resolution encoding internal configuration
 */
struct aom_codec_priv_enc_mr_cfg {
  unsigned int mr_total_resolutions;
  unsigned int mr_encoder_id;
  struct aom_rational mr_down_sampling_factor;
  void *mr_low_res_mode_info;
};

#undef AOM_CTRL_USE_TYPE
#define AOM_CTRL_USE_TYPE(id, typ) \
  static AOM_INLINE typ id##__value(va_list args) { return va_arg(args, typ); }

#undef AOM_CTRL_USE_TYPE_DEPRECATED
#define AOM_CTRL_USE_TYPE_DEPRECATED(id, typ) \
  static AOM_INLINE typ id##__value(va_list args) { return va_arg(args, typ); }

#define CAST(id, arg) id##__value(arg)

/* CODEC_INTERFACE convenience macro
 *
 * By convention, each codec interface is a struct with extern linkage, where
 * the symbol is suffixed with _algo. A getter function is also defined to
 * return a pointer to the struct, since in some cases it's easier to work
 * with text symbols than data symbols (see issue #169). This function has
 * the same name as the struct, less the _algo suffix. The CODEC_INTERFACE
 * macro is provided to define this getter function automatically.
 */
#define CODEC_INTERFACE(id)                          \
  aom_codec_iface_t *id(void) { return &id##_algo; } \
  aom_codec_iface_t id##_algo

/* Internal Utility Functions
 *
 * The following functions are intended to be used inside algorithms as
 * utilities for manipulating aom_codec_* data structures.
 */
struct aom_codec_pkt_list {
  unsigned int cnt;
  unsigned int max;
  struct aom_codec_cx_pkt pkts[1];
};

#define aom_codec_pkt_list_decl(n)     \
  union {                              \
    struct aom_codec_pkt_list head;    \
    struct {                           \
      struct aom_codec_pkt_list head;  \
      struct aom_codec_cx_pkt pkts[n]; \
    } alloc;                           \
  }

#define aom_codec_pkt_list_init(m) \
  (m)->alloc.head.cnt = 0,         \
  (m)->alloc.head.max = sizeof((m)->alloc.pkts) / sizeof((m)->alloc.pkts[0])

int aom_codec_pkt_list_add(struct aom_codec_pkt_list *,
                           const struct aom_codec_cx_pkt *);

const aom_codec_cx_pkt_t *aom_codec_pkt_list_get(
    struct aom_codec_pkt_list *list, aom_codec_iter_t *iter);

#include <stdio.h>
#include <setjmp.h>

struct aom_internal_error_info {
  aom_codec_err_t error_code;
  int has_detail;
  char detail[80];
  int setjmp;
  jmp_buf jmp;
};

#define CLANG_ANALYZER_NORETURN
#if defined(__has_feature)
#if __has_feature(attribute_analyzer_noreturn)
#undef CLANG_ANALYZER_NORETURN
#define CLANG_ANALYZER_NORETURN __attribute__((analyzer_noreturn))
#endif
#endif

void aom_internal_error(struct aom_internal_error_info *info,
                        aom_codec_err_t error, const char *fmt,
                        ...) CLANG_ANALYZER_NORETURN;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AOM_INTERNAL_AOM_CODEC_INTERNAL_H_
