libtoolize --force --copy
autoheader
if [ -d macros -a ! -f /usr/share/aclocal/petig.m4 ]
then
  aclocal -I macros
else
  aclocal
fi
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            