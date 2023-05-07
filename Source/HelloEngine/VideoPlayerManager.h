#pragma once
#include "Globals.h"
#include <map>
#include "FfmpegVideoPlayer.h"

class VideoPlayerComponent;

struct VideoPlayer
{
	FfmpegVideoPlayer* video = nullptr;
	bool isPlaying = false;
};

class VideoPlayerManager
{
public:

	VideoPlayerManager();
	~VideoPlayerManager();

	static void Update();

	static uint AddVideoPlayer(uint videoResourceUID);
	static VideoPlayer* GetVideoPlayer(uint UID);
	static void RemoveVideoPlayer(uint UID);

private:

	static std::map<uint, VideoPlayer> videoPlayers;
	static std::map<uint, VideoPlayerComponent*> videoComponents;

};

