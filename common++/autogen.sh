libtoolize --force --copy
autoheader
aclocal
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            