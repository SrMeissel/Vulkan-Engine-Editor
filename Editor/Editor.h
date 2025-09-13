#pragma once

#include "resource.h"

#include "Utils.h"

#include "Row.h"
#include "EntityList.h"
#include "engine.h"

class Editor : public BaseWindow<Editor> {
public:
	PCWSTR ClassName() const { return L"Row"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	Row row;
	EntityList entityList;
	Engine engine;
};
