#pragma once
extern "C"
{
#include "libavcodec/avcodec.h"  
#include "libavformat/avformat.h"  
#include "libswscale/swscale.h"  
#include "libavutil/imgutils.h" 
}

class FfmpegVideoPlayer
{
public:
	FfmpegVideoPlayer(const char* filename);
	void CleanUp();
	AVFrame* GetFrame();
	double GetFPS();
private:
	bool LoadVideo(const char* filename);

private:
	AVFormatContext* avFormatCtx = nullptr;
	int videoStreamIndex = -1;
	AVCodecParameters* avCodecParams = nullptr;
	const AVCodec* avCodec = nullptr;
	AVCodecContext* avCodecCtx = nullptr;
	AVFrame* avFrame = nullptr;
	AVPacket* avPacket = nullptr;
};

