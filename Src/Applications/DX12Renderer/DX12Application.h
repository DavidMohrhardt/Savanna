/**
 * @file DX12Application.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "Application/IApplication.h"
#include "Window/WindowsRenderWindow.h"

namespace SavannaDX12
{
    class DX12Application : public Savanna::IApplication
    {
    public:
        DX12Application(HINSTANCE hInstance, int nWndCmd);
        ~DX12Application();

        void Run();

    private:
        bool m_Initialized = false;

        WindowsRenderWindow m_RenderWindow;
    };
} // namespace SavannaDX12App
