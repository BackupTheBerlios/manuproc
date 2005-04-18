ln -s Aux Misc
libtoolize --force --copy
autoheader
aclocal-1.7
automake-1.7 --add-missing --copy --gnu
autoconf
./configure $*
make
            