// $Id: compiler_ports.h,v 1.4 2003/05/22 12:50:48 christof Exp $

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

#if defined(__GNUC__)
   // gcc has a parsing problem to determine a_t(b_t(0)) is a function call 
   // not a function declaration. This is the workaround: 
   //    a_t(make_value(b_t(0))
   // and please ignore the "left-hand operand of comma expression has 
   //     no effect" warning
#define make_value(x) (0,x)
#else
#define make_value(x) x
#endif
