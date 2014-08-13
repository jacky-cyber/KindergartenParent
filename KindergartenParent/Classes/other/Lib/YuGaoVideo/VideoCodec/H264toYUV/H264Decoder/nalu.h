

#ifndef  _NALU_H_
#define  _NALU_H_



long  get_nalu(nalu_t *nalu,uchar *buf,long len);
long  rbsp_to_sodb(nalu_t *nalu);


#endif