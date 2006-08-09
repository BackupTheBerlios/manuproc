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
#include <Misc/Query.h>
#include <ExtBezSchema/ExtBezSchema.h>
#include <Lieferschein/Lieferschein.h>
#include <Auftrag/AufEintrag.h>
#include <Instanzen/ppsInstanz.h>
#include <Misc/i18n.h>
#include <Misc/ucompose.hpp>

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
    meldung->Show(_("Die Lieferschein Nr. ist nicht korrekt"));
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
  { meldung->Show(String::ucompose(_("%1: Anmeldung nicht möglich"),a.Msg()));
    return;
  }

 ManuProC::dbdefault(c_to_p.Name());



/****
 cH_LieferscheinVoll we(ppsInstanzID::Kundenauftraege,weid);
 ManuProC::dbdisconnect(c_to_p.Name());   
 ManuProC::dbdefault(); // set to default

***/


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
 
 Query::Row is=q.Fetch();
 while(is.good())
   {
    struct we_entry ws;
    std::string bk;
    int lsid,znr,insid;
    
    is >> Query::Row::MapNull(ws.first_auf_ref,"");
    for(int i=0; i<4; i++)
      {is >> Query::Row::MapNull(bk,"");
       ws.artbez.push_back(bk);
      }
    is >> ws.stueck >> Query::Row::MapNull(ws.menge,0)
      >> Query::Row::MapNull(ws.zi,false)
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
          " a.youraufnr ");
        subq  << (*wee).lseb.Instanz()->Id()
          << (*wee).lseb.Id()
          << (*wee).lseb.Zeile();
          
        is=subq.Fetch();
        while(is.good())
          {std::string refauf;
           int menge;
           is >> Query::Row::MapNull(refauf,"") >> menge;
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
        std::vector<cH_Data_Lieferoffen> const &lo_vec=
			getHandleForAufEntry(
                    (AuftragBase::ID)atoi((*refif).first.c_str()),
                     (*wee).artikel.Id(),(*refif).first);
        std::vector<cH_Data_Lieferoffen>::const_iterator lo_it=lo_vec.begin();
	int last_znr=0;
	int stueck=(*refif).second;
	for(;lo_it!=lo_vec.end() && stueck>0;++lo_it)
        {
         if((*lo_it)->Valid())
          {             
           AufEintrag ae((*lo_it)->getAufEintrag());	 
	   int lief = ae.getRestStk().as_int();
	   lief = lief >= stueck ? stueck : lief;
           last_znr=l->push_back(ae,
                 (*wee).artikel,
                 lief,
                  e.hatMenge()?(*wee).menge:0.0,0);
           (*lo_it)->getAufEintrag().tmp_geliefert+=lief;
	   stueck-=lief;
          }
         else
          {last_znr=l->push_back((*wee).artikel,stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);
	   stueck=0;
	  }
       }
       if(stueck>0 && last_znr>0) // Den Rest auf letzte LiefZeile verteilen
	  {LieferscheinEntryBase leb(auftraglieferschein->getInstanz(),
				l->Id(),last_znr);
	   LieferscheinEntry le(leb);
	   le.changeMenge(le.Stueck()+stueck,e.hatMenge()?(*wee).menge:0.0,false);
	  }
       else if(stueck>0) // Den Rest frei liefern
	{last_znr=l->push_back((*wee).artikel,stueck,
         e.hatMenge()?(*wee).menge:0.0,0);
	 stueck=0;
	}

      }
     }  
   else  
     {
      if((*wee).first_auf_ref.empty())
        l->push_back((*wee).artikel,
                 (*wee).stueck,
                  e.hatMenge()?(*wee).menge:0.0,0);
      else             
       {
        std::vector<cH_Data_Lieferoffen> const &lo_vec=getHandleForAufEntry(
                    (AuftragBase::ID)atoi((*wee).first_auf_ref.c_str()),
                     (*wee).artikel.Id(),(*wee).first_auf_ref);
        std::vector<cH_Data_Lieferoffen>::const_iterator lo_it=lo_vec.begin();
	int last_znr=0;
	int stueck=(*wee).stueck;
	for(;lo_it!=lo_vec.end() && stueck>0;++lo_it)
        {
        if((*lo_it)->Valid())
          {             
           AufEintrag ae((*lo_it)->getAufEintrag());	 
	   int lief = ae.getRestStk().as_int();
	   lief = lief >= stueck ? stueck : lief;
           last_znr=l->push_back(ae,
                 (*wee).artikel,
                 lief,
                  e.hatMenge()?(*wee).menge:0.0,0);
           (*lo_it)->getAufEintrag().tmp_geliefert+=lief;
	   stueck-=lief;
          }
        else
           {last_znr=l->push_back((*wee).artikel,
                 stueck,e.hatMenge()?(*wee).menge:0.0,0);        
	    stueck=0;}
	}
	if(stueck>0 && last_znr>0) // Den Rest verteilen
	  {LieferscheinEntryBase leb(auftraglieferschein->getInstanz(),
				l->Id(),last_znr);
	   LieferscheinEntry le(leb);
	   le.changeMenge(le.Stueck()+stueck,e.hatMenge()?(*wee).menge:0.0,false);
	  }
       }
     }
     
   }

 
 auftraglieferschein->set_tree_daten_content(l->Id());
 auftraglieferschein->set_tree_offen_content();  

}


std::vector<cH_Data_Lieferoffen> const& petig_we::getHandleForAufEntry(
      AuftragBase::ID abid, ArtikelBase::ID artid,
      const std::string youraufnr)
{
 typedef std::vector<cH_RowDataBase>::iterator DVI;
 std::vector<cH_RowDataBase> lo=auftraglieferschein->getLiefOff();
 DVI ai;
 static std::vector<cH_Data_Lieferoffen> retvec;

 retvec.erase(retvec.begin(),retvec.end());

 for(ai=lo.begin(); ai!=lo.end(); ++ai)
   {
    Handle<const Data_Lieferoffen> h_lo=
                        (*ai).cast_dynamic<const Data_Lieferoffen>();
    if(h_lo->getArtikel().Id() == artid)
      {
	// first check internal order id
       if( (h_lo->getAufEintrag().Id()==abid && abid!=0) ||
           (atoi(h_lo->getAufEintrag().getYourAufNr().c_str())
	    ==atoi(youraufnr.c_str()) && atoi(youraufnr.c_str())!=0 ))
          retvec.push_back(cH_Data_Lieferoffen(&*h_lo));
      }  
    
   }
return retvec;           
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
    Query::Row fi=query.FetchOne();
    fi >> (*i).artikel;
   }
 
}




void petig_we::on_petig_we_cancel_clicked()
{  
}
