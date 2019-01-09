#pragma once
#include "SFMLWindow.h"
#ifndef UNICODE
#define UNICODE
#endif // !UNICODE
#include <Windows.h>
#include <UIRibbon.h>
#include "UIApplication.hpp"


class MainWindow
{
	int ribbonHeight = 0;
	HWND hWnd = NULL;
	HWND sfmlHWND = NULL;
	SFMLWindow *SFMLwindow = nullptr;
	static constexpr const wchar_t *CLASS_NAME = L"Sample Window Class";
	bool InitClass(HINSTANCE hInstance);
	bool InitWindow(HINSTANCE hInstance);
	bool InitSFMLwindow(HINSTANCE hInstance);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RealWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	IUIFramework *pFramework = nullptr;
	UIApplication *pApplication = nullptr;
	void HandleResize();
public:
	void HandleRibbonResize(UINT32 current);
	HRESULT HandleCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler **commandHandler); 
	HRESULT HandleDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler *commandHandler);
	MainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);
	void Run();
	~MainWindow();
};

