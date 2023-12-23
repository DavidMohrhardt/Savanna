#include "SavannaScripting.h"


#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>
#include <Types/Manager/GlobalManager.h>

#include <lua.h>
#include <luacode.h>

#include "SavannaScriptingApi.gen.h"

namespace savanna
{
#   define DECL_FUNCTION_TYPES(name) \
        using PFN_##name##_t = decltype(name);

    APPLY_MACRO_SCRIPT_NAMES(DECL_FUNCTION_TYPES);

    template <typename TFunc>
    int InvokeFromScript(lua_State* pState)
    {
        // Does nothing, intended to be specialized
        return 0;
    }

    class ScriptingManager : public GlobalManager<ScriptingManager>
    {
    private:
        lua_State* m_pState;

        void BindScriptingFunctions();

    protected:
        ScriptingManager()
            // : m_pState(lua_newstate(ScriptingAlloc, nullptr))
        {
            // Bind the scripting functions
            BindScriptingFunctions();
        }

        ~ScriptingManager()
        {
            lua_close(m_pState);
        }
    };
} // namespace Savanna

