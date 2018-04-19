#include "event_handler.h"

EventHandler::EventHandler(IiTunes *_player, void (*_onEvent)(long))
{
	player = _player;
	onEvent = _onEvent;
	ref_count = 0;
	ITypeLib *type_lib = nullptr;
	if (FAILED(::LoadRegTypeLib(LIBID_iTunesLib, 1, 13, 0x00, &type_lib))) throw "Error";
	if (FAILED(type_lib->GetTypeInfoOfGuid(DIID__IiTunesEvents, &type_info))) throw "Error";
	type_lib->Release();
}

EventHandler::~EventHandler()
{
}

HRESULT STDMETHODCALLTYPE EventHandler::QueryInterface(REFIID riid, void ** ppvObject)
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

ULONG STDMETHODCALLTYPE EventHandler::AddRef(void)
{
	return ++ref_count;
}

ULONG STDMETHODCALLTYPE EventHandler::Release(void)
{
	return --ref_count;
}

HRESULT STDMETHODCALLTYPE EventHandler::GetTypeInfoCount(UINT * pctinfo)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE EventHandler::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo ** ppTInfo)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE EventHandler::GetIDsOfNames(REFIID riid, LPOLESTR * rgszNames, UINT cNames, LCID lcid, DISPID * rgDispId)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE EventHandler::Invoke(DISPID dispidMember, REFIID, LCID, WORD, DISPPARAMS * pDispParams, VARIANT *, EXCEPINFO *, UINT *)
{
	onEvent(dispidMember);
	return S_OK;
}
