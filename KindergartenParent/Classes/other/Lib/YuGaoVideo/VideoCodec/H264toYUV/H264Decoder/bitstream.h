

#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_


#define GetWord(bit_buf,bit_ptr,bits) { \
	bit_buf |= ((bit_ptr[0] << 8) | bit_ptr[1]) << (bits);	\
	bits -= 16; \
	bit_ptr +=2; \
}

#define NeedBits(bit_buf,bit_ptr,bits) { \
	if( bits > 0 ) { \
	    GetWord(bit_buf,bit_ptr,bits); \
	} \
}

#define UBITS(bit_buf,numbits) (bit_buf>>(32-(numbits)))

#define DUMPBITS(bit_buf,bit_ptr,bits,numbits) { \
	bit_buf <<= (numbits); \
	bits += (numbits); \
	NeedBits(bit_buf,bit_ptr,bits); \
}


static inline void bs_init(bs_t *bs,void *p_date,long len)
{
	ulong tmp;

	bs->p_buffer = p_date;
	bs->length = len;
	tmp = *((ulong*)p_date);			
	
	bs->bit_buf = BSWAP(tmp);
	bs->bit_ptr = bs->p_buffer + sizeof(long);
	bs->bits = -16;
}


static inline long show_bits(bs_t *bs,long numbits)
{
	return UBITS(bs->bit_buf,numbits);
}

static inline long get_bits(bs_t *bs,long numbits)
{
	long rc = UBITS(bs->bit_buf,numbits);
	DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,numbits);
	return rc;
}

static inline void flush_bits(bs_t *bs,long numbits)
{
	DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,numbits);
}

static inline void ally_bits(bs_t *bs)
{	
	long bitsToGo;
	assert(bs->bits<0);
	bitsToGo = bs->bits%8;
	DUMPBITS(bs->bit_buf,bs->bit_ptr,bs->bits,bitsToGo);
}




#endif