libtoolize --force --copy
autoheader
aclocal -I macros
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            