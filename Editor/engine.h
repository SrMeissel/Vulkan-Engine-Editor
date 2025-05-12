#pragma once

#include "Utils.h"

#include <thread>

class Engine : public BaseWindow<Engine> {
public:
	PCWSTR ClassName() const { return L"Engine"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void renderLoop();
	HWND engine = NULL;
};

LRESULT Engine::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		std::thread engineThread(renderLoop);
		break;
	}
}

void Engine::renderLoop() {
	//TOOD: do it
}