
#include <assert.h>

#include "global.h"
#include "common.h"
#include "nalu.h"


//byte stream format  ,  ISO/IEC 14496-10 Annex B
long get_nalu(nalu_t *nalu,uchar *buf,long len)
{
    long pos = 0;
	long start_code_found = 0;
	long i = 0;
	long header_len = 0;
	long counter = 0;

	while ( (pos<len) && (buf[pos++]==0) );

	if( (pos>=len) || (pos<3) ){
		return -1;
	}

	assert(buf[pos-1]==1);  //start code of NALU
	
	header_len = pos;
	nalu->buf = buf + pos;
	assert((nalu->buf[0]&0x80)==0);  // forbidded_bit , must be zero
	nalu->nalu_ref_idc = (nalu->buf[0]>>5)&0x3;
	nalu->nal_unit_type = nalu->buf[0]&0x1f;

	i = pos;
	while( !start_code_found ){
		if(pos>=len){
			nalu->len = i - header_len;
			return pos;
		}
		if( (counter==ZEROBYTES_SHORTSTARTCODE) && (buf[pos]==0x03) ){
			pos++;
			counter = 0;
		}
		buf[i] = buf[pos];
		if( (counter>=ZEROBYTES_SHORTSTARTCODE) && (buf[pos]==0x1)) {
			start_code_found = 1;
		}
		if( !buf[pos] ){
			counter ++;
		} else {
			counter = 0;
		}
		pos ++;
		i ++;
	}

	buf[i-3] = 0;
	buf[i-2] = 0;
	buf[i-1] = 0;
	nalu->len = i - 3;	
 
	return pos-3;
}


long rbsp_to_sodb(nalu_t *nalu)
{
	long last_byte_pos = nalu->len;
	while( !nalu->buf[last_byte_pos-1] ){
		last_byte_pos --;
	}
	nalu->len = last_byte_pos;
	return last_byte_pos;
}