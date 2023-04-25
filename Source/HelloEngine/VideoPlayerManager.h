#pragma once
#include "Globals.h"
#include <map>
#include "FfmpegVideoPlayer.h"

class VideoPlayerComponent;

class VideoPlayerManager
{
public:

	VideoPlayerManager();
	~VideoPlayerManager();

	static void Update();

	static uint AddVideoPlayer(std::string& path);
	static FfmpegVideoPlayer* GetVideoPlayer(uint UID);
	static void RemoveVideoPlayer(uint UID);

private:

	static std::map<uint, FfmpegVideoPlayer> videoPlayers;
	static std::map<uint, VideoPlayerComponent*> videoComponents;

};

