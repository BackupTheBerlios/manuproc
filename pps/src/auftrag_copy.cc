// generated 2003/5/9 17:37:46 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// newer (non customized) versions of this file go to auftrag_copy.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "auftrag_copy.hh"


auftrag_copy::auftrag_copy(const AuftragFull &auftrag)
{
 alt_aufnr->set_text(itos(auftrag.Id()));
 alt_aufdatum->set_value(auftrag.getDatum());
 alt_aufkunde->set_value(auftrag.getKundennr());
 neu_aufkunde->set_value(auftrag.getKundennr());
 
}



void auftrag_copy::on_copy_ok_clicked()
{  
}

void auftrag_copy::on_copy_cancel_clicked()
{  
}

