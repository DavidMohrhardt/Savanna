#pragma once

#include "Utilities/PreprocessorDefinitions/CompilerDefinitions.h"

#if __cplusplus

#define SAVANNA_NO_DISCARD [[ nodiscard ]]

#define SAVANNA_NO_EXCEPT noexcept

#endif