#pragma once

/**
 * @brief Header for exposing engine bindings from the C/C++ layer to Python.
 */

#include "Scripting/SavannaScripting.h"

typedef __se_uint64 PythonClassId_t;

const static PythonClassId_t k_InvalidPythonClassId = 0;

namespace Savanna::Scripting::Python
{
    enum PythonInteropErrorCode
    {
        Success = 0,

        ClassRegistrationError,

        AllocationError,

        kEnsure32Bits = 0xffffffff,
    }

    class PythonClassInteropApi
    {
        friend class PythonScriptClass;
        friend void RegisterSavannaPythonClass(PythonClassId_t pythonClassId, size_t totalDataSize);

    private:
        inline static bool TryRegisterPythonClass(PythonClassId_t pythonClassId, size_t totalDataSize)
        {
            std::lock_guard<std::mutex>(g_GlobalScriptingLock);
            if (s_ScriptingTypeSize.contains(pythonClassId))
            {
                s_PythonClassSize.add(pythonClassId, totalDataSize);
                return true;
            }
            return false;
        }

        inline static bool TryGetPythonClassSize(const PythonClassId_t pythonClassId, size_t* outSize)
        {
            std::lock_guard<std::mutex>(g_GlobalScriptingLock);
            if (s_ScriptingTypeSize.contains(pythonClassId))
            {
                *outSize = s_PythonClassSize[pythonClassId];
                return true;
            }
            return false;
        }

        inline static std::unordered_map<PythonClassId_t, size_t> s_PythonClassSize;
    };

    class PythonScriptClass : public Savanna::Scripting::SavannaScriptingClass
    {
    public:
        void* operator new(PythonClassId_t pythonClassId) SAVANNA_NO_EXCEPT
        {
            SAVANNA_INSERT_SCOPED_PROFILER("Operator PythonClass new()");

            size_t requiredSize;
            if (!TryGetPythonClassSize(pythonClassId, &requiredSize))
            {
                return nullptr;
            }

            SEHandle<Allocator> allocatorHandle = SavannaMemoryManager::GetAllocatorHandle(AllocatorKind::Scripting);

            if (!allocatorHandle.IsValid())
            {
                return nullptr;
            }

            SEHandle<T> handle = SEHandle<T>(SavannaMemoryManager::Allocate(allocatorHandle, GetL1CacheLineCount(requiredSize), initializeMemoryToZero));

            if (!handle.IsValid())
            {
                return nullptr;
            }

            handle.Get()->m_AllocatorHandle = allocatorHandle;
            return handle.Get();
        }
    };

    template<T>
    class InternalScriptClass<T> : public Savanna::Scripting::PythonScriptClass
    {
    private:
        inline static PythonClassId_t s_InternalPythonId = k_InvalidPythonClassId;
        inline static bool s_IsRegistered = false;

    protected:
        InternalScriptClass()
        {
            if (!s_IsRegistered && PythonClassInteropApi::TryRegisterInternalPythonClass(s_InternalPythonId));
            {

            }
        }
    public:
        void* operator new(PythonClassId_t pythonClassId) SAVANNA_NO_EXCEPT
        {
            SAVANNA_INSERT_SCOPED_PROFILER("Operator PythonClass new()");

            size_t requiredSize;
            if (!TryGetPythonClassSize(pythonClassId, &requiredSize))
            {
                return nullptr;
            }

            SEHandle<Allocator> allocatorHandle
                = SavannaMemoryManager::GetAllocatorHandle(AllocatorKind::Scripting);

            if (!allocatorHandle.IsValid())
            {
                return nullptr;
            }

            SEHandle<T> handle = SEHandle<T>(SavannaMemoryManager::Allocate(allocatorHandle, GetL1CacheLineCount(requiredSize), initializeMemoryToZero));

            if (!handle.IsValid())
            {
                return nullptr;
            }

            handle.Get()->m_AllocatorHandle = allocatorHandle;
            return handle.Get();
        }
    };
} // namespace Savanna::Scripting::Python

#define SAVANNA_PYTHON_EXPORT(ret) extern "C" ret

SAVANNA_PYTHON_EXPORT(PythonClassId_t) GetNextAvailablePythonClassId()
{
    // PythonScripting
    // using namespace Savanna::Scripting::Python;
    // if (!PythonScriptClass::TryRegisterPythonClass(pythonClassId, totalDataSize))
    // {
    //     return PythonInteropErrorCode::ClassRegistrationError;
    // }
    // return PythonInteropErrorCode::Success;
}

SAVANNA_PYTHON_EXPORT(Savanna::Scripting::Python::PythonInteropErrorCode)
RegisterSavannaPythonClass(PythonClassId_t pythonClassId, size_t totalDataSize)
{
    using namespace Savanna::Scripting::Python;
    if (!PythonScriptClass::TryRegisterPythonClass(pythonClassId, totalDataSize))
    {
        return PythonInteropErrorCode::ClassRegistrationError;
    }
    return PythonInteropErrorCode::Success;
}

SAVANNA_PYTHON_EXPORT(void*) AllocatePythonScriptingClass(PythonClassId_t pythonClassId)
{
    using namespace Savanna::Scripting::Python;
    PythonScriptingClass* pythonScriptClassInstance = new(pythonClassId) PythonScriptingClass();
    if (pythonScriptClassInstance != nullptr)
    {
        return static_cast<void*>(pythonScriptClassInstance);
    }

    return nullptr;
}

SAVANNA_PYTHON_EXPORT(void*) DeletePythonScriptingClass(PythonScriptingClass* scriptingClassPtr)
{
    delete scriptingClassPtr;
}