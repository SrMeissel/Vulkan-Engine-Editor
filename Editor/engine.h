#pragma once

#include "Resource.h"
#include "Utils.h"
#include "engineControl.h"
#include "keyMap.h"

#include <thread>
#include <string>

void renderLoop();

Key findKey(WPARAM param);

class Engine : public BaseWindow<Engine> {
public:
	PCWSTR ClassName() const { return L"Engine"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void runOnce() { runFrame(); }

private:
	//void renderLoop();
	//HWND engineHandle = NULL;
};

LRESULT Engine::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE: {
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		createEngine(width, height, m_hwnd);
		//MessageBox(NULL, L"engine", L"it works?", MB_OK);
		break;
	}
	case WM_SIZE: {
		RECT rect;
		GetClientRect(m_hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		resize(width, height);
		break;
	}
	case WM_KEYDOWN:
		buttonState(findKey(wParam), true);
		break;
	case WM_KEYUP:
		buttonState(findKey(wParam), false);
		break;
	case WM_LBUTTONDOWN:
		SetFocus(m_hwnd);
		break;
	case WM_DESTROY:
		destroyEngine();
		break;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void renderLoop() {
	createEngine(1280, 720, NULL);
	MessageBox(NULL, L"engine", L"it works?", MB_OK);
	destroyEngine();
}

Key findKey(WPARAM param) {
	auto thing = Win32ToGenericKey.find(static_cast<int>(param));
	if (thing == Win32ToGenericKey.end()) {
		static wchar_t buf[32];
		swprintf(buf, 32, L"Key: %c (VK: 0x%02X)", (char)param, (unsigned int)param);
		MessageBox(NULL, buf, L"Key Pressed", MB_OK);
		return Key::KEY_0;
	}
	else {
		return thing->second;
	}
}