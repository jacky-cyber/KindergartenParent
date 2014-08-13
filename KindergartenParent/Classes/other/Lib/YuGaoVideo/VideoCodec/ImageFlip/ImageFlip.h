

#ifndef __IMAGE_FLIP_H__
#define __IMAGE_FLIP_H__

#include <stdint.h>
#include <stdio.h>

typedef enum {
    IMG_FLIP_NONE    = 0,
    IMG_FLIP_MIRRORH = 1,
    IMG_FLIP_MIRRORV = 2,
    IMG_FLIP_ROTATE  = IMG_FLIP_MIRRORV | IMG_FLIP_MIRRORH,
} ImageFlipType;


typedef struct _YUVImage {
    uint8_t* y;
    uint8_t* u;
    uint8_t* v;
} YUVImage;

class CImageFlip {
public:
    static void ImageFlip(ImageFlipType type, YUVImage *img, int edged_width, int width, int height);
    
private:
    static void ImageMirrorV(YUVImage *img, int edged_width, int width, int height);
    static void ImageMirrorH(YUVImage *img, int edged_width, int width, int height);
    static void ImageRotate (YUVImage *img, int edged_width, int width, int height);
};

#endif
