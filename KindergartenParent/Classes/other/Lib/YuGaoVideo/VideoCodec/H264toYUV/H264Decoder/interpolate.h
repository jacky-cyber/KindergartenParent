

#ifndef _INTERPOLATE_H_
#define _INTERPOLATE_H_



void  get_block_16x16_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void  get_block_16x8_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void  get_block_8x16_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void  get_block_8x8_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);
void  get_block_8x4_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void  get_block_4x8_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void  get_block_4x4_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);
void  get_block_8x8_chroma_c(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv);
void  get_block_4x4_chroma_c(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx);
void  get_block_2x2_chroma_c(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx);


/*
void  get_block_16x16_luma_sse2(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y);
void  get_block_8x8_chroma_sse2(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv);
void  get_block_8x8_luma_sse2(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);
void  get_block_4x4_chroma_sse2(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx);
void  get_block_4x4_luma_sse2(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx);
*/

#endif