#include "MainWindow.h"
#include <UIRibbonPropertyHelpers.h>

#include <Windows.h>
#include "resource.h"
#include "RibbonIDs.h"
#include "PropertySet.h"
#include <string>

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
		L"XDPaint",                 // Window text
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
		100, 100, 200, 200,
		hWnd,                    // Parent window
		NULL,                    // Menu
		hInstance,               // Instance handle
		NULL                     // Additional application data
	);
	if (sfmlHWND == NULL) return true;
	SFMLwindow = new SFMLWindow(sfmlHWND);
	return false;
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
	if (pCommandHandler) {
		delete pCommandHandler;
		pCommandHandler = nullptr;
	}
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
		UIApplication *m_pApplication = NULL;
		UICommandHandler *m_pCommandHandler = NULL;
		void *ppvObj = NULL;

		HRESULT hr = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFramework));
		if (FAILED(hr)) return -1;

		using namespace std::placeholders;

		/* allocate pApplication */
		m_pApplication = new UIApplication(
			std::bind(&MainWindow::HandleOnViewChanged, this, _1, _2, _3, _4, _5),
			std::bind(&MainWindow::HandleOnCreateUICommand, this, _1, _2, _3),
			std::bind(&MainWindow::HandleOnDestroyUICommand, this, _1, _2, _3));

		//m_pCommandHandler = 
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

HRESULT MainWindow::HandleOnViewChanged(UINT32 viewId, UI_VIEWTYPE typeID, IUnknown * view, UI_VIEWVERB verb, INT32 uReasonCode)
{
	HRESULT hr = E_NOTIMPL;
	if (typeID != UI_VIEWTYPE_RIBBON) return hr;
	switch (verb)
	{
	case UI_VIEWVERB_CREATE:
		hr = view->QueryInterface(&pRibbon);
		break;
	case UI_VIEWVERB_DESTROY:
		pRibbon = nullptr;
		hr = S_OK;
		break;
	case UI_VIEWVERB_SIZE:
		if (pRibbon) {
			UINT32 height = 0;
			hr = pRibbon->GetHeight(&height);
			if (SUCCEEDED(hr)) {
				ribbonHeight = height;
				HandleResize();
			}
		}
		break;
	}
	return hr;
}


HRESULT MainWindow::HandleOnCreateUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler ** commandHandler)
{
	using namespace std::placeholders;
	if (!pCommandHandler) pCommandHandler = new UICommandHandler(
		std::bind(&MainWindow::HandleExecute, this, _1, _2, _3, _4, _5),
		std::bind(&MainWindow::HandleUpdateProperty, this, _1, _2, _3, _4));
	return pCommandHandler->QueryInterface(IID_PPV_ARGS(commandHandler));
}

HRESULT MainWindow::HandleOnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler * commandHandler)
{
	return E_NOTIMPL;
}

HRESULT MainWindow::HandleExecute(UINT32 commandId, UI_EXECUTIONVERB verb, const PROPERTYKEY * key, const PROPVARIANT * currentValue, IUISimplePropertySet * commandExecutionProperties)
{
	HRESULT hr = S_OK;

	auto tools = { ID_TOOL_PENCIL, ID_TOOL_TEXT, ID_TOOL_PIPETTE, ID_TOOL_MAGNIFIER };

	switch (commandId)
	{
	case ID_APM_NEW:
		SFMLwindow->clear();
		break;
	case ID_APM_SAVE:
	case ID_APM_SAVE_AS:
	case ID_APM_OPEN:
	{
		OPENFILENAME ofn;       // common dialog box structure
		TCHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		//ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (commandId == ID_APM_OPEN) {
			if (GetOpenFileName(&ofn) == TRUE)
			{
				// use ofn.lpstrFile
			}
		}
		else { //save
			if (GetSaveFileName(&ofn) == TRUE)
			{
				bool zTlem = commandId == ID_APM_SAVE;
				SFMLwindow->saveImage(std::wstring(szFile), zTlem);
			}
		}
		break;
	}
	case ID_SPINNER:
	{
		if (verb == UI_EXECUTIONVERB_EXECUTE) {
			int size = 0;
			// Spinner value is negative.
			if (!(currentValue->decVal.sign == 0))
			{
				// Check if the value supplied by the Spinner is -0
				// and correct the value if necessary.
				// If this value is left uncorrected, the edit field 
				// of the control will display the string "Auto" when 
				// UI_PKEY_MinValue is set to 0.
				if (currentValue->decVal.Lo64 == 0)
				{
					// Initialize a new PROPVARIANT structure.
					PROPVARIANT m_varNewVal;
					PropVariantInit(&m_varNewVal);

					// The replacement DECIMAL value.
					DECIMAL m_dVal;
					hr = VarDecFromI4(1, &m_dVal);
					if (FAILED(hr))
					{
						return hr;
					}

					// Initialize the new DECIMAL value.
					UIInitPropertyFromDecimal(UI_PKEY_DecimalValue, m_dVal, &m_varNewVal);

					// Set the UI_PKEY_DecimalValue to the new DECIMAL value.
					hr = pFramework->SetUICommandProperty(commandId, UI_PKEY_DecimalValue, m_varNewVal);
					if (FAILED(hr))
					{
						return hr;
					}
				}
				// Decrease size of shape in document space.
				size = -currentValue->intVal;
			}
			// Spinner value is positive.
			else
			{
				// Increase size of shape in document space.
				size = currentValue->intVal;
			}

			SFMLwindow->updateOutlineWidth(size);

			break;
		}
	}
	case ID_SHAPEPICKER:
	case ID_TOOL_PENCIL:
	case ID_TOOL_TEXT:
	case ID_TOOL_PIPETTE:
	case ID_TOOL_MAGNIFIER:
	{
		//invalidate tools
		if (verb == UI_EXECUTIONVERB_EXECUTE) {
			if (commandId == ID_TOOL_PENCIL) {
				SFMLwindow->updateTool(SFMLTool::Pencil);
			}
			else if (commandId == ID_TOOL_TEXT)
				SFMLwindow->updateTool(SFMLTool::Text);
			else if (commandId == ID_TOOL_PIPETTE)
				SFMLwindow->updateTool(SFMLTool::Pipette);
			else if (commandId == ID_TOOL_MAGNIFIER)
				SFMLwindow->updateTool(SFMLTool::Magnifier);
			else if (commandId == ID_SHAPEPICKER) {
				SFMLwindow->updateTool(SFMLTool::Shapes);
				UINT selected;
				hr = UIPropertyToUInt32(*key, *currentValue, &selected);
				if (FAILED(hr)) {
					return hr;
				}
				SFMLwindow->updateShape((SFMLShape)(1 << (1 + selected)));
			}

			for (auto &d : tools) {
				pFramework->InvalidateUICommand(d, UI_INVALIDATIONS::UI_INVALIDATIONS_VALUE, key);
			}
			pFramework->InvalidateUICommand(ID_SHAPEPICKER, UI_INVALIDATIONS::UI_INVALIDATIONS_VALUE, key);
		}
		break;
	}
	case ID_COLORPICKER_BACKGROUND:
	case ID_COLORPICKER_FILL:
	case ID_COLORPICKER_OUTLINE:
	{
		UINT color = 0;
		UINT type = UI_SWATCHCOLORTYPE_NOCOLOR;
		RGBTRIPLE rgb;

		// The Ribbon framework passes color type as the primary property.
		if (currentValue != NULL)
		{
			// Retrieve color type.
			hr = UIPropertyToUInt32(UI_PKEY_ColorType, *currentValue, &type);
			if (FAILED(hr))
			{
				return hr;
			}
		}

		// The Ribbon framework passes color as additional property if the color type is RGB.
		if (type == UI_SWATCHCOLORTYPE_RGB && commandExecutionProperties != NULL)
		{
			// Retrieve color.
			PROPVARIANT var;
			hr = commandExecutionProperties->GetValue(UI_PKEY_Color, &var);
			if (FAILED(hr))
			{
				return hr;
			}
			UIPropertyToUInt32(UI_PKEY_Color, var, &color);
		}

		rgb.rgbtRed = GetRValue(color);
		rgb.rgbtGreen = GetGValue(color);
		rgb.rgbtBlue = GetBValue(color);

		// Handle the execution event.
		if (verb == UI_EXECUTIONVERB_PREVIEW)
			SFMLwindow->saveColors();
		if (verb == UI_EXECUTIONVERB_CANCELPREVIEW)
			SFMLwindow->restoreColors();

		if (commandId == ID_COLORPICKER_BACKGROUND)
			SFMLwindow->updateBackgroundColor(rgb.rgbtRed, rgb.rgbtGreen, rgb.rgbtBlue);
		if (commandId == ID_COLORPICKER_FILL)
			SFMLwindow->updateFillColor(rgb.rgbtRed, rgb.rgbtGreen, rgb.rgbtBlue);
		if (commandId == ID_COLORPICKER_OUTLINE)
			SFMLwindow->updateOutlineColor(rgb.rgbtRed, rgb.rgbtGreen, rgb.rgbtBlue);
		break;
	}
	}
	return hr;
}

HRESULT MainWindow::CreateUIImageFromBitmapResource(LPCTSTR pszResource, IUIImage **ppimg)
{
	HRESULT hr = E_FAIL;
	*ppimg = NULL;
	IUIImageFromBitmap* m_pifbFactory = NULL;

	if (NULL == m_pifbFactory)
	{
		hr = CoCreateInstance(CLSID_UIRibbonImageFromBitmapFactory, NULL, CLSCTX_ALL, IID_PPV_ARGS(&m_pifbFactory));
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// Load the bitmap from the resource file.
	HBITMAP hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), pszResource, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hbm)
	{
		// Use the factory implemented by the framework to produce an IUIImage.
		hr = m_pifbFactory->CreateImage(hbm, UI_OWNERSHIP_TRANSFER, ppimg);
		if (FAILED(hr))
		{
			DeleteObject(hbm);
		}
	}
	//m_pifbFactory->Release();
	return hr;
}


HRESULT MainWindow::HandleUpdateProperty(UINT32 commandId, REFPROPERTYKEY key, const PROPVARIANT * currentValue, PROPVARIANT * newValue)
{
	HRESULT hr = S_OK;

	switch (commandId)
	{
	case ID_SPINNER:
	{
		// Set the default value
		if (IsEqualPropertyKey(key, UI_PKEY_DecimalValue))
		{
			ZeroMemory(newValue, sizeof(*newValue));
			newValue->vt = VT_DECIMAL;
			VarDecFromR8(1, &newValue->decVal);
			hr = S_OK;
		}

		// Set the minimum value
		if (IsEqualPropertyKey(key, UI_PKEY_MinValue))
		{
			ZeroMemory(newValue, sizeof(*newValue));
			newValue->vt = VT_DECIMAL;
			VarDecFromR8(0.0, &newValue->decVal);
			hr = S_OK;
		}

		// Set the maximum value
		else if (IsEqualPropertyKey(key, UI_PKEY_MaxValue))
		{
			ZeroMemory(newValue, sizeof(*newValue));
			newValue->vt = VT_DECIMAL;
			VarDecFromR8(8.0, &newValue->decVal);
			hr = S_OK;
		}

		// Set the increment
		else if (IsEqualPropertyKey(key, UI_PKEY_Increment))
		{
			ZeroMemory(newValue, sizeof(*newValue));
			newValue->vt = VT_DECIMAL;
			VarDecFromR8(1.0, &newValue->decVal);
			hr = S_OK;
		}

		// Set the number of decimal places
		else if (IsEqualPropertyKey(key, UI_PKEY_DecimalPlaces))
		{
			hr = InitPropVariantFromUInt32(1, newValue);
			hr = S_OK;
		}

		// Set the format string
		else if (IsEqualPropertyKey(key, UI_PKEY_FormatString))
		{
			hr = InitPropVariantFromString(L"px", newValue);
			hr = S_OK;
		}

		// Set the representative string
		else if (IsEqualPropertyKey(key, UI_PKEY_RepresentativeString))
		{
			hr = InitPropVariantFromString(L"AAAAAAA", newValue);
			hr = S_OK;
		}
		break;
	}
	case ID_SHAPEPICKER:
	{
		if (key == UI_PKEY_Categories) {
			hr = false;
		}
		else if (key == UI_PKEY_ItemsSource) {
			IUICollection* pCollection;
			hr = currentValue->punkVal->QueryInterface(IID_PPV_ARGS(&pCollection));
			if (FAILED(hr))
			{
				return hr;
			}
			int imageIds[] = { IDB_BITMAP11, IDB_BITMAP10, IDB_BITMAP12, IDB_BITMAP13 };
			int labelIds[] = { IDS_STRING218, IDS_STRING216, IDS_STRING217, IDS_STRING219 };

			for (int i = 0; i < _countof(imageIds); i++) {
				CPropertySet* pItem;
				hr = CPropertySet::CreateInstance(&pItem);
				if (FAILED(hr))
				{
					pCollection->Release();
					return hr;
				}

				// Create an IUIImage from a resource id.
				IUIImage* pImg;
				hr = CreateUIImageFromBitmapResource(MAKEINTRESOURCE(imageIds[i]), &pImg);
				if (FAILED(hr))
				{
					pCollection->Release();
					pItem->Release();
					return hr;
				}

				// Load the label from the resource file.
				WCHAR wszLabel[MAX_RESOURCE_LENGTH];
				LoadString(GetModuleHandle(NULL), labelIds[i], wszLabel, MAX_RESOURCE_LENGTH);

				// Initialize the property set with the image and label that were just loaded and no category.
				pItem->InitializeItemProperties(pImg, wszLabel, UI_COLLECTION_INVALIDINDEX);

				// Add the newly-created property set to the collection supplied by the framework.
				pCollection->Add(pItem);

				pImg->Release();
				pItem->Release();
			}
			pCollection->Release();
			hr = S_OK;
		}
		else if (key == UI_PKEY_SelectedItem) {
			UINT index = SFMLwindow && SFMLwindow->getCurrentTool() == SFMLTool::Shapes;
			if (index) {
				switch (SFMLwindow->getCurrentShape()) {
				case SFMLShape::SDiamond:
					index = 0;
					break;
				case SFMLShape::SCircle:
					index = 1;
					break;
				case SFMLShape::SRectangle:
					index = 2;
					break;
				case SFMLShape::STriangle:
					index = 3;
					break;
				default:
					index = UI_COLLECTION_INVALIDINDEX;
					break;
				}
			}
			else {
				index = UI_COLLECTION_INVALIDINDEX;
			}
			hr = UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, index, newValue);
		}
		break;
	}
	case ID_TOOL_PENCIL:
		if (key == UI_PKEY_BooleanValue)
			hr = UIInitPropertyFromBoolean(key, SFMLwindow && SFMLwindow->getCurrentTool() == SFMLTool::Pencil, newValue);
		break;
	case ID_TOOL_TEXT:
		if (key == UI_PKEY_BooleanValue)
			hr = UIInitPropertyFromBoolean(key, SFMLwindow &&SFMLwindow->getCurrentTool() == SFMLTool::Text, newValue);
		break;
	case ID_TOOL_PIPETTE:
		if (key == UI_PKEY_BooleanValue)
			hr = UIInitPropertyFromBoolean(key, SFMLwindow && SFMLwindow->getCurrentTool() == SFMLTool::Pipette, newValue);
		break;
	case ID_TOOL_MAGNIFIER:
		if (key == UI_PKEY_BooleanValue)
			hr = UIInitPropertyFromBoolean(key, SFMLwindow &&SFMLwindow->getCurrentTool() == SFMLTool::Magnifier, newValue);
		break;
	case ID_COLORPICKER_BACKGROUND:
	case ID_COLORPICKER_FILL:
	case ID_COLORPICKER_OUTLINE:
	{
		// Initialize the DDCP color type.
		if (key == UI_PKEY_ColorType)
		{
			hr = UIInitPropertyFromUInt32(key, UI_SWATCHCOLORTYPE_RGB, newValue);
		}
		// Initialize the default selected color for each DDCP.
		else if (key == UI_PKEY_Color)
		{
			if (commandId == ID_COLORPICKER_BACKGROUND)
				hr = UIInitPropertyFromUInt32(key, RGB(255, 255, 255), newValue);
			if (commandId == ID_COLORPICKER_FILL)
				hr = UIInitPropertyFromUInt32(key, RGB(255, 0, 0), newValue);
			if (commandId == ID_COLORPICKER_OUTLINE)
				hr = UIInitPropertyFromUInt32(key, RGB(0, 0, 255), newValue);

		}
		break;
	}
	}

	return hr;
}

void MainWindow::HandleResize()
{
	RECT r;
	if (!GetClientRect(hWnd, &r)) return;
	int height = r.bottom - ribbonHeight;
	if (height < 0) height = 0;
	if (SFMLwindow)
		SFMLwindow->setSize(0, ribbonHeight, r.right, height);
	//SetWindowPos(sfmlHWND, NULL, 0, ribbonHeight, r.right, height, SWP_NOZORDER);
}
