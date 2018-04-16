#pragma once
#include "iTunesCOMInterface.h"

class NowPlaying {
private:
	IiTunes *app;
	IITTrack *track;

	char* convert(BSTR str);

public:
	NowPlaying();
	~NowPlaying();
	char *get_title();
	char *get_artist();
	void update();
};