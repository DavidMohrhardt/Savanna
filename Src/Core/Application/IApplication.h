/**
 * @file IApplication.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "Context/Context.h"

namespace Savanna
{
    /**
     * @brief TODO @DavidMohrhardt Document
     */
    class IApplication
    {
    private:
        Context m_Context;
    protected:
        IApplication(Context&& context) : m_Context(std::move(context)) {}

        IApplication() : m_Context(Context()) {}

        IApplication(const IApplication&) = delete;
        IApplication& operator=(const IApplication&) = delete;
    public:
        virtual ~IApplication() {}
        virtual void Run() = 0;

        Context& GetContext() { return m_Context; }
    };
} // namespace Savanna
