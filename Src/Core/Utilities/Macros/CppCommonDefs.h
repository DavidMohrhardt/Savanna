#pragma once

#include "Utilities/PreprocessorDefinitions/CompilerDefinitions.h"

#define SAVANNA_NO_EXCEPT
#define SAVANNA_CONST_EXPR

#if __cplusplus

#undef SAVANNA_NO_EXCEPT
#define SAVANNA_NO_EXCEPT noexcept

#undef SAVANNA_CONST_EXPR
#define SAVANNA_CONST_EXPR(ret) constexpr ret

#endif // end __cplusplus
