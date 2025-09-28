// Editor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Editor.h"

#include "engine.h"
#include "ComponentTypes.hpp"

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

    Editor editor;
    if (!editor.Create(L"editor", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, 0)) {
        MessageBox(NULL, L"editor", L"damn", MB_OK);
        return 0;
    }
    ShowWindow(editor.Window(), nCmdShow);

    if (!editor.row.Create(L"ROW", WS_CHILD, 0, 0, 0, 1920, 1080, editor.Window()))
    {
        MessageBox(NULL, L"row", L"you suck", MB_OK);
        return 0;
    }
    if (!editor.entityList.Create(L"THE list",  WS_CHILD, 0, 0, 0, 250, 720, editor.row.Window()))
    {
        MessageBox(NULL, L"entityList1", L"you suck", MB_OK);
        return 0;
    }

    if (!editor.engine.Create(L"engine", WS_CHILD, 0, 0, 0, 1280, 720, editor.row.Window()))
    {
        MessageBox(NULL, L"engine", L"you suck", MB_OK);
        return 0;
    }
    
    //menu on row window
    HWND mainWindow = editor.Window();
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, 8008, L"Load Collection");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File");
    SetMenu(mainWindow, hMenu);

    editor.row.addWindow(editor.entityList.Window());
    editor.row.addWindow(editor.engine.Window());

    ShowWindow(editor.row.Window(), nCmdShow);
    ShowWindow(editor.entityList.Window(), nCmdShow);
    ShowWindow(editor.engine.Window(), nCmdShow);

    editor.engine.runOnce();
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
        editor.engine.runOnce();
    }

    return (int) msg.wParam;
}

LRESULT Editor::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 8008: {
            static std::string lastFileName;
            //MessageBox(NULL, L"engine", L"loading collection", MB_OK);
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::string fileName;
            selectFile(fileName);

            std::vector<uint64_t> entities = getAllEntities();
            for (auto entity : entities) {
                //have to correct window heiarchy first.
                entityList.removeEntry(entity);
            }

            loadCollection(fileName);
            unloadData(lastFileName);

            entities = getAllEntities();
            for (auto entity : entities) {

				TransformComponent transform = getTransformComponent(entity);
                if (transform.name.empty()) transform.name = generateGenericName();
                entityList.addEntry(transform.name, entity);
            }

            lastFileName = fileName;
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            std::cout << "time to load: " << fileName << "\n" << frameTime << "ms \n";
            break;
            }
        }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}