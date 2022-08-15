/**
 * @file IApplication.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

namespace Savanna
{
    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    class IApplication
    {
        public:
            /**
             * @brief TODO @DavidMohrhardt Document Destroy the IApplication object
             *
             */
            virtual ~IApplication() {}

            /**
             * @brief TODO @DavidMohrhardt Document
             *
             */
            virtual void Run() = 0;
    };
} // namespace Savanna
