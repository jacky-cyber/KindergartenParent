
#include "ImageFlip.h"

void CImageFlip::ImageFlip(ImageFlipType type, YUVImage *img, int edged_width, int width, int height)
{
    switch (type) {
        case IMG_FLIP_MIRRORH:
            ImageMirrorH(img, edged_width, width, height);
            break;
            
        case IMG_FLIP_MIRRORV:
            ImageMirrorV(img, edged_width, width, height);
            break;
            
        case IMG_FLIP_ROTATE:
            ImageRotate(img, edged_width, width, height);
            break;
            
        case IMG_FLIP_NONE:
        default:
            break;
    }
}

// flip an image by mirroring horizontally
void CImageFlip::ImageMirrorH(YUVImage *img, int edged_width, int width, int height)
{
	int i, j;
	int w = width >> 1;
	int pos0, pos1;
	uint8_t*p0, *p1, tmp;

	// adjust y
	pos0 = 0;
    pos1 = width - 1;
	for (i = 0; i < height; i ++ ) 
	{
		p0 = img->y + pos0;
		p1 = img->y + pos1;
		for (j = w; j > 0; j --) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 --;
		}
		pos0 += edged_width;
		pos1 += edged_width;        
	}
	
    // w&h for u&v
	edged_width >>= 1;
	width >>= 1;
	height >>= 1;
	w = width >> 1;
	
    // adjust u
	pos0 = 0;
    pos1 = width - 1;
	for (i = 0; i < height; i ++ ) 
	{
		p0 = img->u + pos0;
		p1 = img->u + pos1;
		for (j = w; j > 0; j --) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 --;
		}
		pos0 += edged_width;
		pos1 += edged_width;        
	}

    // adjust v
	pos0 = 0;
    pos1 = width - 1;
	for (i = 0; i < height; i ++ ) 
	{
		p0 = img->v + pos0;
		p1 = img->v + pos1;
		for (j = w; j > 0; j --) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 --;
		}
		pos0 += edged_width;
		pos1 += edged_width;        
	}	
}

// flip an image by mirroring vertically
void CImageFlip::ImageMirrorV(YUVImage *img, int edged_width, int width, int height)
{
	int i, j;
	int h = height >> 1;
    int pos0, pos1;
	uint8_t *p0, *p1, tmp;
    
	// adjust y
    pos0 = 0;
    pos1 = height * edged_width - edged_width;
	for (i = 0; i < h; i ++ ) 
	{
        p0 = img->y + pos0;
        p1 = img->y + pos1;
		for (j = 0; j < width; j ++) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 ++;
		}
		pos0 += edged_width;
		pos1 -= edged_width;
	}
	
    // w&h for u&v
	edged_width >>= 1;
	width >>= 1;
	height >>= 1;
	h = height >> 1;
    
    // adjust u
    pos0 = 0;
    pos1 = height * edged_width - edged_width;
	for (i = 0; i < h; i ++ ) 
	{
        p0 = img->u + pos0;
        p1 = img->u + pos1;
		for (j = 0; j < width; j ++) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 ++;
		}
		pos0 += edged_width;
		pos1 -= edged_width;
	}
    
    // adjust v
    pos0 = 0;
    pos1 = height * edged_width - edged_width;
	for (i = 0; i < h; i ++ ) 
	{
        p0 = img->v + pos0;
        p1 = img->v + pos1;
		for (j = 0; j < width; j ++) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 ++;
		}
		pos0 += edged_width;
		pos1 -= edged_width;
	}
}

// flip an image by rotating 180 degrees
void CImageFlip::ImageRotate(YUVImage *img, int edged_width, int width, int height)
{
	int i, j;
	int h = height >> 1;
	int pos0, pos1;
	uint8_t*p0, *p1, tmp;

	// adjust y
    pos0 = 0;
    pos1 = height * edged_width - edged_width + width - 1;
	for (i = 0; i < h; i ++ ) 
	{
		p0 = img->y + pos0;
        p1 = img->y + pos1;
		for (j = 0; j < width; j ++) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 --;
		}
		pos0 += edged_width;
		pos1 -= edged_width;
	}
	
    // w&h for u&v
	edged_width >>= 1;
	width >>= 1;
	height >>= 1;
	h = height >> 1;

    // adjust u
    pos0 = 0;
    pos1 = height * edged_width - edged_width + width - 1;
	for (i = 0; i < h; i ++ ) 
	{
		p0 = img->u + pos0;
        p1 = img->u + pos1;
		for (j = 0; j < width; j ++) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 --;
		}
		pos0 += edged_width;
		pos1 -= edged_width;
	}

    // adjust v
    pos0 = 0;
    pos1 = height * edged_width - edged_width + width - 1;
	for (i = 0; i < h; i ++ ) 
	{
		p0 = img->v + pos0;
        p1 = img->v + pos1;
		for (j = 0; j < width; j ++) 
		{
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0 ++;
			p1 --;
		}
		pos0 += edged_width;
		pos1 -= edged_width;
	}
}
