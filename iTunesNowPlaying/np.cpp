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

	if (FAILED(::CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (void**)&app)))
	{
		throw NowPlayingException("Failed to create instance.");
	}

	IConnectionPointContainer *icpc;

	if (FAILED(app->QueryInterface(IID_IConnectionPointContainer, (PVOID *)&icpc)))
	{
		throw NowPlayingException("COULD NOT FIND INTERFACE");
	}

	IConnectionPoint *connection;

	if (FAILED(icpc->FindConnectionPoint(DIID__IiTunesEvents, &connection)))
	{
		throw NowPlayingException("COULD NOT FIND CONNECTION POINT");
	}
	icpc->Release();
	DWORD cookie;

	auto ev = new EventHandler(this->app);

	auto result = connection->Advise(ev, &cookie);

	switch (result)
	{
	case S_OK:
		throw NowPlayingException("S_OK");
		break;
	case E_POINTER:
		throw NowPlayingException("E_POINTER");
		break;
	default:
		throw NowPlayingException("damessu");
		break;
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

bool NowPlaying::update()
{
	ITPlayerState state;

	app->get_PlayerState(&state);

	if (state == ITPlayerState::ITPlayerStatePlaying) app->get_CurrentTrack(&track);
	return state == ITPlayerState::ITPlayerStatePlaying;
}

NowPlaying::EventHandler::EventHandler(IiTunes *_player)
{
	player = _player;
	ref_count = 0;
	ITypeLib *type_lib = nullptr;
	::LoadRegTypeLib(LIBID_iTunesLib, 1, 5, 0x00, &type_lib);
	type_lib->GetTypeInfoOfGuid(DIID__IiTunesEvents, &type_info);
	type_lib->Release();
}

NowPlaying::EventHandler::EventHandler()
{
}

STDMETHODIMP NowPlaying::EventHandler::QueryInterface(REFIID riid, void ** ppvObject)
{
	if ((riid == IID_IDispatch) || (riid == DIID__IiTunesEvents)) {
		ref_count++;
		*ppvObject = this;//(_IiTunesEvents *)this;
		return S_OK;
	}
	if (riid == IID_IUnknown) {
		ref_count++;
		*ppvObject = this;//(IUnknown *)this;
		return S_OK;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) NowPlaying::EventHandler::AddRef(void)
{
	InterlockedIncrement(&ref_count);
	return ref_count;
}

STDMETHODIMP_(ULONG) NowPlaying::EventHandler::Release(void)
{
	InterlockedDecrement(&ref_count);
	if (ref_count == 0) {
		delete this;
		return 0;
	}
	return ref_count;
}

STDMETHODIMP NowPlaying::EventHandler::GetTypeInfoCount(UINT * pctinfo)
{
	return E_NOTIMPL;
}

STDMETHODIMP NowPlaying::EventHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo)
{
	return E_NOTIMPL;
}

STDMETHODIMP NowPlaying::EventHandler::GetIDsOfNames(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgDispId)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE NowPlaying::EventHandler::Invoke(DISPID dispidMember, REFIID, LCID, WORD, DISPPARAMS * pDispParams, VARIANT *, EXCEPINFO *, UINT *)
{
	printf("%d", dispidMember);
	if (dispidMember == ITEventPlayerPlayingTrackChanged) {
		printf("gyooooo");
	}
	return 1;
}
