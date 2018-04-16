#include "np.h"

char * NowPlaying::convert(BSTR str)
{
	auto length = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	auto *buffer = new char[length];
	memset(buffer, 0, SysStringByteLen(str) * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, length, NULL, NULL);
	return buffer;
}

NowPlaying::NowPlaying()
{
	CoInitializeEx(0, COINIT_APARTMENTTHREADED);

	if (FAILED(CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (PVOID *)&app)))
	{
		throw "Failed to create instance.";
	}
}

NowPlaying::~NowPlaying()
{
	CoUninitialize();
}

char * NowPlaying::get_title()
{
	BSTR title;
	track->get_Name(&title);
	return convert(title);
}

char * NowPlaying::get_artist()
{
	BSTR artist;
	track->get_Artist(&artist);
	return convert(artist);
}

void NowPlaying::update()
{
	ITPlayerState state;

	app->get_PlayerState(&state);

	if (state == ITPlayerState::ITPlayerStatePlaying) app->get_CurrentTrack(&track);
}
