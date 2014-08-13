

#ifndef _MACROBLOCK_H_
#define _MACROBLOCK_H_



void  init_macroblock(decoder_t *decoder);
void  read_macroblock(decoder_t *decoder,bs_t *bs);
void  decode_macroblock(decoder_t *decoder);
long  exit_macroblock(decoder_t *decoder);

void  init_coef_nnz(decoder_t *decoder);

#endif