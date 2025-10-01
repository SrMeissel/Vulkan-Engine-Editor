#pragma once

#include "Utils.h"
#include <vector>
#include <chrono>
#include <iostream>;
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
	void resize(int totalWidth, int totalHeight);
	void resize();
private:
	std::vector<HWND> windows;
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
	}
	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

void Row::addWindow(HWND window) {
	windows.push_back(window);
	resize();
}

/* void Row::resize() {
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
		

	void Row::resize(int width, int height) {
    	if (m_hWnd) {
        MoveWindow(m_hWnd, 0, 0, width, height, TRUE);
		}
    }

} */

// Row.cpp
void Row::resize(int totalWidth, int totalHeight) {
    if (windows.empty()) return;

    // Example proportions: 20% left, 60% middle, 20% right but these can be changed0
    int leftWidth   = totalWidth * 2 / 10;
    int rightWidth  = totalWidth * 2 / 10;
    int middleWidth = totalWidth - (leftWidth + rightWidth);

    int x = 0;

    // Left window (EntityList)
    if (windows.size() > 0 && windows[0])
        MoveWindow(windows[0], x, 0, leftWidth, totalHeight, TRUE);
    x += leftWidth;

    // Middle window (Engine viewport)
    if (windows.size() > 1 && windows[1])
        MoveWindow(windows[1], x, 0, middleWidth, totalHeight, TRUE);
    x += middleWidth;

    // Right window (future panel)
    if (windows.size() > 2 && windows[2])
        MoveWindow(windows[2], x, 0, rightWidth, totalHeight, TRUE);
}

void Row::resize() {
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    resize(rc.right - rc.left, rc.bottom - rc.top);
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
		//MessageBox(NULL, L"engine", stemp.c_str(), MB_OK);
	}
	pItem->Release();
	CoUninitialize();
	return hr;
}