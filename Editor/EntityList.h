#pragma once

#include "Utils.h"

class EntityList : public BaseWindow<EntityList> {
public:
	PCWSTR ClassName() const { return L"Entity List"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    HWND hListBox = NULL;
};


LRESULT EntityList::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            // Create the ListBox control
            hListBox = CreateWindowEx(
                WS_EX_CLIENTEDGE,       // Extended window style
                L"LISTBOX",              // Predefined class name for ListBox
                L"list",                     // No window title
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_STANDARD,
                10,                     // x position
                10,                     // y position
                200,                    // width
                200,                    // height
                m_hwnd,                   // Parent window
                (HMENU)1,               // Control ID
                NULL,                   // Instance handle
                NULL                    // Additional data
            );

            // Add some items to the ListBox
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Item 1");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Item 2");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Item 3");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Item 4");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Item 5");

            return 0;
        case WM_COMMAND:
            // Handle ListBox notifications
            if (LOWORD(wParam) == 1 && HIWORD(wParam) == LBN_SELCHANGE) {
                // Get selected index
                int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

                if (selectedIndex != LB_ERR) {
                    // Get item text
                    char buffer[256];
                    SendMessage(hListBox, LB_GETTEXT, selectedIndex, (LPARAM)buffer);

                    // Display selected item in a message box
                    // MessageBox(hwnd, buffer, "Selected Item", MB_OK);
                }
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
