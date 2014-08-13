

#ifndef _DCT_H_
#define _DCT_H_



void  idct_4x4_dc_c(int16_t *data,long qp);
void  iquant_4x4_c(int16_t *data,long qp_per,long qp_rem);
void  idct_4x4_c(int16_t *data);
void  idct_2x2_dc_c(int16_t *data,long qp_per,long qp_rem);
void  memcpy_4x4_dc_c(int16_t *data,int16_t dc);

/*
void  memcpy_4x4_dc_mmx(int16_t *data,int16_t dc);
void  iquant_4x4_mmx(int16_t *data,long qp_per,long qp_rem);
void  idct_4x4_dc_mmx(int16_t *data,long qp);
void  idct_4x4_mmx(int16_t *data);
*/


#endif