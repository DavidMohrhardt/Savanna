#pragma once

// Common Types
#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Primitive/FloatingPointUnions.h"
#include "Types/Handle.h"

// Class Definitions

// Common Include Utils.
#include "Utilities/Console.h"
#include "Utilities/PreprocessorDefinitions/EngineWideDefinitions.h"

// Profiler Headers

/**
 * @brief General error codes for the engine. Modules may define their own error codes.
 */
typedef enum SavannaErrorCode
{
    SavannaErrorCode_Failure = 0,
    SavannaErrorCode_Success = 1,
    SavannaErrorCode_InvalidArgument = 2,
    SavannaErrorCode_OutOfMemory = 3,
    SavannaErrorCode_NotImplemented = 4,
    SavannaErrorCode_NotSupported = 5,
    SavannaErrorCode_Unknown = 6,
} SavannaErrorCode;
