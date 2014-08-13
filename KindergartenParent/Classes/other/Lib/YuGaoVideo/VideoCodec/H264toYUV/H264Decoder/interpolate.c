
#include <stdlib.h>
#include <assert.h>

#include "global.h"
#include "common.h"
#include "interpolate.h"
#include "access.h"


__attribute__((aligned(16))) static const long  COEF[6] = { 1  , -5  , 20 , 20 , -5 , 1 };

__attribute__((aligned(16))) static const int16_t five_128[8] = { 5 , 5 , 5 , 5 , 5 , 5 , 5 , 5 };

__attribute__((aligned(16))) static const int16_t sixteen_128[8] = { 16,16,16,16,16,16,16,16 };

__attribute__((aligned(16))) static const int16_t thirty_two_128[8] = { 32,32,32,32,32,32,32,32 };

__attribute__((aligned(16))) static const long long  thirty_two_64 = 0x0020002000200020;

__attribute__((aligned(16))) static const long long  sixteen_64 = 0x0010001000100010;


/*
static void  expand8to16_16x16_sse2(int16_t *dst,uchar *src,long stride)
{	
	__asm {
		mov   esi , dword ptr [src]
		mov   edi , dword ptr [dst]
		mov   eax , stride
		mov   ecx , 8
		pxor  xmm6 , xmm6
		pxor  xmm7 , xmm7
EXPAND8TO16_16x16_LOOP:
		movdqu   xmm0 , xmmword ptr [esi]
		movdqu   xmm1 , xmmword ptr [esi+eax]
		movdqa   xmm2 , xmm0
		movdqa   xmm3 , xmm1
		punpcklbw  xmm0 , xmm7
		punpckhbw  xmm2 , xmm6
		punpckhbw  xmm3 , xmm7
		punpcklbw  xmm1 , xmm6
        movdqa   xmmword ptr [edi] , xmm0
		movdqa   xmmword ptr [edi+16] , xmm2
		movdqa   xmmword ptr [edi+32] , xmm1
		movdqa   xmmword ptr [edi+48] , xmm3

		add    esi , eax
		add    edi , 64
		add    esi , eax
		dec    ecx 
		jnz    EXPAND8TO16_16x16_LOOP
	}
}

static void interpolate_halfpel_h_16x16_sse2(int16_t *dst , uchar *src, long stride)
{
	__asm {	
		mov   esi , dword ptr [src]
		mov   edi , dword ptr [dst]
		mov   eax , stride
		sub   esi , 2
		mov   ecx , 16
		pxor  xmm0 , xmm0
INTERPOLATE_HALFPEL_H_16x16_LOOP:
        movdqu  xmm1 , xmmword ptr [esi]
		movdqu  xmm2 , xmmword ptr [esi+8]       
		movdqa  xmm3 , xmm1
		movdqa  xmm4 , xmm1
		psrldq  xmm3 , 2
		psrldq  xmm4 , 3	
		movdqa  xmm5 , xmm2
		movdqa  xmm6 , xmm2
		psrldq  xmm5 , 2
		psrldq  xmm6 , 3
		punpcklbw  xmm3 , xmm0
		punpcklbw  xmm4 , xmm0
		movdqa   xmm7 , xmmword ptr [five_128];
		punpcklbw  xmm5 , xmm0
		punpcklbw  xmm6 , xmm0
		paddw   xmm3 , xmm4
		movdqa  xmm4 , xmm1
		paddw   xmm5 , xmm6
		movdqa  xmm6 , xmm2
		paddw   xmm3 , xmm3
		paddw   xmm5 , xmm5
		psrldq  xmm4 , 1
		psrldq  xmm6 , 1
        paddw   xmm3 , xmm3
		paddw   xmm5 , xmm5
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm6 , xmm0
		psubw   xmm3 , xmm4
		psubw   xmm5 , xmm6
		pshufd  xmm4 , xmm1 , 9
		pshufd  xmm6 , xmm2 , 9		
		punpcklbw xmm4 , xmm0
		punpcklbw xmm6 , xmm0
		psubw   xmm3 , xmm4
		psubw   xmm5 , xmm6
		movdqa  xmm4 , xmm1
		movdqa  xmm6 , xmm2
        pmullw  xmm3 , xmm7
		psrldq  xmm4 , 5
		pmullw  xmm5 , xmm7
		psrldq  xmm6 , 5
        punpcklbw  xmm1 , xmm0
		punpcklbw  xmm2 , xmm0
        movdqa  xmm7 , xmmword ptr [sixteen_128]
        punpcklbw  xmm4 , xmm0
	    punpcklbw  xmm6 , xmm0
        paddw   xmm3 , xmm1
		paddw   xmm5 , xmm2
		paddw   xmm3 , xmm4
		paddw   xmm5 , xmm6
		paddw   xmm3 , xmm7
		paddw   xmm5 , xmm7
		psraw   xmm3 , 5
		psraw   xmm5 , 5
		packuswb  xmm3 , xmm5
		movdqa    xmm1 , xmm3
        punpcklbw  xmm3 , xmm0
		punpckhbw  xmm1 , xmm0
		movdqa xmmword ptr [edi] , xmm3
		movdqa xmmword ptr [edi+16] , xmm1

		add     edi , 32
		add     esi , eax
		dec     ecx 
        jnz     INTERPOLATE_HALFPEL_H_16x16_LOOP
	}
}


static void interpolate_avg_16x16_sse2(int16_t *dst , uchar *src , long stride)
{
	__asm {
		mov  esi , dword ptr [src]
		mov  edi , dword ptr [dst]
		mov  eax , stride
		mov  ecx , 8
		pxor  xmm0 , xmm0

INTERPOLATE_AVG_16x16_LOOP:
		movdqu   xmm1 , xmmword ptr [esi]
		movdqa   xmm2 , xmmword ptr [edi]
		movdqa   xmm3 , xmmword ptr [edi+16]
		movdqu   xmm5 , xmmword ptr [esi+eax]
		movdqa   xmm6 , xmmword ptr [edi+32]
		movdqa   xmm7 , xmmword ptr [edi+48]
		movdqa   xmm4 , xmm1
		punpcklbw  xmm1 , xmm0 
		punpckhbw  xmm4 , xmm0		
        pavgw    xmm1 , xmm2
		pavgw    xmm4 , xmm3
		movdqa   xmm2 , xmm5
		punpcklbw  xmm5 , xmm0
		punpckhbw  xmm2 , xmm0
		pavgw    xmm5 , xmm6
		pavgw    xmm2 , xmm7
		movdqa   xmmword ptr [edi] , xmm1
		movdqa   xmmword ptr [edi+16] , xmm4
		movdqa   xmmword ptr [edi+32] , xmm5
		movdqa   xmmword ptr [edi+48] , xmm2

		add      esi , eax
		add      edi , 64
		add      esi , eax
		dec      ecx
		jnz      INTERPOLATE_AVG_16x16_LOOP
	}
}

static void interpolate_halfpel_v_16x16_sse2(int16_t *dst,uchar *src,long stride)
{	
	__asm {
		mov   eax , stride
		mov   ebx , eax
		mov   esi , dword ptr [src]
		mov   edi , dword ptr [dst]		
		shl   ebx , 1
		mov   edx , eax
		mov   ecx , 16
		add   edx , ebx
		pxor  xmm0 , xmm0

INTERPLOATE_HALFPEL_V_16x16_LOOP:
		neg   ebx
		neg   eax
		movdqu  xmm1 , xmmword ptr [esi+ebx]
		movdqu  xmm2 , xmmword ptr [esi+eax]
		neg   ebx 
		neg   eax
		movdqu  xmm3 , xmmword ptr [esi]
		movdqa  xmm4 , xmm1
		punpcklbw  xmm1 , xmm0
        movdqa  xmm5 , xmm2 
		punpckhbw  xmm4 , xmm0
		movdqa  xmm6 , xmm3
		punpcklbw  xmm3 , xmm0
		punpcklbw  xmm2 , xmm0
		psllw   xmm3 , 2
		punpckhbw  xmm6 , xmm0		
		punpckhbw  xmm5 , xmm0
		psubw   xmm3 , xmm2
		psllw   xmm6 , 2
		movdqa  xmm7 , xmm3
		psllw   xmm3 , 2
		psubw   xmm6 , xmm5
		paddw   xmm3 , xmm7
		movdqa  xmm7 , xmm6
		psllw   xmm6 , 2
		paddw   xmm1 , xmm3
		paddw   xmm6 , xmm7

		movdqu  xmm2 , xmmword ptr [esi+edx]
		paddw   xmm4 , xmm6
		movdqa  xmm7 , xmm2
		punpcklbw xmm2 , xmm0
		punpckhbw xmm7 , xmm0
		paddw   xmm1 , xmm2
		paddw   xmm4 , xmm7
		movdqu  xmm2 , xmmword ptr [esi+ebx]
		movdqu  xmm3 , xmmword ptr [esi+eax]
        movdqa  xmm5 , xmm2
		movdqa  xmm6 , xmm3
		punpcklbw xmm2 , xmm0
		punpcklbw xmm3 , xmm0
		punpckhbw xmm6 , xmm0		
		psllw     xmm3 , 2
		punpckhbw xmm5 , xmm0
		psllw     xmm6 , 2
		psubw     xmm3 , xmm2			
		movdqa    xmm7 , xmm3 
        psubw     xmm6 , xmm5
		psllw     xmm3 , 2		
        paddw     xmm3 , xmm7   
		movdqa    xmm7 , xmm6
		paddw     xmm1 , xmm3
		psllw     xmm6 , 2
		movdqa    xmm5 , xmmword ptr [sixteen_128]
		paddw     xmm6 , xmm7
		paddw     xmm4 , xmm6
		paddw     xmm1 , xmm5
		paddw     xmm4 , xmm5
		psraw     xmm1 , 5
		psraw     xmm4 , 5
		packuswb  xmm1 , xmm4
		movdqa    xmm6 , xmm1
		punpcklbw xmm1 , xmm0
		punpckhbw xmm6 , xmm0
		movdqa    xmmword ptr [edi] , xmm1
		movdqa    xmmword ptr [edi+16] , xmm6

		add  esi , eax
		add  edi , 32
		dec  ecx
		jnz  INTERPLOATE_HALFPEL_V_16x16_LOOP
	}
}


static void interpolate_halfpel_hv_h_16x16_sse2(int16_t *dst,uchar *src,long stride,int16_t *mpr)
{
	__asm {		
		mov esi, dword ptr[src]
		mov edi, dword ptr[dst]
		sub esi, 2
		mov ecx, 21
		mov ebx, stride		
INTERPOLATE_HALFPEL_HV_LOOP_1:
		movdqu	xmm1, xmmword ptr[esi]
		movdqu	xmm2, xmmword ptr[esi+8]
		pxor	xmm0, xmm0
		movdqa	xmm3, xmm1
		movdqa	xmm4, xmm1
		psrldq	xmm3, 2
		psrldq	xmm4, 3
		movdqa	xmm5, xmm2
		movdqa	xmm6, xmm2
		psrldq	xmm5, 2
		psrldq	xmm6, 3
		punpcklbw xmm3,	xmm0
		punpcklbw xmm4,	xmm0
		movdqa	xmm7, xmmword ptr[five_128]
		punpcklbw xmm5,	xmm0
		punpcklbw xmm6,	xmm0
		paddw	xmm3, xmm4
		movdqa	xmm4, xmm1
		paddw	xmm5, xmm6
		movdqa	xmm6, xmm2
		paddw	xmm3, xmm3
		paddw	xmm5, xmm5
		psrldq	xmm4, 1
		psrldq	xmm6, 1
		paddw	xmm3, xmm3
		paddw	xmm5, xmm5
		punpcklbw xmm4,	xmm0
		punpcklbw xmm6,	xmm0
		psubw	xmm3, xmm4
		psubw	xmm5, xmm6
		pshufd	xmm4, xmm1, 9
		pshufd	xmm6, xmm2, 9
		punpcklbw xmm4,	xmm0
		punpcklbw xmm6,	xmm0
		psubw	xmm3, xmm4
		movdqa	xmm4, xmm1
		psubw	xmm5, xmm6
		movdqa	xmm6, xmm2
		pmullw	xmm3, xmm7
		psrldq	xmm4, 5
		pmullw	xmm5, xmm7
		psrldq	xmm6, 5
		punpcklbw xmm1,	xmm0
		punpcklbw xmm2,	xmm0
		paddw	xmm1, xmm3
		paddw	xmm2, xmm5
		punpcklbw xmm4,	xmm0
		punpcklbw xmm6,	xmm0
		paddw	xmm1, xmm4
		paddw	xmm2, xmm6
		movdqa xmmword ptr[edi], xmm1
		movdqa xmmword ptr[edi + 16], xmm2
		add edi, 32
		add esi, ebx
		dec ecx
		jnz INTERPOLATE_HALFPEL_HV_LOOP_1	
		

		mov esi, dword ptr[dst]
		mov edi, dword ptr[mpr]
	    mov ecx, 8
INTERPOLATE_HALFPEL_HV_LOOP_2:
		movdqa	xmm0, xmmword ptr[esi]
		movdqa	xmm5, xmmword ptr[esi+0A0h]
		paddw	xmm0, xmm5
		movdqa	xmm1, xmmword ptr[esi+20h]
		movdqa	xmm3, xmmword ptr[esi+40h]
		movdqa	xmm4, xmm3
		paddw	xmm3, xmm5
		movdqa	xmm2, xmmword ptr[esi+80h]
		movdqa	xmm5, xmmword ptr[esi+0C0h]
		movdqa	xmm6, xmm2
		paddw	xmm2, xmm1
		paddw	xmm1, xmm5
		movdqa	xmm5, xmmword ptr[esi+60h]
		paddw	xmm4, xmm5
		paddw	xmm5, xmm6
		psubw	xmm0, xmm2
		psubw	xmm1, xmm3
		psraw	xmm0, 2
		psraw	xmm1, 2
		psubw	xmm2, xmm4
		psubw	xmm3, xmm5
		psubsw	xmm0, xmm2
		psubsw	xmm1, xmm3
		psraw	xmm0, 2
		psraw	xmm1, 2
		paddw	xmm0, xmm4
		paddw	xmm1, xmm5
		paddw xmm0, xmmword ptr[thirty_two_128]
		paddw xmm1, xmmword ptr[thirty_two_128]
		psraw	xmm0, 6
		psraw	xmm1, 6
		packuswb xmm0, xmm1
		movdqa	xmm1, xmmword ptr[esi+10h]
		movdqa	xmm5, xmmword ptr[esi+0B0h]
		paddw	xmm1, xmm5
		movdqa	xmm7, xmmword ptr[esi+30h]
		movdqa	xmm3, xmmword ptr[esi+50h]
		movdqa	xmm4, xmm3
		paddw	xmm3, xmm5
		movdqa	xmm2, xmmword ptr[esi+90h]
		movdqa	xmm5, xmmword ptr[esi+0D0h]
		movdqa	xmm6, xmm2
		paddw	xmm2, xmm7
		paddw	xmm7, xmm5
		movdqu	xmm5, xmmword ptr[esi+70h]
		paddw	xmm4, xmm5
		paddw	xmm5, xmm6
		psubw	xmm1, xmm2
		psubw	xmm7, xmm3
		psraw	xmm1, 2
		psraw	xmm7, 2
		psubw	xmm2, xmm4
		psubw	xmm3, xmm5
		psubsw	xmm1, xmm2
		psubsw	xmm7, xmm3
		psraw	xmm1, 2
		psraw	xmm7, 2
		paddw	xmm1, xmm4
		paddw	xmm7, xmm5
		paddw xmm1, xmmword ptr[thirty_two_128]
		paddw xmm7, xmmword ptr[thirty_two_128]
		psraw	xmm1, 6
		psraw	xmm7, 6
		packuswb xmm1, xmm7
		
		movdqa	xmm2, xmm0
		punpcklqdq xmm0, xmm1
		punpckhqdq xmm2, xmm1
		pxor xmm5, xmm5
		movdqa xmm3, xmm0
		movdqa xmm4, xmm2
		punpcklbw xmm0, xmm5
		punpckhbw xmm3, xmm5
		punpcklbw xmm2, xmm5
		punpckhbw xmm4, xmm5

		movdqa xmmword ptr[edi], xmm0
		movdqa xmmword ptr[edi + 16], xmm3
		movdqa xmmword ptr[edi + 32], xmm2
		movdqa xmmword ptr[edi + 48], xmm4
		add edi, 40h
		add	esi, 40h
		dec ecx
		jnz INTERPOLATE_HALFPEL_HV_LOOP_2
	}
}

static void interpolate_hv_avg_16x16_sse2(int16_t *dst, int16_t *src, long stride)
{
	__asm {
		mov esi, dword ptr[src]
		mov edi, dword ptr[dst]
		mov ecx, 16
		mov edx, stride
		shl edx, 1		
		movdqa  xmm7 , xmmword ptr [sixteen_128]
		pxor xmm0, xmm0		
INTERPOLATE_HV_AVG_16x16_LOOP:
		movdqa xmm1, xmmword ptr[edi]
		movdqa xmm2, xmmword ptr[edi + 16]
		movdqu xmm3, xmmword ptr[esi]
		movdqu xmm4, xmmword ptr[esi + 16]
		paddw xmm3, xmm7
		paddw xmm4, xmm7
		psraw xmm3, 5
		psraw xmm4, 5
		packuswb xmm3, xmm4
		movdqa xmm5, xmm3
		punpcklbw xmm3, xmm0
		punpckhbw xmm5, xmm0
		pavgw xmm1, xmm3
		pavgw xmm2, xmm5
		movdqa xmmword ptr[edi], xmm1
		movdqa xmmword ptr[edi + 16], xmm2
		add esi, edx
		add edi, 32
		dec ecx
		jnz INTERPOLATE_HV_AVG_16x16_LOOP		
	}
}


static void interpolate_halfpel_hv_v_16x16_sse2(int16_t *dst , uchar *src, long stride, int16_t *mpr)
{
	__asm {
		mov esi, dword ptr[src]
		mov edi, dword ptr[dst]
		mov eax, stride
		mov ecx, 16
		mov ebx, eax
		shl ebx, 1
		mov edx, eax
		add edx, ebx
		pxor xmm0, xmm0
INTERPOLATE_HALFPEL_HV_V_16x16_LOOP_1:
		neg ebx
		neg eax
		movdqu xmm1, xmmword ptr[esi + ebx]
		movdqu xmm2, xmmword ptr[esi + eax]
		neg ebx
		neg eax
		movdqu xmm3, xmmword ptr[esi]
		movdqa xmm4, xmm1
		punpcklbw xmm1, xmm0
		movdqa xmm5, xmm2
		punpcklbw xmm2, xmm0
		movdqa xmm6, xmm3
		punpcklbw xmm3, xmm0
		punpckhbw xmm4, xmm0
		punpckhbw xmm5, xmm0
		psllw xmm3, 2 
		punpckhbw xmm6, xmm0
		psubw xmm3, xmm2
		psllw xmm6, 2
		movdqa xmm7, xmm3
		psllw xmm3, 2
		psubw xmm6, xmm5
		paddw xmm3, xmm7
		movdqa xmm7, xmm6
		psllw xmm6, 2
		paddw xmm1, xmm3
		paddw xmm6, xmm7
			
		movdqu xmm2, xmmword ptr[esi + edx]
		paddw xmm4, xmm6
		movdqa xmm7, xmm2
		punpcklbw xmm2, xmm0
		punpckhbw xmm7, xmm0
		paddw xmm1, xmm2
		paddw xmm4, xmm7
		
		movdqu xmm2, xmmword ptr[esi + ebx]
		movdqu xmm3, xmmword ptr[esi + eax]
		
		
		movdqa xmm5, xmm2
		punpcklbw xmm2, xmm0
		movdqa xmm6, xmm3
		punpcklbw xmm3, xmm0
		punpckhbw xmm5, xmm0
		psllw xmm3, 2 
		punpckhbw xmm6, xmm0
		psubw xmm3, xmm2
		psllw xmm6, 2
		movdqa xmm7, xmm3
		psllw xmm3, 2
		psubw xmm6, xmm5
		paddw xmm3, xmm7
		movdqa xmm7, xmm6
		psllw xmm6, 2
		paddw xmm1, xmm3
		paddw xmm6, xmm7
		paddw xmm4, xmm6
		movdqa xmmword ptr[edi], xmm1
		movdqa xmmword ptr[edi + 16], xmm4
        
		sub esi, ebx
		lea	edx, [esi]
		add edx, 16
		movq	xmm5, qword ptr	[edx]
		movq	xmm1, qword ptr	[edx+eax]
		lea	edx, [edx+ebx]
		punpcklbw xmm5,	xmm0
		movq	xmm2, qword ptr	[edx]
		punpcklbw xmm1,	xmm0
		movq	xmm3, qword ptr	[edx+eax]
		punpcklbw xmm2,	xmm0
		lea	edx, [edx+ebx]
		punpcklbw xmm3,	xmm0
		movq	xmm4, qword ptr	[edx]
		paddw	xmm2, xmm3
		punpcklbw xmm4,	xmm0
		movq	xmm3, qword ptr	[edx+eax]
		psllw	xmm2, 2
		paddw	xmm1, xmm4
		punpcklbw xmm3,	xmm0
		psubsw	xmm2, xmm1
		paddw	xmm5, xmm3
        paddw	xmm5, xmm2
		psllw	xmm2, 2
		paddw	xmm5, xmm2
		movdqa xmmword ptr[edi + 32], xmm5
		
		mov edx, eax
		add edx, ebx
		add edi, 48
		add esi, edx
		dec ecx
		jnz INTERPOLATE_HALFPEL_HV_V_16x16_LOOP_1
  
        mov esi, dword ptr[dst]
		mov edi, dword ptr[mpr]
		mov ecx, 16

INTERPOLATE_HALFPEL_HV_V_16x16_LOOP_2:
		movdqa xmm1, xmmword ptr[esi]
		movdqu xmm3, xmmword ptr[esi + 10]
		movdqa xmm2, xmmword ptr[esi + 16]
		movdqu xmm4, xmmword ptr[esi + 26]
        paddw xmm1, xmm3;
		paddw xmm2, xmm4
		movdqu xmm3, xmmword ptr[esi + 2]
		movdqu xmm5, xmmword ptr[esi + 8]
		movdqu xmm4, xmmword ptr[esi + 18]
		movdqu xmm6, xmmword ptr[esi + 24]
        paddw xmm3, xmm5;
		paddw xmm4, xmm6;
		movdqu xmm5, xmmword ptr[esi + 4]
		movdqu xmm7, xmmword ptr[esi + 6]
		movdqu xmm6, xmmword ptr[esi + 20]
		movdqu xmm0, xmmword ptr[esi + 22]
        paddw xmm5, xmm7
		paddw xmm6, xmm0
		psubsw xmm1, xmm3
		psubsw xmm2, xmm4
		psubsw xmm3, xmm5
		psubsw xmm4, xmm6
		psraw xmm1, 2
		psraw xmm2, 2
		psubsw xmm1, xmm3
		psubsw xmm2, xmm4
		psraw xmm1, 2
		psraw xmm2, 2
		
		paddw xmm1, xmm5
		paddw xmm2, xmm6
		paddw xmm1, xmmword ptr[thirty_two_128]
		paddw xmm2, xmmword ptr[thirty_two_128]
		psraw xmm1, 6
		psraw xmm2, 6
		pxor xmm0, xmm0
		packuswb xmm1, xmm2
		movdqa xmm3, xmm1
		punpcklbw xmm1, xmm0
		punpckhbw xmm3, xmm0

		movdqa xmmword ptr[edi], xmm1
		movdqa xmmword ptr[edi + 16], xmm3

		add esi, 48
		add edi, 32
		dec ecx
		jnz INTERPOLATE_HALFPEL_HV_V_16x16_LOOP_2
		
	}
}

static void interpolate_quarterpel_v_16x16_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {			
		mov   eax , stride
		mov   ebx , eax
		mov   esi , dword ptr [src]
		mov   edi , dword ptr [dst]		
		shl   ebx , 1
		mov   edx , eax
		mov   ecx , 16
		add   edx , ebx
		pxor  xmm0 , xmm0

INTERPLOATE_QUARTPEL_V_16x16_LOOP:
		neg   ebx
		neg   eax
		movdqu  xmm1 , xmmword ptr [esi+ebx]
		movdqu  xmm2 , xmmword ptr [esi+eax]
		neg   ebx 
		neg   eax
		movdqu  xmm3 , xmmword ptr [esi]
		movdqa  xmm4 , xmm1
		punpcklbw  xmm1 , xmm0
        movdqa  xmm5 , xmm2 
		punpckhbw  xmm4 , xmm0
		movdqa  xmm6 , xmm3
		punpcklbw  xmm3 , xmm0
		punpcklbw  xmm2 , xmm0
		psllw   xmm3 , 2
		punpckhbw  xmm6 , xmm0		
		punpckhbw  xmm5 , xmm0
		psubw   xmm3 , xmm2
		psllw   xmm6 , 2
		movdqa  xmm7 , xmm3
		psllw   xmm3 , 2
		psubw   xmm6 , xmm5
		paddw   xmm3 , xmm7
		movdqa  xmm7 , xmm6
		psllw   xmm6 , 2
		paddw   xmm1 , xmm3
		paddw   xmm6 , xmm7

		movdqu  xmm2 , xmmword ptr [esi+edx]
		paddw   xmm4 , xmm6
		movdqa  xmm7 , xmm2
		punpcklbw xmm2 , xmm0
		punpckhbw xmm7 , xmm0
		paddw   xmm1 , xmm2
		paddw   xmm4 , xmm7
		movdqu  xmm2 , xmmword ptr [esi+ebx]
		movdqu  xmm3 , xmmword ptr [esi+eax]
        movdqa  xmm5 , xmm2
		movdqa  xmm6 , xmm3
		punpcklbw xmm2 , xmm0
		punpcklbw xmm3 , xmm0
		punpckhbw xmm6 , xmm0		
		psllw     xmm3 , 2
		punpckhbw xmm5 , xmm0
		psllw     xmm6 , 2
		psubw     xmm3 , xmm2			
		movdqa    xmm7 , xmm3 
        psubw     xmm6 , xmm5
		psllw     xmm3 , 2		
        paddw     xmm3 , xmm7   
		movdqa    xmm7 , xmm6
		paddw     xmm1 , xmm3
		psllw     xmm6 , 2
		movdqa    xmm5 , xmmword ptr [sixteen_128]
		paddw     xmm6 , xmm7
		paddw     xmm4 , xmm6
		paddw     xmm1 , xmm5
		paddw     xmm4 , xmm5
		psraw     xmm1 , 5
		psraw     xmm4 , 5
		packuswb  xmm1 , xmm4
		movdqa    xmm2 , xmmword ptr [edi]
		movdqa    xmm3 , xmmword ptr [edi+16]
		movdqa    xmm6 , xmm1
		punpcklbw xmm1 , xmm0
		punpckhbw xmm6 , xmm0
		pavgw     xmm1 , xmm2
		pavgw     xmm6 , xmm3
		movdqa    xmmword ptr [edi] , xmm1
		movdqa    xmmword ptr [edi+16] , xmm6

		add  esi , eax
		add  edi , 32
		dec  ecx
		jnz  INTERPLOATE_QUARTPEL_V_16x16_LOOP	
	}
}


static void expand8to16_8x8_sse2(int16_t *dst, uchar *src, long stride)
{
	__asm {
		mov    edi , dword ptr [dst]
		mov    esi , dword ptr [src]
		mov    eax , stride
		mov    ecx , 4
		pxor   xmm7 , xmm7
EXPAND8TO16_8x8_LOOP:
        movq   xmm0 , qword ptr [esi]
		movq   xmm1 , qword ptr [esi+eax]
		punpcklbw  xmm0 , xmm7
		punpcklbw  xmm1 , xmm7
		movdqa xmmword ptr [edi] , xmm0
		movdqa xmmword ptr [edi+32] , xmm1		

		add    esi , eax
		add    edi , 64
		add    esi , eax
		dec    ecx
		jnz    EXPAND8TO16_8x8_LOOP
	}
}


static void interpolate_halfpel_h_8x8_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {
		mov   edi , dword ptr [dst]
		mov   esi , dword ptr [src]
		mov   eax , stride
		sub   esi , 2
		mov   ecx , 4
		pxor  xmm0 , xmm0
INTERPOLATE_HALFPEL_H_8x8_LOOP:
        movdqu  xmm1 , xmmword ptr [esi]
        movdqu  xmm2 , xmmword ptr [esi+eax]
		movdqa  xmm3 , xmm1
		movdqa  xmm4 , xmm1
		psrldq  xmm3 , 2
		psrldq  xmm4 , 3
		movdqa  xmm5 , xmm2
		movdqa  xmm6 , xmm2
		psrldq  xmm5 , 2
		psrldq  xmm6 , 3
		punpcklbw  xmm3 , xmm0
		punpcklbw  xmm4 , xmm0
		movdqa  xmm7 , xmmword ptr [five_128]
		punpcklbw  xmm5 , xmm0
		punpcklbw  xmm6 , xmm0
		paddw   xmm3 , xmm4
		paddw   xmm5 , xmm6
		movdqa  xmm4 , xmm1
		movdqa  xmm6 , xmm2
		paddw   xmm3 , xmm3
		paddw   xmm5 , xmm5
		psrldq  xmm4 , 1
		psrldq  xmm6 , 1
		paddw   xmm3 , xmm3
		paddw   xmm5 , xmm5
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm6 , xmm0
		psubw   xmm3 , xmm4
		psubw   xmm5 , xmm6		
		pshufd  xmm4 , xmm1 , 9
		pshufd  xmm6 , xmm2 , 9
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm6 , xmm0
		psubw   xmm3 , xmm4
		psubw   xmm5 , xmm6
		movdqa  xmm4 , xmm1
		movdqa  xmm6 , xmm2
		pmullw  xmm3 , xmm7
		psrldq  xmm4 , 5
		pmullw  xmm5 , xmm7
		psrldq  xmm6 , 5
		punpcklbw  xmm1 , xmm0
		punpcklbw  xmm2 , xmm0
		movdqa  xmm7 , xmmword ptr [sixteen_128] 
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm6 , xmm0
		paddw   xmm3 , xmm1
		paddw   xmm5 , xmm2
		paddw   xmm3 , xmm4
		paddw   xmm5 , xmm6
		paddw   xmm3 , xmm7
		paddw   xmm5 , xmm7
		psraw   xmm3 , 5		
		psraw   xmm5 , 5
		packuswb xmm3 , xmm5
		movdqa   xmm1 , xmm3 
		punpcklbw  xmm3 , xmm0
		punpckhbw  xmm1 , xmm0
        movdqa  xmmword ptr [edi] , xmm3
		movdqa  xmmword ptr [edi+32] , xmm1

		add   esi , eax
		add   edi , 64
		add   esi , eax
		dec   ecx 
		jnz   INTERPOLATE_HALFPEL_H_8x8_LOOP
	}
}


static void  interpolate_avg_8x8_sse2(int16_t *dst, uchar *src, long stride)
{
	__asm {
		mov   edi , dword ptr [dst]
		mov   esi , dword ptr [src]
		mov   eax , stride
		mov   ecx , 4
		pxor  xmm7 , xmm7
INTERPOLATE_AVG_8x8_LOOP:
		movq   xmm0 , qword ptr [esi]
		movq   xmm1 , qword ptr [esi+eax]
		movdqa xmm2 , xmmword ptr [edi]
		movdqa xmm3 , xmmword ptr [edi+32]
		punpcklbw  xmm0 , xmm7
		punpcklbw  xmm1 , xmm7
		pavgw  xmm0 , xmm2 
		pavgw  xmm1 , xmm3
		movdqa xmmword ptr [edi] , xmm0
		movdqa xmmword ptr [edi+32] , xmm1

		add   esi , eax
		add   edi , 64
		add   esi , eax
		dec   ecx
		jnz   INTERPOLATE_AVG_8x8_LOOP
	}
}


static void interpolate_halfpel_v_8x8_sse2(int16_t *dst, uchar *src, long stride)
{
	__asm {
		mov   eax , stride
		mov   edi , dword ptr [dst]
		mov   esi , dword ptr [src]
		mov   ebx , eax
		mov   ecx , 4
		shl   ebx , 1
		pxor  xmm0 , xmm0
		mov   edx , ebx
		movdqa xmm7 , xmmword ptr [sixteen_128]
		add   edx , eax

INTERPOLATE_HALFPEL_V_8x8_LOOP:
		neg   ebx 
	    neg   eax
        movq   xmm1 , qword ptr [esi+ebx]		
		movq   xmm2 , qword ptr [esi+eax]
		neg   ebx 
		neg   eax
		punpcklbw  xmm1 , xmm0
		movq   xmm3 , qword ptr [esi]
		punpcklbw  xmm2 , xmm0
		movq   xmm4 , qword ptr [esi+eax]
		punpcklbw  xmm3 , xmm0
        movq   xmm5 , qword ptr [esi+ebx]
		punpcklbw  xmm4 , xmm0
		movq   xmm6 , qword ptr[esi+edx]
		punpcklbw  xmm5 , xmm0
		paddw   xmm3  , xmm4
		punpcklbw  xmm6 , xmm0
		paddw   xmm2 , xmm5
		psllw   xmm3 , 2
		paddw   xmm1 , xmm6
		psubw   xmm3 , xmm2
		paddw   xmm1 , xmm7
		add     esi , eax
		paddw   xmm1 , xmm3
		psllw   xmm3 , 2
		neg     eax
		paddw   xmm1 , xmm3
        neg     ebx
		psraw   xmm1 , 5

        movq    xmm2 , qword ptr [esi+ebx]
		movq    xmm3 , qword ptr [esi+eax]
		neg   eax
		neg   ebx
		movq    xmm4 , qword ptr [esi]
		punpcklbw  xmm2 , xmm0
		movq    xmm5 , qword ptr [esi+eax]	
		punpcklbw  xmm3 , xmm0		
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm5 , xmm0
		movq    xmm6 , qword ptr [esi+ebx]		
        paddw   xmm4 , xmm5		
		punpcklbw  xmm6 , xmm0		
		movq    xmm5 , qword ptr [esi+edx]
		psllw   xmm4 , 2
		paddw   xmm3 , xmm6
        punpcklbw  xmm5  , xmm0     
		psubw   xmm4 , xmm3 
		paddw   xmm2 , xmm5
		paddw   xmm2 , xmm4
		paddw   xmm2 , xmm7		
		psllw   xmm4 , 2
		paddw   xmm2 , xmm4
		psraw   xmm2 , 5
		packuswb  xmm1 , xmm2
		movdqa    xmm3 , xmm1 
        punpcklbw  xmm1 , xmm0
		punpckhbw  xmm3 , xmm0
		movdqa    xmmword ptr [edi] , xmm1
		movdqa    xmmword ptr [edi+32] , xmm3

		add   esi , eax
		add   edi , 64
		dec   ecx 
		jnz   INTERPOLATE_HALFPEL_V_8x8_LOOP
	}
}


static  void  interpolate_halfpel_hv_h_8x8_sse2(int16_t *dst,uchar *src,long stride,int16_t *mpr)
{
	__asm {
		mov   esi , dword ptr [src]
		mov   edi , dword ptr [dst]
		mov   eax , stride
		sub   esi , 2
		mov   ecx , 6
		pxor  xmm0 , xmm0

INTERPOLATE_HALFPEL_HV_H_8x8_LOOP_1:
        movdqu  xmm1 , xmmword ptr [esi]
		movdqu  xmm2 , xmmword ptr [esi+eax]
		movdqa  xmm3 , xmm1
		movdqa  xmm4 , xmm1
		psrldq  xmm3 , 2
		psrldq  xmm4 , 3
		movdqa  xmm5 , xmm2
		movdqa  xmm6 , xmm2
		psrldq  xmm5 , 2
		psrldq  xmm6 , 3
		punpcklbw  xmm3 , xmm0
		punpcklbw  xmm4 , xmm0
		movdqa  xmm7 , xmmword ptr [five_128]
		punpcklbw  xmm5 , xmm0
	    punpcklbw  xmm6 , xmm0
		paddw   xmm3 , xmm4
		paddw   xmm5 , xmm6
		movdqa  xmm4 , xmm1
		movdqa  xmm6 , xmm2
        psllw   xmm3 , 2
		psllw   xmm5 , 2
		psrldq  xmm4 , 1
		psrldq  xmm6 , 1
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm6 , xmm0
		psubw   xmm3 , xmm4
		psubw   xmm5 , xmm6
		pshufd  xmm4 , xmm1 , 9
		pshufd  xmm6 , xmm2 , 9
		punpcklbw xmm4 , xmm0
		punpcklbw xmm6 , xmm0
		psubw   xmm3 , xmm4
		psubw   xmm5 , xmm6
		movdqa  xmm4 , xmm1
		movdqa  xmm6 , xmm2
		psrldq  xmm4 , 5
		psrldq  xmm6 , 5
		pmullw  xmm3 , xmm7
		punpcklbw  xmm1 , xmm0
		punpcklbw  xmm2 , xmm0
		pmullw  xmm5 , xmm7
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm6 , xmm0
		paddw   xmm1 , xmm3
		paddw   xmm2 , xmm5
		paddw   xmm1 , xmm4
		paddw   xmm2 , xmm6
		movdqa  xmmword ptr [edi] , xmm1
		movdqa  xmmword ptr [edi+32] , xmm2

		add  esi , eax
		add  edi , 64
		add  esi , eax
		dec  ecx 
		jnz  INTERPOLATE_HALFPEL_HV_H_8x8_LOOP_1
		
		movdqu   xmm1 , xmmword ptr [esi]
		movdqa   xmm3 , xmm1
		movdqa   xmm4 , xmm1
		psrldq   xmm3 , 2
		psrldq   xmm4 , 3
		punpcklbw  xmm3 , xmm0
		punpcklbw  xmm4 , xmm0
		movdqa   xmm5 , xmm1
		pshufd   xmm6 , xmm1 , 9
		psrldq   xmm5 , 1
		punpcklbw  xmm6 , xmm0
		punpcklbw  xmm5 , xmm0
        paddw    xmm3 , xmm4
        paddw    xmm5 , xmm6
		psllw    xmm3 , 2
        movdqa   xmm4 , xmm1
		psubw    xmm3 , xmm5
		psrldq   xmm4 , 5
		punpcklbw  xmm1 , xmm0
		punpcklbw  xmm4 , xmm0
		pmullw   xmm3 , xmm7
		paddw    xmm1 , xmm4
		paddw    xmm1 , xmm3
		movdqa   xmmword ptr [edi] , xmm1
		
     
        mov   esi , dword ptr [dst]
		mov   edi , dword ptr [mpr]
		mov   ecx , 4

INTERPOLATE_HALFPEL_HV_H_8x8_LOOP_2:
		movdqa	xmm0, xmmword ptr[esi]
		movdqa	xmm5, xmmword ptr[esi+0A0h]
		paddw	xmm0, xmm5
		movdqa	xmm1, xmmword ptr[esi+20h]
		movdqa	xmm3, xmmword ptr[esi+40h]
		movdqa	xmm4, xmm3
		paddw	xmm3, xmm5
		movdqa	xmm2, xmmword ptr[esi+80h]
		movdqa	xmm5, xmmword ptr[esi+0C0h]
		movdqa	xmm6, xmm2
		paddw	xmm2, xmm1
		paddw	xmm1, xmm5
		movdqa	xmm5, xmmword ptr[esi+60h]
		paddw	xmm4, xmm5
		paddw	xmm5, xmm6
		psubw	xmm0, xmm2
		psubw	xmm1, xmm3
		psraw	xmm0, 2
		psraw	xmm1, 2
		psubw	xmm2, xmm4
		psubw	xmm3, xmm5
		psubsw	xmm0, xmm2
		psubsw	xmm1, xmm3
		psraw	xmm0, 2
		psraw	xmm1, 2
		paddw	xmm0, xmm4
		paddw	xmm1, xmm5
		pxor xmm3, xmm3
		paddw xmm0, xmmword ptr[thirty_two_128]
		paddw xmm1, xmmword ptr[thirty_two_128];
		psraw	xmm0, 6
		psraw	xmm1, 6
		packuswb xmm0, xmm1

        movdqa xmm2, xmm0
		punpcklbw xmm0, xmm3
		punpckhbw xmm2, xmm3
		movdqa xmmword ptr[edi], xmm0
		movdqa xmmword ptr[edi + 32], xmm2
		
		add edi, 40h
		add	esi, 40h        
		dec   ecx 
		jnz   INTERPOLATE_HALFPEL_HV_H_8x8_LOOP_2
	}
}


static void interpolate_hv_avg_8x8_sse2(int16_t *dst , int16_t *src , long stride)
{
	__asm {
        mov   edi , dword ptr [dst]
		mov   esi , dword ptr [src]
		mov   eax , stride
		mov   ecx , 4
		shl   eax , 1
		movdqa  xmm7 , xmmword ptr [sixteen_128]
		pxor  xmm0 , xmm0

INTERPOLATE_HV_AVG_8x8_LOOP:
        movdqu  xmm1 , xmmword ptr [esi]
		movdqu  xmm2 , xmmword ptr [esi+eax]
		movdqa  xmm3 , xmmword ptr [edi]
		movdqa  xmm4 , xmmword ptr [edi+32]
		paddw   xmm1 , xmm7
		paddw   xmm2 , xmm7
		psraw   xmm1 , 5
		psraw   xmm2 , 5
		packuswb xmm1 , xmm2
        movdqa   xmm2 , xmm1
		punpcklbw  xmm1 , xmm0
		punpckhbw  xmm2 , xmm0
		pavgw    xmm1 , xmm3
		pavgw    xmm2 , xmm4
		movdqa   xmmword ptr [edi] , xmm1
		movdqa   xmmword ptr [edi+32] , xmm2

		add  esi , eax
		add  edi , 64
		add  esi , eax
		dec  ecx 
		jnz  INTERPOLATE_HV_AVG_8x8_LOOP
	}
}


static void interpolate_halfpel_hv_v_8x8_sse2(int16_t *dst , uchar *src , long stride, int16_t *mpr)
{
	__asm {
		mov esi, dword ptr[src];
		mov edi, dword ptr[dst];
		mov eax, stride;
		mov ecx, 8;
		mov ebx, eax;
		shl ebx, 1;
		mov edx, eax;
		add edx, ebx;
		pxor xmm0, xmm0;
INTERPOLATE_HALFPEL_HV_V_8x8_LOOP_1:
		neg ebx;
		neg eax;
		movdqu xmm1, xmmword ptr[esi + ebx];
		movdqu xmm2, xmmword ptr[esi + eax];
		neg ebx;
		neg eax;
		movdqu xmm3, xmmword ptr[esi];
		movdqa xmm4, xmm1;
		punpcklbw xmm1, xmm0;
		movdqa xmm5, xmm2;
		punpcklbw xmm2, xmm0;
		movdqa xmm6, xmm3;
		punpcklbw xmm3, xmm0;
		punpckhbw xmm4, xmm0;
		punpckhbw xmm5, xmm0;
		psllw xmm3, 2; 
		punpckhbw xmm6, xmm0;
		psubw xmm3, xmm2;
		psllw xmm6, 2;
		movdqa xmm7, xmm3;
		psllw xmm3, 2
		psubw xmm6, xmm5;
		paddw xmm3, xmm7;
		movdqa xmm7, xmm6;
		psllw xmm6, 2;
		paddw xmm1, xmm3;
		paddw xmm6, xmm7
			
		movdqu xmm2, xmmword ptr[esi + edx];
		paddw xmm4, xmm6
		movdqa xmm7, xmm2;
		punpcklbw xmm2, xmm0;
		punpckhbw xmm7, xmm0;
		paddw xmm1, xmm2;
		paddw xmm4, xmm7;
		
		movdqu xmm2, xmmword ptr[esi + ebx];
		movdqu xmm3, xmmword ptr[esi + eax];
		
		
		movdqa xmm5, xmm2;
		punpcklbw xmm2, xmm0;
		movdqa xmm6, xmm3;
		punpcklbw xmm3, xmm0;
		punpckhbw xmm5, xmm0;
		psllw xmm3, 2; 
		punpckhbw xmm6, xmm0;
		psubw xmm3, xmm2
		psllw xmm6, 2;
		movdqa xmm7, xmm3;
		psllw xmm3, 2
		psubw xmm6, xmm5;
		paddw xmm3, xmm7;
		movdqa xmm7, xmm6;
		psllw xmm6, 2;
		paddw xmm1, xmm3;
		paddw xmm6, xmm7
		paddw xmm4, xmm6;
		movdqa xmmword ptr[edi], xmm1;
		movdqa xmmword ptr[edi + 16], xmm4;
        
		add edi, 48;
		add esi, eax;
		dec ecx;
		jnz INTERPOLATE_HALFPEL_HV_V_8x8_LOOP_1;
  
		mov esi, dword ptr[dst]
		mov edi, dword ptr[mpr]
		mov ecx, 4;

INTERPOLATE_HALFPEL_HV_V_8x8_LOOP_2:
		movdqa xmm1, xmmword ptr[esi]
		movdqu xmm3, xmmword ptr[esi + 10]
		movdqa xmm2, xmmword ptr[esi + 48]
		movdqu xmm4, xmmword ptr[esi + 58]
        paddw xmm1, xmm3;
		paddw xmm2, xmm4
		movdqu xmm3, xmmword ptr[esi + 2]
		movdqu xmm5, xmmword ptr[esi + 8]
		movdqu xmm4, xmmword ptr[esi + 50]
		movdqu xmm6, xmmword ptr[esi + 56]
        paddw xmm3, xmm5;
		paddw xmm4, xmm6;
		movdqu xmm5, xmmword ptr[esi + 4]
		movdqu xmm7, xmmword ptr[esi + 6]
		movdqu xmm6, xmmword ptr[esi + 52]
		movdqu xmm0, xmmword ptr[esi + 54]
        paddw xmm5, xmm7;
		paddw xmm6, xmm0;
		psubsw xmm1, xmm3;
		psubsw xmm2, xmm4;
		psubsw xmm3, xmm5;
		psubsw xmm4, xmm6;
		psraw xmm1, 2;
		psraw xmm2, 2;
		psubsw xmm1, xmm3;
		psubsw xmm2, xmm4;
		psraw xmm1, 2;
		psraw xmm2, 2;
		
		paddw xmm1, xmm5;
		paddw xmm2, xmm6;
		paddw xmm1, xmmword ptr[thirty_two_128]
		paddw xmm2, xmmword ptr[thirty_two_128]
		psraw xmm1, 6;
		psraw xmm2, 6;
		pxor xmm0, xmm0
		packuswb xmm1, xmm2;
		movdqa xmm3, xmm1;
		punpcklbw xmm1, xmm0;
		punpckhbw xmm3, xmm0

		movdqa xmmword ptr[edi], xmm1
		movdqa xmmword ptr[edi + 32], xmm3

		add esi, 96
		add edi, 64
		dec ecx
		jnz INTERPOLATE_HALFPEL_HV_V_8x8_LOOP_2
		
	}
}


static void interpolate_quarterpel_v_8x8_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {
		mov   eax , stride
		mov   esi , dword ptr [src]
		mov   ebx , eax
		mov   edi , dword ptr [dst]
		shl   ebx , 1
		mov   edx , eax
		mov   ecx , 4
		add   edx , ebx
		pxor  xmm0 , xmm0
        movdqa  xmm7 , xmmword ptr [sixteen_128]
        
INTERPOLATE_QUARTERPEL_V_8x8_LOOP:
		neg   ebx
	    neg   eax
		movq  xmm1 , qword ptr [esi + ebx]
		movq  xmm2 , qword ptr [esi + eax]
		neg   ebx 
		neg   eax
		punpcklbw  xmm1 , xmm0
		movq  xmm3 , qword ptr [esi]
		punpcklbw  xmm2 , xmm0
		movq  xmm4 , qword ptr [esi+eax]
		punpcklbw  xmm3 , xmm0
		movq  xmm5 , qword ptr [esi+ebx]
		punpcklbw  xmm4 , xmm0
		movq  xmm6 , qword ptr [esi+edx]
		punpcklbw  xmm5 , xmm0
		paddw xmm3 , xmm4
		punpcklbw  xmm6 , xmm0
		paddw xmm2 , xmm5
		psllw xmm3 , 2
		paddw xmm1 , xmm6
		psubw xmm3 , xmm2
		paddw xmm1 , xmm7
		add   esi , eax
		paddw xmm1 , xmm3		
		psllw xmm3 , 2
		neg   ebx
		paddw xmm1 , xmm3		
		neg   eax
		psraw xmm1 , 5

		movq  xmm2 , qword ptr [esi+ebx]
		movq  xmm3 , qword ptr [esi+eax]
		neg   ebx
		neg   eax
		punpcklbw  xmm2 , xmm0
		movq  xmm4 , qword ptr [esi]
		punpcklbw  xmm3 , xmm0
		movq  xmm5 , qword ptr [esi+eax]
		punpcklbw  xmm4 , xmm0
		punpcklbw  xmm5 , xmm0
		movq  xmm6 , qword ptr [esi+ebx]
		paddw   xmm4 , xmm5
        punpcklbw  xmm6 , xmm0
		movq  xmm5 , qword ptr [esi+edx]
		paddw   xmm3 , xmm6
		psllw   xmm4 , 2
		punpcklbw  xmm5 , xmm0
		paddw   xmm2 , xmm7
		psubw   xmm4 , xmm3
		paddw   xmm2 , xmm5
		paddw   xmm2 , xmm4
		psllw   xmm4 , 2
		paddw   xmm2 , xmm4
		psraw   xmm2 , 5
		movdqa  xmm3 , xmmword ptr [edi]	
		packuswb  xmm1 , xmm2
		movdqa  xmm4 , xmmword ptr [edi+32]
		movdqa  xmm5 , xmm1
		punpcklbw  xmm1 , xmm0
		punpckhbw  xmm5 , xmm0
		pavgw   xmm1 , xmm3 
		pavgw   xmm4 , xmm5
		movdqa  xmmword ptr [edi] , xmm1
		movdqa  xmmword ptr [edi+32] , xmm4
        	
        add   esi , eax
		add   edi , 64 
		dec   ecx
		jnz   INTERPOLATE_QUARTERPEL_V_8x8_LOOP
	}
}
*/


void  get_block_16x16_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y)
{    
	long i,j;
	long dx,dy;
	long pres_x, pres_y;
	long result;
	__attribute__((aligned(16))) int16_t tmp_res_hor[21][16];
	__attribute__((aligned(16))) int16_t tmp_res_ver[16][24];


	dx = x_pos&0x3;
	dy = y_pos&0x3;

	x_pos = x_pos >> 2;
	y_pos = y_pos >> 2;

	x_pos = CLIP3(-18,size_x+1,x_pos);
	y_pos = CLIP3(-18,size_y+1,y_pos);


	if( dx==0 && dy==0 ){
		for( j=0;j<16;j++ ){
			for( i=0;i<16;i++ ){
				mpr[j][i] = img_y[y_pos+j][x_pos+i];
			}
		}
	} else if( dy==0 ){
		for( j=0;j<16;j++ ){
			for( i=0;i<16;i++ ){
				result = img_y[y_pos+j][x_pos+i-2] - img_y[y_pos+j][x_pos+i-1]*5 + 
					img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j][x_pos+i+1]*20 -
                    img_y[y_pos+j][x_pos+i+2]*5 + img_y[y_pos+j][x_pos+i+3];
				mpr[j][i] = max(0,min((result+16)/32,255));
			}
		}
		if( dx&0x1 ){
			dx = dx>>1;
			for ( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					mpr[j][i] = (mpr[j][i] + img_y[y_pos+j][x_pos+i+dx] + 1)>>1;
				}
			}
		}
	} else if( dx==0 ){
		for( j=0;j<16;j++ ){
			for( i=0;i<16;i++ ){
				result = img_y[y_pos+j-2][x_pos+i] - img_y[y_pos+j-1][x_pos+i]*5 + 
					img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j+1][x_pos+i]*20 -
					img_y[y_pos+j+2][x_pos+i]*5 + img_y[y_pos+j+3][x_pos+i];
				mpr[j][i] = max(0,min((result+16)/32,255));
			}
		}
		if( dy&0x1 ){
			dy = dy>>1;
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					mpr[j][i] = (mpr[j][i] + img_y[y_pos+j+dy][x_pos+i] + 1) >> 1;
				}
			}
		}
	} else {
		if( dx==2 ){
			for( j=-2;j<16+3;j++ ){
				for( i=0;i<16;i++ ){
                    tmp_res_hor[j+2][i] = img_y[y_pos+j][x_pos+i-2] - img_y[y_pos+j][x_pos+i-1]*5 + 
						img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j][x_pos+i+1]*20 -
						img_y[y_pos+j][x_pos+i+2]*5 + img_y[y_pos+j][x_pos+i+3];
				}
			}
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					result = tmp_res_hor[j][i] - tmp_res_hor[j+1][i]*5 + 
						tmp_res_hor[j+2][i]*20 + tmp_res_hor[j+3][i]*20 -
						tmp_res_hor[j+4][i]*5 + tmp_res_hor[j+5][i];
					mpr[j][i] = max(0,min((result+512)/1024,255));
				}
			}
			if( dy & 0x1 ){
				dy = dy>>1;
				for( j=0;j<16;j++ ){
					for( i=0;i<16;i++ ){
						mpr[j][i] = (mpr[j][i] + max(0,min((tmp_res_hor[j+2+dy][i]+16)/32,255)) + 1) >> 1;
					}
				}
			}
		} else if( dy == 2 ){
			for( j=0;j<16;j++ ){
				for( i=-2;i<16+3;i++ ){
					tmp_res_ver[j][i+2] = img_y[y_pos+j-2][x_pos+i] - img_y[y_pos+j-1][x_pos+i]*5 +
                        img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j+1][x_pos+i]*20 -
						img_y[y_pos+j+2][x_pos+i]*5 + img_y[y_pos+j+3][x_pos+i];					
				}
			}
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					result = tmp_res_ver[j][i] - tmp_res_ver[j][i+1]*5 + 
						tmp_res_ver[j][i+2]*20 + tmp_res_ver[j][i+3]*20 -
						tmp_res_ver[j][i+4]*5 + tmp_res_ver[j][i+5];
					mpr[j][i] = max(0,min((result+512)/1024,255));
				}
			}
			if( dx&0x1) {
				dx = dx>>1;
				for( j=0;j<16;j++ ){
					for( i=0;i<16;i++ ){
						mpr[j][i] = (mpr[j][i] + max(0,min((tmp_res_ver[j][i+2+dx]+16)/32,255)) + 1)>>1;
					}
				}
			}
		} else {
            pres_x = x_pos + (dx!=1);
			pres_y = y_pos + (dy!=1);
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					result = img_y[pres_y+j][x_pos+i-2] - img_y[pres_y+j][x_pos+i-1]*5 + 
						img_y[pres_y+j][x_pos+i]*20 + img_y[pres_y+j][x_pos+i+1]*20 -
						img_y[pres_y+j][x_pos+i+2]*5 + img_y[pres_y+j][x_pos+i+3];
					mpr[j][i] = max(0,min((result+16)/32,255));
				}
			}
			for( j=0;j<16;j++ ){
				for( i=0;i<16;i++ ){
					result = img_y[y_pos+j-2][pres_x+i] - img_y[y_pos+j-1][pres_x+i]*5 + 
						img_y[y_pos+j][pres_x+i]*20 + img_y[y_pos+j+1][pres_x+i]*20 - 
						img_y[y_pos+j+2][pres_x+i]*5 + img_y[y_pos+j+3][pres_x+i];
					mpr[j][i] = ( mpr[j][i] + max(0,min((result+16)/32,255)) + 1)>>1;
				}
			}
		}
	}
}


void  get_block_16x8_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y)
{
	// TODO
}

void  get_block_8x16_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y)
{
	// TODO
}

void  get_block_8x8_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx)
{
	long i,j;
	long dx,dy;
	long pres_x, pres_y;
	long result;
	__attribute__((aligned(16))) int16_t tmp_res_hor[21][16];
	__attribute__((aligned(16))) int16_t tmp_res_ver[16][24];
	long  i_off, j_off;

    i_off = (idx&0x1)<<3;
	j_off = (idx&0x2)<<2;
	dx = x_pos&0x3;
	dy = y_pos&0x3;

	x_pos = x_pos >> 2;
	y_pos = y_pos >> 2;

	x_pos = CLIP3(-18,size_x+1,x_pos);
	y_pos = CLIP3(-18,size_y+1,y_pos);


	if( dx==0 && dy==0 ){
		for( j=0;j<8;j++ ){
			for( i=0;i<8;i++ ){
				mpr[j+j_off][i+i_off] = img_y[y_pos+j][x_pos+i];
			}
		}
	} else if( dy==0 ){
		for( j=0;j<8;j++ ){
			for( i=0;i<8;i++ ){
				result = img_y[y_pos+j][x_pos+i-2] - img_y[y_pos+j][x_pos+i-1]*5 + 
					img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j][x_pos+i+1]*20 -
                    img_y[y_pos+j][x_pos+i+2]*5 + img_y[y_pos+j][x_pos+i+3];
				mpr[j+j_off][i+i_off] = max(0,min((result+16)/32,255));
			}
		}
		if( dx&0x1 ){
			dx = dx>>1;
			for ( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + img_y[y_pos+j][x_pos+i+dx] + 1)>>1;
				}
			}
		}
	} else if( dx==0 ){
		for( j=0;j<8;j++ ){
			for( i=0;i<8;i++ ){
				result = img_y[y_pos+j-2][x_pos+i] - img_y[y_pos+j-1][x_pos+i]*5 + 
					img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j+1][x_pos+i]*20 -
					img_y[y_pos+j+2][x_pos+i]*5 + img_y[y_pos+j+3][x_pos+i];
				mpr[j+j_off][i+i_off] = max(0,min((result+16)/32,255));
			}
		}
		if( dy&0x1 ){
			dy = dy>>1;
			for( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + img_y[y_pos+j+dy][x_pos+i] + 1) >> 1;
				}
			}
		}
	} else {
		if( dx==2 ){
			for( j=-2;j<8+3;j++ ){
				for( i=0;i<8;i++ ){
                    tmp_res_hor[j+2][i] = img_y[y_pos+j][x_pos+i-2] - img_y[y_pos+j][x_pos+i-1]*5 + 
						img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j][x_pos+i+1]*20 -
						img_y[y_pos+j][x_pos+i+2]*5 + img_y[y_pos+j][x_pos+i+3];
				}
			}
			for( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					result = tmp_res_hor[j][i] - tmp_res_hor[j+1][i]*5 + 
						tmp_res_hor[j+2][i]*20 + tmp_res_hor[j+3][i]*20 -
						tmp_res_hor[j+4][i]*5 + tmp_res_hor[j+5][i];
					mpr[j+j_off][i+i_off] = max(0,min((result+512)/1024,255));
				}
			}
			if( dy & 0x1 ){
				dy = dy>>1;
				for( j=0;j<8;j++ ){
					for( i=0;i<8;i++ ){
						mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + max(0,min((tmp_res_hor[j+2+dy][i]+16)/32,255)) + 1) >> 1;
					}
				}
			}
		} else if( dy == 2 ){
			for( j=0;j<8;j++ ){
				for( i=-2;i<8+3;i++ ){
					tmp_res_ver[j][i+2] = img_y[y_pos+j-2][x_pos+i] - img_y[y_pos+j-1][x_pos+i]*5 +
                        img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j+1][x_pos+i]*20 -
						img_y[y_pos+j+2][x_pos+i]*5 + img_y[y_pos+j+3][x_pos+i];					
				}
			}
			for( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					result = tmp_res_ver[j][i] - tmp_res_ver[j][i+1]*5 + 
						tmp_res_ver[j][i+2]*20 + tmp_res_ver[j][i+3]*20 -
						tmp_res_ver[j][i+4]*5 + tmp_res_ver[j][i+5];
					mpr[j+j_off][i+i_off] = max(0,min((result+512)/1024,255));
				}
			}
			if( dx&0x1) {
				dx = dx>>1;
				for( j=0;j<8;j++ ){
					for( i=0;i<8;i++ ){
						mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + max(0,min((tmp_res_ver[j][i+2+dx]+16)/32,255)) + 1)>>1;
					}
				}
			}
		} else {
            pres_x = x_pos + (dx!=1);
			pres_y = y_pos + (dy!=1);
			for( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					result = img_y[pres_y+j][x_pos+i-2] - img_y[pres_y+j][x_pos+i-1]*5 + 
						img_y[pres_y+j][x_pos+i]*20 + img_y[pres_y+j][x_pos+i+1]*20 -
						img_y[pres_y+j][x_pos+i+2]*5 + img_y[pres_y+j][x_pos+i+3];
					mpr[j+j_off][i+i_off] = max(0,min((result+16)/32,255));
				}
			}
			for( j=0;j<8;j++ ){
				for( i=0;i<8;i++ ){
					result = img_y[y_pos+j-2][pres_x+i] - img_y[y_pos+j-1][pres_x+i]*5 + 
						img_y[y_pos+j][pres_x+i]*20 + img_y[y_pos+j+1][pres_x+i]*20 - 
						img_y[y_pos+j+2][pres_x+i]*5 + img_y[y_pos+j+3][pres_x+i];
					mpr[j+j_off][i+i_off] = ( mpr[j+j_off][i+i_off] + max(0,min((result+16)/32,255)) + 1)>>1;
				}
			}
		}
	}
}

void  get_block_8x4_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y)
{
	// TODO
}

void  get_block_4x8_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y)
{
	// TODO
}

void  get_block_4x4_luma_c(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx)
{
	long i,j;
	long dx,dy;
	long pres_x, pres_y;
	long result;
	__attribute__((aligned(16))) int16_t tmp_res_hor[21][16];
	__attribute__((aligned(16))) int16_t tmp_res_ver[16][24];
	long i_off,j_off;

    i_off = submb_pos_x[idx]<<2;
	j_off = submb_pos_y[idx]<<2;
	dx = x_pos&0x3;
	dy = y_pos&0x3;

	x_pos = x_pos >> 2;
	y_pos = y_pos >> 2;

	x_pos = CLIP3(-18,size_x+1,x_pos);
	y_pos = CLIP3(-18,size_y+1,y_pos);


	if( dx==0 && dy==0 ){
		for( j=0;j<4;j++ ){
			for( i=0;i<4;i++ ){
				mpr[j+j_off][i+i_off] = img_y[y_pos+j][x_pos+i];
			}
		}
	} else if( dy==0 ){
		for( j=0;j<4;j++ ){
			for( i=0;i<4;i++ ){
				result = img_y[y_pos+j][x_pos+i-2] - img_y[y_pos+j][x_pos+i-1]*5 + 
					img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j][x_pos+i+1]*20 -
                    img_y[y_pos+j][x_pos+i+2]*5 + img_y[y_pos+j][x_pos+i+3];
				mpr[j+j_off][i+i_off] = max(0,min((result+16)/32,255));
			}
		}
		if( dx&0x1 ){
			dx = dx>>1;
			for ( j=0;j<4;j++ ){
				for( i=0;i<4;i++ ){
					mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + img_y[y_pos+j][x_pos+i+dx] + 1)>>1;
				}
			}
		}
	} else if( dx==0 ){
		for( j=0;j<4;j++ ){
			for( i=0;i<4;i++ ){
				result = img_y[y_pos+j-2][x_pos+i] - img_y[y_pos+j-1][x_pos+i]*5 + 
					img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j+1][x_pos+i]*20 -
					img_y[y_pos+j+2][x_pos+i]*5 + img_y[y_pos+j+3][x_pos+i];
				mpr[j+j_off][i+i_off] = max(0,min((result+16)/32,255));
			}
		}
		if( dy&0x1 ){
			dy = dy>>1;
			for( j=0;j<4;j++ ){
				for( i=0;i<4;i++ ){
					mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + img_y[y_pos+j+dy][x_pos+i] + 1) >> 1;
				}
			}
		}
	} else {
		if( dx==2 ){
			for( j=-2;j<4+3;j++ ){
				for( i=0;i<4;i++ ){
                    tmp_res_hor[j+2][i] = img_y[y_pos+j][x_pos+i-2] - img_y[y_pos+j][x_pos+i-1]*5 + 
						img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j][x_pos+i+1]*20 -
						img_y[y_pos+j][x_pos+i+2]*5 + img_y[y_pos+j][x_pos+i+3];
				}
			}
			for( j=0;j<4;j++ ){
				for( i=0;i<4;i++ ){
					result = tmp_res_hor[j][i] - tmp_res_hor[j+1][i]*5 + 
						tmp_res_hor[j+2][i]*20 + tmp_res_hor[j+3][i]*20 -
						tmp_res_hor[j+4][i]*5 + tmp_res_hor[j+5][i];
					mpr[j+j_off][i+i_off] = max(0,min((result+512)/1024,255));
				}
			}
			if( dy & 0x1 ){
				dy = dy>>1;
				for( j=0;j<4;j++ ){
					for( i=0;i<4;i++ ){
						mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + max(0,min((tmp_res_hor[j+2+dy][i]+16)/32,255)) + 1) >> 1;
					}
				}
			}
		} else if( dy == 2 ){
			for( j=0;j<4;j++ ){
				for( i=-2;i<4+3;i++ ){
					tmp_res_ver[j][i+2] = img_y[y_pos+j-2][x_pos+i] - img_y[y_pos+j-1][x_pos+i]*5 +
                        img_y[y_pos+j][x_pos+i]*20 + img_y[y_pos+j+1][x_pos+i]*20 -
						img_y[y_pos+j+2][x_pos+i]*5 + img_y[y_pos+j+3][x_pos+i];					
				}
			}
			for( j=0;j<4;j++ ){
				for( i=0;i<4;i++ ){
					result = tmp_res_ver[j][i] - tmp_res_ver[j][i+1]*5 + 
						tmp_res_ver[j][i+2]*20 + tmp_res_ver[j][i+3]*20 -
						tmp_res_ver[j][i+4]*5 + tmp_res_ver[j][i+5];
					mpr[j+j_off][i+i_off] = max(0,min((result+512)/1024,255));
				}
			}
			if( dx&0x1) {
				dx = dx>>1;
				for( j=0;j<4;j++ ){
					for( i=0;i<4;i++ ){
						mpr[j+j_off][i+i_off] = (mpr[j+j_off][i+i_off] + max(0,min((tmp_res_ver[j][i+2+dx]+16)/32,255)) + 1)>>1;
					}
				}
			}
		} else {
            pres_x = x_pos + (dx!=1);
			pres_y = y_pos + (dy!=1);
			for( j=0;j<4;j++ ){
				for( i=0;i<4;i++ ){
					result = img_y[pres_y+j][x_pos+i-2] - img_y[pres_y+j][x_pos+i-1]*5 + 
						img_y[pres_y+j][x_pos+i]*20 + img_y[pres_y+j][x_pos+i+1]*20 -
						img_y[pres_y+j][x_pos+i+2]*5 + img_y[pres_y+j][x_pos+i+3];
					mpr[j+j_off][i+i_off] = max(0,min((result+16)/32,255));
				}
			}
			for( j=0;j<4;j++ ){
				for( i=0;i<4;i++ ){
					result = img_y[y_pos+j-2][pres_x+i] - img_y[y_pos+j-1][pres_x+i]*5 + 
						img_y[y_pos+j][pres_x+i]*20 + img_y[y_pos+j+1][pres_x+i]*20 - 
						img_y[y_pos+j+2][pres_x+i]*5 + img_y[y_pos+j+3][pres_x+i];
					mpr[j+j_off][i+i_off] = ( mpr[j+j_off][i+i_off] + max(0,min((result+16)/32,255)) + 1)>>1;
				}
			}
		}
	}
}


void  get_block_8x8_chroma_c(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv)
{
    long i,j;
	long if0,if1,jf0,jf1;
	long  v = uv << 3;

    if1 = x_pos & 0x7;
	jf1 = y_pos & 0x7;
	if0 = 8 - if1;
	jf0 = 8 - jf1;
	x_pos = x_pos >> 3;
	y_pos = y_pos >> 3;
    x_pos = CLIP3(-8,size_x-1,x_pos);
	y_pos = CLIP3(-8,size_y-1,y_pos);

	for( j=0;j<8;j++ ){
		for( i=0;i<8;i++ ){
			mpr[16+j][v+i] = (img_c[y_pos+j][x_pos+i]*if0*jf0 + img_c[y_pos+j][x_pos+i+1]*if1*jf0 + 
				img_c[y_pos+j+1][x_pos+i]*if0*jf1 + img_c[y_pos+j+1][x_pos+i+1]*if1*jf1 + 32) >> 6;
		}
	}
}

void  get_block_4x4_chroma_c(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx)
{
    long i,j;
	long if0,if1,jf0,jf1;
	long  v = uv << 3;
	long  i_off, j_off;

	i_off = (idx & 0x1)<<2;
	j_off = (idx & 0x2)<<1;
    if1 = x_pos & 0x7;
	jf1 = y_pos & 0x7;
	if0 = 8 - if1;
	jf0 = 8 - jf1;
	x_pos = x_pos >> 3;
	y_pos = y_pos >> 3;
    x_pos = CLIP3(-8,size_x-1,x_pos);
	y_pos = CLIP3(-8,size_y-1,y_pos);

	for( j=0;j<4;j++ ){
		for( i=0;i<4;i++ ){
			mpr[16+j+j_off][v+i+i_off] = (img_c[y_pos+j][x_pos+i]*if0*jf0 + img_c[y_pos+j][x_pos+i+1]*if1*jf0 + 
				img_c[y_pos+j+1][x_pos+i]*if0*jf1 + img_c[y_pos+j+1][x_pos+i+1]*if1*jf1 + 32) >> 6;
		}
	}
}


void  get_block_2x2_chroma_c(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx)
{
	long i,j;
	long if0,if1,jf0,jf1;
	long  v = uv << 3;
	long  i_off, j_off;

	i_off = submb_pos_x[idx]<<1;
	j_off = submb_pos_y[idx]<<1;
    if1 = x_pos & 0x7;
	jf1 = y_pos & 0x7;
	if0 = 8 - if1;
	jf0 = 8 - jf1;
	x_pos = x_pos >> 3;
	y_pos = y_pos >> 3;
    x_pos = CLIP3(-8,size_x-1,x_pos);
	y_pos = CLIP3(-8,size_y-1,y_pos);

	for( j=0;j<2;j++ ){
		for( i=0;i<2;i++ ){
			mpr[16+j+j_off][v+i+i_off] = (img_c[y_pos+j][x_pos+i]*if0*jf0 + img_c[y_pos+j][x_pos+i+1]*if1*jf0 + 
				img_c[y_pos+j+1][x_pos+i]*if0*jf1 + img_c[y_pos+j+1][x_pos+i+1]*if1*jf1 + 32) >> 6;
		}
	}
}

/*
void  get_block_16x16_luma_sse2(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y)
{  
	long dx,dy;
	long pres_x, pres_y;	
	__attribute__((aligned(16))) int16_t tmp_res_hor[21][16];
	__attribute__((aligned(16))) int16_t tmp_res_ver[16][24];
	long stride = size_x + PAD_SIZE*2;


	dx = x_pos&0x3;
	dy = y_pos&0x3;

	x_pos = x_pos >> 2;
	y_pos = y_pos >> 2;

	x_pos = CLIP3(-18,size_x+1,x_pos);
	y_pos = CLIP3(-18,size_y+1,y_pos);


	if( dx==0 && dy==0 ){
		expand8to16_16x16_sse2((int16_t*)mpr,&img_y[y_pos][x_pos],stride);	
	} else if( dy==0 ){		
		interpolate_halfpel_h_16x16_sse2((int16_t*)mpr,&img_y[y_pos][x_pos],stride);
		if( dx&0x1 ){
			dx = dx>>1;			
			interpolate_avg_16x16_sse2((int16_t*)mpr,&img_y[y_pos][x_pos+dx],stride);
		}
	} else if( dx==0 ){		
		interpolate_halfpel_v_16x16_sse2((int16_t*)mpr,&img_y[y_pos][x_pos],stride);
		if( dy&0x1 ){
			dy = dy>>1;			
			interpolate_avg_16x16_sse2((int16_t*)mpr,&img_y[y_pos+dy][x_pos],stride);
		}
	} else {
		if( dx==2 ){			
			interpolate_halfpel_hv_h_16x16_sse2((int16_t*)tmp_res_hor,&img_y[y_pos-2][x_pos],stride,(int16_t*)mpr);
			if( dy & 0x1 ){
				dy = dy>>1;				
				interpolate_hv_avg_16x16_sse2((int16_t*)mpr,&tmp_res_hor[2+dy][0],16);
			}
		} else if( dy == 2 ){			
			interpolate_halfpel_hv_v_16x16_sse2((int16_t*)tmp_res_ver,&img_y[y_pos][x_pos-2],stride,(int16_t*)mpr);			
			dx = dx>>1;				
			interpolate_hv_avg_16x16_sse2((int16_t*)mpr,(int16_t*)&tmp_res_ver[0][2+dx],24);			
		} else {
            pres_x = x_pos + (dx!=1);
			pres_y = y_pos + (dy!=1);			
			interpolate_halfpel_h_16x16_sse2((int16_t*)mpr,&img_y[pres_y][x_pos],stride);		
			interpolate_quarterpel_v_16x16_sse2((int16_t*)mpr,&img_y[y_pos][pres_x],stride);
		}
	}
}
*/

/*
void  get_block_8x8_chroma_sse2(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv)
{
	long if1,jf1;
	long  v = uv << 3;
	uchar  *src;
	int16_t  *dst;	
	long  stride = size_x + PAD_SIZE;


    if1 = x_pos & 0x7;
	jf1 = y_pos & 0x7;
	x_pos = x_pos >> 3;
	y_pos = y_pos >> 3;
    x_pos = CLIP3(-8,size_x-1,x_pos);
	y_pos = CLIP3(-8,size_y-1,y_pos);

	src = &img_c[y_pos][x_pos];
	dst = &mpr[16][v];

	__asm {
        mov     eax , if1
		mov     ebx , jf1
		mov     ecx , eax
		imul    eax , ebx
		mov     edx , 40h
		movd    xmm7 , eax
		shl     ecx  , 3	
		pshufd  xmm7 , xmm7 , 0		
		pslldq  xmm7 , 2		
		shl     ebx  , 3
		sub     ecx  , eax
		sub     edx  , ebx
		movd    xmm6 , ecx
		sub     ebx  , eax
		pshufd  xmm6 , xmm6 , 0
		pslldq  xmm6 , 2
		movd    xmm5 , ebx
		sub     edx  , ecx
		pshufd  xmm5 , xmm5 , 0
		movd    xmm4 , edx		
		por     xmm7 , xmm5
		pshufd  xmm4 , xmm4 , 0
		pxor    xmm0 , xmm0		
		por     xmm6 , xmm4

		
		mov     esi , dword ptr [src]
		mov     edi , dword ptr [dst]
		mov     eax , stride
		mov     ecx , 8
		movq    xmm1 , qword ptr [esi]
		xor     ebx , ebx
		punpcklbw  xmm1 , xmm0	
INTERPOLATE_CHROMA_8x8_LOOP:
        movq    xmm2 , qword ptr [esi+eax]
        mov     bl  , [esi+8]
		movdqa  xmm3 , xmm1
		punpcklbw  xmm2 , xmm0
		psrldq  xmm3 , 2
		pmaddwd xmm1 , xmm6
		movdqa  xmm5 , xmm2
		movdqa  xmm4 , xmm2
		pmaddwd xmm2 , xmm7
		pinsrw  xmm3 , ebx , 7
		psrldq  xmm4 , 2
		mov     bl , [esi+eax+8]
		pinsrw  xmm4 , ebx , 7
		pmaddwd xmm3 , xmm6
		add     esi , eax
		pmaddwd xmm4 , xmm7
		paddd   xmm2 , xmm1
		paddw   xmm2 , xmmword ptr [thirty_two_128]
		movdqa  xmm1 , xmm5
		psrlw   xmm2 , 6
		paddd   xmm3 , xmm4
		paddw   xmm3 , xmmword ptr [thirty_two_128]
		psrlw   xmm3 , 6
		pslldq  xmm3 , 2
        por     xmm2 , xmm3
		movdqa  xmmword ptr [edi] , xmm2

		add     edi  , 32
		dec     ecx  
		jnz     INTERPOLATE_CHROMA_8x8_LOOP
	}
}
*/

/*
void  get_block_8x8_luma_sse2(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx)
{
	long dx,dy;
	long pres_x, pres_y;	
	__attribute__((aligned(16))) int16_t tmp_res_hor[21][16];
	__attribute__((aligned(16))) int16_t tmp_res_ver[16][24];
	long  i_off, j_off;
	long stride = size_x + PAD_SIZE*2;

    i_off = (idx&0x1)<<3;
	j_off = (idx&0x2)<<2;
	dx = x_pos&0x3;
	dy = y_pos&0x3;

	x_pos = x_pos >> 2;
	y_pos = y_pos >> 2;

	x_pos = CLIP3(-18,size_x+1,x_pos);
	y_pos = CLIP3(-18,size_y+1,y_pos);


	if( dx==0 && dy==0 ){	
		expand8to16_8x8_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos],stride);
	} else if( dy==0 ){	
		interpolate_halfpel_h_8x8_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos],stride);
		if( dx&0x1 ){
			dx = dx>>1;			
			interpolate_avg_8x8_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos+dx],stride);
		}
	} else if( dx==0 ){				
		interpolate_halfpel_v_8x8_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos],stride);	
		if( dy&0x1 ){
			dy = dy>>1;			
			interpolate_avg_8x8_sse2(&mpr[j_off][i_off],&img_y[y_pos+dy][x_pos],stride);
		}
	} else {
		if( dx==2 ){						
			interpolate_halfpel_hv_h_8x8_sse2((int16_t*)tmp_res_hor,&img_y[y_pos-2][x_pos],stride,&mpr[j_off][i_off]);
			if( dy & 0x1 ){
				dy = dy>>1;			
				interpolate_hv_avg_8x8_sse2(&mpr[j_off][i_off],&tmp_res_hor[2+dy][0],16);
			}
		} else if( dy == 2 ){				
			interpolate_halfpel_hv_v_8x8_sse2((int16_t*)tmp_res_ver,&img_y[y_pos][x_pos-2],stride,&mpr[j_off][i_off]);			
			dx = dx>>1;		
			interpolate_hv_avg_8x8_sse2(&mpr[j_off][i_off],&tmp_res_ver[0][2+dx],24);
		} else {
            pres_x = x_pos + (dx!=1);
			pres_y = y_pos + (dy!=1);		
			interpolate_halfpel_h_8x8_sse2(&mpr[j_off][i_off],&img_y[pres_y][x_pos],stride);		
			interpolate_quarterpel_v_8x8_sse2(&mpr[j_off][i_off],&img_y[y_pos][pres_x],stride);
		}
	}
}
*/

/*
void   get_block_4x4_chroma_sse2(uchar **img_c,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long uv,long idx)
{
	long if1,jf1;
	long  v = uv << 3;
	long  i_off, j_off;
	long  stride = size_x + PAD_SIZE;
	uchar *src;
	int16_t *dst;

	i_off = (idx & 0x1)<<2;
	j_off = (idx & 0x2)<<1;
    if1 = x_pos & 0x7;
	jf1 = y_pos & 0x7;
	x_pos = x_pos >> 3;
	y_pos = y_pos >> 3;
    x_pos = CLIP3(-8,size_x-1,x_pos);
	y_pos = CLIP3(-8,size_y-1,y_pos);

	src = &img_c[y_pos][x_pos];
	dst = &mpr[16+j_off][v+i_off];
	
	__asm {
		mov	eax, if1
		mov	ebx, jf1
		mov ecx, eax
		imul	eax, ebx
		mov	edx, 40h
		shl ecx, 3
		movd	mm7, eax
		shl ebx, 3
		pshufw	mm7, mm7, 11h
		sub	ecx, eax
		sub	edx, ebx
		movd	mm6, ecx
		sub	ebx, eax
		pshufw	mm6, mm6, 11h
		movd	mm1, ebx
		sub	edx, ecx
		pshufw	mm1, mm1, 44h
		movd	mm2, edx
		por	mm7, mm1
		pshufw	mm2, mm2, 44h
		pxor	mm0, mm0
		por	mm6, mm2
		mov	esi, dword ptr[src]
		mov	edi, dword ptr[dst]
		mov	eax, stride
		mov	ecx, 4
		xor	edx, edx
		movd	mm1, dword ptr [esi]
		punpcklbw mm1, mm0		

INTERPOLATE_CHROMA_4x4_LOOP:
		mov	dl, [esi+4]
		add edi, 16;
		movd	mm2, dword ptr [esi+eax]
		
		pshufw	mm3, mm1, 39h
		punpcklbw mm2, mm0
		pmaddwd	mm1, mm6
		movq	mm5, mm2
		pshufw	mm4, mm2, 39h
		pmaddwd	mm2, mm7
		pinsrw	mm3, edx, 3
		mov	dl, [eax+esi+4]
		add	edi, 16
		pinsrw	mm4, edx, 3
		pmaddwd	mm3, mm6
		add	esi, eax
		pmaddwd	mm4, mm7
		paddd	mm2, mm1
		paddw	mm2, thirty_two_64
		movq	mm1, mm5
		psrlw	mm2, 6
		paddd	mm3, mm4
		paddw	mm3, thirty_two_64
		sub	ecx, 1
		psrlw	mm3, 6
		psllq	mm3, 16
		por	mm2, mm3
		movq	qword ptr [edi-32], mm2
		jg	INTERPOLATE_CHROMA_4x4_LOOP
		emms;
	}
}


static void expand8to16_4x4_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {
		mov    edi , dword ptr [dst]
		mov    esi , dword ptr [src]
		mov    eax , stride
		pxor   mm0 , mm0

        movd   mm1 , dword ptr [esi]
		punpcklbw   mm1 , mm0
		add    esi , eax
		movq   qword ptr [edi] , mm1
		add    edi , 32
		movd   mm1 , dword ptr [esi]
		punpcklbw   mm1 , mm0
		add    esi , eax
		movq   qword ptr [edi] , mm1
		add    edi , 32
		movd   mm1 , dword ptr [esi]
		punpcklbw   mm1 , mm0
		add    esi , eax
		movq   qword ptr [edi] , mm1
		add    edi , 32
		movd   mm1 , dword ptr [esi]
		punpcklbw   mm1 , mm0
		add    esi , eax
		movq   qword ptr [edi] , mm1
		add    edi , 32

        emms
	}
}


static void interpolate_halfpel_h_4x4_sse2(int16_t *dst , uchar *src, long stride)
{
	__asm {
        mov     esi , dword ptr [src]
		mov     edi , dword ptr [dst]
		mov     eax , stride
		mov     ecx , 4
		pxor    mm0 , mm0

INTERPOLATE_HALFPEL_H_4x4_LOOP:
		movq    mm1 , qword ptr [esi-2]
		movq    mm2 , qword ptr [esi-1]
		movq    mm3 , qword ptr [esi]
		punpcklbw  mm1 , mm0
		punpcklbw  mm2 , mm0
		punpcklbw  mm3 , mm0		
		psllw   mm3 , 2
		psubw   mm3 , mm2
		movq    mm4 , qword ptr [esi+1]
		movq    mm7 , mm3
		psllw   mm7 , 2
		movq    mm5 , qword ptr [esi+2]
		paddw   mm3 , mm7
		paddw   mm3 , mm1		
		
		movq    mm6 , qword ptr [esi+3]
		punpcklbw  mm4 , mm0
		punpcklbw  mm5 , mm0
		punpcklbw  mm6 , mm0
		movq    mm1  , qword ptr [sixteen_64]
		psllw   mm4 , 2
		psubw   mm4 , mm5
		movq    mm7 , mm4
		psllw   mm7 , 2
		paddw   mm4 , mm7	
        paddw   mm4 , mm6
        paddw   mm3 , mm4
		paddw   mm3 , mm1
		psraw   mm3 , 5
		packuswb  mm3 , mm4
		punpcklbw  mm3 , mm0
		movq    qword ptr [edi] , mm3        

		add    esi , eax
		add    edi , 32
		dec    ecx
		jnz    INTERPOLATE_HALFPEL_H_4x4_LOOP

		emms
	}
}

static void interpolate_avg_4x4_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {
        mov    esi , dword ptr [src]
		mov    edi , dword ptr [dst]
		mov    eax , stride
	    pxor   mm7 , mm7

        movd   mm0 , dword ptr [esi]
		movd   mm1 , dword ptr [esi+eax]
		punpcklbw   mm0 , mm7
		movq   mm2 , qword ptr [edi]
		punpcklbw   mm1 , mm7
		movq   mm3 , qword ptr [edi+32]
        pavgw  mm0 , mm2		
		pavgw  mm1 , mm3		
		add    esi , eax
        movq   qword ptr [edi] , mm0
		add    esi , eax
		movq   qword ptr [edi+32] , mm1

        add    edi , 64
		movd   mm0 , dword ptr [esi]
		movd   mm1 , dword ptr [esi+eax]
		punpcklbw  mm0 , mm7
		movq   mm2 , qword ptr [edi]
		punpcklbw  mm1 , mm7
		movq   mm3 , qword ptr [edi+32]
		pavgw  mm0 , mm2
		pavgw  mm1 , mm3
		movq   qword ptr [edi] , mm0
		movq   qword ptr [edi+32] , mm1

		emms
	}
}

static void interpolate_halfpel_v_4x4_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {
		mov    eax , stride
		mov    esi , dword ptr [src]
		mov    ebx , eax	
		mov    edx , eax
		mov    edi , dword ptr [dst]
		shl    ebx , 1		
		pxor   mm0 , mm0
		mov    ecx , 4
		movq   mm7 , qword ptr [sixteen_64]
		add    edx , ebx

INTERPOLATE_HALFPEL_V_4x4_LOOP:
        neg    ebx 
		neg    eax
        movq   mm1 , qword ptr [esi+ebx]
		movq   mm2 , qword ptr [esi+eax]
		movq   mm3 , qword ptr [esi]
        neg    ebx
		neg    eax
		punpcklbw  mm3 , mm0
		punpcklbw  mm2 , mm0
		psllw  mm3 , 2
		punpcklbw  mm1 , mm0
		psubw  mm3 , mm2
		movq   mm4 , mm3
        psllw  mm4 , 2
		paddw  mm4 , mm3
		paddw  mm1 , mm4

		movq   mm4 , qword ptr [esi+eax]
		movq   mm5 , qword ptr [esi+ebx]
		movq   mm6 , qword ptr [esi+edx]
		punpcklbw  mm4 , mm0
		punpcklbw  mm5 , mm0
		psllw  mm4 , 2
		punpcklbw  mm6 , mm0
		psubw  mm4 , mm5
		movq   mm3 , mm4
		psllw  mm4 , 2
		paddw  mm3 , mm4
		paddw  mm3 , mm6
		paddw  mm1 , mm3
		paddw  mm1 , mm7
		psraw  mm1 , 5
		packuswb  mm1 , mm2
		punpcklbw  mm1 , mm0
		movq   qword ptr [edi] , mm1

		add    esi , eax
		add    edi , 32
		dec    ecx
		jnz    INTERPOLATE_HALFPEL_V_4x4_LOOP

        emms
	}
}

static void interpolate_halfpel_hv_h_4x4_sse2(int16_t *dst,uchar *src,long stride,int16_t *mpr)
{
	__asm {
		mov      esi , dword ptr [src]
		mov      edi , dword ptr [dst]
		mov      eax , stride
		mov      ecx , 9
		pxor     mm0 , mm0

INTERPOLATE_HALFPEL_HV_H_4x4_LOOP_1:
		movq     mm1 , qword ptr [esi-2]
		movq     mm2 , qword ptr [esi-1]
		movq     mm3 , qword ptr [esi]
		punpcklbw   mm1 , mm0
		movq     mm4 , qword ptr [esi+1]
		punpcklbw   mm2 , mm0
		movq     mm5 , qword ptr [esi+2]
		punpcklbw   mm3 , mm0
		movq     mm6 , qword ptr [esi+3]
		punpcklbw   mm4 , mm0
		punpcklbw   mm5 , mm0
		paddw    mm3 , mm4
		punpcklbw   mm6 , mm0
		paddw    mm2 , mm5
		psllw    mm3 , 2
		paddw    mm1 , mm6
		psubw    mm3 , mm2
		movq     mm4 , mm3
		psllw    mm3 , 2
		paddw    mm1 , mm4
		paddw    mm1 , mm3
		movq     qword ptr [edi] , mm1

		add      edi , 32
		add      esi , eax
		dec      ecx
		jnz      INTERPOLATE_HALFPEL_HV_H_4x4_LOOP_1

        mov      esi , dword ptr [dst]
		mov      edi , dword ptr [mpr]
		movq     mm0 , qword ptr [esi]
		movq     mm5 , qword ptr [esi+0a0h]
		paddw    mm0 , mm5
		movq     mm1 , qword ptr [esi+020h]
		movq     mm3 , qword ptr [esi+040h]
		movq     mm4 , mm3
		paddw    mm3 , mm5
		movq     mm2 , qword ptr [esi+080h]
		movq     mm5 , qword ptr [esi+0c0h]
		movq     mm6 , mm2
		paddw    mm2 , mm1
		paddw    mm1 , mm5
		movq     mm5 , qword ptr [esi+060h]
		paddw    mm4 , mm5
		paddw    mm5 , mm6
		psubw    mm0 , mm2
		psubw    mm1 , mm3
		psraw    mm0 , 2
		psraw    mm1 , 2		
		psubw    mm2 , mm4
		psubw    mm3 , mm5
		psubsw   mm0 , mm2
		psubsw   mm1 , mm3
		psraw    mm0 , 2
		psraw    mm1 , 2
		paddw    mm0 , mm4
		paddw    mm1 , mm5
		pxor     mm3 , mm3
		paddw    mm0 , qword ptr [thirty_two_64]
		paddw    mm1 , qword ptr [thirty_two_64]
		psraw    mm0 , 6
		psraw    mm1 , 6
		packuswb mm0 , mm1
		movq     mm2 , mm0
		punpcklbw  mm0 , mm3
		punpckhbw  mm2 , mm3
		movq     qword ptr [edi] , mm0
		movq     qword ptr [edi+32] , mm2

		add      esi , 64
		add      edi , 64
		movq     mm0 , qword ptr [esi]
		movq     mm5 , qword ptr [esi+0a0h]
		paddw    mm0 , mm5
		movq     mm1 , qword ptr [esi+020h]
		movq     mm3 , qword ptr [esi+040h]
		movq     mm4 , mm3
		paddw    mm3 , mm5
		movq     mm2 , qword ptr [esi+080h]
		movq     mm5 , qword ptr [esi+0c0h]
		movq     mm6 , mm2
		paddw    mm2 , mm1
		paddw    mm1 , mm5
		movq     mm5 , qword ptr [esi+060h]
		paddw    mm4 , mm5
		paddw    mm5 , mm6
		psubw    mm0 , mm2
		psubw    mm1 , mm3
		psraw    mm0 , 2
		psraw    mm1 , 2		
		psubw    mm2 , mm4
		psubw    mm3 , mm5
		psubsw   mm0 , mm2
		psubsw   mm1 , mm3
		psraw    mm0 , 2
		psraw    mm1 , 2
		paddw    mm0 , mm4
		paddw    mm1 , mm5
		pxor     mm3 , mm3
		paddw    mm0 , qword ptr [thirty_two_64]
		paddw    mm1 , qword ptr [thirty_two_64]
		psraw    mm0 , 6
		psraw    mm1 , 6
		packuswb mm0 , mm1
		movq     mm2 , mm0
		punpcklbw  mm0 , mm3
		punpckhbw  mm2 , mm3
		movq     qword ptr [edi] , mm0
		movq     qword ptr [edi+32] , mm2

		emms
	}
}

static void interpolate_hv_avg_4x4_sse2(int16_t *dst,int16_t *src,long stride)
{
	__asm {
		mov    esi , dword ptr [src]
		mov    edi , dword ptr [dst]
		mov    eax , stride
		pxor   mm0 , mm0
		movq   mm7 , qword ptr [sixteen_64]
		shl    eax , 1

		movq   mm1 , qword ptr [esi]
		movq   mm2 , qword ptr [esi+eax]
		movq   mm3 , qword ptr [edi]
		movq   mm4 , qword ptr [edi+32]
		paddw  mm1 , mm7
		paddw  mm2 , mm7
		psraw  mm1 , 5
		psraw  mm2 , 5
		packuswb mm1 , mm2
		add    esi , eax
		movq   mm5 , mm1
		punpcklbw  mm1 , mm0
		punpckhbw  mm5 , mm0
		pavgw  mm1 , mm3
		pavgw  mm5 , mm4
		add    esi , eax
		movq   qword ptr [edi] , mm1
		movq   qword ptr [edi+32] , mm5

		add    edi , 64
		movq   mm1 , qword ptr [esi]
		movq   mm2 , qword ptr [esi+eax]
		movq   mm3 , qword ptr [edi]
		movq   mm4 , qword ptr [edi+32]
		paddw  mm1 , mm7
		paddw  mm2 , mm7
		psraw  mm1 , 5
		psraw  mm2 , 5
		packuswb mm1 , mm2	
		movq   mm5 , mm1
		punpcklbw  mm1 , mm0
		punpckhbw  mm5 , mm0
		pavgw  mm1 , mm3
		pavgw  mm5 , mm4		
		movq   qword ptr [edi] , mm1
		movq   qword ptr [edi+32] , mm5		

		emms
	}
}

static void interpolate_halfpel_hv_v_4x4_sse2(int16_t *dst,uchar *src,long stride,int16_t *mpr)
{
	__asm {
		mov       esi , dword ptr [src]
		mov       edi , dword ptr [dst]
		mov       eax , stride
		pxor      xmm0 , xmm0
		mov       ebx , eax
		mov       edx , eax
		shl       ebx , 1
		add       edx , ebx
		mov       ecx , 4
        
INTERPOLATE_HALFPEL_HV_V_4x4_LOOP:
		neg       ebx
		neg       eax
	    movdqu    xmm1 , xmmword ptr [esi+ebx]
		movdqu    xmm2 , xmmword ptr [esi+eax]
		neg       ebx
		neg       eax
		movdqu    xmm3 , xmmword ptr [esi]
		movdqa    xmm4 , xmm1
		punpcklbw  xmm1 , xmm0
		movdqa    xmm5 , xmm2
		punpcklbw  xmm2 , xmm0
		movdqa    xmm6 , xmm3
		punpcklbw  xmm3 , xmm0
		punpckhbw  xmm4 , xmm0
		punpckhbw  xmm5 , xmm0
		psllw      xmm3 , 2
		punpckhbw  xmm6 , xmm0
		psubw      xmm3 , xmm2
		psllw      xmm6 , 2
        movdqa     xmm7 , xmm3
		psllw      xmm3 , 2
		psubw      xmm6 , xmm5
		paddw      xmm3 , xmm7
		movdqa     xmm7 , xmm6
		psllw      xmm6 , 2
		paddw      xmm1 , xmm3
		paddw      xmm6 , xmm7
		movdqu     xmm2 , xmmword ptr [esi+edx]
		paddw      xmm4 , xmm6
		movdqa     xmm7 , xmm2
		punpcklbw  xmm2 , xmm0
		punpckhbw  xmm7 , xmm0
		paddw      xmm1 , xmm2
		paddw      xmm4 , xmm7
		movdqu     xmm2 , xmmword ptr [esi+ebx]
		movdqu     xmm3 , xmmword ptr [esi+eax]

        movdqa     xmm5 , xmm2
	    movdqa     xmm6 , xmm3
		punpcklbw  xmm2 , xmm0
		punpckhbw  xmm5 , xmm0
		punpcklbw  xmm3 , xmm0
		punpckhbw  xmm6 , xmm0
		psllw      xmm3 , 2
		psllw      xmm6 , 2
		psubw      xmm3 , xmm2
		psubw      xmm6 , xmm5
		movdqa     xmm7 , xmm3
		psllw      xmm3 , 2
		paddw      xmm3 , xmm7
		movdqa     xmm7 , xmm6
		psllw      xmm6 , 2
        paddw      xmm6 , xmm7
		paddw      xmm1 , xmm3
		paddw      xmm4 , xmm6
		movdqa     xmmword ptr [edi] , xmm1
		movdqa     xmmword ptr [edi+16] , xmm4

		add        esi , eax
        add        edi , 48
		dec        ecx 
		jnz        INTERPOLATE_HALFPEL_HV_V_4x4_LOOP
	
		mov        esi , dword ptr [dst]
		mov        edi , dword ptr [mpr]
		movq       mm1 , qword ptr [esi]
		movq       mm3 , qword ptr [esi+10]
		movq       mm2 , qword ptr [esi+48]
		movq       mm4 , qword ptr [esi+58]
		paddw      mm1 , mm3
		paddw      mm2 , mm4
		movq       mm3 , qword ptr [esi+2]
		movq       mm5 , qword ptr [esi+8]
		movq       mm4 , qword ptr [esi+50]
		movq       mm6 , qword ptr [esi+56]
		paddw      mm3 , mm5
		paddw      mm4 , mm6
		movq       mm5 , qword ptr [esi+4]
		movq       mm7 , qword ptr [esi+6]
		movq       mm6 , qword ptr [esi+52]
		movq       mm0 , qword ptr [esi+54]
		paddw      mm5 , mm7
		paddw      mm6 , mm0
		psubsw     mm1 , mm3
		psubsw     mm2 , mm4
		psubsw     mm3 , mm5
		psubsw     mm4 , mm6
		psraw      mm1 , 2
		psraw      mm2 , 2
		psubsw     mm1 , mm3
		psubsw     mm2 , mm4
		psraw      mm1 , 2
        psraw      mm2 , 2
		paddw      mm1 , mm5
		paddw      mm2 , mm6
		paddw      mm1 , qword ptr [thirty_two_64]
		paddw      mm2 , qword ptr [thirty_two_64]
		psraw      mm1 , 6
		psraw      mm2 , 6
		pxor       mm0 , mm0
		packuswb   mm1 , mm2
		movq       mm3 , mm1
		punpcklbw  mm1 , mm0
		punpckhbw  mm3 , mm0
		movq       xmmword ptr [edi] , mm1
		movq       xmmword ptr [edi+32] , mm3

		add        esi , 96
		add        edi , 64
		movq       mm1 , qword ptr [esi]
		movq       mm3 , qword ptr [esi+10]
		movq       mm2 , qword ptr [esi+48]
		movq       mm4 , qword ptr [esi+58]
		paddw      mm1 , mm3
		paddw      mm2 , mm4
		movq       mm3 , qword ptr [esi+2]
		movq       mm5 , qword ptr [esi+8]
		movq       mm4 , qword ptr [esi+50]
		movq       mm6 , qword ptr [esi+56]
		paddw      mm3 , mm5
		paddw      mm4 , mm6
		movq       mm5 , qword ptr [esi+4]
		movq       mm7 , qword ptr [esi+6]
		movq       mm6 , qword ptr [esi+52]
		movq       mm0 , qword ptr [esi+54]
		paddw      mm5 , mm7
		paddw      mm6 , mm0
		psubsw     mm1 , mm3
		psubsw     mm2 , mm4
		psubsw     mm3 , mm5
		psubsw     mm4 , mm6
		psraw      mm1 , 2
		psraw      mm2 , 2
		psubsw     mm1 , mm3
		psubsw     mm2 , mm4
		psraw      mm1 , 2
        psraw      mm2 , 2
		paddw      mm1 , mm5
		paddw      mm2 , mm6
		paddw      mm1 , qword ptr [thirty_two_64]
		paddw      mm2 , qword ptr [thirty_two_64]
		psraw      mm1 , 6
		psraw      mm2 , 6
		pxor       mm0 , mm0
		packuswb   mm1 , mm2
		movq       mm3 , mm1
		punpcklbw  mm1 , mm0
		punpckhbw  mm3 , mm0
		movq       xmmword ptr [edi] , mm1
		movq       xmmword ptr [edi+32] , mm3      

		emms
	}
}


static void interpolate_quarterpel_v_4x4_sse2(int16_t *dst,uchar *src,long stride)
{
	__asm {
		mov        eax , stride
		mov        esi , dword ptr [src]
		mov        ebx , eax
		mov        edx , eax
		mov        edi , dword ptr [dst]
		shl        ebx , 1
		pxor       mm0 , mm0
		add        edx , ebx
		movq       mm7 , qword ptr [sixteen_64]

		neg        ebx
		neg        eax
		movq       mm1 , qword ptr [esi+ebx]
		movq       mm2 , qword ptr [esi+eax]
		neg        ebx
		neg        eax
		punpcklbw  mm1 , mm0		
		movq       mm3 , qword ptr [esi]
		punpcklbw  mm2 , mm0
		movq       mm4 , qword ptr [esi+eax]
		punpcklbw  mm3 , mm0
		movq       mm5 , qword ptr [esi+ebx]
		punpcklbw  mm4 , mm0
		movq       mm6 , qword ptr [esi+edx]
		punpcklbw  mm5 , mm0
		paddw      mm3 , mm4
		punpcklbw  mm6 , mm0
		paddw      mm2 , mm5
		psllw      mm3 , 2
		paddw      mm1 , mm6
		psubw      mm3 , mm2
		paddw      mm1 , mm7
		add        esi , eax
		paddw      mm1 , mm3
		neg        ebx
		psllw      mm3 , 2
		paddw      mm1 , mm3
		neg        eax
		psraw      mm1 , 5

		movq       mm2 , qword ptr [esi+ebx]
		movq       mm3 , qword ptr [esi+eax]
		neg        ebx
		neg        eax
		punpcklbw  mm2 , mm0
		movq       mm4 , qword ptr [esi]
		punpcklbw  mm3 , mm0
		movq       mm5 , qword ptr [esi+eax]
		punpcklbw  mm4 , mm0
		punpcklbw  mm5 , mm0
		movq       mm6 , qword ptr [esi+ebx]
		paddw      mm4 , mm5
		punpcklbw  mm6 , mm0
		movq       mm5 , qword ptr [esi+edx]
		paddw      mm3 , mm6
		psllw      mm4 , 2
		punpcklbw  mm5 , mm0
		paddw      mm2 , mm7
	    psubw      mm4 , mm3
		paddw      mm2 , mm5
		paddw      mm2 , mm4
		psllw      mm4 , 2
		paddw      mm2 , mm4
		psraw      mm2 , 5

		movq       mm3 , qword ptr [edi]
		packuswb   mm1 , mm2
		movq       mm4 , qword ptr [edi+32]
		movq       mm5 , mm1
		punpcklbw  mm1 , mm0
		punpckhbw  mm5 , mm0
		pavgw      mm1 , mm3
		pavgw      mm5 , mm4
		movq       qword ptr [edi] , mm1
		movq       qword ptr [edi+32] , mm5

		
		add        esi , eax
		add        edi , 64
		neg        ebx 
		neg        eax
		movq       mm1 , qword ptr [esi+ebx]
		movq       mm2 , qword ptr [esi+eax]
		neg        ebx
		neg        eax
		punpcklbw  mm1 , mm0		
		movq       mm3 , qword ptr [esi]
		punpcklbw  mm2 , mm0
		movq       mm4 , qword ptr [esi+eax]
		punpcklbw  mm3 , mm0
		movq       mm5 , qword ptr [esi+ebx]
		punpcklbw  mm4 , mm0
		movq       mm6 , qword ptr [esi+edx]
		punpcklbw  mm5 , mm0
		paddw      mm3 , mm4
		punpcklbw  mm6 , mm0
		paddw      mm2 , mm5
		psllw      mm3 , 2
		paddw      mm1 , mm6
		psubw      mm3 , mm2
		paddw      mm1 , mm7
		add        esi , eax
		paddw      mm1 , mm3
		neg        ebx
		psllw      mm3 , 2
		paddw      mm1 , mm3
		neg        eax
		psraw      mm1 , 5

		movq       mm2 , qword ptr [esi+ebx]
		movq       mm3 , qword ptr [esi+eax]
		neg        ebx
		neg        eax
		punpcklbw  mm2 , mm0
		movq       mm4 , qword ptr [esi]
		punpcklbw  mm3 , mm0
		movq       mm5 , qword ptr [esi+eax]
		punpcklbw  mm4 , mm0
		punpcklbw  mm5 , mm0
		movq       mm6 , qword ptr [esi+ebx]
		paddw      mm4 , mm5
		punpcklbw  mm6 , mm0
		movq       mm5 , qword ptr [esi+edx]
		paddw      mm3 , mm6
		psllw      mm4 , 2
		punpcklbw  mm5 , mm0
		paddw      mm2 , mm7
		psubw      mm4 , mm3
		paddw      mm2 , mm5
		paddw      mm2 , mm4
		psllw      mm4 , 2
		paddw      mm2 , mm4
		psraw      mm2 , 5

		movq       mm3 , qword ptr [edi]
		packuswb   mm1 , mm2
		movq       mm4 , qword ptr [edi+32]
		movq       mm5 , mm1
		punpcklbw  mm1 , mm0
		punpckhbw  mm5 , mm0
		pavgw      mm1 , mm3
		pavgw      mm5 , mm4
		movq       qword ptr [edi] , mm1
		movq       qword ptr [edi+32] , mm5        		
        
		emms
	}
}
*/

/*
void  get_block_4x4_luma_sse2(uchar **img_y,long x_pos,long y_pos,int16_t mpr[24][16],long size_x,long size_y,long idx)
{ 
	long dx,dy;
	long pres_x, pres_y;
	__attribute__((aligned(16))) int16_t tmp_res_hor[21][16];
	__attribute__((aligned(16))) int16_t tmp_res_ver[16][24];
	long i_off,j_off;
	long stride = size_x + PAD_SIZE*2;

    i_off = submb_pos_x[idx]<<2;
	j_off = submb_pos_y[idx]<<2;
	dx = x_pos&0x3;
	dy = y_pos&0x3;

	x_pos = x_pos >> 2;
	y_pos = y_pos >> 2;

	x_pos = CLIP3(-18,size_x+1,x_pos);
	y_pos = CLIP3(-18,size_y+1,y_pos);


	if( dx==0 && dy==0 ){	
        expand8to16_4x4_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos],stride);
	} else if( dy==0 ){		
        interpolate_halfpel_h_4x4_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos],stride);
		if( dx&0x1 ){
			dx = dx>>1;			
			interpolate_avg_4x4_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos+dx],stride);
		}
	} else if( dx==0 ){		
        interpolate_halfpel_v_4x4_sse2(&mpr[j_off][i_off],&img_y[y_pos][x_pos],stride);
		if( dy&0x1 ){
			dy = dy>>1;			
			interpolate_avg_4x4_sse2(&mpr[j_off][i_off],&img_y[y_pos+dy][x_pos],stride);
		}
	} else {
		if( dx==2 ){
			interpolate_halfpel_hv_h_4x4_sse2((int16_t*)tmp_res_hor,&img_y[y_pos-2][x_pos],stride,(int16_t*)&mpr[j_off][i_off]);									
			if( dy & 0x1 ){
				dy = dy>>1;				
				interpolate_hv_avg_4x4_sse2(&mpr[j_off][i_off],&tmp_res_hor[2+dy][0],16);
			}			
		} else if( dy == 2 ){
			interpolate_halfpel_hv_v_4x4_sse2((int16_t*)tmp_res_ver,&img_y[y_pos][x_pos-2],stride,&mpr[j_off][i_off]);		
			dx = dx>>1;		
			interpolate_hv_avg_4x4_sse2(&mpr[j_off][i_off],&tmp_res_ver[0][2+dx],24);
		} else {
            pres_x = x_pos + (dx!=1);
			pres_y = y_pos + (dy!=1);			
            interpolate_halfpel_h_4x4_sse2(&mpr[j_off][i_off],&img_y[pres_y][x_pos],stride);			
			interpolate_quarterpel_v_4x4_sse2(&mpr[j_off][i_off],&img_y[y_pos][pres_x],stride);
		}
	}
}
*/
