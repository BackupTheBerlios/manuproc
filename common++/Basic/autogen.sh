libtoolize --force --copy
autoheader
aclocal -Imacros
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            