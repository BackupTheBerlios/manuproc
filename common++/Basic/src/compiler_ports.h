// $Id: compiler_ports.h,v 1.1 2003/01/17 10:21:45 christof Exp $

// this macros cater for the various incompatible compilers

#if defined(__GNUC__) && __GNUC__>=3
#  define NOTGCC295(x) x
#else
#  define NOTGCC295(x)
#endif
