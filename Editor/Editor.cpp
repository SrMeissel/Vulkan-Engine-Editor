// Editor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Editor.h"
#include "EntityList.h"
#include "Row.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EDITOR, szWindowClass, MAX_LOADSTRING);

    Row row;
    if (!row.Create(L"ROW", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, 0))
    {
        MessageBox(NULL, L"row", L"you suck", MB_OK);
        return 0;
    }
    EntityList entityList1;
    if (!entityList1.Create(L"THE list",  WS_CHILD, 0, 0, 0, 250, 250, row.Window()))
    {
        MessageBox(NULL, L"entityList1", L"you suck", MB_OK);
        return 0;
    }
    EntityList entityList2;
    if (!entityList2.Create(L"THE list", WS_CHILD, 0, 0, 0, 250, 250, row.Window()))
    {
        MessageBox(NULL, L"entityList2", L"you suck", MB_OK);
        return 0;
    }
    row.addWindow(entityList1.Window());
    row.addWindow(entityList2.Window());

    ShowWindow(row.Window(), nCmdShow);
    ShowWindow(entityList1.Window(), nCmdShow);
    ShowWindow(entityList2.Window(), nCmdShow);


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}