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
    namespace Savanna { typedef __cName __cppName; }

#undef DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF
#define DECLARE_SAVANNA_EXTENDED_NAMESPACED_CPP_TYPE_DEF(__namespace, __cName, __cppName) \
    namespace Savanna::__namespace { typedef __cName __cppName; }

#endif // end __cplusplus
