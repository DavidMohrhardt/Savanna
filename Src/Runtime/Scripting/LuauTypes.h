#pragma once

#include <SavannaEngine.h>

#include <lua.h>
#include <luacode.h>

#include <Types\Containers\Arrays\dynamic_array.h>

namespace savanna::luau
{
    template <typename T, typename U>
    struct PrimitiveType
    {
        using luau_t = T;
        using native_t = U
        static_assert(sizeof(luau_t) == sizeof(native_t));
        union
        {
            luau_t m_ScriptingValue;
            native_t m_NativeValue;
        };

        luau_t& operator*() { return m_ScriptingValue; }
        native_t& operator*() { return m_ScriptingValue; }

        luau_t& GetData() { return m_ScriptingValue; }
    };

    template <typename T>
    inline int ReadStackPrimitive(lua_State* pState, int idx, T& outData)
    {
        static_assert(false, "Must be specialized.");
        return -1;
    }

    template<typename TPrimitive>
    void PushStack(lua_State* pState, TPrimitive& data)
    {
        static_assert(false, "Must be specialized.");)
    }

#define DECL_LUAU_PRIMITIVE(name, luauType, nativeType) \
    using name = PrimitiveType< luauType , nativeType >; \
    template <> \
    inline int ReadStackPrimitive< name >(lua_State* pState, int idx, name& outData) \
    { \
        int result = 0; \
        outData.m_ScriptingValue = lua_to##name##x(pState, idx, &result); \
        return result; \
    } \
    template <> \
    inline void PushStack< name >(lua_State* pState, name& data) \
    { \
        lua_push##name(pState, data.m_ScriptingValue); \
    }

    DECL_LUAU_PRIMITIVE(number, lua_Number, float64);
    DECL_LUAU_PRIMITIVE(integer, lua_Integer, int);

#undef DECL_LUAU_PRIMITIVE

    template <>
    inline int ReadStackPrimitive<bool>(lua_State* pState, int idx, bool& outData)
    {
        int result = 0;
        outData = lua_toboolean(pState, idx);
        return result;
    }
    template <>
    inline void PushStack< bool >(lua_State* pState, bool& data)
    {
        lua_pushboolean(pState, data);
    }

    /**
    LUA_TNIL = 0,     // must be 0 due to lua_isnoneornil
    LUA_TBOOLEAN = 1, // must be 1 due to l_isfalse


    LUA_TLIGHTUSERDATA,
    LUA_TNUMBER,
    LUA_TVECTOR,

    LUA_TSTRING, // all types above this must be value types, all types below this must be GC types - see iscollectable


    LUA_TTABLE,
    LUA_TFUNCTION,
    LUA_TUSERDATA,
    LUA_TTHREAD,
    LUA_TBUFFER,

    // values below this line are used in GCObject tags but may never show up in TValue type tags
    LUA_TPROTO,
    LUA_TUPVAL,
    LUA_TDEADKEY,

    // the count of TValue type tags
    LUA_T_COUNT = LUA_TPROTO
     *
     */

    struct ObjectBase {};

    template <typename T>
    class Object : public ObjectBase
    {
        virtual void Push() = 0;
        virtual void LoadStack() = 0;
    };
} // namespace Savanna
