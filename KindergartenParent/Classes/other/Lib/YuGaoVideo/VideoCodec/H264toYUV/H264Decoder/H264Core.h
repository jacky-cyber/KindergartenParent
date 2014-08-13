

#ifndef _H264CORE_H_
#define _H264CORE_H_




long  init_decoder(decoder_t *decoder);
long  decode_nalu(decoder_t *decoder,nalu_t *nalu);
long  free_decoder(decoder_t *decoder);




#endif