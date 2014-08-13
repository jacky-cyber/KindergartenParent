#include "avilib.h"
#define DEFAULFFPS 15

AVIWriter::AVIWriter()
{
	fp = 0;
	total_size=0;
	counter = 0;
	audio_counter = 0;
	width = 0;
	height = 0;
	nVideoFormat = 0;
	nAudioChannel = 0;
	nAudioRate = 0;
	m_nWaveFormat = 0x0031;//WAVE_FORMAT_GSM610;
}

AVIWriter::~AVIWriter()
{
	if( fp ){
        closeAVI();
	}
}


long AVIWriter::openAVI(const char* pName,long x ,long y, float fps,short ch,
						short baudio,long rate,int nVideoFormat)
{	
	if( fp !=  0 ){
		closeAVI();
	}
	fp = fopen(pName, "wb");	
	if( NULL == fp ){
		return -1;
	} 
	width = x;
	height = y;
	this->fps = fps;
	this->nVideoFormat = nVideoFormat;
	nAudioChannel = ch;
	nAudioRate = rate;
	WriteHeader();
	indexlist.clear();
	return 0;
}

void AVIWriter::SetVideoSize (long x ,long y)
{
	width = x;
	height = y;
}

void AVIWriter::SetWaveFormat (int nWaveFormat, int nChannelCount, int nAudioRate)
{
    m_nWaveFormat = nWaveFormat;
	this->nAudioChannel = nChannelCount;
	this->nAudioRate = nAudioRate;
}

void AVIWriter::WriteFrame(unsigned char *framepre, long sizepre, 
						   unsigned char *frame, long size, 
						   bool bVideo, bool bKeyframe)
{
	if( NULL == fp ){
		return;
	}

	long pos = ftell(fp);
	if(pos & 0x01)
	{ 
	    unsigned char c = 0;
		fwrite(&c, 1, 1, fp);
	    total_size++;
	}

	tMovieHeader MovieHeader;
    MovieHeader.fccID = bVideo? (bKeyframe ? Tag00db : Tag00dc) : Tag01wb;
	MovieHeader.dwSize = size + sizepre;
	fwrite(&MovieHeader, sizeof(MovieHeader), 1, fp);
	total_size += sizeof(MovieHeader);
	fwrite(framepre, sizepre, 1, fp);
	total_size += sizepre;
	fwrite(frame, size, 1, fp);
	total_size += size;

	tIndexEntry IndexEntry;
    IndexEntry.ckid = bVideo? (bKeyframe ? Tag00db : Tag00dc) : Tag01wb;
    IndexEntry.flag = bVideo? (bKeyframe ? 0x10 : 0x0) : 0x10;
	IndexEntry.length = size + sizepre;
	if(indexlist.size() != 0)
	{
		IndexEntry.offset = indexlist[indexlist.size()-1].offset + 
			                indexlist[indexlist.size()-1].length + sizeof(MovieHeader);
		if(indexlist[indexlist.size()-1].length  & 0x01 ){
			IndexEntry.offset += 1; //pad
		}
	} 
	else 
	{
		IndexEntry.offset = 0x4;
	}	
	indexlist.push_back(IndexEntry);
}

void AVIWriter::writeFrame(unsigned char *frame,long size,bool bKeyframe)
{
	WriteFrame(0, 0, frame, size, true, bKeyframe);
	counter ++;
}

void AVIWriter::writeFrame(unsigned char *framepre,long sizepre,unsigned char *frame,long size,bool bKeyframe)
{
	WriteFrame(framepre, sizepre, frame, size, true, bKeyframe);
	counter++;
}

void AVIWriter::writeAudio(unsigned char *frame,long size)
{
	WriteFrame(0, 0, frame, size, false, true);
	audio_counter++;
}

void AVIWriter::closeAVI()
{
	if( 0 == fp ){
		return;
	}
    createIndex();
	long filesize;
	fflush(fp);
    filesize = ftell(fp);
	WriteHeader (filesize);
	fclose(fp);
	fp = 0;
	total_size=0;
	counter = 0;
	audio_counter = 0;
	width = 0;
	height = 0;
	nVideoFormat = 0;
	nAudioChannel = 0;
	nAudioRate = 0;
}

void AVIWriter::WriteHeader (int nFileSize)
{
	fseek(fp, 0, SEEK_SET);
	tChunkHeader ChunkHeader;
	ChunkHeader.fccID = RIFFtag;
	ChunkHeader.dwSize = nFileSize;
	ChunkHeader.fccType = AVItag;
	fwrite(&ChunkHeader,sizeof(ChunkHeader),1,fp);
	ChunkHeader.fccID = LISTtag;
	//  later
	ChunkHeader.dwSize = 4+sizeof(tMainAVIHeader)+sizeof(tChunkHeader)+
                         sizeof(tAVIStreamHeader)+sizeof(tBitmapInfoHeader);
	ChunkHeader.dwSize += sizeof(tChunkHeader) + sizeof(tAVIStreamHeader)+
						sizeof(tAudioInfoHeader);
	                         
	ChunkHeader.fccType = hdrltag;
	fwrite(&ChunkHeader,sizeof(ChunkHeader),1,fp);

    tMainAVIHeader MainAVIHeader = {0};

    MainAVIHeader.fccID = avihtag; 
    MainAVIHeader.dwSize = sizeof(MainAVIHeader)-8;  
	float fWriteFPS = fps? fps : DEFAULFFPS;
    MainAVIHeader.dwMicroSecPerFrame = (unsigned long)(1000000 / fWriteFPS);
    MainAVIHeader.dwFlags = 0x0810; 
    MainAVIHeader.dwTotalFrames = counter;// + audio_counter;	//Video Frames 
    MainAVIHeader.dwStreams = 2; 
    MainAVIHeader.dwSuggestedBufferSize = (3*width*height); 
    MainAVIHeader.dwWidth = width; 
    MainAVIHeader.dwHeight = height; 

	fwrite(&MainAVIHeader, sizeof(MainAVIHeader), 1, fp);//12 + 12 + 64

	ChunkHeader.fccID   = LISTtag; 
    ChunkHeader.dwSize  = sizeof(tAVIStreamHeader) + sizeof(tBitmapInfoHeader) + 4; 
    ChunkHeader.fccType = strltag; 
    fwrite(&ChunkHeader,sizeof(ChunkHeader),1,fp);	//100

    tAVIStreamHeader  AVIStreamHeader = {0};
	AVIStreamHeader.fccID = strhtag;
	AVIStreamHeader.dwSize = sizeof(AVIStreamHeader) - 8;
	AVIStreamHeader.fccType = vidstag;
	switch(nVideoFormat) {
	case 0:
		AVIStreamHeader.fccHandler = xvidtag;
		break;
	case 1:
		AVIStreamHeader.fccHandler = tsxvidtag;
		break;
	case 2:
		AVIStreamHeader.fccHandler = Tshdtag;
        break;
    case 3:
		AVIStreamHeader.fccHandler = h264tag;
        break;
	}
    AVIStreamHeader.dwScale = 100; 
    AVIStreamHeader.dwRate = (unsigned long)(100 * fWriteFPS + 0.5); 
    AVIStreamHeader.dwLength = counter; //140
    AVIStreamHeader.dwSuggestedBufferSize = (3*width*height); 
    AVIStreamHeader.wSizeX = width; 
    AVIStreamHeader.wSizeY = height; 

	fwrite(&AVIStreamHeader,sizeof(AVIStreamHeader),1,fp);//164

    tBitmapInfoHeader BitmapInfoHeader = {0};
	BitmapInfoHeader.fccID = strftag;
	BitmapInfoHeader.dwSize = sizeof(BitmapInfoHeader)-8; 
	BitmapInfoHeader.biSize = sizeof(BitmapInfoHeader)-8; 
	BitmapInfoHeader.biWidth = width; 
	BitmapInfoHeader.biHeight = height; 
	BitmapInfoHeader.biPlanes = 1; 
	BitmapInfoHeader.biBitCount = 24; 
	switch(nVideoFormat) {
	case 0:
		BitmapInfoHeader.biCompression = xvidtag;
		break;
	case 1:
		BitmapInfoHeader.biCompression = tsxvidtag;
		break;
	case 2:
		BitmapInfoHeader.biCompression = Tshdtag;
        break;
    case 3:
		BitmapInfoHeader.biCompression = h264tag;
        break;
	}
	BitmapInfoHeader.biSizeImage = width * height * 3; 
	fwrite(&BitmapInfoHeader,sizeof(BitmapInfoHeader),1,fp);//164 + 48
	
	ChunkHeader.fccID   = LISTtag; 
	ChunkHeader.dwSize  = sizeof(tAVIStreamHeader)+sizeof(tAudioInfoHeader)+4; 
	ChunkHeader.fccType = strltag; 
	fwrite(&ChunkHeader, sizeof(ChunkHeader),1,fp); //224
	
	AVIStreamHeader.fccID = strhtag; 
	AVIStreamHeader.dwSize = sizeof(AVIStreamHeader)-8; 
	AVIStreamHeader.fccType = audstag; 
	AVIStreamHeader.fccHandler = 0;
	AVIStreamHeader.dwFlags = 0; 
	AVIStreamHeader.dwReserved1 = 0; 
	AVIStreamHeader.dwInitialFrames = 0; 
	AVIStreamHeader.dwScale = nAudioChannel * 2 ;
	AVIStreamHeader.dwRate = nAudioChannel * 2 * nAudioRate;// * 33 / 320 ; 
	AVIStreamHeader.dwStart = 0; 
	AVIStreamHeader.dwLength = audio_counter; //audio counter
	AVIStreamHeader.dwSuggestedBufferSize = 0; 
	AVIStreamHeader.dwQuality = 0; 
	AVIStreamHeader.dwSampleSize = 0;
	AVIStreamHeader.dwReserved2 = 0; 
	AVIStreamHeader.wSizeX = 0; 
	AVIStreamHeader.wSizeY = 0; 
	fwrite(&AVIStreamHeader, sizeof(AVIStreamHeader),1,fp); //224 + 64
	
	tAudioInfoHeader AudioInfoHeader;

	AudioInfoHeader.fccID = strftag ;
	AudioInfoHeader.dwSize = sizeof(AudioInfoHeader)-8 ;
	AudioInfoHeader.format = m_nWaveFormat;
	AudioInfoHeader.channels = nAudioChannel ;
	AudioInfoHeader.rate = nAudioRate ;
	AudioInfoHeader.av_bps = nAudioRate * nAudioChannel * 2 ;
	AudioInfoHeader.blockalign = 2;
	AudioInfoHeader.bitspersample = 16;
	AudioInfoHeader.cbSize = 0 ;
	
	fwrite(&AudioInfoHeader, sizeof(AudioInfoHeader),1,fp); //288 + 26

	ChunkHeader.fccID   = LISTtag; 
    ChunkHeader.dwSize  = total_size + 4;		//totol size + 4
    ChunkHeader.fccType = movitag; 
    fwrite(&ChunkHeader, sizeof(ChunkHeader),1,fp); //
}

void AVIWriter::createIndex()
{
	unsigned char c = 0;
	long pos = ftell(fp);
	if( pos & 0x01 ){
		fwrite(&c,1,1,fp);
	}
    tMovieHeader MovieHeader;
	MovieHeader.fccID = idx1tag;
	MovieHeader.dwSize = (indexlist.size()) * 16;
	fwrite(&MovieHeader,sizeof(MovieHeader),1,fp);
	unsigned int i;
	for(i=0;i<indexlist.size();i++){
		fwrite(&indexlist[i],sizeof(tIndexEntry),1,fp);
	}
}

long AVIWriter::curSize()
{
	if( 0 == fp ){
		return 0;
	}
	fflush(fp);
	return ftell(fp);
}

