#pragma once
#include "combaseapi.h" 
#include "shobjidl.h"
#include <atlbase.h> //COM smart pointer

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hWnd = hWnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    BaseWindow() : m_hWnd(NULL) { }
    ~BaseWindow() {
        //DestroyWindow(m_hwnd);
    }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);

        //Automatically fit the client area
        if(nWidth == CW_USEDEFAULT || nHeight == CW_USEDEFAULT){
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        

        if(nWidth == CW_USEDEFAULT) nWidth = screenWidth;
        if(nHeight == CW_USEDEFAULT) nHeight = screenHeight;
        }
     /*  {
            RECT rc = { 0, 0, nWidth == CW_USEDEFAULT ? 800 : nWidth, nHeight == CW_USEDEFAULT ? 600 : nHeight };
            AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
            nWidth = rc.right - rc.left;
            nHeight = rc.bottom - rc.top;
        }
            */

        m_hWnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (m_hWnd ? TRUE : FALSE);
    }

    HWND Window() const { return m_hWnd; }

protected:

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hWnd;
};

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}