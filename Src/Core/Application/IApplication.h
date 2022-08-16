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
        Context m_ApplicationContext;
    protected:
        IApplication(Context&& context) : m_ApplicationContext(std::move(context)) {}

        IApplication() = delete;
        IApplication(const IApplication&) = delete;
        IApplication& operator=(const IApplication&) = delete;
    public:
        virtual ~IApplication() {}
        virtual void Run() = 0;

        Context& GetContext() { return m_ApplicationContext; }
    };
} // namespace Savanna
