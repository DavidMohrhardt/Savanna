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

#include "Public/ISavannaEngine.h"

// Common types
#include "Types/Enums/Enumeration.h"
#include "Types/Enums/Flags.h"

// Assertions
#include "Assertions/SavannaAssertions.h"

// These macro headers are guarded in case compiling with C rather than C++ so it's safe to include
#include "Macros/CppCommonDefs.h"
#include "Macros/CppFunctionality.h"
#include "Macros/Enums.h"
#include "Macros/PlatformSpecific.h"

// Some common math helpers
#include "Math/MathHelpers.h"
#include "Math/PointerMath.h"

#ifdef __cplusplus
#include "Public/ISavannaEngine.hpp"

// For headers that explicitly only work in C++
#include "Types/Classes/Immovable.h"
#include "Types/Classes/NonCopyable.h"
#include "Types/Exceptions/SavannaException.h"

#include "Utilities/Semantics/Move.h"
#include "Utilities/Semantics/SizeOf.h"
#endif
