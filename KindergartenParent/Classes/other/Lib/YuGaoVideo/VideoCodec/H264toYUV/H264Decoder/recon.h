

#ifndef _RECON_H_
#define _RECON_H_


void  recon_mb_p_c(decoder_t *decoder);
void  recon_chroma_for_mb_4x4_c(decoder_t *decoder);
void  expand_pic(picture_t *pic);

void  recon_mb_p_sse2(decoder_t *decoder);
void  recon_chroma_for_mb_4x4_sse2(decoder_t *decoder);

#endif