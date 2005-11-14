aclocal
libtoolize --force --copy
gettextize
if test ! -e po/Makevars
then cp po/Makevars.template po/Makevars
fi
if test ! -e po/LINGUAS
then touch po/LINGUAS
fi
autoheader
automake --add-missing --copy --gnu
autoconf
./configure $*
make
            