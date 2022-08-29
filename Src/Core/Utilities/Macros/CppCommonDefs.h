#pragma once

#include "Utilities/PreprocessorDefinitions/CompilerDefinitions.h"

#undef SAVANNA_NO_EXCEPT
#define SAVANNA_NO_EXCEPT

#undef SAVANNA_CONST_EXPR
#define SAVANNA_CONST_EXPR

#if __cplusplus

#undef SAVANNA_NO_EXCEPT
#define SAVANNA_NO_EXCEPT noexcept

#undef SAVANNA_CONST_EXPR
#define SAVANNA_CONST_EXPR(ret) constexpr ret

#else // define common declaration modifiers as empty for C

#define constexpr
#define noexcept
#define consteval

#endif // end __cplusplus
