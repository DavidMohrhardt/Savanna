/**
 * @file DX12Application.cpp
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "DX12Application.h"

namespace SavannaDX12
{
    DX12Application::DX12Application(HINSTANCE hInstance, int nWndCmd)
        : m_RenderWindow(1920, 1080, "SavannaDX12", hInstance, nWndCmd)
        , m_Initialized(false)
    {
    }

    DX12Application::~DX12Application()
    {
        if (m_Initialized)
        {
            m_Initialized = false;
        }
    }

    void DX12Application::Run()
    {
        m_RenderWindow.DisplayWindow();
        MSG msg = {};
        while (m_Initialized)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    return;
                }
            }
        }
    }
}