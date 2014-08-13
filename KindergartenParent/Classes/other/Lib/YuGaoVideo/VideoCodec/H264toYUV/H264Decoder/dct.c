

#include "global.h"
#include "common.h"
#include "dct.h"



#define  MMX_ADDSUB_2(mmr0,mmr1,mmr2,mmr3,mmr4) \
     __asm movq     mmr4 , mmr0                 \
     __asm paddw    mmr0 , mmr2                 \
	 __asm psubw    mmr4 , mmr2                 \
	 __asm movq     mmr2 , mmr1                 \
	 __asm psraw    mmr1 , 1                    \
	 __asm psubw    mmr1 , mmr3                 \
	 __asm psraw    mmr3 , 1                    \
	 __asm paddw    mmr2 , mmr3             


#define  MMX_ADDSUB(mmr0,mmr1,mmr2,mmr3,mmr4)  \
	__asm movq     mmr4 , mmr0                 \
	__asm paddw    mmr0 , mmr3                 \
	__asm psubw    mmr4 , mmr3                 \
	__asm movq     mmr3 , mmr1                 \
	__asm paddw    mmr1 , mmr2                 \
	__asm psubw    mmr3 , mmr2                 


#define  MMX_TRANSPOSE(mmr0,mmr1,mmr2,mmr3,mmr4)  \
	__asm movq         mmr4 , mmr0                \
	__asm punpckhwd    mmr4 , mmr1                \
	__asm punpcklwd    mmr0 , mmr1                \
	__asm movq         mmr1 , mmr2                \
	__asm punpckhwd    mmr1 , mmr3                \
	__asm punpcklwd    mmr2 , mmr3                \
	__asm movq         mmr3 , mmr4                \
	__asm punpckhdq    mmr3 , mmr1                \
	__asm punpckldq    mmr4 , mmr1                \
	__asm movq         mmr1 , mmr0                \
	__asm punpckhdq    mmr1 , mmr2                \
	__asm punpckldq    mmr0 , mmr2


static const long long  one_64 = 0x0001000100010001;
static const long long  thirty_two_64 = 0x0020002000200020;


void idct_4x4_dc_c(int16_t *data,long qp)
{	
	long i;
	long qp_per = qp/6;
	long qp_rem = qp%6;
	int16_t s[4];
	int16_t invdata[16];
	long j;
	for (i = 0; i < 4; i ++){
	    j = i << 2;
        s[0] = data[j] + data[j + 2];
        s[1] = data[j] - data[j + 2];
        s[2] = data[j + 1] - data[j + 3];
        s[3] = data[j + 1] + data[j + 3];

        invdata[i] = s[0] + s[3];
        invdata[i + 12] = s[0] - s[3];
        invdata[i + 4] = s[1] + s[2];
        invdata[i + 8] = s[1] - s[2];
    }
	for (i = 0; i < 4; i ++){
		j = i << 2;
        s[0] = invdata[j] + invdata[j + 2];
        s[1] = invdata[j] - invdata[j + 2];
        s[2] = invdata[j + 1] - invdata[j + 3];
        s[3] = invdata[j + 1] + invdata[j + 3];

        data[j] = (((s[0] + s[3])*dequant_coef[qp_rem][0][0]<<qp_per)+2)>>2;
        data[j + 3] = (((s[0] - s[3])*dequant_coef[qp_rem][0][0]<<qp_per)+2)>>2;
        data[j + 1] = (((s[1] + s[2])*dequant_coef[qp_rem][0][0]<<qp_per)+2)>>2;
        data[j + 2] = (((s[1] - s[2])*dequant_coef[qp_rem][0][0]<<qp_per)+2)>>2;
	}
}

void iquant_4x4_c(int16_t *data,long qp_per,long qp_rem)
{
    int16_t *dequant_table = (int16_t *)dequant_coef[qp_rem];
	long i;
	long j;
	data[0] = (data[0] * dequant_table[0]) << qp_per;
	j = 1;
	for(i = 1; i < 16; i++){
		if(!(i&3)){
			j += 12;
		}
		data[j] = (data[j] * dequant_table[i]) << qp_per;
		j++;
	}   
}



void  idct_4x4_c(int16_t *data)
{
	long i;
	long j;
	long j4;
	int16_t s[4];
	int16_t invdata[16];
	for (i = 0; i < 4; i++){
	    j = i << 4;
        s[0] = data[j] + data[j + 2];
        s[1] = data[j] - data[j + 2];
        s[2] = (data[j + 1] >> 1) - data[j + 3];
        s[3] = data[j + 1] + (data[j + 3] >> 1);
		invdata[i] = s[0] + s[3];
        invdata[i + 12] = s[0] - s[3];
        invdata[i + 4] = s[1] + s[2];
        invdata[i + 8] = s[1] - s[2];
    }
	for (i = 0; i < 4; i ++){
		j = i << 2;
		j4 = i << 4;
        s[0] = invdata[j] + invdata[j + 2];
        s[1] = invdata[j] - invdata[j + 2];
        s[2] = (invdata[j + 1] >> 1) - invdata[j + 3];
        s[3] = invdata[j + 1] + (invdata[j + 3] >> 1);
		data[j4] = ((s[0] + s[3]) + 32) >> 6;
        data[j4 + 3] = ((s[0] - s[3]) + 32) >> 6;
        data[j4 + 1] = ((s[1] + s[2]) + 32) >> 6;
        data[j4 + 2] = ((s[1] - s[2]) + 32) >> 6;
	}
}



void  idct_2x2_dc_c(int16_t *data,long qp_per_uv,long qp_rem_uv)
{
	int16_t s[4];
	long i;
	s[0] = (data[0] + data[1] + data[2] + data[3]);
	s[1] = (data[0] - data[1] + data[2] - data[3]);
	s[2] = (data[0] + data[1] - data[2] - data[3]);
	s[3] = (data[0] - data[1] - data[2] + data[3]);
	for (i=0;i<4;i++)
		data[i] = ((s[i] * dequant_coef[qp_rem_uv][0][0])<<qp_per_uv) >> 1;
}

void memcpy_4x4_dc_c(int16_t *data,int16_t dc)
{
	long i;
	long j;
	data[0] = (dc + 32) >> 6;
	j = 1;
	for(i = 1; i < 16; i++){
		if(!(i&3))
			j += 12;
		data[j] = (dc + 32) >> 6;
		j++;
	}
}

void iquant_4x4_mmx(int16_t *data,long qp_per,long qp_rem)
{
    /*
	int16_t *dequant_table = (int16_t*)dequant_coef[qp_rem];
	__asm {
        mov esi , dword ptr [data]
        mov ebx , dword ptr [dequant_table]
		mov eax , qp_per

		movq mm7 , one_64
        movd mm4 , eax
		psllw mm7 , mm4
		movq mm0 , qword ptr [esi]
		movq mm1 , qword ptr [ebx]
		movq mm2 , qword ptr [esi+32]
		movq mm3 , qword ptr [ebx+8]		
		pmullw mm0 , mm1
		pmullw mm2 , mm3
		pmullw mm0 , mm7
		pmullw mm2 , mm7
		movq qword ptr [esi] , mm0
		movq qword ptr [esi+32] , mm2
        
		movq mm0 , qword ptr [esi+64]
		movq mm1 , qword ptr [ebx+16]
		movq mm2 , qword ptr [esi+96]
		movq mm3 , qword ptr [ebx+24]
		pmullw mm0 , mm1
        pmullw mm2 , mm3
		pmullw mm0 , mm7
		pmullw mm2 , mm7
		movq qword ptr [esi+64] , mm0
		movq qword ptr [esi+96] , mm2

		emms
	}
     
     */
}

void  idct_4x4_mmx(int16_t *data)
{
    /*
	__asm {
		mov   eax , dword ptr [data] 
		movq  mm0 , qword ptr [eax]
		movq  mm1 , qword ptr [eax+32]
		movq  mm2 , qword ptr [eax+64]
		movq  mm3 , qword ptr [eax+96]
		movq  mm5 , thirty_two_64
		MMX_ADDSUB_2(mm0,mm1,mm2,mm3,mm4)  ;  mm0 mm4 mm1 mm2
		MMX_ADDSUB(mm0,mm4,mm1,mm2,mm3)    ;  mm0 mm4 mm2 mm3
		MMX_TRANSPOSE(mm0,mm4,mm2,mm3,mm1) ;  mm0 mm4 mm1 mm3
		MMX_ADDSUB_2(mm0,mm4,mm1,mm3,mm2)  ;  mm0 mm2 mm4 mm1
		MMX_ADDSUB(mm0,mm2,mm4,mm1,mm3)    ;  mm0 mm2 mm1 mm3
		paddw  mm0 , mm5
		paddw  mm2 , mm5
		paddw  mm1 , mm5
		paddw  mm3 , mm5
		psraw  mm0 , 6
		psraw  mm2 , 6
		psraw  mm1 , 6
		psraw  mm3 , 6
		movq   qword ptr [eax] , mm0
		movq   qword ptr [eax+32] , mm2
		movq   qword ptr [eax+64] , mm1
		movq   qword ptr [eax+96] , mm3
		emms
	}
    
    */
}

void  idct_4x4_dc_mmx(int16_t *data,long qp)
{
    /*
    long qp_per , qp_rem;

	qp_per = qp/6;
	qp_rem = qp%6;

	__asm {
		mov   eax , dword ptr [data]
		movq  mm0 , qword ptr [eax]
		movq  mm1 , qword ptr [eax+8]
		movq  mm2 , qword ptr [eax+16]
		movq  mm3 , qword ptr [eax+24]
        MMX_ADDSUB(mm0,mm1,mm3,mm2,mm4)
		MMX_ADDSUB(mm0,mm4,mm2,mm1,mm3)
		MMX_TRANSPOSE(mm0,mm4,mm1,mm3,mm2)
		MMX_ADDSUB(mm0,mm4,mm3,mm2,mm1)
		MMX_ADDSUB(mm0,mm1,mm2,mm4,mm3)
		movq  qword ptr [eax] , mm0
		movq  qword ptr [eax+8] , mm1
		movq  qword ptr [eax+16] , mm4
		movq  qword ptr [eax+24] , mm3
		emms
	}

	data[0] = (((data[0] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[1] = (((data[1] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[2] = (((data[2] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[3] = (((data[3] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[4] = (((data[4] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[5] = (((data[5] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[6] = (((data[6] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[7] = (((data[7] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[8] = (((data[8] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[9] = (((data[9] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[10] = (((data[10] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[11] = (((data[11] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[12] = (((data[12] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[13] = (((data[13] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[14] = (((data[14] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
	data[15] = (((data[15] * dequant_coef[qp_rem][0][0])<<qp_per)+2)>>2;
     
     */
}
