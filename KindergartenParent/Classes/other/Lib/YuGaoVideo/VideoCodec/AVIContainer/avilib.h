#ifndef _AVILIB_
#define _AVILIB_
#pragma warning(disable : 4786)
#pragma warning(disable : 4244)

#include <stdlib.h>
#include <stdio.h>
#include <vector>

#pragma pack(1)

#undef  MAKEFOURCC
#define MAKEFOURCC(a,b,c,d)  ((unsigned long)a | ((unsigned long)b<<8) | ((unsigned long)c<<16) | ((unsigned long)d<<24) )


#define RIFFtag MAKEFOURCC('R','I','F','F') 
#define LISTtag MAKEFOURCC('L','I','S','T') 
#define JUNKtag MAKEFOURCC('J','U','N','K') 
#define AVItag  MAKEFOURCC('A','V','I',' ') 
#define hdrltag MAKEFOURCC('h','d','r','l') 
#define avihtag MAKEFOURCC('a','v','i','h') 
#define strltag MAKEFOURCC('s','t','r','l') 
#define strhtag MAKEFOURCC('s','t','r','h') 
#define strftag MAKEFOURCC('s','t','r','f') 
#define movitag MAKEFOURCC('m','o','v','i') 
#define rectag  MAKEFOURCC('r','e','c',' ') 
#define vidstag MAKEFOURCC('v','i','d','s') 
#define audstag MAKEFOURCC('a','u','d','s') 
#define Tag00db MAKEFOURCC('0','0','d','b') 
#define Tag00dc MAKEFOURCC('0','0','d','c') 
#define Tag01wb MAKEFOURCC('0','1','w','b') 
 
/* possible video formats */ 
#define TagMJPG MAKEFOURCC('M','J','P','G') 
#define Tagmjpg MAKEFOURCC('m','j','p','g') 
#define TagDIB  MAKEFOURCC('D','I','B',' ') 
#define TagRGB  MAKEFOURCC('R','G','B',' ') 
#define TagYVU9 MAKEFOURCC('Y','V','U','9') 
#define idx1tag MAKEFOURCC('i','d','x','1')

#define xvidtag   MAKEFOURCC('x','v','i','d')
#define XVIDtag   MAKEFOURCC('X','V','I','D')
#define tsxvidtag MAKEFOURCC('t','s','x','d')
#define TSXVIDtag MAKEFOURCC('T','S','X','D')

#define Tshdtag   MAKEFOURCC('t','s','h','d')
#define TSHDtag   MAKEFOURCC('T','S','H','D')
#define h264tag   MAKEFOURCC('h','2','6','4')
#define H264tag   MAKEFOURCC('H','2','6','4')


typedef struct {
	unsigned long fccID;
	unsigned long dwSize;
	unsigned long fccType;
} tChunkHeader;

typedef struct {
	unsigned long  fccID; 
	unsigned long  dwSize; 
	unsigned long  dwMicroSecPerFrame; 
	unsigned long  dwMaxBytesPerSec; 
	unsigned long  dwReserved1; 
	unsigned long  dwFlags; 
	unsigned long  dwTotalFrames; 
	unsigned long  dwInitialFrames; 
	unsigned long  dwStreams; 
	unsigned long  dwSuggestedBufferSize; 
	unsigned long  dwWidth; 
	unsigned long  dwHeight; 
	unsigned long  dwReserved2a; 
	unsigned long  dwReserved2b; 
	unsigned long  dwReserved2c; 
	unsigned long  dwReserved2d; 
} tMainAVIHeader;

typedef struct { 
	unsigned long fccID; 
	unsigned long  dwSize; 
	unsigned long fccType; 
	unsigned long fccHandler; 
	unsigned long dwFlags; 
	unsigned long dwReserved1; 
	unsigned long dwInitialFrames; 
	unsigned long dwScale; 
	unsigned long dwRate; 
	unsigned long dwStart; 
	unsigned long dwLength; 
	unsigned long dwSuggestedBufferSize; 
	unsigned long dwQuality; 
	unsigned long dwSampleSize; 
	unsigned long dwReserved2; 
	unsigned short  wSizeX; 
	unsigned short  wSizeY; 
} tAVIStreamHeader; 

typedef struct { 
  unsigned long  fccID; 
  unsigned long  dwSize; 
  unsigned long  biSize; 
  long   biWidth; 
  long   biHeight; 
  unsigned short  biPlanes; 
  unsigned short  biBitCount; 
  unsigned long  biCompression; 
  unsigned long  biSizeImage; 
  long   biXPelsPerMeter; 
  long   biYPelsPerMeter; 
  unsigned long  biClrUsed; 
  unsigned long  biClrImportant; 
} tBitmapInfoHeader; 


typedef struct {
  unsigned long fccID;
  unsigned long dwSize;
  unsigned short format;
  unsigned short channels;
  unsigned long rate;
  unsigned long av_bps;
  unsigned short blockalign;
  unsigned short bitspersample;
  unsigned short cbSize;
} tAudioInfoHeader ;
 
typedef struct { 
  unsigned long fccID; 
  unsigned long  dwSize; 
} tMovieHeader; 

typedef struct {
	unsigned long ckid;
	unsigned long flag;
	unsigned long offset;
	unsigned long length;
} tIndexEntry;

#pragma pack()

class AVIWriter
{
public:
    long openAVI(const char* pName, long x ,long y, float fps, 
		short ch,short baudio,long rate,int nVideoFormat);

    void SetVideoSize (long x ,long y);
    void SetWaveFormat (int nWaveFormat, int nChannelCount, int nAudioRate);

    void writeFrame(unsigned char *frame,long size,bool bKeyframe);
	void writeFrame(unsigned char *framepre,long sizepre,unsigned char *frame,long size,bool bKeyframe);
    void writeAudio(unsigned char *frame,long size);

    void closeAVI();
	long curSize();
	inline float GetFps () const {return fps;}
	inline void SetFps (float fFps) {fps = fFps;}
	inline bool IsOpen () const {return fp? true : false;}

protected:
    void  createIndex();
	void  WriteHeader (int nFileSize = 0);
	void  WriteFrame (unsigned char *framepre, long sizepre, unsigned char *frame, long size, bool bVideo, bool bKeyframe);

private:
	FILE *fp ;
    long  counter ;
    long  audio_counter ;
    long  total_size ;
	float    fps;
	long  width;
	long  height;
	long	 nVideoFormat;
	long  nAudioChannel;
	long  nAudioRate;
    long  m_nWaveFormat;

	std::vector<tIndexEntry>  indexlist;
public:
	AVIWriter();
	virtual ~AVIWriter();

};


#endif
