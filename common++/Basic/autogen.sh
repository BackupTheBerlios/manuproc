libtoolize --force --copy
autoheader
if [ -d macros ]
then
  aclocal -I macros
else
  aclocal
fi
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            