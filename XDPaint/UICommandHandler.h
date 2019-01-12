#pragma once
#include <functional>
#include <UIRibbon.h>

class UICommandHandler :
	IUICommandHandler {
	LONG OutstandingObjects = 0;

	std::function<HRESULT(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY *, const PROPVARIANT *, IUISimplePropertySet *)> ExecuteHandler;
	std::function<HRESULT(UINT32, REFPROPERTYKEY, const PROPVARIANT *, PROPVARIANT *)> UpdatePropertyHandler;
public:
	UICommandHandler(
		std::function<HRESULT(UINT32, UI_EXECUTIONVERB, const PROPERTYKEY *, const PROPVARIANT *, IUISimplePropertySet *)> executeHandler = nullptr,
		std::function<HRESULT(UINT32, REFPROPERTYKEY, const PROPVARIANT *, PROPVARIANT *)> updatePropertyHandler = nullptr);
	void *operator new(size_t size);
	void operator delete(void *p);

	// Inherited via IUICommandHandler
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject) override;
	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;
	virtual HRESULT __stdcall Execute(UINT32 commandId, UI_EXECUTIONVERB verb, const PROPERTYKEY * key, const PROPVARIANT * currentValue, IUISimplePropertySet * commandExecutionProperties) override;
	virtual HRESULT __stdcall UpdateProperty(UINT32 commandId, REFPROPERTYKEY key, const PROPVARIANT * currentValue, PROPVARIANT * newValue) override;
};
