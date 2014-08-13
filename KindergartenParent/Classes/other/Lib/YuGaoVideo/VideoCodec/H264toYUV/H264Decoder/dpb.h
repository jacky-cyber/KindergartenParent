

#ifndef _DPB_H_
#define _DPB_H_




void   init_dpb(decoder_t *decoder,dpb_t *dpb);
void   store_picture(decoder_t *decoder,dpb_t *dpb);
picture_t *get_unused_pic(decoder_t *decoder,dpb_t *dpb);
void   init_dpb_list(decoder_t *decoder,dpb_t *dpb,long slice_type);
void   reorder_dpb_list(decoder_t *decoder,dpb_t *dpb,slice_t *slice);
void   free_dpb(decoder_t *decoder,dpb_t *dpb);
void   output_picture(dpb_t *dpb,h264_dec_param_t *param);







#endif