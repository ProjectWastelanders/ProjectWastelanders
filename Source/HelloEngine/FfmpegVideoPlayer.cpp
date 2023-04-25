#include "Headers.h"
#include "FfmpegVideoPlayer.h"

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
		printf("Couldn't format context\n");
		return 0;
	}
	if (avformat_open_input(&avFormatCtx, filename, NULL, NULL)) // fill context with video data
	{
		printf("Couldn't open video\n");
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
		printf("Couldn't find video stream\n");
		return false;
	}

	// Set up a codec context for the decoder

	avCodecCtx = avcodec_alloc_context3(avCodec); // Allocate an avCodecContext to store params data
	if (!avCodecCtx)
	{
		printf("Couldn't create AvCodecContext\n");
		return false;
	}

	// Set AvCodecContext parameters 
	if (avcodec_parameters_to_context(avCodecCtx, avCodecParams) < 0)
	{
		printf("Couldn't initialize AvCodecContext\n");
		return false;
	}
	// Open Codec
	if (avcodec_open2(avCodecCtx, avCodec, NULL) < 0)
	{
		printf("Couldn't open codec\n");
		return false;
	}

	// Allocate memory for the frame and packet variables. They will be used in the GetFrame() function.
	avFrame = av_frame_alloc();
	if (!avFrame)
	{
		printf("Couldn't allocate frame\n");
		return 0;
	}
	avPacket = av_packet_alloc();
	frameRate = 1.0 / GetFPS();

	// Get first frame and get init variables.
	AVFrame* firstFrame = GetFrame();
	width = firstFrame->width;
	height = firstFrame->height;
	frameData = new uint8_t[width * height * 4]; // 4 = RGBA

	av_image_copy_plane(frameData, 0, firstFrame->data[0], firstFrame->linesize[0], width * 4, height);

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

		// If no error is given on this point, we successfully recieved a frame
		return avFrame;
	}
	return nullptr;
}

double FfmpegVideoPlayer::GetFPS()
{
	auto stream = avFormatCtx->streams[videoStreamIndex];
	const double FPS = (double)stream->r_frame_rate.num / (double)stream->r_frame_rate.den;
	return FPS;
}

void FfmpegVideoPlayer::CleanUp()
{
	av_packet_unref(avPacket);
	av_packet_free(&avPacket);
	av_frame_free(&avFrame);
	avformat_close_input(&avFormatCtx);
	avformat_free_context(avFormatCtx);
	avcodec_free_context(&avCodecCtx);

	glDeleteTextures(1, &glTexture);
	delete[] frameData;
}

void FfmpegVideoPlayer::Update()
{
	currentTime += EngineTime::EngineTimeDeltaTime();

	if (currentTime >= frameRate)
	{
		currentTime = 0;
		AVFrame* frame = GetFrame();

		if (frame == nullptr)
		{
			// Video ended
			return;
		}
		// Update texture
		
		av_image_copy_plane(frameData, 0, frame->data[0], frame->linesize[0], width * 4, height);

		// Create texture
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameData);

		//2D case

		//3D case
		// Obtain videoPlayerComponent and change its texture.	
	}
}
