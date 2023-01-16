
#ifndef SAVANNA_API_H
#define SAVANNA_API_H

#ifdef SAVANNA_EXPORTS_BUILT_AS_STATIC
#  define SAVANNA_API
#  define SAVANNA_API_NO_EXPORT
#else
#  ifndef SAVANNA_API
#    ifdef Core_shared_EXPORTS
        /* We are building this library */
#      define SAVANNA_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define SAVANNA_API __declspec(dllimport)
#    endif
#  endif

#  ifndef SAVANNA_API_NO_EXPORT
#    define SAVANNA_API_NO_EXPORT 
#  endif
#endif

#ifndef SAVANNA_API_DEPRECATED
#  define SAVANNA_API_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef SAVANNA_API_DEPRECATED_EXPORT
#  define SAVANNA_API_DEPRECATED_EXPORT SAVANNA_API SAVANNA_API_DEPRECATED
#endif

#ifndef SAVANNA_API_DEPRECATED_NO_EXPORT
#  define SAVANNA_API_DEPRECATED_NO_EXPORT SAVANNA_API_NO_EXPORT SAVANNA_API_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef SAVANNA_API_NO_DEPRECATED
#    define SAVANNA_API_NO_DEPRECATED
#  endif
#endif

#endif /* SAVANNA_API_H */
