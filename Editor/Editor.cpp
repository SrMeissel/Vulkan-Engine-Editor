// Editor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Editor.h"
#include "EntityList.h"
#include "Row.h"
#include "engine.h"

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
    if (!row.Create(L"ROW", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, 0))
    {
        MessageBox(NULL, L"row", L"you suck", MB_OK);
        return 0;
    }
    EntityList entityList1;
    if (!entityList1.Create(L"THE list",  WS_CHILD, 0, 0, 0, 250, 720, row.Window()))
    {
        MessageBox(NULL, L"entityList1", L"you suck", MB_OK);
        return 0;
    }
    EntityList entityList2;
    if (!entityList2.Create(L"THE list", WS_CHILD, 0, 0, 0, 250, 720, row.Window()))
    {
        MessageBox(NULL, L"entityList2", L"you suck", MB_OK);
        return 0;
    }
    Engine engine;
    if (!engine.Create(L"engine", WS_CHILD, 0, 0, 0, 1280, 720, row.Window()))
    {
        MessageBox(NULL, L"engine", L"you suck", MB_OK);
        return 0;
    }
    
    //menu on row window
    HWND mainWindow = row.Window();
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, 8008, L"Load Collection");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
    SetMenu(mainWindow, hMenu);

    row.addWindow(entityList1.Window());
    row.addWindow(engine.Window());
    row.addWindow(entityList2.Window());

    ShowWindow(row.Window(), nCmdShow);
    ShowWindow(entityList1.Window(), nCmdShow);
    ShowWindow(engine.Window(), nCmdShow);
    ShowWindow(entityList2.Window(), nCmdShow);

    engine.runOnce();
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITOR));
    MSG msg;
    bool loop = true;

    while(loop) {
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                if (msg.message == WM_QUIT) {
                    loop = false;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        engine.runOnce();
    }

    return (int) msg.wParam;
}