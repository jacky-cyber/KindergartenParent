

#include "global.h"




void (*memcpy_4x4_dc) ( int16_t *data , int16_t dc );
void (*iquant_4x4) ( int16_t *data , long qp_per, long qp_rem );
void (*idct_4x4) ( int16_t *data );
void (*idct_4x4_dc) ( int16_t *data , long qp );
void (*recon_mb_p) ( decoder_t *decoder );
void (*get_block_16x16_luma) (uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void (*get_block_8x8_chroma) (uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv);
void (*get_block_8x8_luma) (uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);
void (*get_block_4x4_chroma) (uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx);
void (*filter_luma_ver) (loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
void (*filter_luma_hor) (loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
void (*filter_chroma_ver) (decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
void (*filter_chroma_hor) (decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
void (*recon_chroma_for_mb_4x4) (decoder_t *decoder);
void (*get_block_4x4_luma) (uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);




