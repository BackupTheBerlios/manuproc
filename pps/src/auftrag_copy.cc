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
#include <Misc/i18n.h>
#include <DynamicEnums/DynamicConfig.h>

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
#ifdef HAS_ADDR_GROUP_Auftragsadresse
 neu_aufkunde->EinschraenkenKdGr(KundengruppeID::Auftragsadresse);
#endif
 neu_aufkunde->setExpandStr2(true);
}



void auftrag_copy::on_copy_ok_clicked()
{  
 Transaction tr;
 int provsatz;

 try {
   std::auto_ptr<AuftragFull> auftrag(new AuftragFull(Auftrag::Anlegen(
   		alt_auftrag->Instanz()),
   	neu_aufkunde->get_value()));
     auftrag->setYourAufNr(neu_youraufnr->get_text());

     if(auftrag->getKundennr() == alt_auftrag->getKundennr())
       {auftrag->Zahlart(alt_auftrag->Zahlart()->Id());
        auftrag->setRabatt(alt_auftrag->getAuftragsRabatt().as_float());
        provsatz=-1;
       }
     else
       {cH_Kunde k(auftrag->getKundennr());
        auftrag->Zahlart(k->zahlungsart()->Id());     
        if(alt_auftrag->getAuftragsRabatt()!=0)
          auftrag->setRabatt(k->rabatt().as_float());
        provsatz=0;  
       }


	int stueck=0;
	if(stueck_uebernehmen->get_active())
	  {
	   if(!neu_stueck->get_value_as_int()>0)
	    {meldung->Show(_("Stück muß größer als 0 sein"));
    	     return;
    	    }
	   stueck=neu_stueck->get_value_as_int();
	  }			
			
			
	ManuProC::Datum ld;
	if(liefdate_uebernehmen->get_active())
	  {
	   if(!neu_lieferdatum->get_value().valid())
	     {meldung->Show(_("Ungültiges Lieferdatum"));
    	      return;
    	     }	   
	   ld=neu_lieferdatum->get_value();
	  }
			
			
	AuftragBase::ID neu_aufid=auftrag->Id();

	ArgumentList args;
	std::string sql="insert into auftragentry (auftragid,zeilennr,bestellt,"
		"geliefert,lastedit_uid,artikelid,status,preis,"
		"rabatt,lieferdate,preismenge,instanz,preisliste,provsatz) "
		"(select ?,zeilennr,";
        args << neu_aufid;
        if (!stueck) sql+="bestellt,";
        else { sql+="?,"; args << stueck; }
        sql+="0,?,artikelid,0,preis,rabatt,";
        args << getuid();
        if (!ld.valid()) sql+="lieferdate,";
        else { sql+="?,"; args << ld; }
        sql+="preismenge,instanz,preisliste,";
        if (!provsatz) sql+="provsatz ";
        else { sql+="? "; args << provsatz; }
        sql+="from auftragentry where (instanz,auftragid)=(?,?) "
            "and status!=(?))";
        args << AuftragBase(*alt_auftrag);
        args << (copy_storno->get_active() ? (AufStatVal)STORNO : 
					       (AufStatVal)NOSTAT);
	(Query(sql) << args).Check100();

      std::auto_ptr<AuftragFull> full(new AuftragFull(
      	AuftragBase(auftrag->Instanz(),auftrag->Id())));

	// set provision when different customer
      if(auftrag->getKundennr() != alt_auftrag->getKundennr())
        {
         cH_Kunde k(auftrag->getKundennr());
         fixedpoint<2> ps;
         AuftragFull::AufEintragList::iterator i=full->begin();
         for(;i!=full->end();++i)
           {ps=k->getProvSatz_Artikel((*i).Artikel(),(*i).Rabatt());
            Query("update auftragentry set provsatz=? where " 
            	" (auftragid,instanz,zeilennr)=(?,?,?)")
            	<< ps << auftrag->Id() << auftrag->Instanz()
            	<< (*i).getZnr();
           }
        }


      full->setStatusAuftragFull((AufStatVal)OPEN); 
      auftragbearbeiten->new_aufid_from_copy=auftrag->Id();
      }
      
 catch(SQLerror &e)
   {if(e.Code()!=100)
      meldung->Show(e);
    else meldung->Show(_("keine Zeilen zum Kopieren gefunden"));
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
