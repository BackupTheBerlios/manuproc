// generated 2004/8/30 13:51:10 CEST by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to petig_we.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "petig_we.hh"
#include <Misc/dbconnect.h>
#include <Kunde/Kunde.h>
#include "MyMessage.h"
#include "auftrag_lieferschein.hh"
#include <Misc/FetchIStream.h>

extern MyMessage *meldung;
extern auftrag_lieferschein *auftraglieferschein;


void petig_we::on_petig_we_ok_clicked()
{ 
 ManuProC::Connection c_to_p;
 
 c_to_p.setHost("localhost");
 c_to_p.setDbase("petigdb");
 c_to_p.Port(25432);
 c_to_p.Name("remote_petig");
 c_to_p.User("mabella");

 int weid=atoi(we_lfrsid->get_text().c_str());
 
 if(weid<40000)
   {
    meldung->Show("Die Liefrschein Nr. ist nicht korrekt");
    return;
   } 

 try{ 
 ManuProC::dbconnect(c_to_p); 
 }
 catch(SQLerror &e)
  { meldung->Show(e);
    return;
  }
 catch(ManuProC::AuthError &a)
  { meldung->Show(a.Msg()+": Anmeldung nicht möglich");
    return;
  }

 ManuProC::dbdefault(c_to_p.Name());

 Query q("select a.youraufnr,"
       "artikelkomponente(artikelid,?,1,1), " 
       "artikelkomponente(artikelid,?,1,2), "
       "artikelkomponente(artikelid,?,1,3), "
       "artikelkomponente(artikelid,?,1,4), "
       "stueck from mabella_lieferscheinentry l left "
       " join auftrag a on (a.auftragid=l.refauftragid and "
       " a.instanz=l.instanz) where lfrsid=?");
     
 Kunde::ID lid=auftraglieferschein->getKdNr();  
 q << lid << lid << lid << lid << weid;
 
 FetchIStream is=q.Fetch();
 while(is.good())
   {
    struct we_entry ws;
    std::string bk;
    is >> FetchIStream::MapNull(ws.auftrag_referenz,"");
    for(int i=0; i<4; i++)
      {is >> bk;
       ws.artbez.push_back(bk);
      }
    is >> ws.stueck;
    we_ls.push_back(ws);
    is=q.Fetch();
   }
   
 ManuProC::dbdisconnect(c_to_p.Name());   
   
/*
 lieferschein->push_back(artikel,anzahl->get_value_as_int(),
                  e.hatMenge()?liefermenge->get_value_as_float():0.0,
                  Palette->get_value_as_int());
 lieferschein->push_back(auftragentry, artikel, anzahl->get_value_as_int(),
     		e.hatMenge()?liefermenge->get_value_as_float():0.0,
     		Palette->get_value_as_int());
*/

}

void petig_we::on_petig_we_cancel_clicked()
{  
}
