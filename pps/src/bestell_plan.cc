// generated 2005/1/27 10:54:39 CET by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to bestell_plan.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "bestell_plan.hh"
#include <Artikel/ArtikelStamm.h>
#include <Artikel/ArtikelBezeichnung.h>


extern bestell_plan *BP;

void bestell_plan::on_bp_quit_clicked()
{  
 if(BP!=NULL)
   BP=NULL;

 destroy(); 
}

void bestell_plan::on_prev_artikel_clicked()
{  
}

void bestell_plan::on_next_artikel_clicked()
{  
}

void bestell_plan::on_plan_artikel_activate()
{  
 akt_artikel=plan_artikel->get_value();
 load_artikel_list();
}


bestell_plan::bestell_plan(const ArtikelBase ab)
{
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

 Query q(query);

 q.FetchArray(artikel); 
 
 next_artikel->grab_focus();
}


void bestell_plan::clear_all()
{
  bp_lagerbestand->set_text("");
  bp_offauftraege->set_text("");
  bp_verfuegbar->set_text("");
  bp_bestellt->set_text("");
  bp_mindbestand->set_text("");
  bp_abv12m->set_text("");
  bp_abv12m1->set_text("");
  bp_abv12m2->set_text("");
  bp_abv12m3->set_text("");
}


