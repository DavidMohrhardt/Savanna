#pragma once

#include "Utilities/PreprocessorDefinitions/CompilerDefinitions.h"

#if __cplusplus

#define SAVANNA_NO_DISCARD [[ nodiscard ]]

#define SAVANNA_BRANCH_LIKELY [[ likely ]]

#define SAVANNA_BRANCH_UNLIKELY [[ unlikely ]]

#define SAVANNA_NO_EXCEPT noexcept

#endif