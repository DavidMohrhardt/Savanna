#pragma once

/**
 * @brief
 *
 */
#define DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(__cName, __cppName)

/**
 * @brief
 *
 */
#define DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(__namespace, __cName, __cppName)

#ifdef __cplusplus

#undef DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF
#define DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(__cName, __cppName) \
    namespace Savanna { using __cppName = __cName; }

#undef DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF
#define DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(__namespace, __cName, __cppName) \
    namespace Savanna::__namespace { using __cppName = __cName; }

#endif // end __cplusplus
