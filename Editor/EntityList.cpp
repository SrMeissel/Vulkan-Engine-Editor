#include "EntityList.h"

void EntityList::addEntry(std::string name, uint64_t ID) {
    entries.emplace_back(name, ID);
    EntityEntry& entry = entries.back();

    RECT rect;
    GetClientRect(m_hwnd, &rect);
    if (!entry.Create(std::to_wstring(entry.ID).c_str(), WS_CHILD, 0, 0, 0, rect.right - rect.left, 75, m_hwnd)) {
        MessageBox(NULL, L"Entry", L"fail", MB_OK);
        return;
    }
    resize();
}

void EntityList::removeEntry(uint64_t ID) {
    auto iter = std::find(entries.begin(), entries.end(), ID);
    if (iter == entries.end()) return;
    DestroyWindow(iter->Window());
    entries.erase(iter);
    resize();
}

void EntityList::resize() {
    int currentHeight = 0;
    for (auto& entry : entries) {
        SetWindowPos(entry.Window(), NULL, 0, currentHeight, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        ShowWindow(entry.Window(), SW_SHOW);

        RECT entryRect;
        GetClientRect(entry.Window(), &entryRect);
        currentHeight += entryRect.bottom - entryRect.top;
    }
}

LRESULT EntityList::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        resize();
        break;
    case WM_UPDATESELECTION: {
        EntityEntry* newEntry= (EntityEntry*)lParam;
        if (selectedEntry == newEntry->ID) break;

        newEntry->selected = true;
        InvalidateRect(newEntry->Window(), NULL, TRUE);

        auto iter = std::find(entries.begin(), entries.end(), selectedEntry);
        if (iter != entries.end()) {
            iter->selected = false;
            InvalidateRect(iter->Window(), NULL, TRUE);
        }

        selectedEntry = newEntry->ID;
        break;
        }
    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(m_hwnd, &rect);
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
        break;
        }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

LRESULT EntityEntry::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_LBUTTONDOWN :
            SendMessage(GetParent(m_hwnd), WM_UPDATESELECTION, 0, (LPARAM)this);
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);

            if (selected) {
                RECT rect;
                GetClientRect(m_hwnd, &rect);
                HBRUSH hBrush = CreateSolidBrush(RGB(0, 120, 215)); // Blue border
                FrameRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
            }
            std::wstring displayString = std::wstring(name.begin(), name.end());
            TextOutW(hdc, 10, 10, displayString.c_str(), static_cast<int>(name.size()));

            std::wstring intString = std::to_wstring(ID);
            TextOutW(hdc, 10, 30, intString.c_str(), static_cast<int>(intString.size()));

            EndPaint(m_hwnd, &ps);
            break;
        }
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rect;
            GetClientRect(m_hwnd, &rect);
            HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
            break;
        }
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}