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
    RECT rect;
    GetWindowRect(m_hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    switch (uMsg)
    {
        case WM_CREATE:
            // Create the ListBox control
            hListBox = CreateWindowEx(
                0,
                L"LISTBOX",              // Predefined class name for ListBox
                L"list",                     // No window title
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_STANDARD,
                0,                     // x position
                0,                     // y position
                width,                    // width
                height,                    // height
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

            break;
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
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rect;
            GetClientRect(m_hwnd, &rect);
            HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0)); // Green background
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
            break;
        }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
