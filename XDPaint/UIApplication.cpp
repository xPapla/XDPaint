#include "UIApplication.hpp"
#include "Windows.h" //GlobalAlloc, GlobalFree

UIApplication::UIApplication(std::function<void(UINT32)> onHeightChangedHandler,
	std::function<HRESULT(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler)> onCreateUICommandHandler,
	std::function<HRESULT(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler)> onDestroyUICommandHandler)
{
	OnHeightChangedHandler = onHeightChangedHandler;
	OnCreateUICommandHandler = onCreateUICommandHandler;
	OnDestroyUICommandHandler = onDestroyUICommandHandler;
}

void *UIApplication::operator new(size_t size)
{
	//void *p = ::new UIApplication();
	//void *p = malloc(size);
	void *p = GlobalAlloc(GMEM_FIXED, size);
	return p;
}

void UIApplication::operator delete(void * p)
{
	GlobalFree(p);
}

HRESULT UIApplication::QueryInterface(REFIID riid, void ** ppvObject)
{
	/* No match */
	if (!IsEqualIID(riid, __uuidof(IUIApplication)) && !IsEqualIID(riid, IID_IUnknown)) {
		*ppvObject = 0;
		return(E_NOINTERFACE);
	}

	/* Point our IUIApplication interface to our ribbon obj */
	*ppvObject = this;

	/* Ref count */
	AddRef();

	return(NOERROR);
}

ULONG UIApplication::AddRef(void)
{
	return(InterlockedIncrement(&OutstandingObjects));
}

ULONG UIApplication::Release(void)
{
	ULONG i = InterlockedDecrement(&OutstandingObjects);
	if (i == 0)
		delete this;

	return i;
}

HRESULT UIApplication::OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeID, IUnknown * view, UI_VIEWVERB verb, INT32 uReasonCode)
{
	HRESULT hr = E_NOTIMPL;
	if (typeID != UI_VIEWTYPE_RIBBON) return hr;
	switch (verb)
	{
	case UI_VIEWVERB_CREATE:
		hr = view->QueryInterface(&Ribbon);
		break;
	case UI_VIEWVERB_DESTROY:
		Ribbon = nullptr;
		hr = S_OK;
		break;
	case UI_VIEWVERB_SIZE:
		if (Ribbon) {
			UINT32 height = 0;
			hr = Ribbon->GetHeight(&height);
			if (SUCCEEDED(hr)) {
				OnHeightChangedHandler(height);
			}
		}
		break;
	}
	return hr;
}

HRESULT UIApplication::OnCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler ** commandHandler)
{
	if (OnCreateUICommandHandler)
		return OnCreateUICommandHandler(commandId, typeID, commandHandler);
	return E_NOTIMPL;
}

HRESULT UIApplication::OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler * commandHandler)
{
	if (OnDestroyUICommandHandler)
		return OnDestroyUICommandHandler(commandId, typeID, commandHandler);
	return E_NOTIMPL;
}
