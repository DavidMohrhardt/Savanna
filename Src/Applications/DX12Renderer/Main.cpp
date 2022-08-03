#include "DX12Application.h"

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    SavannaDX12::DX12Application application(hInstance, nCmdShow);
    application.Run();

    return 0;
}