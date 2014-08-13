
//#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

#include "global.h"
#include "common.h"



__attribute__((aligned(16)))  const int16_t dequant_coef[6][4][4] = {
  {{10, 13, 10, 13},{ 13, 16, 13, 16},{10, 13, 10, 13},{ 13, 16, 13, 16}},
  {{11, 14, 11, 14},{ 14, 18, 14, 18},{11, 14, 11, 14},{ 14, 18, 14, 18}},
  {{13, 16, 13, 16},{ 16, 20, 16, 20},{13, 16, 13, 16},{ 16, 20, 16, 20}},
  {{14, 18, 14, 18},{ 18, 23, 18, 23},{14, 18, 14, 18},{ 18, 23, 18, 23}},
  {{16, 20, 16, 20},{ 20, 25, 20, 25},{16, 20, 16, 20},{ 20, 25, 20, 25}},
  {{18, 23, 18, 23},{ 23, 29, 23, 29},{18, 23, 18, 23},{ 23, 29, 23, 29}}
};


__attribute__((aligned(16))) const uchar QP_SCALE_CR[52] = 
{
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,
   12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
   28,29,29,30,31,32,32,33,34,34,35,35,36,36,37,37,
   37,38,38,38,39,39,39,39
};

/*
long check_cpu()
{
	long eax, ebx, ecx, edx;
 
	long cpu_flag = CPU_ASM;

	////cpu_cpuid(0,&eax,&ebx,&ecx,&edx);

	if( eax == 0 ){  
		return cpu_flag;
	}

	////cpu_cpuid(1,&eax,&ebx,&ecx,&edx);
	if( edx&0x00800000 ){
		cpu_flag |= CPU_MMX;
	} else {
		return cpu_flag;
	}

	if( eax&0x02000000 ){
		cpu_flag |= CPU_SSE;
	}

	if( (edx&0x04000000) ){
		cpu_flag |= CPU_SSE2;
	}
	return cpu_flag;
}
*/

void* align_malloc(long size)
{
    uchar  *buf = 0;
	uchar  *ally_buf = 0;

	buf = (uchar*)malloc(size + 15 + sizeof(void**) + sizeof(long));
    ally_buf = buf + 15 + sizeof(void**) + sizeof(long);
	ally_buf -= (ulong)ally_buf & 0xf;

	*((void**)(ally_buf-sizeof(void**))) = buf;
	*((long*)(ally_buf-sizeof(void**)-sizeof(long))) = size;

	memset(ally_buf,0,size);

	return ally_buf;
}

void  align_free(void *p)
{
	free( *((void**)p-1) );
}

void alloc_2d(uchar ***array2d,long width,long height,long pad)
{
	long i;
	long  pad2 = pad<<1;

	width += pad2;
	height += pad2;

	*array2d = align_malloc(sizeof(uchar**)*height);
	(*array2d)[0] = align_malloc(width * height*sizeof(uchar));

	(*array2d)[0] += pad;
	for( i=1;i<height;i++ ){
		(*array2d)[i] = (*array2d)[i-1] + width;
	}

	*array2d += pad;
}

void free_2d(uchar **array2d,long pad)
{
	if( array2d ){
		align_free(&array2d[-pad][-pad]);
		align_free(&array2d[-pad]);
	}
}

picture_t*  alloc_picture(long width,long height)
{
	picture_t *p = align_malloc(sizeof(picture_t));
    
	p->width = width;
	p->height = height;
	p->width_cr = width >> 1;
	p->height_cr = height >> 1;
	p->stride_luma = width + (PAD_SIZE*2);
	p->stride_chroma = p->width_cr + PAD_SIZE;

	p->front_mv = align_malloc(MAX_SIZE_IN_MBS * 16 * sizeof(mv_t) + 16);
	p->front_mv += 16;

	p->front_ref_idx = align_malloc(MAX_SIZE_IN_MBS * 16 * sizeof(long)+ 16);	
	memset(p->front_ref_idx,0xff,sizeof(long)*16);
	p->front_ref_idx += 16;


	alloc_2d(&p->img_y,width,height,PAD_SIZE);
	alloc_2d(&p->img_u,p->width_cr,p->height_cr,PAD_SIZE>>1);
	alloc_2d(&p->img_v,p->width_cr,p->height_cr,PAD_SIZE>>1);	

	return p;
}


void     free_picture(picture_t *p)
{
	if( p ){
		align_free(p->front_mv-16);	
		align_free(p->front_ref_idx-16);
		free_2d(p->img_y,PAD_SIZE);
		free_2d(p->img_u,PAD_SIZE>>1);
		free_2d(p->img_v,PAD_SIZE>>1);		
		align_free(p);
	}
}