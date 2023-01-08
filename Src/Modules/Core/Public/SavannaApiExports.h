
#ifndef SAVANNA_API_H
#define SAVANNA_API_H

#ifdef SAVANNA_STATIC_DEFINE
#  define SAVANNA_API
#  define SAVANNA_NO_EXPORT
#else
#  ifndef SAVANNA_API
#    ifdef Core_EXPORTS
        /* We are building this library */
#      define SAVANNA_API 
#    else
        /* We are using this library */
#      define SAVANNA_API 
#    endif
#  endif

#  ifndef SAVANNA_NO_EXPORT
#    define SAVANNA_NO_EXPORT 
#  endif
#endif

#ifndef SAVANNA_DEPRECATED
#  define SAVANNA_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef SAVANNA_DEPRECATED_EXPORT
#  define SAVANNA_DEPRECATED_EXPORT SAVANNA_API SAVANNA_DEPRECATED
#endif

#ifndef SAVANNA_DEPRECATED_NO_EXPORT
#  define SAVANNA_DEPRECATED_NO_EXPORT SAVANNA_NO_EXPORT SAVANNA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SAVANNA_NO_DEPRECATED
#    define SAVANNA_NO_DEPRECATED
#  endif
#endif

#endif /* SAVANNA_API_H */
