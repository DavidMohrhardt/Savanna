#pragma once

#ifdef __cplusplus

/**
 * @brief
 *
 */
#define DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(cName, cppName) \
    namespace Savanna { typedef cName cppName; }

/**
 * @brief
 *
 */
#define DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(namesp, cName, cppName) \
    namespace Savanna::namesp { typedef cName cppName; }

#else

/**
 * @brief
 *
 */
#define DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(cName, cppName)

/**
 * @brief
 *
 */
#define DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(namesp, cName, cppName)

#endif // end __cplusplus