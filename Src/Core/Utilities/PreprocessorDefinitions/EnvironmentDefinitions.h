#pragma once

#include <cstdint>

#if INTPTR_MAX == INT32_MAX
    #define SAVANNA_ENV_32 1
    #define SAVANNA_ENV_64 0
#elif INTPTR_MAX == INT64_MAX
    #define SAVANNA_ENV_32 0
    #define SAVANNA_ENV_64 1
#else
    #error "Environment is not 32 or 64-bit!"
#endif