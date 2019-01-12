#include "UIApplication.hpp"
#include <Windows.h> //GlobalAlloc, GlobalFree

UIApplication::UIApplication(
	std::function<HRESULT(UINT32, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT32)> onViewChangedHandler,
	std::function<HRESULT(UINT32, UI_COMMANDTYPE, IUICommandHandler**)> onCreateUICommandHandler,
	std::function<HRESULT(UINT32, UI_COMMANDTYPE, IUICommandHandler*)> onDestroyUICommandHandler)
{
	OnViewChangedHandler = onViewChangedHandler;
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
	if (OnViewChangedHandler)
		return OnViewChangedHandler(viewId, typeID, view, verb, uReasonCode);
	return E_NOTIMPL;
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
