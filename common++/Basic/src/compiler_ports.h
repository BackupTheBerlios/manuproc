// $Id: compiler_ports.h,v 1.7 2005/06/17 15:35:48 christof Exp $

// this macros cater for the various incompatible compilers

#if !defined(__GNUC__) || __GNUC__>=3
#  define NOTGCC295(x) x
#  define GCC295(x)
#else
#  define NOTGCC295(x)
#  define GCC295(x) x
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
#define make_value(x) ((void)0,x)
#else
#define make_value(x) x
#endif

// g++-3.0 has problems to decide which operator to chose
#if defined(__GNUC__) && __GNUC__==3 && __GNUC_MINOR__==0
#define std_neq(x,y) std::operator!=((x),(y))
#else
#define std_neq(x,y) (x)!=(y)
#endif
