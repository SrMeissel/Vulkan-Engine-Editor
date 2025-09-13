#pragma once

#include "Utils.h"
#include <String>
#include <list>

#define WM_UPDATESELECTION (WM_USER + 1)

class EntityEntry : public BaseWindow<EntityEntry> {
public:
	EntityEntry(std::string name, uint64_t ID) : name{ name }, ID{ ID } {}

	bool operator==(uint64_t value) {
		return value == ID;
	}

	PCWSTR ClassName() const { return L"Entity Entry"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::string name{};
	uint64_t ID{ 0 };

	bool selected = false;
};

class EntityList : public BaseWindow<EntityList> {
public:

	PCWSTR ClassName() const { return L"Entity List"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void addEntry(std::string name, uint64_t ID);
	void removeEntry(uint64_t ID);

private:
	std::list<EntityEntry> entries{};
	uint64_t selectedEntry{ 0 };

	void resize();
};