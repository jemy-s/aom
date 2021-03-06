##
## Copyright (c) 2017, Alliance for Open Media. All rights reserved
##
## This source code is subject to the terms of the BSD 2 Clause License and
## the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
## was not distributed with this source code in the LICENSE file, you can
## obtain it at www.aomedia.org/license/software. If the Alliance for Open
## Media Patent License 1.0 was not distributed with this source code in the
## PATENTS file, you can obtain it at www.aomedia.org/license/patent.
##
ARCH_ARM=${RTCD_ARCH_ARM}
HAVE_NEON=${RTCD_HAVE_NEON}

CONFIG_ACCOUNTING=${RTCD_CONFIG_ACCOUNTING}
CONFIG_INSPECTION=${RTCD_CONFIG_INSPECTION}
CONFIG_ADAPT_SCAN=${RTCD_CONFIG_ADAPT_SCAN}
CONFIG_ALT_INTRA=${RTCD_CONFIG_ALT_INTRA}
CONFIG_ANS=${RTCD_CONFIG_ANS}
CONFIG_AOM_HIGHBITDEPTH=${RTCD_CONFIG_AOM_HIGHBITDEPTH}
CONFIG_AOM_QM=${RTCD_CONFIG_AOM_QM}
CONFIG_AV1=${RTCD_CONFIG_AV1}
CONFIG_AV1_DECODER=${RTCD_CONFIG_AV1_DECODER}
CONFIG_AV1_ENCODER=${RTCD_CONFIG_AV1_ENCODER}
CONFIG_BIG_ENDIAN=${RTCD_CONFIG_BIG_ENDIAN}
CONFIG_BITSTREAM_DEBUG=${RTCD_CONFIG_BITSTREAM_DEBUG}
CONFIG_CB4X4=${RTCD_CONFIG_CB4X4}
CONFIG_CDEF=${RTCD_CONFIG_CDEF}
CONFIG_CHROMA_2X2=${RTCD_CONFIG_CHROMA_2X2}
CONFIG_CODEC_SRCS=${RTCD_CONFIG_CODEC_SRCS}
CONFIG_COEFFICIENT_RANGE_CHECKING=${RTCD_CONFIG_COEFFICIENT_RANGE_CHECKING}
CONFIG_COEF_INTERLEAVE=${RTCD_CONFIG_COEF_INTERLEAVE}
CONFIG_COMPOUND_SEGMENT=${RTCD_CONFIG_COMPOUND_SEGMENT}
CONFIG_CONVOLVE_ROUND=${RTCD_CONFIG_CONVOLVE_ROUND}
CONFIG_DAALA_DIST=${RTCD_CONFIG_DAALA_DIST}
CONFIG_DAALA_EC=${RTCD_CONFIG_DAALA_EC}
CONFIG_DEBUG=${RTCD_CONFIG_DEBUG}
CONFIG_DEBUG_LIBS=${RTCD_CONFIG_DEBUG_LIBS}
CONFIG_DECODERS=${RTCD_CONFIG_DECODERS}
CONFIG_DECODE_PERF_TESTS=${RTCD_CONFIG_DECODE_PERF_TESTS}
CONFIG_DELTA_Q=${RTCD_CONFIG_DELTA_Q}
CONFIG_DEPENDENCY_TRACKING=${RTCD_CONFIG_DEPENDENCY_TRACKING}
CONFIG_DEPENDENT_HORZTILES=${RTCD_CONFIG_DEPENDENT_HORZTILES}
CONFIG_DUAL_FILTER=${RTCD_CONFIG_DUAL_FILTER}
CONFIG_EC_ADAPT=${RTCD_CONFIG_EC_ADAPT}
CONFIG_EC_MULTISYMBOL=${RTCD_CONFIG_EC_MULTISYMBOL}
CONFIG_EMULATE_HARDWARE=${RTCD_CONFIG_EMULATE_HARDWARE}
CONFIG_ENCODERS=${RTCD_CONFIG_ENCODERS}
CONFIG_ENCODE_PERF_TESTS=${RTCD_CONFIG_ENCODE_PERF_TESTS}
CONFIG_ENTROPY=${RTCD_CONFIG_ENTROPY}
CONFIG_ENTROPY_STATS=${RTCD_CONFIG_ENTROPY_STATS}
CONFIG_ERROR_CONCEALMENT=${RTCD_CONFIG_ERROR_CONCEALMENT}
CONFIG_EXPERIMENTAL=${RTCD_CONFIG_EXPERIMENTAL}
CONFIG_EXTERNAL_BUILD=${RTCD_CONFIG_EXTERNAL_BUILD}
CONFIG_EXT_INTER=${RTCD_CONFIG_EXT_INTER}
CONFIG_EXT_INTRA=${RTCD_CONFIG_EXT_INTRA}
CONFIG_EXT_PARTITION=${RTCD_CONFIG_EXT_PARTITION}
CONFIG_EXT_PARTITION_TYPES=${RTCD_CONFIG_EXT_PARTITION_TYPES}
CONFIG_EXT_REFS=${RTCD_CONFIG_EXT_REFS}
CONFIG_EXT_TILE=${RTCD_CONFIG_EXT_TILE}
CONFIG_EXT_TX=${RTCD_CONFIG_EXT_TX}
CONFIG_FILTER_7BIT=${RTCD_CONFIG_FILTER_7BIT}
CONFIG_FILTER_INTRA=${RTCD_CONFIG_FILTER_INTRA}
CONFIG_FP_MB_STATS=${RTCD_CONFIG_FP_MB_STATS}
CONFIG_FRAME_SIZE=${RTCD_CONFIG_FRAME_SIZE}
CONFIG_GCC=${RTCD_CONFIG_GCC}
CONFIG_GCOV=${RTCD_CONFIG_GCOV}
CONFIG_GLOBAL_MOTION=${RTCD_CONFIG_GLOBAL_MOTION}
CONFIG_GPROF=${RTCD_CONFIG_GPROF}
CONFIG_INSTALL_BINS=${RTCD_CONFIG_INSTALL_BINS}
CONFIG_INSTALL_DOCS=${RTCD_CONFIG_INSTALL_DOCS}
CONFIG_INSTALL_LIBS=${RTCD_CONFIG_INSTALL_LIBS}
CONFIG_INSTALL_SRCS=${RTCD_CONFIG_INSTALL_SRCS}
CONFIG_INTERNAL_STATS=${RTCD_CONFIG_INTERNAL_STATS}
CONFIG_INTRA_INTERP=${RTCD_CONFIG_INTRA_INTERP}
CONFIG_LIBYUV=${RTCD_CONFIG_LIBYUV}
CONFIG_LOOPFILTERING_ACROSS_TILES=${RTCD_CONFIG_LOOPFILTERING_ACROSS_TILES}
CONFIG_LOOP_RESTORATION=${RTCD_CONFIG_LOOP_RESTORATION}
CONFIG_LOWBITDEPTH=${RTCD_CONFIG_LOWBITDEPTH}
CONFIG_LV_MAP=${RTCD_CONFIG_LV_MAP}
CONFIG_MASKED_TX=${RTCD_CONFIG_MASKED_TX}
CONFIG_MOTION_VAR=${RTCD_CONFIG_MOTION_VAR}
CONFIG_MSVS=${RTCD_CONFIG_MSVS}
CONFIG_MULTITHREAD=${RTCD_CONFIG_MULTITHREAD}
CONFIG_MV_COMPRESS=${RTCD_CONFIG_MV_COMPRESS}
CONFIG_NCOBMC=${RTCD_CONFIG_NCOBMC}
CONFIG_NEW_QUANT=${RTCD_CONFIG_NEW_QUANT}
CONFIG_NEW_TOKENSET=${RTCD_CONFIG_NEW_TOKENSET}
CONFIG_ONTHEFLY_BITPACKING=${RTCD_CONFIG_ONTHEFLY_BITPACKING}
CONFIG_OS_SUPPORT=${RTCD_CONFIG_OS_SUPPORT}
CONFIG_PALETTE=${RTCD_CONFIG_PALETTE}
CONFIG_PALETTE_THROUGHPUT=${RTCD_CONFIG_PALETTE_THROUGHPUT}
CONFIG_PARALLEL_DEBLOCKING=${RTCD_CONFIG_PARALLEL_DEBLOCKING}
CONFIG_PIC=${RTCD_CONFIG_PIC}
CONFIG_POSTPROC=${RTCD_CONFIG_POSTPROC}
CONFIG_POSTPROC_VISUALIZER=${RTCD_CONFIG_POSTPROC_VISUALIZER}
CONFIG_PVQ=${RTCD_CONFIG_PVQ}
CONFIG_RAWBITS=${RTCD_CONFIG_RAWBITS}
CONFIG_RD_DEBUG=${RTCD_CONFIG_RD_DEBUG}
CONFIG_REALTIME_ONLY=${RTCD_CONFIG_REALTIME_ONLY}
CONFIG_RECT_TX=${RTCD_CONFIG_RECT_TX}
CONFIG_REFERENCE_BUFFER=${RTCD_CONFIG_REFERENCE_BUFFER}
CONFIG_REF_ADAPT=${RTCD_CONFIG_REF_ADAPT}
CONFIG_REF_MV=${RTCD_CONFIG_REF_MV}
CONFIG_RUNTIME_CPU_DETECT=${RTCD_CONFIG_RUNTIME_CPU_DETECT}
CONFIG_RVCT=${RTCD_CONFIG_RVCT}
CONFIG_SHARED=${RTCD_CONFIG_SHARED}
CONFIG_SIZE_LIMIT=${RTCD_CONFIG_SIZE_LIMIT}
CONFIG_SMALL=${RTCD_CONFIG_SMALL}
CONFIG_SPATIAL_RESAMPLING=${RTCD_CONFIG_SPATIAL_RESAMPLING}
CONFIG_STATIC=${RTCD_CONFIG_STATIC}
CONFIG_STATIC_MSVCRT=${RTCD_CONFIG_STATIC_MSVCRT}
CONFIG_SUB8X8_MC=${RTCD_CONFIG_SUB8X8_MC}
CONFIG_SUPERTX=${RTCD_CONFIG_SUPERTX}
CONFIG_TEMPMV_SIGNALING=${RTCD_CONFIG_TEMPMV_SIGNALING}
CONFIG_TILE_GROUPS=${RTCD_CONFIG_TILE_GROUPS}
CONFIG_TPL_MV=${RTCD_CONFIG_TPL_MV}
CONFIG_TRIPRED=${RTCD_CONFIG_TRIPRED}
CONFIG_TX64X64=${RTCD_CONFIG_TX64X64}
CONFIG_UNIT_TESTS=${RTCD_CONFIG_UNIT_TESTS}
CONFIG_UNPOISON_PARTITION_CTX=${RTCD_CONFIG_UNPOISON_PARTITION_CTX}
CONFIG_VAR_TX=${RTCD_CONFIG_VAR_TX}
CONFIG_WARPED_MOTION=${RTCD_CONFIG_WARPED_MOTION}
CONFIG_WEBM_IO=${RTCD_CONFIG_WEBM_IO}
CONFIG_XIPHRC=${RTCD_CONFIG_XIPHRC}
