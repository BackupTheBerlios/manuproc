// generated 2005/1/27 10:54:39 CET by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to bestell_plan.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "bestell_plan.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>
#include "auftrag_bearbeiten.hh"
#include <Misc/FetchIStream.h>

extern bestell_plan *BP;
extern auftrag_bearbeiten *auftragbearbeiten;

void bestell_plan::on_bp_quit_clicked()
{  
 if(BP!=NULL)
   BP=NULL;

 destroy(); 
}

void bestell_plan::set_akt_index()
{
 akt_index=0;
 int idx=0;
 for(std::vector<ArtikelBase>::const_iterator i=artikel.begin();
     i!=artikel.end(); ++i,idx++)
     {if(akt_artikel.Id()==(*i).Id())
        {akt_index=idx;
         break;
        }
     }  
}

void bestell_plan::on_next_artikel_clicked()
{if(akt_index < (artikel.size()-1)) 
   akt_index++;  
 else
   akt_index=0;

 akt_artikel=artikel[akt_index];
 plan_artikel->set_value(akt_artikel);
 load_data(akt_artikel);
}

void bestell_plan::on_prev_artikel_clicked()
{if(akt_index>0) 
   akt_index--;  
 else
   akt_index=artikel.size()-1;

 akt_artikel=artikel[akt_index];
 plan_artikel->set_value(akt_artikel);  
 load_data(akt_artikel);
}

void bestell_plan::on_plan_artikel_activate()
{  
 akt_artikel=plan_artikel->get_value();
 load_artikel_list();
 load_data(akt_artikel);
 set_akt_index();
}

#include <Auftrag/auftrag_status.h>
#include <Auftrag/AuftragBase.h>
#include <Lager/FertigWarenLager.h>
void bestell_plan::load_data(const ArtikelBase a) throw(SQLerror)
{
 std::vector<std::pair<int,int> > bestand;
 int offauf,bestellt;
 
 std::vector<int> lagerids;
 std::vector<std::string> lagernames;
 
 Query q("select lagerid from lager order by lagerid");
 
 q.FetchArray(lagerids);
 for(std::vector<int>::const_iterator i=lagerids.begin();
     i!=lagerids.end(); ++i)
     {
      FertigWarenLager fl(*i);
      
      int b,m;
      std::string query("select coalesce(bestand,0), coalesce(mindbest,0)");
      query += " from "+fl.ViewTabelle()+" where artikelid=?";
      Query(query) << a.Id() >> b >> m;
      bestand.push_back(std::pair<int,int>(b,m));
      lagernames.push_back(fl.Bezeichnung());
     }
 
 Query("SELECT coalesce(sum(bestellt-geliefert),0) from auftrag a join auftragentry e"
    " on (a.instanz=e.instanz and a.auftragid=e.auftragid and "
    "a.instanz=? and a.stat=e.status and a.stat=? and "
    " bestellt>geliefert and e.artikelid=? and a.auftragid>=?)")
    << ppsInstanzID::Kundenauftraege << OPEN << a.Id() 
    << AuftragBase::handplan_auftrag_id >> offauf;

 Query("SELECT coalesce(sum(bestellt-geliefert),0) from auftrag a join auftragentry e"
    " on (a.instanz=e.instanz and a.auftragid=e.auftragid and "
    "a.instanz=? and a.stat=e.status and a.stat=? and "
    " bestellt>geliefert and e.artikelid=? and a.auftragid>=?)")
    << ppsInstanzID::Einkauf << OPEN << a.Id() 
    << AuftragBase::handplan_auftrag_id >> bestellt;     

/* int abverkauf=0;

 Query q1("select coalesce(menge,0) from abverkauf_12m where artikelid=?");
   q1 << a.Id();
 FetchIStream fi=q1.Fetch();
 if(fi.good()) fi >> abverkauf; 
*/
     
// set entreis
 if(bestand.size()>0)
   {bp_lagerbestand->set_text(itos(bestand[0].first));
    bp_mindbestand->set_text(itos(bestand[0].second));
    bp_verfuegbar->set_text(itos(bestand[0].first - offauf));
    lager_name1->set_text(lagernames[0]);
    
    if(bestand.size()>1)
      {bp_lagerbestand2->set_text(itos(bestand[1].first));  
       lager_name2->set_text(lagernames[1]);
       }
   }

 bp_offauftraege->set_text(itos(offauf));
 bp_bestellt->set_text(itos(bestellt));
// bp_abv12m->set_text(itos(abverkauf)); 
 
 KumVal kv=KumVal(reinterpret_cast<int>(abverkauf_kumul->
                   get_menu()->get_active()->get_user_data()));
 load_abverkauf(a,kv);
 
}


void bestell_plan::load_abverkauf(const ArtikelBase a,KumVal kv)
{
 abverkauf_tree->detach_from_clist();
 abverkauf_tree->clear();
 std::vector<cH_RowDataBase> datavec;
 
 Query q("SELECT coalesce(geliefertam,l.datum),stueck*coalesce(menge,1)"
      " from lieferscheinentry e join lieferschein l on "
      "((l.instanz,l.lfrsid)=(e.instanz,e.lfrsid) and l.instanz=?"
      " and e.artikelid=?) left join rechnung r on (r.rngid=l.rngid"
      " and r.rngart='R')");
      
 q << ppsInstanzID::Kundenauftraege << a.Id();

 FetchIStream fi=q.Fetch();
 ManuProC::Datum ld;
 int m;
 while(fi.good())
   {
    fi >> ld >> m;
    datavec.push_back(new Data_Abverkauf(ld,m,kv));
    fi=q.Fetch();
   }
  
 abverkauf_tree->setDataVec(datavec);  
}


bestell_plan::bestell_plan(const ArtikelBase ab)
{
 std::vector<std::string> ct;
 ct.push_back("Lieferzeit");
 ct.push_back("Menge");
 abverkauf_tree->setTitles(ct);
 abverkauf_tree->set_NewNode(&Data_AbverkaufNode::create);

 if(ab.valid())
   load_artikel_list();
}

void bestell_plan::load_artikel_list()
{
 if(!akt_artikel.valid()) return;

 clear_all(); 

 ArtikelStamm as(akt_artikel);
 cH_ArtikelBezeichnung ab(akt_artikel);
 cH_ExtBezSchema ebz=ab->getExtBezSchema();

 std::string tabelle("artbez_");
 tabelle+=itos(ebz->Typ())+"_"+itos(ebz->Id());

 ArtikelBezeichnung::const_sigiterator abit=ab->sigbegin();
 ExtBezSchema::const_psigiterator pit=ebz->psigbegin(true);

 std::string query("select b.id from ");
 query+=tabelle+" b where ";

 for(;abit!=ab->sigend(),pit!=ebz->psigend(true);++abit)
   {// alle Komponenten bis auf die letzte vergleichen.
    // Liefert z.B. alle Artikel, die sich nur in Farbe unterscheiden
    if(abit.BezKomp().bezkomptype==(*pit).bezkomptype)
      {query+=" b."+abit.BezKomp().spaltenname+"='"+(*abit)->getStrVal()+"' and ";
       ++pit; 
      }
   }
 query+=" true ";
 
 std::string order_col;
 ExtBezSchema::const_sigiterator sit=ebz->sigbegin(1);
 for(;sit!=ebz->sigend(1);)
   {
    order_col+=" "+(*sit).spaltenname;
    ++sit;
    if(sit!=ebz->sigend(1)) order_col+=",";
   } 
   
 if(!order_col.empty())  
   query+=" order by "+order_col; 

 Query q(query);

 artikel.clear();
 q.FetchArray(artikel); 
 
 bp_lagerbestand->grab_focus();
}


void bestell_plan::clear_all()
{
  bp_lagerbestand->set_text("");
  bp_offauftraege->set_text("");
  bp_verfuegbar->set_text("");
  bp_bestellt->set_text("");
  bp_mindbestand->set_text("");
  lager_name1->set_text("Lager 1");
  lager_name2->set_text("Lager 2");  
/*  bp_abv12m->set_text("");
  bp_abv12m1->set_text("");
  bp_abv12m2->set_text("");
  bp_abv12m3->set_text("");
*/  
}


void bestell_plan::on_abverkauf_kumul_clicked()
{  
}


const cH_EntryValue Data_Abverkauf::Value(guint seqnr,gpointer gp) const
{

 switch(seqnr) 
   {
    case LIEF_ZEIT : 
        {switch(timecumulate) {
         case KUM_DATUM :
           return cH_EntryValueDatum( datum );
         case KUM_WOCHE :
           return cH_EntryValueKalenderwoche(
                datum.valid() ? datum.KW() : Kalenderwoche());
         case KUM_MONAT :
           return cH_EntryValueMonat(datum);
         case KUM_QUARTAL :
           return cH_EntryValueQuartal(datum);
         case KUM_JAHR :
           return cH_EntryValueIntString(
                datum.valid() ? datum.Jahr() : 0);
         default : return cH_EntryValueDatum(datum);
         }
        }
    case MENGE :  return cH_EntryValueIntString(menge);    
  } 
 return cH_EntryValue();
}

