/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// generated 2001/3/28 15:07:10 CEST by jacek@mimi.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to auftrag_rechnung.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "auftrag_rechnung.hh"
#include<Aux/Ausgabe_neu.h>
#include<Aux/itos.h>
#include <Artikel/Artikelpreis.h>
#include <gtk--/menu.h>
#include <tclistleaf.h>
#include "auftrag_rechnung_classes.h"
#include"MyMessage.h"
extern MyMessage *meldung;
#include <Lieferschein/RechnungVoll.h>

void auftrag_rechnung::on_rng_close()
{   
   timeout_connection.disconnect();
   destroy();
}

void auftrag_rechnung::on_rng_neu()
{   
 if(lieferkunde->get_value()!=-1 && lieferkunde->get_value()!=0)
   {
    newRechnung(lieferkunde->get_value());
    rngnr->setContent(Formatiere(rechnung.Id(),0,6,"","",'0'),rechnung.Id());
    on_rngnr_activate();
   }
 else
  lieferkunde->grab_focus();
}


void auftrag_rechnung::newRechnung(const Kunde::ID kid)
{
 try{ rechnung=Rechnung(cH_Kunde(kid)); }
 catch(SQLerror &e)
   {meldung->Show(e);}
  set_rtree_daten_content(rechnung.Id());
}

void auftrag_rechnung::on_rng_save()
{   
}

void auftrag_rechnung::on_rng_preview()
{   
   if (rngnr->get_text()=="") return;
   string command = "auftrag_drucken -a Rechnung -n "
         +rngnr->get_text()
         +" -i "+itos(instanz->Id());
   system(command.c_str());
}

gint auftrag_rechnung::on_rng_print(GdkEventButton *ev)
{   
   if (rngnr->get_text()=="") return false;
   std::string firma="";
   if(checkbutton_firmenpapier->get_active()) firma=" --firma ";
   std::string kopie="";
   if(checkbutton_kopie->get_active()) kopie=" --kopie " ;

   if (ev->button==1)
    {
     std::string command = "auftrag_drucken "+firma+kopie+"-a Rechnung -n "+rngnr->get_text()
         +" -p -i "+itos(instanz->Id())
         ;
     system(command.c_str());
    }
   if (ev->button==2)
    {
     std::string command = "auftrag_drucken --firma -a Rechnung -n "+rngnr->get_text()+" -p -i "+itos(instanz->Id());
     system(command.c_str());
     command = "auftrag_drucken --kopie -a Rechnung -n "+rngnr->get_text()
         +" -p -i "+itos(instanz->Id());
     system(command.c_str());
    }
   if (ev->button==3)
    {
     std::string command = "auftrag_drucken --kopie -a Rechnung -n "+rngnr->get_text()+" -p -i "+itos(instanz->Id());
     system(command.c_str());
     command = "auftrag_drucken --kopie --firma -a Rechnung -n "+rngnr->get_text()
      +" -p -i "+itos(instanz->Id());
     system(command.c_str());
     command = "auftrag_drucken --firma -a Rechnung -n "+rngnr->get_text()
         +" -p -i "+itos(instanz->Id());
     system(command.c_str());
    }
 return false;
}

void auftrag_rechnung::rngzeile_delete()
{
 try{   
// if(selectedrow_rng)
//   {//const Rechnung &rg=rechnung_liste->getRechnung();
   if(!(rechnung.Bezahlt()))
	{
//	 cH_Data_Rechnung entry((dynamic_cast<TreeRow*>(selectedrow_rng))->LeafData());
//	 const_cast<Rechnung&>(rg).deleteLieferschein(entry->LiefId());
    try{
     cH_Data_Rechnung dt(rtree_daten->getSelectedRowDataBase_as<cH_Data_Rechnung>());
     RechnungEntry RE = dt->get_RechnungEntry();
     rechnung.deleteLieferschein(RE.Lfrs_Id());
     } catch(...){}
	 on_rngnr_activate();
//	}
   }
 }
 catch(SQLerror &e)
  {meldung->Show(e); return;}

// selectedrow_rng=NULL;
 rngentry_del->set_sensitive(false); 
}

void auftrag_rechnung::on_rngdate_activate()
{   
// Rechnung &rg = rechnung_liste->getRechnung();
// rg.setze_Datum(rngdatum->get_value());
 rechnung.setze_Datum(rngdatum->get_value());
 label_rechnung_ctl->set_text("Rechungsdatum geändert");
 timeout_connection = Gtk::Main::timeout.connect(slot(this,&auftrag_rechnung::timeout),1000);
}

gint auftrag_rechnung::timeout()
{ 
   label_rechnung_ctl->set_text("");
   return 0; 
}
       

void auftrag_rechnung::redisplay()
{try{ 
// rechnung_liste->clear();
// rechnung_liste->showRechnung(rngnr->Content());
  set_rtree_daten_content(rngnr->Content());
 }
 catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_rechnung::on_rngnr_activate()
{try{
 redisplay();
 lieferkunde->set_value(rechnung.KdNr());   
// offene_lieferscheine->setKunde(cH_Kunde(lieferkunde->get_value()));
// offene_lieferscheine->clear();
// offene_lieferscheine->showOffLief();
 set_rtree_offen_content();
 rng_WWaehrung->set_History(rechnung.getWaehrung()->get_enum());

 fixedpoint<2> rabatt=rechnung.Rabatt();
 if (rabatt<0.0) { rabatt=-rabatt; rabatt_typ->set_history(rabatt_typ::Zuschlag); }
 else rabatt_typ->set_history(rabatt_typ::Rabatt);
 rabatt_wert->set_value(rabatt);
 rngdatum->set_Datum(rechnung.getDatum());
 optionmenu_zahlart->set_history(rechnung.getZahlungsart()->Id());

// rechnung_liste->show();
 rtree_daten->show();
 vbox_n_b_lieferscheine->show();

 }
 catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_rechnung::on_lieferkunde_activate()
{
// offene_lieferscheine->setKunde(cH_Kunde(lieferkunde->get_value()));
// offene_lieferscheine->showOffLief();   
 set_rtree_offen_content();
}

void auftrag_rechnung::preis_activate()
{   
}

void auftrag_rechnung::lieferschein_uebernehmen()
{   
 try{
 if(rtree_offen->selection().size())
   {if (rechnung.Id()<1) on_rng_neu();
   if(!(rechnung.Bezahlt()))
	{
    try {
       cH_Data_RLieferoffen dt(rtree_offen->getSelectedRowDataBase_as<cH_Data_RLieferoffen>());
       rechnung.addLieferschein(dt->get_Lieferschein()->Id());
   	 on_rngnr_activate();
      }
    catch(std::exception &e) {cerr << e.what();}
	}
   }
 }
 catch(SQLerror &e)
 {meldung->Show(e);return;}

}

void auftrag_rechnung::on_rdaten_leaf_selected(cH_RowDataBase d)
{
//  const Data_Rechnung *dt=dynamic_cast<const Data_Rechnung*>(&*d);

  rngentry_del->set_sensitive(true);
}
void auftrag_rechnung::on_unselectrow_rtree(int row, int col, GdkEvent* b)
{
// selectedrow_rng=NULL;
 rngentry_del->set_sensitive(false); 
}


/*
void auftrag_rechnung::on_selectrow_rechnung(int row, int col, GdkEvent* b)
{
 TCListRow_API *tclapi=(TCListRow_API*)(rechnung_liste->get_row_data(row));
 selectedrow_rng=(TreeRow*)(*tclapi).get_user_data();

 if(!selectedrow_rng->Leaf()) return;

 // es ging nicht mit dynamic_cast auf OffAuf_Leaf ??!!
 cH_Rg_RowData entry((dynamic_cast<TreeRow*>(selectedrow_rng))->LeafData());

 
 rngentry_del->set_sensitive(true); 
}
*/

void auftrag_rechnung::on_roffen_leaf_selected(cH_RowDataBase d)
{
 const Data_RLieferoffen *dt=dynamic_cast<const Data_RLieferoffen*>(&*d);
 
 lieferscheinnr->set_text(itos(dt->get_Lieferschein()->Id()));   
 lieferscheindatum->set_value(dt->get_Lieferschein()->LsDatum());
 lief_uebernehmen->set_sensitive(true);
}

/*
void auftrag_rechnung::on_selectrow_offlief(int row, int col, GdkEvent* b)
{
 TCListRow_API *tclapi=(TCListRow_API*)(offene_lieferscheine->get_row_data(row));
 selectedrow_lief=(TreeRow*)(*tclapi).get_user_data();

 // es ging nicht mit dynamic_cast auf OffAuf_Leaf ??!!
 cH_OffLief_RowData entry((dynamic_cast<TreeRow*>(selectedrow_lief))->LeafData());

 lieferscheinnr->set_text(itos(entry->Lief().Id()));
 lieferscheindatum->set_value(entry->Lief().LsDatum()); 
 
 lief_uebernehmen->set_sensitive(true); 
}
*/

void auftrag_rechnung::on_unselectrow_rtree_offen(int row, int col, GdkEvent* b)
{
 lieferscheinnr->set_text("");
 lieferscheindatum->set_value(Petig::Datum::today());  
 lief_uebernehmen->set_sensitive(false); 
}        

void auftrag_rechnung::Preis_setzen()
{  
// ist das noch aktuell? 14.11.2001 MAT
#warning TODO: nur aktiven Preis setzen, wenn selection
   RechnungVoll rg(rechnung.Id());
   for (RechnungVoll::iterator i=rg.begin();i!=rg.end();++i)
   {  Artikelpreis p(cH_Kunde(lieferkunde->get_value())->Preisliste(),i->ArtikelID());
      if (!!p)
      {  i->setzePreis(p.In(rg.getWaehrung()));
      }
   }
   redisplay();
}

void auftrag_rechnung::Preis_ergaenzen()
{  RechnungVoll rg=rechnung.Id();
   for (RechnungVoll::iterator i=rg.begin();i!=rg.end();++i)
   {  if (!(i->getPreis()))
      {  Artikelpreis p(cH_Kunde(lieferkunde->get_value())->Preisliste(),i->ArtikelID());
         if (!(!p))
         {  i->setzePreis(p.In(rg.getWaehrung()));
         }
      }
   }
   redisplay();
}

void auftrag_rechnung::waehrung_geaendert()
{
 rechnung.setzeWaehrung(rng_WWaehrung->get_enum());
 Preis_setzen();
}

void auftrag_rechnung::rabatt_geaendert()
{  
  if ( rechnung.Id()!=-1)
   {
    gtk_spin_button_update(rabatt_wert->gtkobj());
    int plus_minus=(rabatt_typ::enum_t((int)(rabatt_typ->get_menu()->get_active()->get_user_data())))==rabatt_typ::Rabatt?+1:-1;
    rechnung.setze_Rabatt(rabatt_wert->get_value_as_float()*plus_minus);
   }
}


auftrag_rechnung::auftrag_rechnung(cH_ppsInstanz _instanz)
: instanz(_instanz)
{
   set_tree_titles();
   fill_optionmenu_zahlungsart();
   optionmenu_zahlart->get_menu()->deactivate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::optionmenu_zahlart_deactivate));
   rngdatum->setLabel("");
   rtree_daten->hide();
   vbox_n_b_lieferscheine->hide();
   set_rtree_offen_content();
}

void auftrag_rechnung::set_tree_titles()
{
 vector<std::string> t1;
 t1.push_back("Zeile");
 t1.push_back("Lieferschein");
 t1.push_back("Artikel");   
 t1.push_back("Liefermenge");
 t1.push_back("E-Preis");
 t1.push_back("G-Preis");
 rtree_daten->setTitles(t1);  

 vector<std::string> t2;
 t2.push_back("Lieferschein");
 t2.push_back("Liefersch.Datum");
 t2.push_back("Lieferung an");
 rtree_offen->setTitles(t2);
}


void auftrag_rechnung::set_rtree_daten_content(RechnungBase::ID rngid)
{
 RechnungVoll rechnungvoll(RechnungBase::none_id);
 try {
  if(rngid!=RechnungBase::none_id)
    { rechnungvoll = RechnungVoll(rngid);
      rechnung=Rechnung(rngid); 
    }
  else  rechnung=Rechnung(RechnungBase::none_id);
  }
 catch(SQLerror &e) { meldung->Show(e); return; }

 vector<cH_RowDataBase> datavec;
 for( RechnungVoll::const_iterator i=rechnungvoll.begin();i!=rechnungvoll.end();++i)
  {
   datavec.push_back(new Data_Rechnung(*i));
  }
 rtree_daten->setDataVec(datavec);
 
}

void auftrag_rechnung::optionmenu_zahlart_deactivate()
{
  int z = int(optionmenu_zahlart->get_menu()->get_active()->get_user_data());
  if (rechnung.Id()!=-1 && rechnung.Id()!=0)
     rechnung.setze_Zahlungsart(cH_Zahlungsart(z));
}

