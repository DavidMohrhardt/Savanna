/**
 * @file Console.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief Provides console logging utils for the engine.
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <stdio.h>
#include <map>
#include <mutex>
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
        Savanna::Console::PrintfSynchronous(__VA_ARGS__); \
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

        inline const static std::map<Tag, const char*> k_Tags
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
        static void ScheduleLogToFile(const char* format, Args&&... args)
        {
            // TODO
        }

        template<typename... Args>
        static void PrintfSynchronous(const char* format, Args&&... args)
        {
            const std::lock_guard<std::mutex> lock(s_UniversalConsoleLock);
            printf(format, args...);
        }

        template<typename... Args>
        static void LogTagged(Tag tag, const char* format, Args&&... args)
        {
            auto formatString = std::string(k_Tags.at(tag)).append(format).append("\n");
            PrintfSynchronous(formatString.c_str(), args...);
        }

        template<typename... Args>
        static void PrintLineSynchronous(const char* format, Args&&... args)
        {
            PrintfSynchronous(std::string(format).append("\n"), args);
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
