libtoolize --force --copy
autoheader2.50
aclocal
automake --add-missing --copy --gnu
autoconf2.50
./configure $*
make
            