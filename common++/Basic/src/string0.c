// $Id: string0.c,v 1.4 2002/05/09 12:46:00 christof Exp $

// these are external functions for the static inline versions in string.h
// just in case you don't want to optimize

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef __MINGW32__
int vsnprintf(char *str, size_t size, const char  *format, va_list ap);
#endif

int snprintf0(char *str,unsigned long n,char const *fmt,...)
{  int retval;
   va_list args;
   va_start (args, fmt);
   retval = vsnprintf (str, n, fmt, args);
   va_end (args);
   if (retval < 0) str[n-1] = '\0';
   return retval;
}

char *strncpy0(char *dest,const char *src, unsigned long n)
{  strncpy(dest,src,n);
   dest[n-1]=0;
   return dest;
}

