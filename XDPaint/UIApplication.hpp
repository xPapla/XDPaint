#pragma once
#include <UIRibbon.h>

//maybe? HRESULT hRes = CComObject<CMyCircle>::CreateInstance(&pCircle);

class UIApplication :
	IUIApplication {
	LONG OutstandingObjects;
public:
	UIApplication();
	void *operator new(size_t size);
	void operator delete(void *p);
	// Inherited via IUIApplication
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;
	HRESULT __stdcall OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeID, IUnknown *view, UI_VIEWVERB verb, INT32 uReasonCode) override;
	HRESULT __stdcall OnCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler) override;
	HRESULT __stdcall OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler) override;
};
