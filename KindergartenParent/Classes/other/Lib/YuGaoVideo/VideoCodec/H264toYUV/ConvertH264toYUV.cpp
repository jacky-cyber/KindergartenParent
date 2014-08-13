/*
 *  ConvertH264toYUV.c
 *  Camera
 *
 *  Created by apple on 11-1-18.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "ConvertH264toYUV.h"

int ConvertH264toYUV(h264_dec_handle_t handle, unsigned char *pH264Data, int nH264Len, 
                     unsigned char *pYUVData,   int *pYUVLen, 
                     int *pWidth, int *pHeight, int *pTotalY, int *pTotalUV)
{
    int frameCount = 0;
    int nYUVDataLen = 0;

    //h264_dec_handle_t  handle;
	h264_dec_param_t frame;
    
    debugPrintf("---------------------ConvertH264toYUV start--------------------\n");

	//handle = h264_dec_open();
    
	frame.nal    = pH264Data;
	frame.nalLen = nH264Len;
	while ( frame.nalLen > 0 ){
       
        h264_dec_decode(handle,&frame);
        
		if( frame.img[0] ){ 
			int i;
            unsigned char *p;
			p = frame.img[0];
		 	for( i=0;i<frame.height;i++ ){
                memcpy(pYUVData + nYUVDataLen, p, frame.width);
                nYUVDataLen += frame.width;
                p += frame.stride;
			}
			p = frame.img[1]; 
			for( i=0;i<frame.height/2;i++ ){
                memcpy(pYUVData + nYUVDataLen, p, frame.width/2);
                nYUVDataLen += frame.width/2;
                p += frame.stride/2;
			}
			p = frame.img[2];
			for( i=0;i<frame.height/2;i++ ){
                memcpy(pYUVData + nYUVDataLen, p, frame.width/2);
                nYUVDataLen += frame.width/2;
                p += frame.stride/2;
            }			
			frameCount ++;
		}
	}
	
	frame.nal = 0;
	frame.nalLen = 0;
	do {
        
        h264_dec_decode(handle,&frame);
        
		if( frame.img[0] ){ 
			int i;
            unsigned char *p;
			p = frame.img[0];
			for( i=0;i<frame.height;i++ ){
                memcpy(pYUVData + nYUVDataLen, p, frame.width);
                nYUVDataLen += frame.width;
				p += frame.stride;
			}
			p = frame.img[1];
			for( i=0;i<frame.height/2;i++ ){
                memcpy(pYUVData + nYUVDataLen, p, frame.width/2);
                nYUVDataLen += frame.width/2;
                p += frame.stride/2;
			}
			p = frame.img[2];
			for( i=0;i<frame.height/2;i++ ){
                memcpy(pYUVData + nYUVDataLen, p, frame.width/2);
                nYUVDataLen += frame.width/2;
                p += frame.stride/2;
			}			
			frameCount ++;
		}		
	} while (frame.img[0]!=0 );
    
    //h264_dec_close(handle);
    
    if (pWidth != NULL) {
        *pWidth   = frame.width;        
    }
    if (pHeight != NULL) {
        *pHeight  = frame.height;
    }
    if (pTotalY != NULL) {
        *pTotalY  = frame.stride;
    }
    if (pTotalUV != NULL) {
        *pTotalUV = frame.stride / 2;
    }
    if (pYUVLen != NULL) {
        *pYUVLen  = nYUVDataLen;
    }
    
    //int nFrameLen = frame.width * frame.height * 1.5;    
    //debugPrintf("ConvertH264toYUV: nYUVDataLen = %d; nFrameLen = %d. frameCount = %d.\n", nYUVDataLen, nFrameLen, frameCount);
    debugPrintf("ConvertH264toYUV: width = %ld; heigth = %ld; total_y =  %ld; total_uv = %ld.\n", frame.width,  frame.height, frame.stride, frame.stride / 2);
    
    debugPrintf("----------------------ConvertH264toYUV End-------------------\n\n");
    
	return 0;
}
