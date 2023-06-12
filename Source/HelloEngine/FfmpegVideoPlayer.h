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
	~FfmpegVideoPlayer();
	void CleanUp();
	// Returns true if the frame has changed.
	void Update();

	uint GetOpenGLTexture() { return glTexture; }

	void SetFPS(float newFPS) { frameRate = 1.0f / newFPS; }
	double GetFPS();

	void ResetVideo();
	bool videoEnded = false;
private:
	bool LoadVideo(const char* filename);
	AVFrame* GetFrame();
private:
	// Context should be unique?
	AVFormatContext* avFormatCtx = nullptr;
	int videoStreamIndex = -1;
	AVCodecParameters* avCodecParams = nullptr;
	const AVCodec* avCodec = nullptr;
	AVCodecContext* avCodecCtx = nullptr;
	AVFrame* avFrame = nullptr;
	AVPacket* avPacket = nullptr;
	SwsContext* swsContext = nullptr;
	double frameRate = 0;
	double currentTime = 0;
	int width = 0;
	int height = 0;
	uint glTexture = 0;
	uint8_t* frameData = nullptr;

};

