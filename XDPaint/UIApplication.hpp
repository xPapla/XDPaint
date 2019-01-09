#pragma once
#include <functional>
#include <UIRibbon.h>

//maybe? HRESULT hRes = CComObject<CMyCircle>::CreateInstance(&pCircle);

class UIApplication :
	IUIApplication {
	LONG OutstandingObjects = 0;
	IUIRibbon *Ribbon = nullptr;
	std::function<void(UINT32)> OnHeightChangedHandler; //takes UINT32, returns void 
	std::function<HRESULT(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler)> OnCreateUICommandHandler;
	std::function<HRESULT(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler)> OnDestroyUICommandHandler;
public:
	UIApplication(std::function<void(UINT32)> onHeightChangedHandler,
		std::function<HRESULT(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler)> onCreateUICommandHandler,
		std::function<HRESULT(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler)> onDestroyUICommandHandler);
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
