// $Id: compiler_ports.h,v 1.2 2003/01/27 13:52:47 christof Exp $

// this macros cater for the various incompatible compilers

#if defined(__GNUC__) && __GNUC__>=3
#  define NOTGCC295(x) x
#else
#  define NOTGCC295(x)
#endif

/*
 * taken from include/linux/compiler.h
 *
 * Allow us to mark functions as 'deprecated' and have gcc emit a nice
 * warning for each use, in hopes of speeding the functions removal.
 * Usage is:
 *              int __deprecated foo(void)
 */
#if ( __GNUC__ == 3 && __GNUC_MINOR__ > 0 ) || __GNUC__ > 3
#define __deprecated    __attribute__((deprecated))
#else
#define __deprecated
#endif
