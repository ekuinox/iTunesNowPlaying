#pragma once
#include "iTunesCOMInterface.h"
#include <iostream>
#include <exception>

class NowPlayingException : public std::exception {
public:
	NowPlayingException(const char *str) : std::exception(str) {};
};

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
	bool update();

private:
	class EventHandler : public _IiTunesEvents {
	private:
		IiTunes *player;
		long ref_count;
		ITypeInfo *type_info;
	public:
		EventHandler(IiTunes* _player);
		EventHandler();
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
		ULONG STDMETHODCALLTYPE AddRef(void);
		ULONG STDMETHODCALLTYPE Release(void);

		//
		// Implements IDispatch
		// 

		HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
		HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
		HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames,
			LCID lcid, DISPID *rgDispId);
		HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
			DISPPARAMS *pDispParams, VARIANT *pVarResult,
			EXCEPINFO *pExcepInfo, UINT *puArgErr);
	};
};