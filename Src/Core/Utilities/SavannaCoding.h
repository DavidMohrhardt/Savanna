/**
 * @file SavannaCoding.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief The header is a simple header to bring in other non-destructive headers
 * to simplify the process of writing code within the expectations of the engine.
 * @version 0.1
 * @date 2022-07-31
 *
 */
#pragma once

// Preprocessor definitions
#include "PreprocessorDefinitions/ArchitectureDefinitions.h"
#include "PreprocessorDefinitions/CompilerDefinitions.h"
#include "PreprocessorDefinitions/EngineVersionDefinitions.h"
#include "PreprocessorDefinitions/EngineWideDefinitions.h"
#include "PreprocessorDefinitions/InterfaceDefinitions.h"
#include "PreprocessorDefinitions/PlatformDefinitions.h"

// Common types
#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Primitive/FloatingPointUnions.h"

#include "Types/Enums/Enumeration.h"
#include "Types/Enums/Flags.h"

// Assertions
#include "Assertions/SavannaAssertions.h"

// These macro headers are guarded in case compiling with C rather than C++ so it's safe to include
#include "Macros/CppAttributes.h"
#include "Macros/CppCommonDefs.h"
#include "Macros/CppTypeDefs.h"
#include "Macros/Enums.h"

// Some common math helpers
#include "Math/MathHelpers.h"
#include "Math/PointerMath.h"

#ifdef __cplusplus

// For headers that explicitly only work in C++
#include "Types/Exceptions/SavannaException.h"

#endif
