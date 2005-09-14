dnl @synopsis AC_LATEX_PACKAGE(<package>,<class>,<variable>)
dnl
dnl This macro test if package in <class> exists and set <variable> to
dnl the right value
dnl
dnl  AC_LATEX_PACKAGE(varioref,book,vbook)
dnl  should set $vbook="yes"
dnl
dnl  AC_LATEX_PACKAGE(xyz,book,vbook)
dnl  should set $vbook="no"
dnl
dnl @category LaTeX
dnl @author Mathieu Boretti boretti@bss-network.com
dnl @version 2005-01-21
dnl @license GPLWithACException

AC_DEFUN([AC_LATEX_PACKAGE],[
if test "$[ac_cv_latex_class_]translit($2,[-],[_])" = "" ;
then
	AC_LATEX_CLASS($2,boretti_classesansparametre)
	export boretti_classesansparametre;
else
	boretti_classesansparametre=$[ac_cv_latex_class_]translit($2,[-],[_]) ;
	export boretti_classesansparemetre;
fi;
if test $boretti_classesansparametre = "no" ;
then
    AC_MSG_ERROR([Unable to find $2 class])
fi
AC_CACHE_CHECK([for $1 in class $2],[ac_cv_latex_]translit($1,[-],[_])[_]translit($2,[-],[_]),[
_AC_LATEX_TEST([
\documentclass{$2}
\usepackage{$1}
\begin{document}
\end{document}
],[ac_cv_latex_]translit($1,[-],[_])[_]translit($2,[-],[_]))
])
$3=$[ac_cv_latex_]translit($1,[-],[_])[_]translit($2,[-],[_]); export $3;
AC_SUBST($3)
])

dnl @synopsis AC_LATEX_CLASSE(<class1>,<var>)
dnl
dnl Test if class1 exists and set $var to the right value
dnl
dnl  AC_LATEX_CLASSES([book],book)
dnl  should set $book="yes"
dnl
dnl  AC_LATEX_CLASSES(allo,book)
dnl  should set $book="no"
dnl
dnl @category LaTeX
dnl @author Mathieu Boretti boretti@bss-network.com
dnl @version 2005-01-21
dnl @license GPLWithACException

AC_DEFUN([AC_LATEX_CLASS],[
AC_CACHE_CHECK([for class $1],[ac_cv_latex_class_]translit($1,[-],[_]),[
_AC_LATEX_TEST([
\begin{document}
\end{document}
],[ac_cv_latex_class_]translit($1,[-],[_]),$1)
])
$2=$[ac_cv_latex_class_]translit($1,[-],[_]) ; export $2;
AC_SUBST($2)
])

dnl @synopsis AC_LATEX_PACKAGE_OPT(<package>,<class>,<variable>,<option>)
dnl
dnl Internal macro for other LaTeX-related tests.
dnl
dnl @category LaTeX
dnl @author Mathieu Boretti <boretti@bss-network.com>
dnl @version 2005-01-21
dnl @license GPLWithACException

AC_DEFUN([_AC_LATEX_TEST],[
AC_REQUIRE([AC_PROG_LATEX])
rm -rf .tmps_latex
mkdir .tmps_latex
cd .tmps_latex
ifelse($#,2,[
$2="no"; export $2;
cat > testconf.tex << \EOF
[$1]
EOF
],$#,3,[
echo "\\documentclass{$3}" > testconf.tex
cat >> testconf.tex << \EOF
[$1]
EOF
],$#,4,[
echo "\\documentclass{$3}" > testconf.tex
echo "\\usepackage{$4}" > testconf.tex
cat >> testconf.tex << \EOF
[$1]
])
cat testconf.tex | $latex 2>&1 1>/dev/null && $2=yes; export $2;
cd ..
rm -rf .tmps_latex
])

dnl @synopsis AC_PROG_LATEX
dnl
dnl This macro test if latex is installed. If latex is installed, it
dnl set $latex to the right value
dnl
dnl @category LaTeX
dnl @author Mathieu Boretti <boretti@bss-network.com>
dnl @version 2005-01-21
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_LATEX],[
AC_CHECK_PROGS(latex,[latex elatex lambda],no)
export latex;
if test $latex = "no" ;
then
	AC_MSG_ERROR([Unable to find a LaTeX application]);
fi
AC_SUBST(latex)
])
