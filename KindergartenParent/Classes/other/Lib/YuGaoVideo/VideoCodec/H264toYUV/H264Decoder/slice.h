

#ifndef _SLICE_H_
#define _SLICE_H_


void     init_picture(decoder_t *decoder);
void     exit_picture(decoder_t *decoder);
long     decode_slice(decoder_t *decoder,bs_t *bs);
void     free_slice(slice_t *slice);





#endif