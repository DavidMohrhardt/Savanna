/**
 * @file Console.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Provides console logging utils for the engine.
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

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

#define SAVANNA_VERBOSE_LOGGING_ENABLED 1
#define SAVANNA_DEBUG_LOGGING_ENABLED 1
#define SAVANNA_WARNING_LOGGING_ENABLED 1

#if SAVANNA_VERBOSE_LOGGING_ENABLED

#undef SAVANNA_DEBUG_LOGGING_ENABLED
#define SAVANNA_DEBUG_LOGGING_ENABLED 1

#endif

#if SAVANNA_DEBUG_LOGGING_ENABLED

#undef SAVANNA_WARNING_LOGGING_ENABLED
#define SAVANNA_WARNING_LOGGING_ENABLED 1

#endif

#define SAVANNA_FATAL_LOG(...) \
{ \
    Savanna::Console::LogTagged(Savanna::Console::Tag::FATAL, __VA_ARGS__); \
}

#if SAVANNA_WARNING_LOGGING_ENABLED
#define SAVANNA_WARNING_LOG(...) \
    { \
        Savanna::Console::LogTagged(Savanna::Console::Tag::WARNING, __VA_ARGS__); \
    }
#else
    #define SAVANNA_WARNING_LOG(...)
#endif

#if SAVANNA_DEBUG_LOGGING_ENABLED
    #define SAVANNA_DEBUG_LOG(...) \
    { \
        Savanna::Console::LogTagged(Savanna::Console::Tag::DEBUG, __VA_ARGS__); \
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

        inline const static std::map<Tag, std::string> k_Tags
        {
            { Tag::DEBUG, "[DEBUG]" },
            { Tag::WARNING, "[WARNING]" },
            { Tag::FATAL, "[FATAL]" },
        };

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
        static void LogTagged(Tag tag, std::string format, Args&&... args)
        {
            auto formatString = std::string(k_Tags.at(tag)).append(format);
            Print(formatString.c_str(), args...);
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
