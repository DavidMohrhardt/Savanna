/**
 * @file SavannaCoding.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief The header is a simple header to bring in other non-destructive headers
 * to simplify the process of writing code in the engine.
 * @version 0.1
 * @date 2022-07-31
 *
 * @copyright none
 *
 */

#pragma once

#include "Types/Primitive/PrimitiveTypes.h"
#include "Types/Primitive/PrimitiveUnions.h"

#include "Macros/CppCommonDefs.h"
#include "Macros/CppTypeDefs.h"
#include "Macros/Enums.h"

#include "PreprocessorDefinitions/ArchitectureDefinitions.h"
#include "PreprocessorDefinitions/CompilerDefinitions.h"
#include "PreprocessorDefinitions/EngineVersionDefinitions.h"
#include "PreprocessorDefinitions/EngineWideDefinitions.h"
#include "PreprocessorDefinitions/InterfaceDefinitions.h"
#include "PreprocessorDefinitions/PlatformDefinitions.h"

#if __cplusplus

#include "Types/Exceptions/SavannaException.h"

#endif