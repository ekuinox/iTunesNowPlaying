#pragma once
#include "event_handler.h"
#include <iostream>
#include <exception>

class NowPlayingException : public std::runtime_error {
public:
	NowPlayingException(const char *str) : std::runtime_error(str) {};
};

class NowPlaying {
private:
	IiTunes *app;
	IITTrack *track;
	DWORD cookie;
	IConnectionPoint *connection;
	char* convert(BSTR str);
	void onEvent(long disp);

public:
	NowPlaying();
	~NowPlaying();
	char *get_title();
	char *get_artist();
	bool update();
	EventHandler *ev;
	IUnknown *sink;
};