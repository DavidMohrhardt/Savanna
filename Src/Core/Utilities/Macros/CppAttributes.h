#pragma once

#include "Utilities/PreprocessorDefinitions/CompilerDefinitions.h"

#define SAVANNA_NO_DISCARD
#define SAVANNA_BRANCH_LIKELY
#define SAVANNA_BRANCH_UNLIKELY

#if __cplusplus

#undef SAVANNA_NO_DISCARD
#define SAVANNA_NO_DISCARD [[ nodiscard ]]

#undef SAVANNA_BRANCH_LIKELY
#define SAVANNA_BRANCH_LIKELY [[ likely ]]

#undef SAVANNA_BRANCH_UNLIKELY
#define SAVANNA_BRANCH_UNLIKELY [[ unlikely ]]

#endif // end __cplusplus
