// generated 2001/3/29 11:37:38 CEST by jacek@mimi.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to dialogInfo.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _DIALOGINFO_HH
#  include "dialogInfo_glade.hh"
#  define _DIALOGINFO_HH
#include "glademm_support.hh"

#include<Aux/SQLerror.h>

class dialogInfo : public dialogInfo_glade
{   
        
        friend class dialogInfo_glade;

public:
 dialogInfo(const SQLerror &e);

};
#endif

