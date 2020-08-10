// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

#define MAX_RESOURCE_LENGTH     256
#include <uiribbon.h>

// The implementation of IUISimplePropertySet. This handles all of the properties used for the 
// ItemsSource and Categories PKEYs and provides functions to set only the properties required 
// for each type of gallery contents.
class CPropertySet
	: public IUISimplePropertySet
{
public:

	static HRESULT CreateInstance(CPropertySet **ppPropertySet);


	void InitializeCommandProperties(int categoryId, int commandId, UI_COMMANDTYPE commandType);

	void InitializeItemProperties(IUIImage *image, PCWSTR label, int categoryId);

	void InitializeCategoryProperties(PCWSTR label, int categoryId);

	STDMETHOD(GetValue)(REFPROPERTYKEY key, PROPVARIANT *ppropvar);

	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

private:
	CPropertySet()
		: m_pimgItem(NULL),
		m_categoryId(UI_COLLECTION_INVALIDINDEX),
		m_commandId(-1),
		m_commandType(UI_COMMANDTYPE_UNKNOWN),
		m_cRef(1)
	{
		m_wszLabel[0] = L'\0';
	}

	~CPropertySet()
	{
		if (m_pimgItem != NULL)
		{
			m_pimgItem->Release();
		}
	}

	WCHAR m_wszLabel[MAX_RESOURCE_LENGTH]; // Used for items and categories.
	int m_categoryId; // Used for items, categories, and commands.
	IUIImage* m_pimgItem; // Used for items only.
	int m_commandId; // Used for commands only.
	UI_COMMANDTYPE m_commandType; // Used for commands only.

	LONG m_cRef;
};

