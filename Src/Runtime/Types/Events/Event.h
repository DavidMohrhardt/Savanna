#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace Savanna
{
    template <typename... Args>
    struct Delegate
    {
        using delegate_function = std::function<void(Args...)>;

        Delegate() = default;
        Delegate(const Delegate&) = default;
        Delegate(Delegate&&) = default;
        Delegate& operator=(const Delegate&) = default;

        Delegate(std::function<void(Args...)> func)
            : m_Func(func)
        {
        }

        void operator()(Args... args)
        {
            m_Func(args...);
        }

        void operator=(std::function<void(Args...)> func)
        {
            m_Func = func;
        }

        bool operator==(const Delegate& other) const
        {
            return m_Func.target_type() == other.m_Func.target_type();
        }

        bool operator!=(const Delegate& other) const
        {
            return !(*this == other);
        }
    };

    template <typename... Args>
    class Event
    {
        using delegate = Delegate<Args...>;
        using delegate_function = typename delegate::delegate_function;

        std::vector<delegate> m_Delegates;

    public:
        Event() = default;
        Event(const Event&) = default;
        Event(Event&&) = default;
        Event& operator=(const Event&) = default;

        virtual ~Event() = default;

        virtual void operator()(Args... args)
        {
            for (auto& func : m_Delegates)
            {
                func(args...);
            }
        }

        virtual void operator+=(const delegate& func)
        {
            m_Delegates.push_back(func);
        }

        virtual void operator-=(const delegate& func)
        {
            m_Delegates.erase(std::remove(m_Delegates.begin(), m_Delegates.end(), func), m_Delegates.end());
        }

        virtual void operator+=(const delegate_function& func)
        {
            m_Delegates.push_back(func);
        }
    };

    template <typename... Args>
    class ThreadSafeEvent : public Event<Args...>
    {
        using delegate = Delegate<Args...>;
        using delegate_function = typename delegate::delegate_function;

        std::mutex m_Mutex;

    public:
        ThreadSafeEvent() = default;
        ThreadSafeEvent(const ThreadSafeEvent&) = default;
        ThreadSafeEvent(ThreadSafeEvent&&) = default;
        ThreadSafeEvent& operator=(const ThreadSafeEvent&) = default;

        virtual ~ThreadSafeEvent() = default;

        virtual void operator()(Args... args) override
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            Event<Args...>::operator()(args...);
        }

        virtual void operator+=(const delegate& func) override
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            Event<Args...>::operator+=(func);
        }

        virtual void operator-=(const delegate& func) override
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            Event<Args...>::operator-=(func);
        }

        virtual void operator+=(const delegate_function& func) override
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            Event<Args...>::operator+=(func);
        }
    };
} // namespace Savanna
