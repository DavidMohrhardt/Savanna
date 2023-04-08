#pragma once

#include "CppCommonDefs.h"

#if __cplusplus
#define SAVANNA_EXECUTE_ONCE(__lambda) \
    { static auto __unusedSentinel = __lambda(); }

#else
#define SAVANNA_EXECUTE_ONCE(__lambda)

#endif // end __cplusplus
