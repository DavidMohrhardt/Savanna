/**
 * @file SavannaEngine.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief The primary internal header for the engine.
 * @version 0.1
 * @date 2023-01-08
 *
 * @copyright Copyright (c) 2023
 *
 * @note This header allows a lean and mean mode to be enabled. This mode disables all non-essential features of the engine.
 * Additionally, should a header file only want to include the lean and mean features but it's source file wants to include
 * the full features, the header can define SE_LEAN_AND_MEAN before including this header. This will allow the source file
 * to undefine SE_LEAN_AND_MEAN and include this header again to get the rest of the features.
 */
#undef SE_LEAN_AND_MEAN_DEFINED
#if defined(SE_LEAN_AND_MEAN)
    #define SE_LEAN_AND_MEAN_DEFINED 1
    #undef SE_LEAN_AND_MEAN
#else
    #define SE_LEAN_AND_MEAN_DEFINED 0
#endif

#ifndef __SAVANNA_ENGINE_H_MINIMAL__
#define __SAVANNA_ENGINE_H_MINIMAL__

    #include "Public/ISavannaEngine.hpp"

    // Required
    #include "Memory/MemoryManager.h"

#endif // ifndef __SAVANNA_ENGINE_H_MINIMAL__

#if SE_LEAN_AND_MEAN_DEFINED

    // Common Utilities
    #include "Utilities/Console.h"

#else // !SE_LEAN_AND_MEAN_DEFINED

    #ifndef SAVANNA_ENGINE_H_EXTRA__
    #define SAVANNA_ENGINE_H_EXTRA__

        // Coding includes
        #include "Utilities/SavannaCoding.h"

        // Profiling
        #include "Profiling/Profiler.h"
        #include "Utilities/Console.h"

    #endif // end SAVANNA_ENGINE_H_EXTRA__

#endif // end SE_LEAN_AND_MEAN_DEFINED

#ifndef __SAVANNA_ENGINE_H__
#define __SAVANNA_ENGINE_H__

#endif // ifndef __SAVANNA_ENGINE_H__
