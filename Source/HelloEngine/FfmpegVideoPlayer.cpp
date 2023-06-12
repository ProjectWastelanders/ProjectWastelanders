#include "Headers.h"
#include "FfmpegVideoPlayer.h"
#include "LayerGame.h"

FfmpegVideoPlayer::FfmpegVideoPlayer(const char* filename)
{
	LoadVideo(filename);
}

FfmpegVideoPlayer::~FfmpegVideoPlayer()
{
	CleanUp();
}

bool FfmpegVideoPlayer::LoadVideo(const char* filename)
{
	//First we need to create a context to store the video data int
	avFormatCtx = avformat_alloc_context(); // Create Context
	if (!avFormatCtx)
	{
		LOG("Couldn't format context\n");
		return 0;
	}
	if (avformat_open_input(&avFormatCtx, filename, NULL, NULL)) // fill context with video data
	{
		LOG("Couldn't open video\n");
		return 0;
	}

	// We must find the stream that contains the video data
	// Find video and audio streams from the context
	for (int i = 0; i < avFormatCtx->nb_streams; i++)
	{
		auto stream = avFormatCtx->streams[i];
		avCodecParams = avFormatCtx->streams[i]->codecpar;
		avCodec = avcodec_find_decoder(avCodecParams->codec_id);

		if (!avCodec) //  If there is no codec we look for the next stream. This should never happen on a normal video file.
		{
			continue;
		}

		if (avCodecParams->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO) // If this is the video stream, we save its index
		{
			videoStreamIndex = i;
			break;
		}
	}

	// If video stream index is still -1, we didn't find a video stream index. Therefore, there is no video.
	if (videoStreamIndex == -1)
	{
		LOG("Couldn't find video stream\n");
		return false;
	}

	// Set up a codec context for the decoder

	avCodecCtx = avcodec_alloc_context3(avCodec); // Allocate an avCodecContext to store params data
	if (!avCodecCtx)
	{
		LOG("Couldn't create AvCodecContext\n");
		return false;
	}

	// Set AvCodecContext parameters 
	if (avcodec_parameters_to_context(avCodecCtx, avCodecParams) < 0)
	{
		LOG("Couldn't initialize AvCodecContext\n");
		return false;
	}
	// Open Codec
	if (avcodec_open2(avCodecCtx, avCodec, NULL) < 0)
	{
		LOG("Couldn't open codec\n");
		return false;
	}

	// Allocate memory for the frame and packet variables. They will be used in the GetFrame() function.
	avFrame = av_frame_alloc();
	if (!avFrame)
	{
		LOG("Couldn't allocate frame\n");
		return 0;
	}
	avPacket = av_packet_alloc();
	frameRate = 1.0 / GetFPS();

	// Get first frame and get init variables.
	AVFrame* firstFrame = GetFrame();
	width = firstFrame->width;
	height = firstFrame->height;
	frameData = new uint8_t[width * height * 4]; // 4 = RGBA

	swsContext = sws_getContext(width, height, avCodecCtx->pix_fmt, width, height, AV_PIX_FMT_RGB0,
		SWS_BILINEAR, NULL, NULL, NULL);

	if (!swsContext)
	{
		LOG("Failed to intialize sws context.");
		return 0;
	}

	uint8_t* dst[4] = { frameData, NULL, NULL, NULL };
	int dstLineSize[4] = { width * 4, NULL,NULL,NULL };

	sws_scale(swsContext, firstFrame->data, firstFrame->linesize, 0, height, dst, dstLineSize);

	// Create texture
	glGenTextures(1, &glTexture);
	glBindTexture(GL_TEXTURE_2D, glTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameData);
	return true;
}

AVFrame* FfmpegVideoPlayer::GetFrame()
{
	int result;
	// We loop until we are out of packets or find a video packet 
	while (av_read_frame(avFormatCtx, avPacket) >= 0)
	{
		// If this is not the video stream, we skip this packet
		if (avPacket->stream_index != videoStreamIndex)
		{
			av_packet_unref(avPacket);
			continue;
		}
		// Check for every possible error on this step
		result = avcodec_send_packet(avCodecCtx, avPacket);
		if (result < 0)
		{
			printf("Faild to decode packet\n");
			return 0;
		}
		result = avcodec_receive_frame(avCodecCtx, avFrame);
		if (result == AVERROR(EAGAIN) || result == AVERROR_EOF) // If this frame has already been decoded or it is the end of the file
		{
			continue;
		}
		else if (result < 0)
		{
			printf("Faild to decode packet\n");
			return 0;
		}

		av_packet_unref(avPacket);

		// If no error is given on this point, we successfully recieved a frame
		return avFrame;
	}
	return nullptr;

}

double FfmpegVideoPlayer::GetFPS()
{
	auto stream = avFormatCtx->streams[videoStreamIndex];
	const double FPS = (double)stream->avg_frame_rate.num / (double)stream->avg_frame_rate.den;
	return FPS;
}

void FfmpegVideoPlayer::ResetVideo()
{
	// Reser AVCodec context buffers
	avcodec_flush_buffers(avCodecCtx);
	av_seek_frame(avFormatCtx, videoStreamIndex, 0, AVSEEK_FLAG_BACKWARD);

	// Recieve first frame
	AVFrame* frame = GetFrame();

	if (frame == nullptr)
	{
		// Video ended
		return;
	}
	// Update texture

	uint8_t* dst[4] = { frameData, NULL, NULL, NULL };
	int dstLineSize[4] = { width * 4, NULL,NULL,NULL };

	sws_scale(swsContext, frame->data, frame->linesize, 0, height, dst, dstLineSize);

	// Create texture
	glBindTexture(GL_TEXTURE_2D, glTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameData);
}

void FfmpegVideoPlayer::CleanUp()
{
	av_packet_unref(avPacket);
	av_packet_free(&avPacket);
	av_frame_free(&avFrame);
	avformat_close_input(&avFormatCtx);
	avformat_free_context(avFormatCtx);
	avcodec_free_context(&avCodecCtx);
	sws_freeContext(swsContext);
	glDeleteTextures(1, &glTexture);
	delete[] frameData;
}

void FfmpegVideoPlayer::Update()
{
	currentTime += LayerGame::S_IsPlaying() ? EngineTime::RealTimeDeltaTime() : EngineTime::EngineTimeDeltaTime();

	if (currentTime >= frameRate)
	{
		currentTime = 0;
		AVFrame* frame = GetFrame();

		if (frame == nullptr)
		{
			// Video ended
			videoEnded = true;
			return;
		}
		videoEnded = false; // sorry
		// Update texture
		
		uint8_t* dst[4] = { frameData, NULL, NULL, NULL };
		int dstLineSize[4] = { width * 4, NULL,NULL,NULL };

		sws_scale(swsContext, frame->data, frame->linesize, 0, height, dst, dstLineSize);

		// Create texture
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameData);

	}
}
