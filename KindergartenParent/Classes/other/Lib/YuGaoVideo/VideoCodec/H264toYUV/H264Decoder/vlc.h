

#ifndef _VLC_H_
#define _VLC_H_



  
long golumb_u1(bs_t *bs);
long golumb_u(bs_t *bs,long numbits);
long golumb_ue(bs_t *bs);
long golumb_se(bs_t *bs);

 
long  read_intra4x4_pred_mode_cavlc(bs_t *bs);
long  read_ref_idx_cavlc(bs_t *bs,long ref_idx_active_flag);
long  read_cbp_intra_cavlc(bs_t *bs);
long  read_cbp_inter_cavlc(bs_t *bs);
long  read_coeffs_4x4_block_cavlc(bs_t *bs,long pred_nnz,long max_coeff_num,long cdc,long *levarr,long *runarr);



#endif