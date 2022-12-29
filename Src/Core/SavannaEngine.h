#pragma once

// Common Types
#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Primitive/FloatingPointUnions.h"
#include "Types/Handle.h"
#include "Utilities/PreprocessorDefinitions/EngineWideDefinitions.h"

// Common Utilities
#include "Utilities/Console.h"

// Profiler Headers

/**
 * @brief General error codes for the engine. Modules may define their own additional error codes.
 */
typedef enum se_ErrorCode_t
{
    SavannaErrorCodeFailure = 0,
    SavannaErrorCodeSuccess = 1,
    SavannaErrorCodeInvalidArgument = 2,
    SavannaErrorCodeOutOfMemory = 3,
    SavannaErrorCodeNotImplemented = 4,
    SavannaErrorCodeNotSupported = 5,
    SavannaErrorCodeUnknown = 6,
} se_ErrorCode_t;
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(se_ErrorCode_t, ErrorCode);
