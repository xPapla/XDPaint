#pragma once
#include <functional>
#include <UIRibbon.h>

class UIApplication :
	IUIApplication {
	LONG OutstandingObjects = 0;

	std::function<HRESULT(UINT32, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT32)> OnViewChangedHandler;
	std::function<HRESULT(UINT32, UI_COMMANDTYPE, IUICommandHandler**)> OnCreateUICommandHandler;
	std::function<HRESULT(UINT32, UI_COMMANDTYPE, IUICommandHandler*)> OnDestroyUICommandHandler;
public:
	UIApplication(
		std::function<HRESULT(UINT32, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT32)> onViewChangedHandler = nullptr,
		std::function<HRESULT(UINT32, UI_COMMANDTYPE, IUICommandHandler**)> onCreateUICommandHandler = nullptr,
		std::function<HRESULT(UINT32, UI_COMMANDTYPE, IUICommandHandler*)> onDestroyUICommandHandler = nullptr);
	void *operator new(size_t size);
	void operator delete(void *p);

	// Inherited via IUIApplication
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) override;
	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall OnViewChanged(UINT32 viewId, UI_VIEWTYPE typeID, IUnknown *view, UI_VIEWVERB verb, INT32 uReasonCode) override;
	virtual HRESULT __stdcall OnCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler) override;
	virtual HRESULT __stdcall OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler) override;
};
