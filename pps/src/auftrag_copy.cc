// generated 2003/5/9 17:37:46 MEST by jacek@ijacek.jacek.de
// using glademm V1.1.3f_cvs
//
// newer (non customized) versions of this file go to auftrag_copy.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "auftrag_copy.hh"
#include <unistd.h>
#include <sys/types.h>
#include "auftrag_bearbeiten.hh"
#include "MyMessage.h"  

extern auftrag_bearbeiten *auftragbearbeiten;
extern MyMessage *meldung;

auftrag_copy::auftrag_copy(AuftragFull *auftrag)
: alt_auftrag(auftrag)
{
 alt_aufnr->set_text(itos(alt_auftrag->Id()));
 alt_aufdatum->set_value(alt_auftrag->getDatum());
 alt_aufdatum->setLabel("");
 alt_aufkunde->set_value(alt_auftrag->getKundennr());
 neu_aufkunde->set_value(alt_auftrag->getKundennr());
 neu_lieferdatum->setLabel("");
 neu_lieferdatum->set_value(ManuProC::Datum());
  
}



void auftrag_copy::on_copy_ok_clicked()
{  
 try {
   AuftragFull *auftrag = new AuftragFull(Auftrag::Anlegen(
   		alt_auftrag->Instanz()),
   	neu_aufkunde->get_value());
     auftrag->setYourAufNr(neu_youraufnr->get_text());

     if(auftrag->getKundennr() == alt_auftrag->getKundennr())
       {auftrag->Zahlart(alt_auftrag->Zahlart()->Id());
        auftrag->setRabatt(alt_auftrag->getAuftragsRabatt().as_float());
       }
     else
       {cH_Kunde k(auftrag->getKundennr());
        auftrag->Zahlart(k->zahlungsart()->Id());     
        if(alt_auftrag->getAuftragsRabatt()!=0)
          auftrag->setRabatt(k->rabatt().as_float());
       }


	int stueck=stueck_uebernehmen.activate() ? neu_stueck:0;
	ManuProC::Datum ld=liefdate_uebernehemen.activate() ? neu_liefdate



      auftrag->setStatusAuftragFull((AufStatVal)OPEN,getuid()); 
      auftragbearbeiten->new_aufid_from_copy=auftrag->Id();
      }
      
 catch(SQLerror &e)
   {meldung->Show(e);
    auftragbearbeiten->new_aufid_from_copy=AuftragBase::none_id;
    return;
   }
}

void auftrag_copy::on_copy_cancel_clicked()
{  
}

