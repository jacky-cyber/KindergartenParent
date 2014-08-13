
#include <string.h>
#include "BlendText.h"

#define Y_R_IN			0.257
#define Y_G_IN			0.504
#define Y_B_IN			0.098
#define Y_ADD_IN		16

#define U_R_IN			0.148
#define U_G_IN			0.291
#define U_B_IN			0.439
#define U_ADD_IN		128

#define V_R_IN			0.439
#define V_G_IN			0.368
#define V_B_IN			0.071
#define V_ADD_IN		128

#define SCALEBITS_IN	8
#define FIX_IN(x)		((unsigned short) ((x) * (1L<<SCALEBITS_IN) + 0.5))


void RGB_TO_YUV(unsigned long clRGB, unsigned char *Y, unsigned char *U, unsigned char *V)
{
	unsigned char R = (unsigned char)(clRGB >> 0);
	unsigned char G = (unsigned char)(clRGB >> 8);
	unsigned char B = (unsigned char)(clRGB >> 16);
	*Y = (unsigned char) ((FIX_IN(Y_R_IN) * R + FIX_IN(Y_G_IN) * G + FIX_IN(Y_B_IN) * B) >> SCALEBITS_IN) + 16;
	*U = (unsigned char) ((-FIX_IN(U_R_IN) * R - FIX_IN(U_G_IN) * G + FIX_IN(U_B_IN) * B) >> (SCALEBITS_IN + 2)) + U_ADD_IN;
	*V = (unsigned char) ((FIX_IN(V_R_IN) * R - FIX_IN(V_G_IN) * G - FIX_IN(V_B_IN) * B) >> (SCALEBITS_IN + 2)) + V_ADD_IN;	
}

void OutputImageToStream(IMAGE* img, BLENDIMAGE *pBlendImage)
{
	int edged_width = img->edged_width;
	int height = img->height;
	if (!img || !img->y || !img->u || !img->v) {
		return;
	}
	int h = pBlendImage->nTextHeight;
	int w = (pBlendImage->nTextWidth + 1) & ~1;
	int t = pBlendImage->nTextTop;
	int l = pBlendImage->nTextLeft;
	unsigned char *y = (unsigned char *)(pBlendImage->pTextBits);
	unsigned char *u = y + pBlendImage->nTextHeight * w;
	unsigned char *v = u + pBlendImage->nTextHeight * w / 4;
	int nH = height;
	int nW = edged_width;
	unsigned char YTrans, UTrans, VTrans;
	RGB_TO_YUV (pBlendImage->nTransColor, &YTrans, &UTrans, &VTrans);
	for(int i = 0; i < h; i++) 
	{
		if(t + i >= nH) {
			break;
		}
		int nOffset = l + (t + i) * nW;
		for (int j = 0; j < w; j++) 
		{
			if (*y > YTrans + 0 || *y < YTrans - 0) {
                img->y[nOffset] = *y;
			}
			y ++;
			nOffset ++;
		}
	}
	w >>= 1;
	h >>= 1;
	l >>= 1;
	t >>= 1;
	nH >>= 1;
	nW >>= 1;
	for(int i = 0; i < h; i++)
	{
		if (t + i >= nH) {
			break;
		}
		int nOffset = l + (t + i) * nW;
		for(int j = 0; j < w; j++) 
		{
			if (*u > UTrans + 0 || *u < UTrans - 0) {
                img->u[nOffset] = *u;
			}
			if (*v > VTrans + 0 || *v < VTrans - 0) {
                img->v[nOffset] = *v;
			}
			u ++;
			v ++;
			nOffset ++;
		}
	}
}


inline unsigned char MakeByte(int nImage)
{
	return static_cast<unsigned char>(nImage > 0xFF ? 0xFF : (nImage < 0 ? 0 : nImage));
}
inline unsigned char MakeY(int R, int G, int B)
{
	return MakeByte(((66 * R + 129 * G + 25 * B + 128) >> 8) + 16);
}
inline unsigned char MakeU(int R, int G, int B)
{
	return MakeByte(((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128);
}
inline unsigned char MakeV(int R, int G, int B)
{
	return MakeByte(((112 * R - 94 * G - 18 * B + 128) >> 8) + 128);
}
int RGB_2_YUV420(const unsigned char *pRGB,
	             int nWidth,
	             int nHeight,
	             unsigned char *pY,
	             unsigned char *pU,
	             unsigned char *pV)
{
	if( pRGB == NULL ||
       nWidth <= 0 ||
       nHeight <= 0 ||
       pY == NULL ||
       pU == NULL ||
       pV == NULL){
		return -1;
	}
    
	int *pULine = new int[nWidth];
	int *pVLine = new int[nWidth];
	if(pULine == NULL || pVLine == NULL)
	{
		delete []pULine;
		delete []pVLine;
		return -1;
	}
	memset(pULine, 0, nWidth * sizeof(int));
	memset(pVLine, 0, nWidth * sizeof(int));
    
	const unsigned char *pRGBTemp = pRGB;
	unsigned char *pYTemp = pY;
	unsigned char *pUTemp = pU;
	unsigned char *pVTemp = pV;
	for(long nh = 0; nh < nHeight; ++nh)
	{
		int *pULineTemp = pULine;
		int *pVLineTemp = pVLine;
		for(long nw = 0; nw < nWidth; ++nw)
		{
			*pYTemp = MakeY(pRGBTemp[0], pRGBTemp[1], pRGBTemp[2]);
			*pULineTemp += MakeU(pRGBTemp[0], pRGBTemp[1], pRGBTemp[2]);
			*pVLineTemp += MakeV(pRGBTemp[0], pRGBTemp[1], pRGBTemp[2]);
			++pYTemp;
			++pULineTemp;
			++pVLineTemp;
			pRGBTemp += 3;
		}
		if((nh & 0x01) != 0)
		{
			int *pULineTemp = pULine + 1;
			int *pVLineTemp = pVLine + 1;
			for(long i = 1; i < nWidth; i += 2)
			{
				*pUTemp = MakeByte((pULineTemp[0] + pULineTemp[-1]) / 4);
				*pVTemp = MakeByte((pVLineTemp[0] + pVLineTemp[-1]) / 4);
				++pUTemp;
				++pVTemp;
				pULineTemp += 2;
				pVLineTemp += 2;
			}
            
			//////////////////////////////////////////////////////////////////////////
			// 如果事先保证宽度是偶数,则不需要这一步
			if((nWidth & 0x01) != 0)
			{
				*pUTemp = MakeByte(pULineTemp[-1] / 2);
				*pVTemp = MakeByte(pVLineTemp[-1] / 2);
				++pUTemp;
				++pVTemp;
			}
			//////////////////////////////////////////////////////////////////////////
            
			memset(pULine, 0, nWidth * sizeof(int));
			memset(pVLine, 0, nWidth * sizeof(int));
		}
	}
    
	//////////////////////////////////////////////////////////////////////////
	// 如果事先保证高度是偶数,则不需要这一步
	if((nHeight & 0x01) != 0)
	{
		int *pULineTemp = pULine + 1;
		int *pVLineTemp = pVLine + 1;
		for(long i = 1; i < nWidth; i += 2)
		{
			*pUTemp = MakeByte((pULineTemp[0] + pULineTemp[-1]) / 2);
			*pVTemp = MakeByte((pVLineTemp[0] + pVLineTemp[-1]) / 2);
			++pUTemp;
			++pVTemp;
			pULineTemp += 2;
			pVLineTemp += 2;
		}
		if((nWidth & 0x01) != 0)
		{
			*pUTemp = MakeByte(pULineTemp[-1]);
			*pVTemp = MakeByte(pVLineTemp[-1]);
			++pUTemp;
			++pVTemp;
		}
		memset(pULine, 0, nWidth * sizeof(int));
		memset(pVLine, 0, nWidth * sizeof(int));
	}
	//////////////////////////////////////////////////////////////////////////
    
	delete []pULine;
	delete []pVLine;
	return 0;
}
