
#ifndef SAVANNA_EXPORT_H
#define SAVANNA_EXPORT_H

#ifdef SAVANNA_STATIC_DEFINE
#  define SAVANNA_EXPORT
#  define SAVANNA_NO_EXPORT
#else
#  ifndef SAVANNA_EXPORT
#    ifdef Savanna_EXPORTS
        /* We are building this library */
#      define SAVANNA_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define SAVANNA_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef SAVANNA_NO_EXPORT
#    define SAVANNA_NO_EXPORT 
#  endif
#endif

#ifndef SAVANNA_DEPRECATED
#  define SAVANNA_DEPRECATED __declspec(deprecated)
#endif

#ifndef SAVANNA_DEPRECATED_EXPORT
#  define SAVANNA_DEPRECATED_EXPORT SAVANNA_EXPORT SAVANNA_DEPRECATED
#endif

#ifndef SAVANNA_DEPRECATED_NO_EXPORT
#  define SAVANNA_DEPRECATED_NO_EXPORT SAVANNA_NO_EXPORT SAVANNA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SAVANNA_NO_DEPRECATED
#    define SAVANNA_NO_DEPRECATED
#  endif
#endif

#endif /* SAVANNA_EXPORT_H */
