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
#include <Misc/Transaction.h>
#include <memory>

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

 std::string nuraktiv(" and coalesce(aktiv,true)=true");
 neu_aufkunde->Einschraenkung(nuraktiv,true);
 neu_aufkunde->EinschraenkenKdGr(KundengruppeID::Auftragsadresse);  
 neu_aufkunde->setExpandStr2(true);
}



void auftrag_copy::on_copy_ok_clicked()
{  
 Transaction tr;

 try {
   std::auto_ptr<AuftragFull> auftrag(new AuftragFull(Auftrag::Anlegen(
   		alt_auftrag->Instanz()),
   	neu_aufkunde->get_value()));
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


	int stueck=0;
	if(stueck_uebernehmen->get_active())
	  {
	   if(!neu_stueck->get_value_as_int()>0)
	    {meldung->Show("Stück muß größer als 0 sein");
    	     return;
    	    }
	   stueck=neu_stueck->get_value_as_int();
	  }			
			
			
	ManuProC::Datum ld;
	if(liefdate_uebernehmen->get_active())
	  {
	   if(!neu_lieferdatum->get_value().valid())
	     {meldung->Show("Ungültiges Lieferdatum");
    	      return;
    	     }	   
	   ld=neu_lieferdatum->get_value();
	  }
			
			
	AuftragBase::ID neu_aufid=auftrag->Id();


	((Query("insert into auftragentry (auftragid,zeilennr,bestellt,"
		"geliefert,lastedit_uid,artikelid,status,preis,"
		"rabatt,lieferdate,preismenge,instanz,preisliste) "
		"(select ?,zeilennr,?,0,?,artikelid,0,preis,rabatt,?,"
		"preismenge,instanz,preisliste from auftragentry"
		" where (auftragid,instanz)=(?,?))")
		<< neu_aufid
		).add_argument(
			stueck==0 ? "bestellt" : itos(stueck))
		<< getuid()
		).add_argument(
			ld.valid() ? 
			ld.postgres_null_if_invalid() : "lieferdate") 
		<< alt_auftrag->Id()
		<< alt_auftrag->Instanz();
	SQLerror::test(__FILELINE__);

      std::auto_ptr<AuftragFull> full(new AuftragFull(
      	AuftragBase(auftrag->Instanz(),auftrag->Id())));

      full->setStatusAuftragFull((AufStatVal)OPEN,getuid()); 
      auftragbearbeiten->new_aufid_from_copy=auftrag->Id();
      }
      
 catch(SQLerror &e)
   {meldung->Show(e);
    auftragbearbeiten->new_aufid_from_copy=AuftragBase::none_id;
    return;
   }
   
 tr.commit();  
}

void auftrag_copy::on_copy_cancel_clicked()
{  
}

void auftrag_copy::on_liefdate_uebernehmen_toggled()
{  
 neu_lieferdatum->set_sensitive(liefdate_uebernehmen->get_active());
}

void auftrag_copy::on_stueck_uebernehmen_toggled()
{  
 neu_stueck->set_sensitive(stueck_uebernehmen->get_active());
}
