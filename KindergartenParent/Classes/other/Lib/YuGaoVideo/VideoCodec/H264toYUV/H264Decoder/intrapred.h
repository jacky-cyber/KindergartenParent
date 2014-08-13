

#ifndef _INTRAPRED_H_
#define _INTRAPRED_H_


void   intra_pred_luma_16x16(decoder_t *decoder,macroblock_t *cur_mb);
void   intra_pred_luma_4x4(decoder_t *decoder,macroblock_t *cur_mb,long idx);
void   intra_pred_chroma(decoder_t *decoder,macroblock_t *cur_mb,long uv);

#endif