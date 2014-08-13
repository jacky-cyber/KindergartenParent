

#ifndef _SYNTAX_H_
#define _SYNTAX_H_



void  read_mb_type_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs);
void  interpret_mb_type(ulong slice_type,macroblock_t *cur_mb);
void  read_submb_type_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs);
void  get_pskip_mv(decoder_t *decoder,macroblock_t *cur_mb);
void  read_ipred_type_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs);
void  read_inter_pred_ref_idx_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs);
void  read_inter_pred_mvd_calvc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs);
void  read_coeffs_cavlc(decoder_t *decoder, macroblock_t *cur_mb,bs_t *bs);


#endif