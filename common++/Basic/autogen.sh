libtoolize --force --copy
autoheader2.50
if [ -d macros -a ! -f /usr/share/aclocal/petig.m4 ]
then
  aclocal-1.7 -I macros
else
  aclocal-1.7
fi
automake-1.7 --add-missing --copy --gnu
autoconf2.50
./configure $*
make
            