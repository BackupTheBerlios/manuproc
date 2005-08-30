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
#include <Instanzen/ppsInstanz.h>

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

 Query q("select (case when zusatzinfo=false then a.youraufnr else '' end),"
       "artikelkomponente(artikelid,?,1,1), " 
       "artikelkomponente(artikelid,?,1,2), "
       "artikelkomponente(artikelid,?,1,3), "
       "artikelkomponente(artikelid,?,1,4), "
       "l.stueck,l.menge,l.zusatzinfo,l.lfrsid,l.zeile,l.instanz "
       " from mabella_lieferscheinentry l left "
       " join auftrag a on (a.auftragid=l.refauftragid and "
       " a.instanz=l.instanz) where lfrsid=?");
     
 Kunde::ID lid=auftraglieferschein->getKdNr();  
 q << lid << lid << lid << lid << weid;
 
 FetchIStream is=q.Fetch();
 while(is.good())
   {
    struct we_entry ws;
    std::string bk;
    int lsid,znr,insid;
    
    is >> FetchIStream::MapNull(ws.first_auf_ref,"");
    for(int i=0; i<4; i++)
      {is >> FetchIStream::MapNull(bk,"");
       ws.artbez.push_back(bk);
      }
    is >> ws.stueck >> FetchIStream::MapNull(ws.menge,0)
      >> FetchIStream::MapNull(ws.zi,false)
      >> lsid >> znr >> insid;
    ws.lseb=LieferscheinEntryBase(LieferscheinBase(
           cH_ppsInstanz((ppsInstanz::ID)insid),lsid),znr);
    
    we_ls.push_back(ws);
    is=q.Fetch();
   }
   
 std::vector<struct we_entry>::iterator wee;
 for(wee=we_ls.begin(); wee!=we_ls.end(); ++wee)
   {  
    if((*wee).zi==true)
      {
       Query subq("select a.youraufnr,sum(menge) "
          " from lieferscheinentryzusatz z left join auftrag a on "
          " (a.auftragid=z.auftragid and  a.instanz=z.instanz)"
          " where (z.instanz,z.lfrsid,z.lfsznr)=(?,?,?) group by"
          " a.youraufnr");
        subq  << (*wee).lseb.Instanz()->Id()
          << (*wee).lseb.Id()
          << (*wee).lseb.Zeile();
          
        is=subq.Fetch();
        while(is.good())
          {std::string refauf;
           int menge;
           is >> FetchIStream::MapNull(refauf,"") >> menge;
           (*wee).auftrag_referenz[refauf]=menge;
           is=subq.Fetch();
          }
      }
    }
   
 ManuProC::dbdisconnect(c_to_p.Name());   
 ManuProC::dbdefault(); // set to default
   
 // find internal article id
 try {identify_article();}
 catch(SQLerror &e)
  { meldung->Show(e);
    return;
  }

 H_Lieferschein l=auftraglieferschein->getLieferschein();
 
 for(wee=we_ls.begin(); wee!=we_ls.end(); ++wee)
   {
    if((*wee).artikel.Id() == ArtikelBase::none_id) continue;

    Einheit e((*wee).artikel);

    if((*wee).zi==true)
      {
      std::map<std::string,int>::const_iterator refif=
                                  (*wee).auftrag_referenz.begin();

      for(;refif!=(*wee).auftrag_referenz.end(); ++refif)
       {
        cH_Data_Lieferoffen h_lo=getHandleForAufEntry(
                    (AuftragBase::ID)atoi((*refif).first.c_str()),
                     (*wee).artikel.Id(),(*refif).first);
        if(h_lo->Valid())
          {             
           AufEintrag ae(h_lo->getAufEintrag());	 
           l->push_back(ae,
                 (*wee).artikel,
                 (*refif).second,
                  e.hatMenge()?(*wee).menge:0.0,0);
           h_lo->getAufEintrag().tmp_geliefert+=(*wee).stueck;
          }
        else
           l->push_back((*wee).artikel,
                  (*refif).second,
                  e.hatMenge()?(*wee).menge:0.0,0);
       }
     }  
   else  
     {
      if((*wee).first_auf_ref.empty())
        l->push_back((*wee).artikel,
                 (*wee).stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);
      else             
       {cH_Data_Lieferoffen h_lo=getHandleForAufEntry(
                    (AuftragBase::ID)atoi((*wee).first_auf_ref.c_str()),
                     (*wee).artikel.Id(),(*wee).first_auf_ref);
        if(h_lo->Valid())
          {             
           AufEintrag ae(h_lo->getAufEintrag());	 
           l->push_back(ae,
                 (*wee).artikel,
                 (*wee).stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);
           h_lo->getAufEintrag().tmp_geliefert+=(*wee).stueck;
          }
        else
           l->push_back((*wee).artikel,
                 (*wee).stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);        
       }
     }
     
   }

 
 auftraglieferschein->set_tree_daten_content(l->Id());
 auftraglieferschein->set_tree_offen_content();  
}


cH_Data_Lieferoffen petig_we::getHandleForAufEntry(
      AuftragBase::ID abid, ArtikelBase::ID artid,
      const std::string youraufnr)
{
 typedef std::vector<cH_RowDataBase>::iterator DVI;
 std::vector<cH_RowDataBase> lo=auftraglieferschein->getLiefOff();
 DVI ai;

 for(ai=lo.begin(); ai!=lo.end(); ++ai)
   {
    Handle<const Data_Lieferoffen> h_lo=
                        (*ai).cast_dynamic<const Data_Lieferoffen>();
    if(h_lo->getArtikel().Id() == artid)
      {
       if(h_lo->getAufEintrag().Id()==abid)
         return cH_Data_Lieferoffen(&*h_lo);    
       if(atoi(h_lo->getAufEintrag().getYourAufNr().c_str())==
          atoi(youraufnr.c_str()))
         return cH_Data_Lieferoffen(&*h_lo);           
      }  
   }
 return cH_Data_Lieferoffen(new Data_Lieferoffen());
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
            ((*bez).empty()?"''":"'"+(*bez)+"'")
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
