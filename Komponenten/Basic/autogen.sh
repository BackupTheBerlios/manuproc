#!/bin/sh

if test ! -f install-sh ; then touch install-sh ; fi

MAKE=`which gnumake`
if test ! -x "$MAKE" ; then MAKE=`which gmake` ; fi
if test ! -x "$MAKE" ; then MAKE=`which make` ; fi
HAVE_GNU_MAKE=`$MAKE --version|grep -c "Free Software Foundation"`

if test "$HAVE_GNU_MAKE" != "1"; then 
echo Only non-GNU make found: $MAKE
else
echo `$MAKE --version | head -1` found
fi

if which autoconf2.50 >/dev/null
then AC_POSTFIX=2.50
elif which autoconf >/dev/null
then AC_POSTFIX=""
else 
  echo 'you need autoconfig (2.58+ recommended) to generate the Makefile'
  exit 1
fi
echo `autoconf$AC_POSTFIX --version | head -1` found

if which automake-1.8 >/dev/null
then AM_POSTFIX=-1.8
elif which automake-1.7 >/dev/null
then AM_POSTFIX=-1.7
elif which automake-1.6 >/dev/null
then AM_POSTFIX=-1.6
elif which automake >/dev/null
then AM_POSTFIX=""
else
  echo 'you need automake (1.7.8+ recommended) to generate the Makefile'
  exit 1
fi
echo `automake$AM_POSTFIX --version | head -1` found

echo This script runs automake/-conf, configure and make...
echo You did remember necessary arguments for configure, right?
echo

libtoolize --force --copy
if test -f /usr/share/aclocal/petig.m4
then aclocal$AM_POSTFIX
else aclocal$AM_POSTFIX -I macros
fi
autoheader$AC_POSTFIX
automake$AM_POSTFIX --add-missing --copy --gnu
autoconf$AC_POSTFIX
./configure $* && $MAKE
