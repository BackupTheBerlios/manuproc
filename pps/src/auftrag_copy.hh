// generated 2003/5/9 17:37:46 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// newer (non customized) versions of this file go to auftrag_copy.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _AUFTAG_COPY_HH
#  include "auftrag_copy_glade.hh"
#  define _AUFTAG_COPY_HH

#include <Auftrag/AuftragFull.h>


class auftrag_copy : public auftrag_copy_glade
{  
 AuftragFull *alt_auftrag;

        void on_copy_ok_clicked();
        void on_copy_cancel_clicked();

public:
                     auftrag_copy(AuftragFull *auftrag);

};
#endif
