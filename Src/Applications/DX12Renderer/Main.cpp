/**
 * @file Main.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Implements the "Main" function for the Application.
 * @version 0.1
 * @date 2022-08-02
 *
 **/
#include "DX12Application.h"

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    SavannaDX12::DX12Application application(hInstance, nCmdShow);
    application.Run();

    return 0;
}
