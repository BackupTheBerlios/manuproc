// $Id: compiler_ports.h,v 1.3 2003/02/12 13:54:32 christof Exp $

// this macros cater for the various incompatible compilers

#if !defined(__GNUC__) || __GNUC__>=3
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
