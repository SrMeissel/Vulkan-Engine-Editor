#pragma once

#include "Utils.h"

#include <vector>
#include <chrono>
#include <iostream>;

#include "engineControl.h"

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

