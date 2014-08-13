

#ifndef _LOOPFILTER_H_
#define _LOOPFILTER_H_




void deblock_picture(decoder_t *decoder);
void deblock_p_mb(decoder_t *decoder,loop_filter_t *loop_filter);
void deblock_intra_mb(decoder_t *decoder,loop_filter_t *loop_filter);
void edge_loop(decoder_t *decoder,loop_filter_t *loop_filter,uchar deblock_strength[2][4][4]);
void edge_loop_for_skip_mb(decoder_t *decoder, loop_filter_t *loop_filter, uchar deblock_strength[2][4][4]);
void filter_luma_ver_c(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
//void filter_luma_ver_mmx(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
void filter_luma_hor_c(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
//void filter_luma_hor_mmx(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge);
void filter_chroma_ver_c(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
//void filter_chroma_ver_mmx(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
void filter_chroma_hor_c(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);
//void filter_chroma_hor_mmx(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge);

/*
void filter_luma_ver_eq4_mmx(uchar *src,long alpha, long beta, long stride);
void filter_luma_ver_lt4_mmx(uchar *src,long alpha, long beta, long c0, long stride);
void filter_luma_hor_eq4_mmx(uchar *src,long alpha, long beta, long stride);
void filter_luma_hor_lt4_mmx(uchar *src,long alpha, long beta, long c0, long stride);
void filter_chroma_ver_eq4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long stride);
void filter_chroma_ver_lt4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long c0, long stride);
void filter_chroma_hor_eq4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long stride);
void filter_chroma_hor_lt4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long c0, long stride);
*/




#endif