// Editor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Editor.h"
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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EDITOR, szWindowClass, MAX_LOADSTRING);

    Editor editor;
    if (!editor.Create(L"editor", WS_OVERLAPPEDWINDOW)) {
        MessageBox(NULL, L"editor", L"damn", MB_OK);
        return 0;
    }
    ShowWindow(editor.Window(), nCmdShow);

    if (!editor.row.Create(L"ROW", WS_CHILD, 0, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, editor.Window()))
    {
        MessageBox(NULL, L"row", L"you suck", MB_OK);
        return 0;
    }
    if (!editor.entityList.Create(L"THE list",  WS_CHILD, 0, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, editor.row.Window()))
    {
        MessageBox(NULL, L"entityList1", L"you suck", MB_OK);
        return 0;
    }

    if (!editor.engine.Create(L"engine", WS_CHILD, 0, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, editor.row.Window()))
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

    RECT rc;
    GetClientRect(editor.Window(), &rc);
    editor.row.resize(rc.right - rc.left, rc.bottom - rc.top);

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
    /*
    switch (uMsg)
    case WM_SIZE:{
        int width  = LOWORD(lParam);
        int height = HIWORD(lParam);

        // Resize row to fill the editor
        row.resize(width, height);
        break;
    }
    
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
                //have to correct window heiarchy first.
                entityList.addEntry("Hi im freddy", entity);
            }

            lastFileName = fileName;
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            std::cout << "time to load: " << fileName << "\n" << frameTime << "ms \n";
            break;
            }
        }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam); */

    switch (uMsg) {
        case WM_SIZE: {
            int width  = LOWORD(lParam);
            int height = HIWORD(lParam);

            // Resize row to fill the editor
            row.resize(width, height);
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 8008: {
                    static std::string lastFileName;
                    auto currentTime = std::chrono::high_resolution_clock::now();
                    std::string fileName;
                    selectFile(fileName);

                    std::vector<uint64_t> entities = getAllEntities();
                    for (auto entity : entities)
                        entityList.removeEntry(entity);

                    loadCollection(fileName);
                    unloadData(lastFileName);

                    entities = getAllEntities();
                    for (auto entity : entities)
                        entityList.addEntry("Hi im freddy", entity);

                    lastFileName = fileName;
                    auto newTime = std::chrono::high_resolution_clock::now();
                    float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
                    std::cout << "time to load: " << fileName << "\n" << frameTime << "ms \n";
                    break;
                }
            }
            break;
        }

        default:
            return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
    return 0;
}