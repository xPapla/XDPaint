#include "main.hpp"
#include <iostream>

#include <UIRibbon.h>
#include "UIApplication.hpp"

/*
	IUIApplicationVtbl is defined in Uiribbon.h and
	CINTERFACE symbol stops IntelliSense from
	complaining about undefined IUIApplicationVtbl identifier
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	CoInitialize(0);//Initializes the COM library


	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hWnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Test window",                 // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hWnd == NULL)
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);

	// Run the message loop.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE: 
	{
		IUIFramework* pFramework = NULL;
		HRESULT hr = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFramework));
		UIApplication *pApplication = NULL;
		void *ppvObj = NULL;

		if (SUCCEEDED(hr)) {
			/* allocate pApplication */
			pApplication = new UIApplication();

			if (!pApplication) {
				return FALSE;
			}
			pApplication->QueryInterface(__uuidof(IUIApplication), &ppvObj);
		}

		if (SUCCEEDED(hr))
		{
			hr = pFramework->Initialize(hwnd, (IUIApplication *)ppvObj);
			if (pApplication) pApplication->Release();
		}

		if (SUCCEEDED(hr)) {
			//	static const int MaxResStringLength = 100;
			//wchar_t ribbonMarkup[MaxResStringLength] = { 0 };
			//// Obtain the name of the BML resource
			//::LoadString(
			//	HINST_THISCOMPONENT, IDS_RIBBON_MARKUP,
			//	ribbonMarkup, MaxResStringLength);
			
			hr = pFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
		}
		if (SUCCEEDED(hr)) {
			return 0;
		}
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 0));
		EndPaint(hwnd, &ps);
		return 0;
	}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//
//void DestroyRibbon()
//{
//	HRESULT hr;
//	if (g_pFramework)
//	{
//		/* release and destroy */
//		hr = ((IUIFramework *)g_pFramework)->lpVtbl->Destroy(g_pFramework);
//		g_pFramework = NULL;
//	}
//
//}