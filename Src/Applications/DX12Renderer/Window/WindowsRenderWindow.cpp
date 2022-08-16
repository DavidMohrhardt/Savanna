/**
 * @file WindowsRenderWindow.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "WindowsRenderWindow.h"

#include <windows.h>

namespace SavannaDX12
{
    WindowsRenderWindow::WindowsRenderWindow(Savanna::uint16 width, Savanna::uint16 height, std::string title, HINSTANCE hInstance, int nWndCmd)
        : m_Width(width)
        , m_Height(height)
        , m_FullscreenMode(false)
        , m_WindowRect()
    {
        // Create Window
        // Initialize the window class.
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = WindowProc;

        // Gets the current executable instance. If the class is in a dll then this is wrong.
        windowClass.hInstance = hInstance;

        windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

        // Convert to LPCSTR
        windowClass.lpszClassName = title.c_str();
        RegisterClassEx(&windowClass);

        m_WindowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };

        // Create window and store a handle
        m_Hwnd = CreateWindow(
            windowClass.lpszClassName,
            title.c_str(), // pSample->GetTitle(),
            m_WindowStyle,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            m_WindowRect.right - m_WindowRect.left,
            m_WindowRect.bottom - m_WindowRect.top,
            nullptr,        // We have no parent window.
            nullptr,        // We aren't using menus.
            hInstance,
            this);

        ShowWindow(m_Hwnd, nWndCmd);
    }

    WindowsRenderWindow::~WindowsRenderWindow()
    {
    }

    void WindowsRenderWindow::DisplayWindow() const
    {
    }

    void WindowsRenderWindow::HideWindow() const
    {
    }

    // Main message handler for the sample.
    LRESULT CALLBACK WindowsRenderWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        WindowsRenderWindow* rendererWindowPtr = reinterpret_cast<WindowsRenderWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        switch (message)
        {
        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;

        //case WM_KEYDOWN:
        //    if (rendererWindowPtr)
        //    {
        //        rendererWindowPtr->GetInputHandler().OnKeyDown(static_cast<UINT8>(wParam));
        //    }
        //    return 0;

        //case WM_KEYUP:
        //    if (rendererWindowPtr)
        //    {
        //        rendererWindowPtr->GetInputHandler().OnKeyUp(static_cast<UINT8>(wParam));
        //    }
        //    return 0;

        //case WM_SYSKEYDOWN:
        //    // Handle ALT+ENTER:
        //    if ((wParam == VK_RETURN) && (lParam & (1 << 29)))
        //    {
        //        if (rendererWindowPtr && rendererWindowPtr->GetDeviceResources()->IsTearingSupported())
        //        {
        //            ToggleFullscreenWindow(rendererWindowPtr->GetSwapchain());
        //            return 0;
        //        }
        //    }
        //    // Send all other WM_SYSKEYDOWN messages to the default WndProc.
        //    break;

        case WM_PAINT:
            if (rendererWindowPtr)
            {
                rendererWindowPtr->OnUpdate();
                rendererWindowPtr->OnRender();
            }
            return 0;

        case WM_SIZE:
            if (rendererWindowPtr)
            {
                RECT windowRect = {};
                GetWindowRect(hWnd, &windowRect);
                rendererWindowPtr->SetWindowBounds(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

                RECT clientRect = {};
                GetClientRect(hWnd, &clientRect);
                rendererWindowPtr->OnSizeChanged(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, wParam == SIZE_MINIMIZED);
            }
            return 0;

        case WM_MOVE:
            if (rendererWindowPtr)
            {
                RECT windowRect = {};
                GetWindowRect(hWnd, &windowRect);
                rendererWindowPtr->SetWindowBounds(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

                int xPos = (int)(short)LOWORD(lParam);
                int yPos = (int)(short)HIWORD(lParam);
                //rendererWindowPtr->OnWindowMoved(xPos, yPos);
            }
            return 0;

        case WM_DISPLAYCHANGE:
            if (rendererWindowPtr)
            {
                //rendererWindowPtr->OnDisplayChanged();
            }
            return 0;

        /*case WM_MOUSEMOVE:
            if (rendererWindowPtr && static_cast<UINT8>(wParam) == MK_LBUTTON)
            {
                UINT x = LOWORD(lParam);
                UINT y = HIWORD(lParam);
                rendererWindowPtr->OnMouseMove(x, y);
            }
            return 0;

        case WM_LBUTTONDOWN:
        {
            UINT x = LOWORD(lParam);
            UINT y = HIWORD(lParam);
            rendererWindowPtr->GetInputHandler.OnLeftButtonDown(x, y);
        }
        return 0;

        case WM_LBUTTONUP:
        {
            UINT x = LOWORD(lParam);
            UINT y = HIWORD(lParam);
            rendererWindowPtr->OnLeftButtonUp(x, y);
        }
        return 0;*/

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        // Handle any messages the switch statement didn't.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

}
