

#ifndef _PARSET_H_
#define _PARSET_H_




long   process_sps(decoder_t *decoder,bs_t *bs);
long   process_pps(decoder_t *decoder,bs_t *bs);
void      active_pps(decoder_t *decoder,pic_parameter_set_t *pps);
void      active_sps(decoder_t *decoder,seq_parameter_set_t *sps);




#endif

