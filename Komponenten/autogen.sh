aclocal
libtoolize --force --copy
tar cvzf backup.tgz configure.in Makefile.am
gettextize
tar xvzf backup.tgz
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
            