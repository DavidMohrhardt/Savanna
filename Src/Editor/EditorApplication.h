#pragma once

#include "Application/IApplication.h"

namespace Savanna::Editor
{
    class EditorApplication final : public Savanna::IApplication
    {
    public:
        EditorApplication();
        ~EditorApplication();

        void Run();
    };
}
