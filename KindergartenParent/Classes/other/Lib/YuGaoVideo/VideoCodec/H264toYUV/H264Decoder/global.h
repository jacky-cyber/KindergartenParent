

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define TRACE

////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../MyThrowException.h"


#undef  assert(e)

#ifdef NDEBUG
#define	assert(e)	((void)0)
#else
#define assert(e)   \
do {    \
    if (!(e)) {   \
        printf("My Assertion failed: (%s), function %s, file %s, line %d.\n", (#e), __func__, __FILE__, __LINE__); \
        throwException();   \
    }   \
} while(0)
#endif // NDEBUG
 

// __declspec(align(16))     __attribute__(aligned(16))

#define min(A, B)   ((A) < (B) ? (A) : (B))
#define max(A, B)   ((A) > (B) ? (A) : (B))


//  common type
typedef unsigned char            uchar;
//typedef char                     char;
typedef unsigned short           uint16_t;
typedef short                    int16_t;
typedef unsigned long            ulong;
//typedef long                     long;
//typedef unsigned __int64         ulong long;
//typedef __int64                  long long;

////////////////////////////////////////////////////////////////////////////////

// CPU flag
#define    CPU_ASM              0x0
#define    CPU_MMX              0x00000001
#define    CPU_MMXEXT           0x00000002
#define    CPU_SSE              0x00000004
#define    CPU_SSE2             0x00000008
#define    CPU_3DNOW            0x00000010
#define    CPU_3DNOWEXT         0x00000020
#define    CPU_ALTIVEC          0x00000040
#define    CPU_SSE3             0x00000080


#define    MAX_RBSP_SIZE                64000
#define    ZEROBYTES_SHORTSTARTCODE     2

#define    NALU_PRIORITY_HIGHEST        3
#define    NALU_PRIORITY_HIGH           2
#define    NALU_PRIORITY_LOW            1
#define    NALU_PRIORITY_DISPOSABLE     0

#define    NALU_TYPE_SLICE              1
#define    NALU_TYPE_DPA                2
#define    NALU_TYPE_DPB                3
#define    NALU_TYPE_DPC                4
#define    NALU_TYPE_IDR                5
#define    NALU_TYPE_SEI                6
#define    NALU_TYPE_SPS                7
#define    NALU_TYPE_PPS                8
#define    NALU_TYPE_AUD                9
#define    NALU_TYPE_EOSEQ              10
#define    NALU_TYPE_EOSTREAM           11
#define    NALU_TYPE_FILL               12
#define    NALU_TYPE_SPSEX              13
#define    NALU_TYPE_ACP                19


#define    MAXnum_ref_frames_in_pic_order_cnt_cycle    256
#define    MAX_SPS      32
#define    MAX_PPS      256
#define    MAXnum_slice_groups_minus1                  8

#define    PROFILE_BASELINE         66
#define    PROFILE_MAIN             77
#define    PROFILE_EXTENDED         88
#define    PROFILE_HIGH             100
#define    PROFILE_HIGH10           110
#define    PROFILE_HIGH422          122
#define    PROFILE_HIGH444          144


#define    P_SLICE                   0
#define    B_SLICE                   1
#define    I_SLICE                   2
#define    SP_SLICE                  3
#define    SI_SLICE                  4
//#define  P_SLICE                   5
//#define  B_SLICE                   6
//#define  I_SLICE                   7
//#define  SP_SLICE                  8
//#define  SI_SLICE                  9

#define   MAX_REFERENCE_PICTURES     32

#define   FRAME           0 
#define   TOP_FIELD       1
#define   BOTTOM_FIELD    2


#define   MAX_WIDTH_IN_MBS          48
#define   MAX_HEIGHT_IN_MBS         36
#define   MAX_SIZE_IN_MBS           1872
#define   PAD_SIZE                  32


#define   EOS                        1   
#define   SOP                        2   
#define   SOS                        3   


#define   P8x8                                     8
#define   I4MB                                     9
#define   I16MB                                    10
#define   IBLOCK                                   11
#define   SI4MB                                    12
#define   I8MB                                     13
#define   IPCM                                     14

#define   INTRA_CHROMA_PRED_MODE_DC                0
#define   INTRA_CHROMA_PRED_MODE_HORIZONTAL        1
#define   INTRA_CHROMA_PRED_MODE_VERTICAL          2
#define   INTRA_CHROMA_PRED_MODE_PLANE             3


#define   MVPRED_MEDIAN                            0
#define   MVPRED_L                                 1
#define   MVPRED_U                                 2
#define   MVPRED_UR                                3

#define   VERT_PRED                                0
#define   HOR_PRED                                 1
#define   DC_PRED                                  2
#define   DIAG_DOWN_LEFT_PRED                      3
#define   DIAG_DOWN_RIGHT_PRED                     4
#define   VERT_RIGHT_PRED                          5
#define   HOR_DOWN_PRED                            6
#define   VERT_LEFT_PRED                           7
#define   HOR_UP_PRED                              8

#define   VERT_PRED_16                             0
#define   HOR_PRED_16                              1
#define   DC_PRED_16                               2
#define   PLANE_16                                 3


#define   VERT_PRED_8                              0
#define   HOR_PRED_8                               1
#define   DC_PRED_8                                2
#define   PLANE_8                                  3

#define RUNBEFORE_NUM                              7

#define  MAX_LIST_SIZE                             33
#define  MAX_QP                                    51

typedef  enum _block_type {
	BT_16x16 = 0,
	BT_16x8 = 1,
	BT_8x16 = 2,
	BT_8x8 = 3,
	BT_8x4 = 4,
	BT_4x8 = 5,
	BT_4x4 = 6,
	BT_SIZE = 7
} block_type_t;

//  bitstream
typedef struct _bs {
    uchar   *p_buffer;
	long   length;
    uchar   *bit_ptr;
	ulong   bit_buf;
    long   bits;
} bs_t;

//nalu
typedef struct _nalu {
	long    nalu_ref_idc;
	long    nal_unit_type;
	uchar    *buf;
	long    len;
} nalu_t;


typedef struct _vui_seq_parameters{
	long    not_used;
} vui_seq_parameters_t;

//sps
typedef struct _seq_parameter_set {
	ulong    profile_idc;
    ulong    constraint_set0_flag;
	ulong    constraint_set1_flag;
	ulong    constraint_set2_flag;
	ulong    constraint_set3_flag;
	ulong    level_idc;
	ulong    seq_parameter_set_id;
	/*
	ulong    chroma_format_idc;
	ulong    residual_color_trasform_flag;
	ulong    bit_depth_luma_minus8;
	ulong    bit_depth_chroma_minus8;
	ulong    qpprime_y_zero_transform_bypass_flag;
	long    seq_scaling_matrix_present_flag;
	long    seq_scaling_list_present_flag[8];
    */

	ulong    log2_max_frame_num_minus4;
	ulong    pic_order_cnt_type;
    ulong    log2_max_pic_order_cnt_lsb_minus4;
	ulong    delta_pic_order_always_zero_flag;
	long     offset_for_non_ref_pic;
	long     offset_for_top_to_bottom_field;
	ulong    num_ref_frames_in_pic_order_cnt_cycle;
	long     offset_for_ref_frame[MAXnum_ref_frames_in_pic_order_cnt_cycle];

	ulong    num_ref_frames;
	ulong    gaps_in_frame_num_value_allowed_flag;
	ulong    pic_width_in_mbs_minus1;
	ulong    pic_height_in_map_units_minus1;
	ulong    frame_mbs_only_flag;
	ulong    mb_adaptive_frame_field_flag;
	ulong    direct_8x8_inference_flag;
	ulong    frame_cropping_flag;
	ulong    frame_cropping_rect_left_offset;
	ulong    frame_cropping_rect_right_offset;
	ulong    frame_cropping_rect_top_offset;
	ulong    frame_cropping_rect_bottom_offset;
	ulong    vui_parameters_present_flag;
	vui_seq_parameters_t  vui_seq_parameters;
} seq_parameter_set_t;


typedef struct _pic_parameter_set{
	ulong   pic_parameter_set_id;
	ulong   seq_parameter_set_id;
	ulong   entropy_coding_flag;
	ulong   poc_order_present_flag;
	ulong   num_slice_groups_minus1;
	ulong   slice_group_map_type;
	ulong   run_length_minus1[MAXnum_slice_groups_minus1];
	ulong   top_left[MAXnum_slice_groups_minus1];
	ulong   bottom_right[MAXnum_slice_groups_minus1];
	ulong   slice_group_change_direction_flag;
	ulong   slice_group_change_rate_minus1;
	ulong   num_slice_group_map_units_minus1;
	ulong   *slice_group_id;
	ulong   num_ref_idx_l0_active_minus1;
	ulong   num_ref_idx_l1_active_minus1;
	ulong   weighted_pred_flag;
	ulong   weighted_bipred_idc;
	long    pic_init_qp_minus26;
	long    pic_init_qs_minus26;
	long    chroma_qp_index_offset;
	ulong   deblocking_filter_control_present_flag;
	ulong   constrained_intra_pred_flag;
	ulong   redundant_pic_cnt_present_flag;
} pic_parameter_set_t;


typedef struct _dec_ref_pic_marking {
	ulong  memory_management_control_operation;
	ulong difference_of_pic_nums_minus1;
	ulong long_term_pic_num;
	ulong long_term_frame_idx;
	ulong max_long_term_frame_idx_plus1;
	struct _dec_ref_pic_marking *next;
} dec_ref_pic_marking_t;


typedef struct _slice {
	ulong  start_mb_nr;
	ulong  slice_type;
	ulong  pic_parameter_set_id;
	long   frame_num;
	ulong  pic_field_flag;
	ulong  bottom_field_flag;
	ulong  idr_pic_id;
	long   pic_order_cnt_lsb;
	long   delta_pic_order_cnt_bottom;
    long   delta_pic_order_cnt[2];
	ulong  redundant_pic_cnt;
	ulong  direct_spatial_mv_pred_flag;
    ulong  num_ref_idx_l0_active;
	ulong  num_ref_idx_l1_active;
	ulong  ref_pic_list_reordering_flag_l0;
	ulong  remapping_of_pic_nums_idc_l0[MAX_REFERENCE_PICTURES];
	ulong  abs_diff_pic_num_minus1_l0[MAX_REFERENCE_PICTURES];
	ulong  long_term_pic_num_l0[MAX_REFERENCE_PICTURES];
	ulong  ref_pic_list_reordering_flag_l1;
	ulong  remapping_of_pic_nums_idc_l1[MAX_REFERENCE_PICTURES];
	ulong  abs_diff_pic_num_minus1_l1[MAX_REFERENCE_PICTURES];
	ulong  long_term_pic_num_l1[MAX_REFERENCE_PICTURES];
	//   weighted pred is ignore ,  may i will complete it later
    ulong  no_output_of_prior_pics_flag;
	ulong  long_term_reference_flag;
	ulong  adaptive_ref_pic_buffering_flag;
	dec_ref_pic_marking_t   *dec_ref_pic_marking_buf;
	ulong  cabac_init_idc;
	long   slice_qp;

	ulong  disable_deblocking_filter_idc;
	long   slice_alpha_c0_offset_div2;
	long   slice_beta_offset_div2;
    ulong  slice_group_change_cycle;    
} slice_t;

typedef struct _mv {
	long   x;
	long   y;
} mv_t;


typedef struct _pos {
	long   x;
	long   y;
} pos_t;

typedef struct _sub_mb_position {
	long  mb_offset;
	long  sub_idx;
} sub_mb_position_t;

typedef struct _mb_access {
	pos_t    mb_pos[MAX_SIZE_IN_MBS];
	long  neighbour_mb_addr[MAX_SIZE_IN_MBS][6];
} mb_access_t;

typedef struct _pic_order_cnt {
	long  frame_poc;
	long  top_field_poc;
	long  bottom_field_poc;
	long  prev_pic_order_cnt_msb;
	long  prev_pic_order_cnt_lsb;
	long  last_mmco5_flag;
	long  last_pic_bottom_field_flag;	
	long  this_poc;
	long  frame_num_offset;
	long  prev_frame_num_offset;
	long  prev_frame_num;
	long  abs_frame_num;
} pic_order_cnt_t;


typedef struct  _num_total_coeff_and_trailingones {
	char TotalCoeff;
	char TrailingOnes;
} num_total_coeff_and_trailingones_t;

typedef struct _macroblock {
    long  qp;
	long  slice_nr;
	long  delta_qp;
	long  mb_type;	
	long  cbp;
	long  cbp_blk;
	long  cbp_bits;
	long  i16mode;
	long  b8mode[4];
	long  b8pdir[4];
	long  disable_deblock_filter_idc;
	long  alpha_c0_offset;
	long  beta_offset;
	long  c_ipred_mode;
	long  skip_flag;
	long  all_ref_zero;	
} macroblock_t;



typedef struct _picture
{
    long   pic_structure;  
    long   frame_poc;
	long   top_field_poc;
	long   bottmo_field_poc;
	long   width;
	long   height;
	long   width_cr;
	long   height_cr;
	long   stride_luma;
	long   stride_chroma;
	long   chroma_vector_adjustment;
	long   frame_num;
	long   frame_num_wrap;
	long   pic_num;
	long   is_output;
	long   is_reference;
	long   is_long_term;
	long   long_term_frame_idx;

	mv_t      *front_mv;
	long   *front_ref_idx;

	uchar   **img_y;
	uchar   **img_u;
	uchar   **img_v;
} picture_t;


typedef struct _dpb {
    picture_t *listX[MAX_LIST_SIZE];  
	picture_t **pic_closet;
	long  last_output_poc;
	long  num_ref_frames;
	long  dpb_size;
	long  max_size;
} dpb_t;


typedef struct _loop_filter {
	long mb_addr;
	long filter_left_edge_flag;
	long filter_top_edge_flag;
	long mb_x,mb_y;
	uchar **img_y;
	uchar **img_u;
	uchar **img_v;
	mv_t      *front_mv;
	long   *front_ref_idx;
	long stride_luma;
	long stride_chroma;
} loop_filter_t;
 
typedef struct _decoder {
	int16_t   cof[24][16];
	int16_t   mpr[24][16];
	long   nnz_offset_a[16];
	long   nnz_offset_b[16];
	nalu_t  *cur_nalu;
	bs_t    *bs;
	seq_parameter_set_t   sps[MAX_SPS];
	seq_parameter_set_t   *active_sps;
	pic_parameter_set_t   pps[MAX_PPS];
	pic_parameter_set_t   *active_pps;
	slice_t  *cur_slice;
	mb_access_t  *mb_access;
	pic_order_cnt_t  *poc;
	macroblock_t     *mb_date;
	picture_t       *cur_pic;
	uchar         *ipred_mode;	
	char          *nz_coeffs_y;
	char          *nz_coeffs_u;
	char          *nz_coeffs_v;
	dpb_t           *dpb;

	long  max_frame_num;
	long  frame_num;
	long  pic_width_in_mbs;
	long  pic_height_in_map_units;
	long  frame_height_in_mbs;
	long  frame_size_in_mbs;
	long  pic_height_in_mbs;
	long  pic_size_in_mbs;
    long  width;
	long  height;
	long  width_cr;
	long  height_cr;
    long  idr_flag;	
	long   nalu_ref_idc;
	long   struction;
	long   next_header;
	long   mb_skip_run;
	long   cur_slice_nr;
	long   cur_mb_nr;
	long   num_dec_mb;
	long   num_dec_nalu;
	long   num_dec_frame;
	long   mb_x;
	long   mb_y;	
	long   qp;	
#ifdef TRACE 
    long long   frequency;
	long long   start;
	long long   stop;
	long   total;
#endif
} decoder_t;


/*extern*/ void (*memcpy_4x4_dc) ( int16_t *data , int16_t dc );
/*extern*/ void (*iquant_4x4) ( int16_t *data , long qp_per, long qp_rem );
/*extern*/ void (*idct_4x4) ( int16_t *data );
/*extern*/ void (*idct_4x4_dc) ( int16_t *data , long qp );
/*extern*/ void (*recon_mb_p) ( decoder_t *decoder );
/*extern*/ void (*get_block_16x16_luma) (uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
/*extern*/ void (*get_block_8x8_chroma) (uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv);
/*extern*/ void (*get_block_8x8_luma) (uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);
/*extern*/ void (*get_block_4x4_chroma) (uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx);
/*extern*/ void (*filter_luma_ver) (loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
/*extern*/ void (*filter_luma_hor) (loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
/*extern*/ void (*filter_chroma_ver) (decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
/*extern*/ void (*filter_chroma_hor) (decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
/*extern*/ void (*recon_chroma_for_mb_4x4) (decoder_t *decoder);
/*extern*/ void (*get_block_4x4_luma) (uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);

#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4311 )

#endif