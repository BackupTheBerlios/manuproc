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
#include <ExtBezSchema/ExtBezSchema.h>
#include <Lieferschein/Lieferschein.h>
#include <Auftrag/AufEintrag.h>


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

 int sret=system("mabellas_ssh");
 
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
       "stueck,menge from mabella_lieferscheinentry l left "
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
      {is >> FetchIStream::MapNull(bk,"");
       ws.artbez.push_back(bk);
      }
    is >> ws.stueck >> FetchIStream::MapNull(ws.menge,0);
    we_ls.push_back(ws);
    is=q.Fetch();
   }
   
 ManuProC::dbdisconnect(c_to_p.Name());   
 ManuProC::dbdefault(); // set to default
   
 // find internal article id
 try {identify_article();}
 catch(SQLerror &e)
  { meldung->Show(e);
    return;
  }

 std::vector<struct we_entry>::iterator wee;
  
 H_Lieferschein l=auftraglieferschein->getLieferschein();
 
 for(wee=we_ls.begin(); wee!=we_ls.end(); ++wee)
   {
    if((*wee).artikel.Id() == ArtikelBase::none_id) continue;
    Einheit e((*wee).artikel);

    const AufEintrag AE;
    
    typedef std::vector<cH_RowDataBase>::iterator DVI;
    std::vector<cH_RowDataBase> lo=auftraglieferschein->getLiefOff();
    DVI ai;
    
    for(ai=lo.begin(); ai!=lo.end(); ++ai)
       {
        Handle<const Data_Lieferoffen> h_lo=
                            (*ai).cast_dynamic<const Data_Lieferoffen>();
        int aufid=atoi((*wee).auftrag_referenz.c_str());
         if(h_lo->getAufEintrag().Id()!=aufid)
           if(atoi(h_lo->getAufEintrag().getYourAufNr().c_str())!=aufid)
	     continue;
	 if(AE.getRestStk().as_int()<=0) continue;

         AufEintrag ae(h_lo->getAufEintrag());	 
         l->push_back(ae,
                 (*wee).artikel,
                 (*wee).stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);
         h_lo->getAufEintrag().tmp_geliefert+=(*wee).stueck;
	 break;
       }
       
    if(ai==lo.end()) // without order reference
      l->push_back((*wee).artikel,
                 (*wee).stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);
   }


 

 auftraglieferschein->set_tree_daten_content(l->Id());
 auftraglieferschein->set_tree_offen_content();  
}

/*
void petig_we::print_protokol()
{
 H_Lieferschein l=auftraglieferschein->getLieferschein();
 
 for(wee=we_ls.begin(); wee!=we_ls.end(); ++wee)
   {
    
   }
}
*/


void petig_we::identify_article() throw(SQLerror)
{
 std::vector<struct we_entry>::iterator i;
 int signifikanz=1;
 std::string tabelle("artbez_");
 tabelle+=itos(ArtikelTyp::default_ID)+"_"+itos(ExtBezSchema::default_id);
 
 cH_ExtBezSchema ebz(ExtBezSchema::default_id);

 
 for(i=we_ls.begin(); i!=we_ls.end(); ++i)
   {
    std::string q("select id from ");
    q+=tabelle+" where true "; // true due to "and" before each bezkomp compar.
    
    std::vector<std::string>::iterator bez;

    ExtBezSchema::const_sigiterator bk=ebz->sigbegin(signifikanz);
    for(bez=(*i).artbez.begin();
        bk!=ebz->sigend(signifikanz) &&
        bez!=(*i).artbez.end();++bk,++bez)
      q+=" and "+bk->spaltenname+"=trim("+
            ((*bez).empty()?"''":(*bez))
            +")";
    Query query(q);
    SQLerror::test(__FILELINE__,100);
    if(query.Result()==100) continue;
    FetchIStream fi=query.FetchOne();
    fi >> (*i).artikel;
   }
 
}




void petig_we::on_petig_we_cancel_clicked()
{  
}
