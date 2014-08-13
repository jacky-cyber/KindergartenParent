

#ifndef _H264DEC_H_
#define _H264DEC_H_

#ifdef __cplusplus
extern "C" {
#endif


#define    ERR_NOERR         0
#define    ERR_FAILED        1


typedef     void*    h264_dec_handle_t;
typedef struct _h264_dec_param
{
	unsigned char   *nal;   /*[in]*/
	long            nalLen; /*[in]*/
	long            width;  /*[out]*/
	long            height; /*[out]*/
	long            stride; /*[out]*/	
    unsigned char   *img[3]; /*[out]*/
} h264_dec_param_t;


h264_dec_handle_t    h264_dec_open();
long                 h264_dec_decode(h264_dec_handle_t handle,h264_dec_param_t *param);
void                 h264_dec_close(h264_dec_handle_t handle);



#ifdef __cplusplus
}
#endif


#endif