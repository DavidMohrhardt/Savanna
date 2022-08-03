#pragma once

#include "Application/IApplication.h"

namespace SavannaEditor
{
    class EditorApplication : public Savanna::IApplication
    {
    public:
        void Initialize();
        void Run();
        void Deinitialize();
    };
}
