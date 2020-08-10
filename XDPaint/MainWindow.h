#pragma once
#include "SFMLWindow.h"

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include <Windows.h>
#include <UIRibbon.h>

#include "UIApplication.hpp"
#include "UICommandHandler.h"

class MainWindow
{
	//Window handlers
	HWND hWnd = NULL;
	HWND sfmlHWND = NULL;
	SFMLWindow *SFMLwindow = nullptr;
	//COM
	IUIFramework *pFramework = nullptr;
	IUIRibbon *pRibbon = nullptr;
	UIApplication *pApplication = nullptr;
	UICommandHandler *pCommandHandler = nullptr;

	int ribbonHeight = 0;
	static constexpr const wchar_t *CLASS_NAME = L"Sample Window Class";

	bool InitClass(HINSTANCE hInstance);
	bool InitWindow(HINSTANCE hInstance);
	bool InitSFMLwindow(HINSTANCE hInstance);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RealWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void HandleResize();
	//UIApplication events
	HRESULT HandleOnViewChanged(UINT32 viewId, UI_VIEWTYPE typeID, IUnknown * view, UI_VIEWVERB verb, INT32 uReasonCode);
	HRESULT HandleOnCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler);
	HRESULT HandleOnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler);
	//UICommandHandler events
	HRESULT HandleExecute(UINT32 commandId, UI_EXECUTIONVERB verb, const PROPERTYKEY * key, const PROPVARIANT * currentValue, IUISimplePropertySet * commandExecutionProperties);
	HRESULT CreateUIImageFromBitmapResource(LPCTSTR pszResource, IUIImage ** ppimg);
	HRESULT HandleUpdateProperty(UINT32 commandId, REFPROPERTYKEY key, const PROPVARIANT * currentValue, PROPVARIANT * newValue);
public:
	MainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);
	void Run();
	~MainWindow();
};

