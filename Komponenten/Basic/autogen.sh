libtoolize --force --copy
autoheader
if test -d macros
then
  aclocal -I macros
else
  aclocal
fi
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            