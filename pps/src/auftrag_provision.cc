// generated 2003/6/3 11:30:07 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// newer (non customized) versions of this file go to auftrag_provision.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "auftrag_provision.hh"


auftrag_provision::auftrag_provision(AuftragFull *auftrag)
{
 prov_aufnr->set_text(itos(auftrag->Id()));
 prov_aufdatum->set_value(auftrag->getDatum());
 prov_aufdatum->setLabel("");
 prov_aufkunde->set_value(auftrag->getKundennr());

 if(auftrag->getVerknr()==Kunde::none_id)
   {
    verkprov_frame->set_sensitive(false);
   }
 else
   {
    prov_verkaeufer->set_value(auftrag->getVerknr());
    verkprov_frame->set_sensitive(true);   
    prov_allpos->set_active(true);
 //   fillProvEntryList();
   }

}

void auftrag_provision::on_prov_ok_clicked()
{  
}

void auftrag_provision::on_prov_apply_clicked()
{  
}

void auftrag_provision::on_prov_cancel_clicked()
{  
}

void auftrag_provision::on_prov_enable_toggled()
{  
}

void auftrag_provision::on_prov_verk_activate()
{  
}


