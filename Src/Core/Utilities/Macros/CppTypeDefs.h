#pragma once

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

#ifdef __cplusplus

#undef DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF
#define DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(cName, cppName) \
    namespace Savanna { typedef cName cppName; }

#undef DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF
#define DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(namesp, cName, cppName) \
    namespace Savanna::namesp { typedef cName cppName; }

#endif // end __cplusplus
