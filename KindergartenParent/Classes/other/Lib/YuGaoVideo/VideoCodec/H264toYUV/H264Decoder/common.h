
#ifndef _COMMON_H_
#define _COMMON_H_



#define SWAP32(x) (((x & 0xff) << 24) | ((x & 0xff00) << 8) \
                    | ((x & 0xff0000) >> 8) | ((x & 0xff000000) >> 24))
#define SWAP16(x) (((x & 0xff) << 8) | ((x & 0xff00) >> 8))

///*
static inline ulong  BSWAP(ulong a)
{
    /*
    __asm {
		mov eax , a
		bswap eax
		mov a , eax
	}
      */
    
    a = SWAP32(a);   ////----
    
    return a;
}
//*/

static inline long  is_zero_motion_vector(mv_t *mv)
{
	return (mv->x==0) && (mv->y==0);
}

// cpu
extern long       cpu_cpuid(long op, long *eax, long *ebx, long *ecx, long *edx);
////long              check_cpu();


// memory
void*                align_malloc(long size);
void                 align_free(void *p);

void                 alloc_2d(uchar ***array2d,long width,long height,long pad);
void                 free_2d(uchar **array2d,long pad);


picture_t*           alloc_picture(long  width,long height);
void                 free_picture(picture_t *p);


#define            CLIP3(min,max,val) (((val)<(min))?(min):(((val)>(max))?(max):(val)))
#define            CLIP1(val)        CLIP3(0,255,val)


extern const int16_t dequant_coef[6][4][4];
extern const uchar QP_SCALE_CR[52];



#endif
