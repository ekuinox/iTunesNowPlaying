#include "np.h"

void _onEvent(long disp)
{
	if (disp == ITEvent::ITEventDatabaseChanged) {
		printf("ƒnƒA[");
	}
	if (disp == ITEvent::ITEventPlayerPlay) {
		printf("‹È‚ªÄ¶‚³‚ê‚Ü‚µ‚½");
	}
	printf("%ld\n", disp);
}

char * NowPlaying::convert(BSTR str)
{
	auto length = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	auto *buffer = new char[length];
	memset(buffer, 0, SysStringByteLen(str) * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, length, NULL, NULL);
	return buffer;
}

void NowPlaying::onEvent(long disp)
{
	printf("%ld\n", disp);
}


NowPlaying::NowPlaying()
{
	if (FAILED(::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (void**)&app)))
	{
		throw NowPlayingException("Failed to create instance.");
	}

	IConnectionPointContainer *icpc;

	if (FAILED(app->QueryInterface(IID_IConnectionPointContainer, (PVOID *)&icpc)))
	{
		throw NowPlayingException("COULD NOT FIND INTERFACE");
	}

	if (FAILED(icpc->FindConnectionPoint(DIID__IiTunesEvents, &connection)))
	{
		throw NowPlayingException("COULD NOT FIND CONNECTION POINT");
	}
	icpc->Release();

	ev = new EventHandler(this->app, &_onEvent);
	auto r = connection->Advise(ev, &cookie);

	if (FAILED(r)) {
		throw NowPlayingException("ADVISE ERROR");
	}
}

NowPlaying::~NowPlaying()
{
	app->Release();
	connection->Unadvise(cookie);
	connection->Release();
}

char * NowPlaying::get_title()
{
	if (!track) return (char*)"";
	BSTR title;
	track->get_Name(&title);
	return convert(title);
}

char * NowPlaying::get_artist()
{
	if (!track) return (char*)"";
	BSTR artist;
	track->get_Artist(&artist);
	return convert(artist);
}

bool NowPlaying::update()
{
	ITPlayerState state;

	app->get_PlayerState(&state);

	if (state == ITPlayerState::ITPlayerStatePlaying) {
		app->get_CurrentTrack(&track);
		return true;
	}
	else {
		return false;
	}
}