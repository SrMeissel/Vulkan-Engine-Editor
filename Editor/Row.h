#pragma once

#include "Utils.h"

#include <vector>

#include "engineControl.h"

#include "ShObjIdl.h"
HRESULT selectFile(std::string& fileName);

class Row : public BaseWindow<Row> {
public:
	PCWSTR ClassName() const { return L"Row"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void addWindow(HWND);

	bool fixedWidth{ false };
	bool fixedHeight{ false };
private:
	std::vector<HWND> windows;
	void resize();
};

LRESULT Row::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
		case WM_CREATE:
			resize();
			break;
		case WM_SIZE:	
			resize();
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case 8008: {
				MessageBox(NULL, L"engine", L"loading collection", MB_OK);
				unloadData();
				std::string fileName;
				selectFile(fileName);
				loadCollection(fileName);
				break;
			}
			default:
				break;
			}
			break;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void Row::addWindow(HWND window) {
	windows.push_back(window);
	resize();
}

void Row::resize() {
	int currentXposition = 0;
	RECT rowRect;
	GetWindowRect(m_hwnd, &rowRect);
	int rowWidth = rowRect.right - rowRect.left;

	for (auto window : windows)
	{
		if (window == NULL) continue;

		RECT windowRect;
		GetWindowRect(window, &windowRect);
		int width = windowRect.right - windowRect.left;
		//if (fixedWidth) width = rowWidth / windows.size();

		SetWindowPos(window, NULL, currentXposition, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		currentXposition += width;
	}
}

HRESULT selectFile(std::string& fileName) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); // init COM
	//if (FAILED(hr)) return hr;
	CComPtr<IFileOpenDialog> pFileOpen; // COM Smart Pointer
	hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpen));
	//if (FAILED(hr)) return hr;
	hr = pFileOpen->Show(NULL);
	//if (FAILED(hr)) return hr;
	IShellItem* pItem;
	hr = pFileOpen->GetResult(&pItem);
	if (SUCCEEDED(hr))
	{
		PWSTR pszFilePath;
		hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
		fileName.resize(size_needed);
		WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &fileName[0], size_needed, NULL, NULL);
		CoTaskMemFree(pszFilePath);
		std::wstring stemp = std::wstring(fileName.begin(), fileName.end());
		MessageBox(NULL, L"engine", stemp.c_str(), MB_OK);
	}
	pItem->Release();
	CoUninitialize();
	return hr;
}