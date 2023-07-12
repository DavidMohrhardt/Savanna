/**
 * @file Console.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides console logging utils for the engine.
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include "SavannaCoreConfiguration.h"

// include va_start, va_end, va_list
#include <stdarg.h>
#include <stdio.h>

#include <format>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>

#define SAVANNA_FATAL_LOG(...) \
{ \
    Savanna::Console::Print("[FATAL]: " __VA_ARGS__); \
}

#if SAVANNA_LOG_LEVEL > 0
#define SAVANNA_WARNING_LOG(...) \
    { \
        Savanna::Console::Print("[WARNING]: " __VA_ARGS__); \
    }
#else
    #define SAVANNA_WARNING_LOG(...)
#endif

#if SAVANNA_LOG_LEVEL >= SAVANNA_LOG_LEVEL_DEBUG
    #define SAVANNA_DEBUG_LOG(...) \
    { \
        Savanna::Console::Print("[DEBUG]: " __VA_ARGS__); \
    }
#else
    #define SAVANNA_DEBUG_LOG(...)
#endif

#define SAVANNA_LOG(...) \
    { \
        Savanna::Console::Print(__VA_ARGS__); \
    }

namespace Savanna
{
    // Forward declare log
    class ConsoleLogger;

    class Console
    {
    public:
        enum Tag
        {
            DEBUG,
            WARNING,
            FATAL
        };

        inline static std::mutex s_UniversalConsoleLock = std::mutex();
        inline static std::mutex s_FileLoggingLock = std::mutex();

        inline static bool fileLoggingEnabled = false;

        bool IsFileLoggingEnabled()
        {
            return fileLoggingEnabled;
        }

        template<typename... Args>
        static void ScheduleLogToFile(std::string format, Args&&... args)
        {
            // TODO
        }

        // Example from https://en.cppreference.com/w/cpp/utility/format/format
        template <typename... Args>
        inline static std::string dyna_print(std::string_view rt_fmt_str, Args&&... args)
        {
            return std::vformat(rt_fmt_str, std::make_format_args(args...));
        }

        template<typename... Args>
        static void Print(std::string_view format, Args&&... args)
        {
            const std::lock_guard<std::mutex> lock(s_UniversalConsoleLock);
            std::cout << dyna_print(format, args...) << std::endl;
        }

        template<typename... Args>
        static void PrintLineSynchronous(std::string format, Args&&... args)
        {
            Print(format, args...);
        }

        static void EnableFileLogging(bool shouldEnable)
        {
            fileLoggingEnabled = shouldEnable;
            if (fileLoggingEnabled)
            {
                // CreateFileLoggingThread();
            }
        }
    };
}
