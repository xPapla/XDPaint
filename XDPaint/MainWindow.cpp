#include "MainWindow.h"

bool MainWindow::InitClass(HINSTANCE hInstance)
{
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	return RegisterClass(&wc) == 0;
}

bool MainWindow::InitWindow(HINSTANCE hInstance)
{
	hWnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Test window",                 // Window text
		WS_OVERLAPPEDWINDOW,            // Window style
		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,                    // Parent window
		NULL,                    // Menu
		hInstance,               // Instance handle
		static_cast<LPVOID>(this)// Additional application data
	);
	return hWnd == NULL;
}

bool MainWindow::InitSFMLwindow(HINSTANCE hInstance)
{
	sfmlHWND = CreateWindow(
		L"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		// Size and position
		100, 100, 500, 781,
		hWnd,                    // Parent window
		NULL,                    // Menu
		hInstance,               // Instance handle
		NULL                     // Additional application data
	);
	if (sfmlHWND == NULL) return true;
	SFMLwindow = new SFMLWindow(sfmlHWND);
	return false;
}


LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/* on window creation save pointer to mainwindow class handling window */
	if (uMsg == WM_CREATE) {
		CREATESTRUCT *data = (CREATESTRUCT *)lParam;
		MainWindow *me = static_cast<MainWindow *>(data->lpCreateParams);
		if (me) {
			me->hWnd = hwnd;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(me));
			return me->RealWindowProc(hwnd, uMsg, wParam, lParam);
		}
		else {/* should never occur */ }
	}
	/* try to call parent func previously saved */
	MainWindow *me = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealWindowProc(hwnd, uMsg, wParam, lParam);

	/* fallback (should only handle 2 messages before WM_CREATE) */
	/* https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-createwindowa#remarks */
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT MainWindow::RealWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		IUIFramework* pFramework = NULL;
		UIApplication *m_pApplication = NULL;
		void *ppvObj = NULL;

		HRESULT hr = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFramework));
		if (FAILED(hr)) return -1;

		using namespace std::placeholders;

		/* allocate pApplication */
		m_pApplication = new UIApplication(
			std::bind(&MainWindow::HandleRibbonResize, this, _1),
			std::bind(&MainWindow::HandleCreateUICommand, this, _1, _2, _3),
			std::bind(&MainWindow::HandleDestroyUICommand, this, _1, _2, _3)); 
		//TODO: \/ fix to be UIApplication

		hr = m_pApplication->QueryInterface(__uuidof(IUIApplication), (void **)&pApplication);
		if (FAILED(hr)) {
			delete m_pApplication; //as auto cleanup is not available here
			return -1;
		}

		hr = pFramework->Initialize(hwnd, (IUIApplication *)pApplication);
		pApplication->Release(); //auto cleanup on failure
		if (FAILED(hr)) return -1;

		//	static const int MaxResStringLength = 100;
		//wchar_t ribbonMarkup[MaxResStringLength] = { 0 };
		//// Obtain the name of the BML resource
		//::LoadString(
		//	HINST_THISCOMPONENT, IDS_RIBBON_MARKUP,
		//	ribbonMarkup, MaxResStringLength);

		hr = pFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
		if (FAILED(hr)) return -1;

		return 0;
	}
	case WM_SIZE:
	{
		HandleResize();
		//InvalidateRect(hWnd, NULL, 1);
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	case WM_DESTROY:
	{
		this->~MainWindow();
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void MainWindow::HandleRibbonResize(UINT32 current)
{
	ribbonHeight = current;
	HandleResize();
}

HRESULT MainWindow::HandleCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler ** commandHandler)
{
	return E_NOTIMPL;
}

HRESULT MainWindow::HandleDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler * commandHandler)
{
	return E_NOTIMPL;
}

void MainWindow::HandleResize()
{
	RECT r;
	if (!GetClientRect(hWnd, &r)) return;
	int height = r.bottom - ribbonHeight;
	if (height < 0) height = 0;
	if (SFMLwindow)
	SFMLwindow->Resize(0, ribbonHeight, r.right, height);
	//SetWindowPos(sfmlHWND, NULL, 0, ribbonHeight, r.right, height, SWP_NOZORDER);
}

MainWindow::MainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	if (CoInitialize(0) != S_OK) {
		throw "COM init failed";
	}
	if (InitClass(hInstance)) {
		throw "InitClass failed";
	}
	if (InitWindow(hInstance)) {
		throw "InitWindow failed";
	}
	if (InitSFMLwindow(hInstance)) {
		throw "InitSFMLwindow failed";
	}
	ShowWindow(hWnd, nCmdShow);
}

void MainWindow::Run() {
	SFMLwindow->Start();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

MainWindow::~MainWindow()
{
	if (SFMLwindow) {
		delete SFMLwindow;
		SFMLwindow = nullptr;
	}
	if (pApplication) {
		delete pApplication;
		pApplication = nullptr;
	}
	if (pFramework) {
		delete pFramework;
		pFramework = nullptr;
	}
}
