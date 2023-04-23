#include "Headers.h"
#include "FfmpegVideoPlayer.h"

FfmpegVideoPlayer::FfmpegVideoPlayer(const char* filename)
{
	LoadVideo(filename);
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
}