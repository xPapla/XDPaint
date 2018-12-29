#include "UIApplication.hpp"
#include "Windows.h" //GlobalAlloc, GlobalFree

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

UIApplication::UIApplication()
{
	OutstandingObjects = 0;
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
	return InterlockedDecrement(&OutstandingObjects);
}

HRESULT UIApplication::OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeID, IUnknown * view, UI_VIEWVERB verb, INT32 uReasonCode)
{
	return E_NOTIMPL;
}

HRESULT UIApplication::OnCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler ** commandHandler)
{
	return E_NOTIMPL;
}

HRESULT UIApplication::OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler * commandHandler)
{
	return E_NOTIMPL;
}
