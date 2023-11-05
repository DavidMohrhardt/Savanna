/**
 * @file WindowsRenderWindow.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include <Public/ISavannaEngine.h>

#include <string>
#include <wtypes.h>

namespace SavannaDX12
{
    class WindowsRenderWindow
    {
    public:
        WindowsRenderWindow(Savanna::uint16 width, Savanna::uint16 height, std::string title, HINSTANCE hInstance, int nWndCmd);
        ~WindowsRenderWindow();

        void DisplayWindow() const;
        void HideWindow() const;

    private:
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        void OnUpdate()
        {

        }

        void OnSizeChanged(LONG windowWidth, LONG windowHeight, WPARAM wParam)
        {
            m_Width = windowWidth;
            m_Height = windowHeight;
            m_WindowRect = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
            SetWindowBounds(m_WindowRect.left, m_WindowRect.top, m_WindowRect.right, m_WindowRect.bottom);
        }

        void SetWindowBounds(LONG left, LONG top, LONG right, LONG bottom) {}

        void OnInit() {}

        void OnRender() {}

        void OnDestroy()
        {
            DestroyWindow(m_Hwnd);
        }

        void OnKeyDown(UINT8 keyCode)
        {

        }

        void OnKeyUp(UINT8 keyCode)
        {

        }

        void OnWindowMoved(int x, int y)
        {
            if (!SetWindowPos(m_Hwnd, HWND_TOPMOST, x, y, static_cast<int>(m_Width), static_cast<int>(m_Height), SWP_DRAWFRAME))
            {
                throw std::exception();
            }
        }

        void OnMouseMove(UINT x, UINT y)
        {

        }

        void OnLeftButtonDown(UINT x, UINT y) {}
        void OnLeftButtonUp(UINT x, UINT y) {}
        void OnDisplayChanged() {}

    private:
        Savanna::int64 m_Width;
        Savanna::int64 m_Height;
        RECT m_WindowRect;

        bool m_FullscreenMode;

        HWND m_Hwnd;

    private:
        static const UINT m_WindowStyle = WS_OVERLAPPEDWINDOW;
    };
} // namespace SavannaDX12
