#!/bin/sh
# $Id: autogen.sh,v 1.4 2004/04/29 14:24:40 christof Exp $

MAKE=`which gnumake`
if test ! -x "$MAKE" ; then MAKE=`which gmake` ; fi
if test ! -x "$MAKE" ; then MAKE=`which make` ; fi
HAVE_GNU_MAKE=`$MAKE --version|grep -c "Free Software Foundation"`

if test "$HAVE_GNU_MAKE" != "1"; then 
echo !!!! Warning: not tested with non Gnu-Make $MAKE
else
echo Found GNU Make at $MAKE ... good.
fi

echo This script runs configure and make...
echo You did remember necessary arguments for configure, right?

libtoolize --force --copy 
autoheader 
aclocal 
automake --add-missing --copy --gnu 
autoconf 
./configure $*
$MAKE
