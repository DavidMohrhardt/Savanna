/**
 * @file Promise.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief Represents a value that will be available in the future.
 * @version 0.1
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include <future>
#include <thread>

namespace Savanna
{
    template <typename T, typename... Args>
    using PromiseFunc = std::function<T(Args...)>;

    template<typename T>
    class Promise
    {
    public:
        using result_type = T;

        Promise() = default;
        ~Promise() = default;

        Promise(const Promise&) = delete;
        Promise& operator=(const Promise&) = delete;

        Promise(Promise&&) = default;
        Promise& operator=(Promise&&) = default;

        template <typename... Args>
        Promise(PromiseFunc<result_type, Args...> func, Args... args) : m_Future(std::async(std::launch::async, func, args...)) {}

        Promise(std::future<result_type>&& future) : m_Future(std::move(future)) {}

        result_type Get() { return m_Future.get(); }

        bool IsReady() const { return m_Future.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }

        const std::future<result_type>& GetFuture() const { return m_Future; }

        result_type Await() { m_Future.wait(); return m_Future.get(); }

        result_type WaitFor(std::chrono::milliseconds timeout) { m_Future.wait_for(timeout); return m_Future.get(); }
    };
} // namespace Savanna

