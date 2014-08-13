/*
 *  rgb2bmp.cpp
 *  Camera
 *
 *  Created by apple on 11-1-21.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#pragma pack(2)     // #define BMPHEADER_LEN    54  // sizeof(BITMAPFILEHEADER) = 14; sizeof(BITMAPINFOHEADER) = 40

#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "rgb2bmp.h"
#include "rgb2bmpDef.h"

void FillBMPHeader(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, int width, int height);

int ConvertRGBtoBMP(char* filename, unsigned char* BMPData, int width, int height, unsigned char* pRGBData)
{
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;    
    
    FillBMPHeader(&bitmapFileHeader, &bitmapInfoHeader, width, height);
    
    int index = 0;
    memcpy(BMPData + index, &bitmapFileHeader, sizeof(BITMAPFILEHEADER));
    index += sizeof(BITMAPFILEHEADER);
    memcpy(BMPData + index, &bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
    index += sizeof(BITMAPINFOHEADER);
    memcpy(BMPData + index, pRGBData, bitmapInfoHeader.biSizeImage);
    index += bitmapInfoHeader.biSizeImage;
        
    return 1;
}

unsigned char* FillBMPHeaderWithRGB(unsigned char* pBMPData, int width, int height)
{    
    if (pBMPData == NULL) {
        return pBMPData;
    }
    
    BITMAPFILEHEADER *bitmapFileHeader = (BITMAPFILEHEADER *)pBMPData;
    BITMAPINFOHEADER *bitmapInfoHeader = (BITMAPINFOHEADER *)(pBMPData+sizeof(BITMAPFILEHEADER));
    FillBMPHeader(bitmapFileHeader, bitmapInfoHeader, width, height);
    
    /*
    static bool isSave = false;
    if (!isSave) {
        // write rgb files
        char *filename = "/Users/apple/test/test.rgb";
        FILE *filePtr = fopen(filename, "wb");
        if(filePtr == NULL) { debugPrintf("filePtr==NULL\n"); return 0; }
        int n = fwrite(pBMPData+54, 1, width*height*3, filePtr);
        fclose(filePtr);
        debugPrintf("ConvertRGBtoBMP: biSizeImage fwriteLen = %d.\n", n);
        
        // write bmp files
        char *filename2 = "/Users/apple/test/test.bmp";
        FILE *filePtr2 = fopen(filename2, "wb");
        if(filePtr2 == NULL) { debugPrintf("filePtr2==NULL\n"); return 0; }
        int n2 = fwrite(pBMPData, 1, width*height*3+54, filePtr2);
        fclose(filePtr2);
        debugPrintf("ConvertRGBtoBMP2: biSizeImage fwriteLen = %d.\n", n2);
        
        isSave = true;
    }
    */
    
    return pBMPData;
}

void FillBMPHeader(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, int width, int height)
{
    unsigned int widthStep  = (((width * 24) + 31) & (~31)) / 8;  /////---- == width*3
    
    // bitmapFileHeader
    //bitmapFileHeader.bfType = 0x4D42;
    bitmapFileHeader->bfType = (('M' << 8) | 'B'); /////----'BM'
    //bitmapFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width*height*3;
    bitmapFileHeader->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + widthStep * height; /////----
    bitmapFileHeader->bfReserved1 = 0;
    bitmapFileHeader->bfReserved2 = 0;
    bitmapFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // bitmapInfoHeader
    bitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfoHeader->biPlanes = 1;
    bitmapInfoHeader->biBitCount = 24;
    bitmapInfoHeader->biCompression = BI_RGB;
    //bitmapInfoHeader->biSizeImage = width*height*3;
    bitmapInfoHeader->biSizeImage = widthStep*height;   /////----
    bitmapInfoHeader->biXPelsPerMeter = 0;
    bitmapInfoHeader->biYPelsPerMeter = 0;
    bitmapInfoHeader->biClrImportant  = 0;
    bitmapInfoHeader->biClrUsed = 0;
    bitmapInfoHeader->biWidth  = width;
    bitmapInfoHeader->biHeight = height;
}
