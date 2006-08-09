// $Id: bestell_plan.cc,v 1.12 2006/08/09 15:36:51 christof Exp $

#include "config.h"
#include "bestell_plan.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>
#include "auftrag_bearbeiten.hh"
#include <Misc/Query.h>
#include <sigc++/compatibility.h>
#include <Misc/i18n.h>

extern bestell_plan *BP;
extern auftrag_bearbeiten *auftragbearbeiten;

void bestell_plan::on_bp_quit_clicked()
{  
 if(BP!=NULL)
   BP=NULL;
 delete this;
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
 int offauf,bestellt,stat,commited,uncommited;
 
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
    "a.instanz=? and a.stat=e.status and a.stat in (?,?) and "
    " bestellt>geliefert and e.artikelid=? and a.auftragid>=?)")
    << ppsInstanzID::Kundenauftraege << OPEN << UNCOMMITED << a.Id() 
    << AuftragBase::handplan_auftrag_id >> offauf;

#ifdef MABELLA_EXTENSIONS
 Query q_best("SELECT e.status,min(coalesce(e.lieferdate,now()::date)),"
    "coalesce(sum(bestellt-geliefert),0) from auftrag a join auftragentry e"
    " on (a.instanz=e.instanz and a.auftragid=e.auftragid and "
    "a.instanz=? and e.status in (?,?) and a.stat in (?,?) and "
    " bestellt>geliefert and e.artikelid=? and a.auftragid>=?) "
	" group by e.status");
  q_best << ppsInstanzID::Einkauf << OPEN << UNCOMMITED << OPEN << UNCOMMITED 
	<< a.Id() << AuftragBase::handplan_auftrag_id;
 Query::Row fi_best=q_best.Fetch();

 bestellt=commited=uncommited=0;
 ManuProC::Datum kw,dummy;
 while(fi_best.good())
   {fi_best >> stat;
    int uc,c;
    if(stat==OPEN) 
      {fi_best >> kw >> c; commited+=c;}
    else if(stat==UNCOMMITED)
      {fi_best >> dummy >> uc; uncommited+=uc;}
    fi_best=q_best.Fetch();
   }
 bestellt=commited+uncommited;
#endif

 int abverkauf=0;

 Query q1("select coalesce(menge,0) from abverkauf_12m where artikelid=?");
   q1 << a.Id();
 Query::Row fi=q1.Fetch();
 if(fi.good()) fi >> abverkauf; 

 ArtikelBase::ID rohart;
 int rohbestellt=0;
 Query q2("select altartikelid from artikelzusammensetzung where id=?");
 q2 << a.Id();
  fi=q2.Fetch();
 if(fi.good())
   {fi >> rohart;
#ifdef MABELLA_EXTENSIONS
    Query("SELECT coalesce(sum(bestellt-geliefert),0) from auftrag a join auftragentry e"
    " on (a.instanz=e.instanz and a.auftragid=e.auftragid and "
    "a.instanz=? and a.stat=e.status and a.stat in (?,?) and "
    " bestellt>geliefert and e.artikelid=? and a.auftragid>=?)")
    << ppsInstanzID::Einkauf << OPEN << UNCOMMITED << rohart 
    << AuftragBase::handplan_auftrag_id >> rohbestellt;
#endif
   }
  
    
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
 bp_commited->set_text(itos(commited));
#ifdef MABELLA_EXTENSIONS
 if(kw.valid()) 
   {std::string kwstr("bestätigt (ab <b>");
    kwstr+=kw.to_locale();
    commited_label->set_text(kwstr+"</b>)");
    commited_label->set_use_markup(true);
   }
 else 
#endif
   commited_label->set_text("bestätigt");
 bp_uncommited->set_text(itos(uncommited));
 bp_rohbestellt->set_text(itos(rohbestellt));
 abverk_12m->set_text(itos(abverkauf)); 
 
 KumVal kv=KumVal(reinterpret_cast<long>(abverkauf_kumul->
                   get_menu()->get_active()->get_data("user_data")));
 load_abverkauf(a,kv);
 
}


void bestell_plan::load_abverkauf(const ArtikelBase a,KumVal kv)
{
 std::vector<cH_RowDataBase> datavec;
 
 Query q("SELECT coalesce(geliefertam,l.datum),stueck*coalesce(menge,1)"
      " from lieferscheinentry e join lieferschein l on "
      "((l.instanz,l.lfrsid)=(e.instanz,e.lfrsid) and l.instanz=?"
      " and e.artikelid=?) left join rechnung r on (r.rngid=l.rngid"
      " and r.rngart='R')");
      
 q << ppsInstanzID::Kundenauftraege << a.Id();

 Query::Row fi=q.Fetch();
 ManuProC::Datum ld;
 int m;
 KumVal ldk=KumVal(reinterpret_cast<long>(liefdate_kumul->
                    get_menu()->get_active()->get_data("user_data")));
 while(fi.good())
   {
    fi >> ld >> m;
    datavec.push_back(new Data_Abverkauf(ld,m,kv,ldk));
    fi=q.Fetch();
   }

 abverkauf_tree->clear();  
 abverkauf_tree->setDataVec(datavec);

}


bestell_plan::bestell_plan(const ArtikelBase ab)
{
 std::vector<std::string> ct(abverkauf_tree->MaxColumns());
 
 ct[SP_COMP_ZEIT]=_("Vergleichszeit 1");
 ct[SP_COMP_ZEIT2]=_("Vergleichszeit 2"); 
 ct[SP_LIEF_ZEIT]=_("Lieferzeit");
 ct[SP_MENGE]=_("Menge");
 abverkauf_tree->setTitles(ct);
 std::vector<float> just(abverkauf_tree->MaxColumns());
 just[SP_COMP_ZEIT]=1;
 just[SP_COMP_ZEIT2]=1;
 just[SP_LIEF_ZEIT]=1;
 just[SP_MENGE]=1;
 
 abverkauf_tree->set_NewNode(&Data_AbverkaufNode::create);
 
 abverkauf_kumul->get_menu()->signal_deactivate().connect(
   SigC::slot(*static_cast<class bestell_plan*>(this),
   &bestell_plan::on_abverkauf_reload_clicked));

 liefdate_kumul->get_menu()->signal_deactivate().connect(
   SigC::slot(*static_cast<class bestell_plan*>(this),
   &bestell_plan::on_abverkauf_reload_clicked));   
 
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
  bp_commited->set_text("");
  commited_label->set_text("bestätigt");
  bp_uncommited->set_text("");
  bp_rohbestellt->set_text("");
  bp_mindbestand->set_text("");
  lager_name1->set_text(_("Lager 1"));
  lager_name2->set_text(_("Lager 2"));
  abverk_12m->set_text("");
}


void bestell_plan::on_abverkauf_reload_clicked()
{  
 KumVal kv=KumVal(reinterpret_cast<long>(abverkauf_kumul->
                   get_menu()->get_active()->get_data("user_data")));
 load_abverkauf(akt_artikel,kv);
}

