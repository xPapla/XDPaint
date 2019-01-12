#include "UICommandHandler.h"
#include <Windows.h>

UICommandHandler::UICommandHandler(
	std::function<HRESULT(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY*, const PROPVARIANT*, IUISimplePropertySet*)> executeHandler, 
	std::function<HRESULT(UINT32, REFPROPERTYKEY, const PROPVARIANT*, PROPVARIANT*)> updatePropertyHandler)
{
	ExecuteHandler = executeHandler;
	UpdatePropertyHandler = updatePropertyHandler;
}

void *UICommandHandler::operator new(size_t size)
{
	//void *p = ::new UIApplication();
	//void *p = malloc(size);
	void *p = GlobalAlloc(GMEM_FIXED, size);
	return p;
}

void UICommandHandler::operator delete(void * p)
{
	GlobalFree(p);
}

HRESULT __stdcall UICommandHandler::QueryInterface(REFIID riid, void ** ppvObject)
{
	/* No match */
	if (!IsEqualIID(riid, __uuidof(IUICommandHandler)) && !IsEqualIID(riid, IID_IUnknown)) {
		*ppvObject = 0;
		return(E_NOINTERFACE);
	}

	/* Point our IUIApplication interface to our ribbon obj */
	*ppvObject = this;

	/* Ref count */
	AddRef();

	return(NOERROR);
}

ULONG __stdcall UICommandHandler::AddRef(void)
{
	return(InterlockedIncrement(&OutstandingObjects));
}

ULONG __stdcall UICommandHandler::Release(void)
{
	ULONG i = InterlockedDecrement(&OutstandingObjects);
	if (i == 0)
		delete this;

	return i;
}

HRESULT __stdcall UICommandHandler::Execute(UINT32 commandId, UI_EXECUTIONVERB verb, const PROPERTYKEY * key, const PROPVARIANT * currentValue, IUISimplePropertySet * commandExecutionProperties)
{
	if (ExecuteHandler)
		ExecuteHandler(commandId, verb, key, currentValue, commandExecutionProperties);
	return E_NOTIMPL;
}

HRESULT __stdcall UICommandHandler::UpdateProperty(UINT32 commandId, REFPROPERTYKEY key, const PROPVARIANT * currentValue, PROPVARIANT * newValue)
{
	if (UpdatePropertyHandler)
		UpdatePropertyHandler(commandId, key, currentValue, newValue);
	return E_NOTIMPL;
}
