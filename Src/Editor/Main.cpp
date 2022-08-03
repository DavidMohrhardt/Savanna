#include "EditorApplication.h"

int main(const int argv, const char** argvs)
{
    SavannaEditor::EditorApplication application = SavannaEditor::EditorApplication();
    application.Initialize();
    application.Run();
    application.Deinitialize();
}
