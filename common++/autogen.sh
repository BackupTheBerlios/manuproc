libtoolize --force --copy
autoheader2.50
aclocal-1.7
automake-1.7 --add-missing --copy --gnu
autoconf2.50
./configure $*
make
            