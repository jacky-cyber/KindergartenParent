
#include <memory.h>

#include "global.h"
#include "common.h"
#include "recon.h"




void  recon_mb_p_c(decoder_t *decoder)
{
	picture_t *pic = decoder->cur_pic;
	long  i,j;
	long  x, y;
	long  x_cr,y_cr;

	x = decoder->mb_x << 4;
	y = decoder->mb_y << 4;
	x_cr = x >> 1;
	y_cr = y >> 1;

	for( j=0;j<16;j++ ){
		for( i=0;i<16;i++ ){
			pic->img_y[y+j][x+i] = CLIP1(decoder->cof[j][i] + decoder->mpr[j][i]);
		}
	}      

	for( j=0;j<8;j++ ){
		for( i=0;i<8;i++ ){
			pic->img_u[y_cr+j][x_cr+i] = CLIP1(decoder->cof[16+j][i] + decoder->mpr[16+j][i]);
		}
	}

	for( j=0;j<8;j++ ){
		for( i=0;i<8;i++ ){
			pic->img_v[y_cr+j][x_cr+i] = CLIP1(decoder->cof[16+j][i+8] + decoder->mpr[16+j][i+8]);
		}
	}

	memset(decoder->cof,0,sizeof(decoder->cof));
}


void expand_pic(picture_t *pic)
{
	uchar **img_y = pic->img_y;
	long stride = pic->stride_luma;
	uchar  *p = &img_y[0][0];
	uchar  *p1;
	long  i;

	
	p -= 24;
	for( i=0;i<pic->height;i++ ){
		memset(p,p[24],24);
		memset(p+24+pic->width,p[23+pic->width],24);
		p += stride;
	}
	p1 = p - stride;
	for( i=0;i<PAD_SIZE/2+4;i++ ){
		memcpy(p,p1,pic->width+48);
		p += stride;
	}
    p = &img_y[0][0];
	p -= 24;
	p1 = p;
	p -= stride;
	for( i=0;i<PAD_SIZE/2+4;i++ ){
		memcpy(p,p1,pic->width+48);
		p -= stride;
	}

	stride = pic->stride_chroma;
	p = &pic->img_u[0][0];
	p -= 16;
	for( i=0;i<pic->height_cr;i++ ){
		memset(p,p[16],16);
		memset(p+pic->width_cr+16,p[15+pic->width_cr],16);
        p += stride;		
	}
	p1 = p - stride;
	for( i=0;i<PAD_SIZE/4+1;i++ ){
		memcpy(p,p1,pic->width_cr+32);
		p += stride;
	}
	p = &pic->img_u[0][0];
	p -= 16;
	p1 = p;
	p -= stride;
	for( i=0;i<PAD_SIZE/4+1;i++ ){
		memcpy(p,p1,pic->width_cr+32);
		p -= stride;
	}

	p = &pic->img_v[0][0];
	p -= 16;
	for( i=0;i<pic->height_cr;i++ ){
		memset(p,p[16],16);
		memset(p+pic->width_cr+16,p[15+pic->width_cr],16);
        p += stride;		
	}
	p1 = p - stride;
	for( i=0;i<PAD_SIZE/4+1;i++ ){
		memcpy(p,p1,pic->width_cr+32);
		p += stride;
	}
	p = &pic->img_v[0][0];
	p -= 16;
	p1 = p;
	p -= stride;
	for( i=0;i<PAD_SIZE/4+1;i++ ){
		memcpy(p,p1,pic->width_cr+32);
		p -= stride;
	}
}

void recon_chroma_for_mb_4x4_c(decoder_t *decoder)
{
	picture_t *pic = decoder->cur_pic;
	long  i,j;
	long  x, y;
	long  x_cr,y_cr;

	x = decoder->mb_x << 4;
	y = decoder->mb_y << 4;
	x_cr = x >> 1;
	y_cr = y >> 1; 

	for( j=0;j<8;j++ ){
		for( i=0;i<8;i++ ){
			pic->img_u[y_cr+j][x_cr+i] = CLIP1(decoder->cof[16+j][i] + decoder->mpr[16+j][i]);
			decoder->cof[16+j][i] = 0;
		}
	}

	for( j=0;j<8;j++ ){
		for( i=0;i<8;i++ ){
			pic->img_v[y_cr+j][x_cr+i] = CLIP1(decoder->cof[16+j][i+8] + decoder->mpr[16+j][i+8]);
			decoder->cof[16+j][i+8] = 0;
		}
	}
}

/*
void recon_mb_p_sse2(decoder_t *decoder)
{
	picture_t *pic = decoder->cur_pic;
	long x = decoder->mb_x << 4;
	long y = decoder->mb_y << 4;
	long x_cr = x >> 1;
	long y_cr = y >> 1;
	uchar *img_y = &pic->img_y[y][x];
	uchar *img_u = &pic->img_u[y_cr][x_cr];
	uchar *img_v = &pic->img_v[y_cr][x_cr];
	long stride_lum = pic->stride_luma;
	long stride_chroma = pic->stride_chroma;
	int16_t *mpr = &decoder->mpr[0][0];
	int16_t *cof = &decoder->cof[0][0];

	__asm {
        mov  eax , dword ptr [mpr]
		mov  esi , dword ptr [cof]
		mov  edi , dword ptr [img_y]
		mov  ebx , stride_lum
		mov  ecx , 8
RECON_LUMA_LOOP:     		
		movdqa  xmm0 , xmmword ptr [eax]
		movdqa  xmm1 , xmmword ptr [esi]		
		movdqa  xmm2 , xmmword ptr [eax+16]
		movdqa  xmm3 , xmmword ptr [esi+16]
		paddw   xmm0 , xmm1
		paddw   xmm2 , xmm3
		movdqa  xmm4 , xmmword ptr [eax+32]
		movdqa  xmm5 , xmmword ptr [esi+32]
		movdqa  xmm6 , xmmword ptr [eax+48]
		movdqa  xmm7 , xmmword ptr [esi+48]
		pxor    xmm1 , xmm1
		paddw   xmm4 , xmm5
		paddw   xmm6 , xmm7
		movdqa  xmmword ptr [esi] , xmm1	
		movdqa  xmmword ptr [esi+16] , xmm1
		packuswb  xmm0 , xmm2
		movdqa  xmmword ptr [esi+32] , xmm1		
		movdqa  xmmword ptr [esi+48] , xmm1
		packuswb  xmm4 , xmm6			
		movdqa  xmmword ptr [edi] , xmm0		
		movdqa  xmmword ptr [edi+ebx] , xmm4			

		add  eax , 64
		add  esi , 64 
		add  edi , ebx
		add  edi , ebx 
		dec  ecx
		jnz  RECON_LUMA_LOOP

		mov  edi , dword ptr [img_u]
		mov  edx , dword ptr [img_v]
		mov  ebx , stride_chroma
		mov  ecx , 4
RECON_CHROMA_LOOP:
		movdqa  xmm0 , xmmword ptr [eax]
		movdqa  xmm1 , xmmword ptr [esi]
		movdqa  xmm2 , xmmword ptr [eax+16]
		movdqa  xmm3 , xmmword ptr [esi+16]
		paddw   xmm0 , xmm1
		paddw   xmm2 , xmm3
		movdqa  xmm4 , xmmword ptr [eax+32]
		movdqa  xmm5 , xmmword ptr [esi+32]
		movdqa  xmm6 , xmmword ptr [eax+48]
		movdqa  xmm7 , xmmword ptr [esi+48]
		paddw   xmm4 , xmm5
		paddw   xmm6 , xmm7
		packuswb  xmm0 , xmm2
		packuswb  xmm4 , xmm6
		pxor    xmm1 , xmm1
		movq    qword ptr [edi] , xmm0
		movq    qword ptr [edi+ebx] , xmm4
		psrldq  xmm0 , 8
		psrldq  xmm4 , 8
        movdqa  xmmword ptr [esi] , xmm1
		movq    qword ptr [edx] , xmm0
		movdqa  xmmword ptr [esi+16] , xmm1
		movq    qword ptr [edx+ebx] , xmm4
		movdqa  xmmword ptr [esi+32] , xmm1
		movdqa  xmmword ptr [esi+48] , xmm1

		add   eax , 64
		add   esi , 64
		add   edi , ebx
		add   edx , ebx
		add   edi , ebx
		add   edx , ebx
		dec   ecx
		jnz   RECON_CHROMA_LOOP
	}
}



void recon_chroma_for_mb_4x4_sse2(decoder_t *decoder)
{    
	picture_t *pic = decoder->cur_pic;
	long  x, y;
	long  x_cr,y_cr;
	uchar  *img_u,*img_v;
	long  stride_chroma = pic->stride_chroma;
	int16_t *mpr = &decoder->mpr[16][0];
	int16_t *cof = &decoder->cof[16][0];

	x = decoder->mb_x << 4;
	y = decoder->mb_y << 4;
	x_cr = x >> 1;
	y_cr = y >> 1; 

	img_u = &pic->img_u[y_cr][x_cr];
	img_v = &pic->img_v[y_cr][x_cr];
	
	__asm {
		mov   esi , dword ptr [cof]
		mov   eax , dword ptr [mpr]
		mov   edi , dword ptr [img_u]
		mov   edx , dword ptr [img_v]
		mov   ebx , stride_chroma
		mov   ecx , 4
RECON_CHROMA_FOR_4x4_MB_LOOP:
		movdqa   xmm0 , xmmword ptr [eax]
		movdqa   xmm1 , xmmword ptr [esi]
		movdqa   xmm2 , xmmword ptr [eax+16]
		movdqa   xmm3 , xmmword ptr [esi+16]
		paddw    xmm0 , xmm1
		paddw    xmm2 , xmm3
		movdqa   xmm4 , xmmword ptr [eax+32]
		movdqa   xmm5 , xmmword ptr [esi+32]
		movdqa   xmm6 , xmmword ptr [eax+48]
		movdqa   xmm7 , xmmword ptr [esi+48]
		paddw    xmm4 , xmm5
		paddw    xmm6 , xmm7
		packuswb xmm0 , xmm2
		packuswb xmm4 , xmm6
		pxor     xmm1 , xmm1
		movq     qword ptr [edi] , xmm0
		movq     qword ptr [edi+ebx] , xmm4
		psrldq   xmm0 , 8
		psrldq   xmm4 , 8
		movdqa   xmmword ptr [esi] , xmm1
		movq     qword ptr [edx] , xmm0
		movdqa   xmmword ptr [esi+16] , xmm1
		movq     qword ptr [edx+ebx] , xmm4
		movdqa   xmmword ptr [esi+32] , xmm1
		movdqa   xmmword ptr [esi+48] , xmm1        

		add   eax , 64
		add   esi , 64
		add   edx , ebx
		add   edi , ebx
		add   edx , ebx
		add   edi , ebx
		dec   ecx 
		jnz   RECON_CHROMA_FOR_4x4_MB_LOOP
	}
}
*/
