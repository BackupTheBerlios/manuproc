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

#include "auftrag_lieferschein.hh"
#include"MyMessage.h"
#include<Kunde/Kunde.h>
#include<Aux/Ausgabe_neu.h>
#include<Lieferschein/LieferscheinEntry.h>
#include <Auftrag/selFullAufEntry.h>
#include <Artikel/Einheiten.h>
#include<typeinfo>
#include <tclistleaf.h>
#include "lieferscheinliste.hh"
#include "auftrag_lieferschein_classes.h" // erforderlich?
#include <Lager/Lager.hh>
#include <SearchComboContent.h>

extern MyMessage *meldung;

void auftrag_lieferschein::on_liefer_close()
{   
 destroy();
}

void auftrag_lieferschein::on_liefer_neu()
{   
 if (liefer_kunde->get_value()<1)
 {  liefer_kunde->grab_focus();
    return;
 }
 lieferschein = new Lieferschein(instanz,cH_Kunde(liefer_kunde->get_value()));

 tree_daten->clear();
 liefernr->setContent(Formatiere(lieferschein->Id(),0,6,"","",'0'),lieferschein->Id());
 vbox_eingabe->show();
 tree_daten->show();
}

void auftrag_lieferschein::on_lief_save()
{   
}

void auftrag_lieferschein::on_lief_preview()
{  
   if (liefernr->get_text()=="") return;
   std::string command = "auftrag_drucken -a Lieferschein -n "+liefernr->get_text()+" -i "+ itos(instanz->Id());
   system(command.c_str());
}

void auftrag_lieferschein::on_lief_print()
{  
   if (liefernr->get_text()=="") return;
   std::string command = "auftrag_drucken -a Lieferschein -n "+liefernr->get_text()+" -p -i "+ itos(instanz->Id());
   system(command.c_str()); 
}

void auftrag_lieferschein::display(int lfrsid)
{
 set_tree_daten_content(lfrsid);
 display2(lieferschein->KdNr());
 int rng = lieferschein->RngNr();
 if (rng)
    rngnr->set_text(Formatiere(rng,0,6,"","",'0'));
 else rngnr->set_text("");
 vbox_eingabe->show();
 tree_daten->show();
 liefdate->set_value(lieferschein->getDatum());
}

void auftrag_lieferschein::display2(int kdnr)
{
try{
 liefer_kunde->set_value(kdnr);
 set_tree_offen_content();
   cH_Kunde k(kdnr);
   if(instanz->BestellungFuer()!=ppsInstanz::None)
    { artikelbox->setExtBezSchema(cH_Kunde(Kunde::default_id)->getSchema(ArtikelTyp::Garn)); }
   else if (artikelbox->getBezSchema()->Id() != k->getSchemaId())
    { artikelbox->setExtBezSchema(k->getSchema(ArtikelTyp::AufgemachtesBand)); }
 }
 catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_lieferschein::on_liefnr_activate()
{
 try{
   lieferschein = new Lieferschein(instanz,liefernr->Content());
   display(liefernr->Content());
 }catch(SearchComboContent<int>::ContentError &e)
 { display(atoi(liefernr->get_text().c_str()));
 }
}

void auftrag_lieferschein::on_lieferkunde_activate()
{
try{display2(liefer_kunde->get_value());
 liefernr->reset();
 } catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_lieferschein::on_liefdate_activate()
{ 
try{  
   lieferschein->setDatum(liefdate->get_value());
  } catch(SQLerror &e) {meldung->Show(e);}
}

// ist eigentlich anzahl oder?
void auftrag_lieferschein::on_liefermenge_activate()
{
if (!tree_offen->selection().size()) 
   artikelbox->grab_focus();

}

void auftrag_lieferschein::on_newlieferentry_ok()
{  on_Palette_activate();
}

void auftrag_lieferschein::on_newlieferentryall_ok()
{   
}

void auftrag_lieferschein::on_offen_leaf_selected(cH_RowDataBase d)
{
 const Data_Lieferoffen *dt=dynamic_cast<const Data_Lieferoffen*>(&*d);
 if(dt->getAufEintrag().getRestStk()<=0) return;

 Einheit e(dt->getAufEintrag().Artikel());
 menge_einheit->set_text(e);
 if (e==Einheit::Stueck) 
 {  anzahl->set_value(dt->getAufEintrag().getRestStk());
    liefermenge->set_value(0.0);
    label_menge->hide();
    liefermenge->hide();
 }
 else 
 {  liefermenge->set_value(dt->getAufEintrag().getRestStk());
    anzahl->set_value(1);
    label_menge->show();
    liefermenge->show();
 }
 artikelbox->set_value(dt->getAufEintrag().Artikel());
 auftragnr->set_text(Formatiere(dt->getAufEintrag().Id()));
}


void auftrag_lieferschein::on_unselectrow_offauf(int row, int col, GdkEvent* b)
{
 clear_input();   
}

auftrag_lieferschein::auftrag_lieferschein(cH_ppsInstanz _instanz)
:instanz(_instanz) 
{
 set_tree_titles();
 liefernr->set_value_in_list(false,false);
 liefernr->set_always_fill(false);

 tree_daten->hide();
 vbox_eingabe->hide();
}

void auftrag_lieferschein::set_tree_titles()
{
 std::vector<std::string> t1;
 t1.push_back("Lief.Zeile");
 t1.push_back("Artikel");
 t1.push_back("Auftrag");
 t1.push_back("Liefermenge");
 tree_daten->setTitles(t1); 

 std::vector<std::string> t2;
 t2.push_back("Auftrag");
 t2.push_back("Artikel");
 t2.push_back("Lieferdatum");
 t2.push_back("offen");
 t2.push_back("geliefert");
 tree_offen->setTitles(t2); 

 set_title("Lieferschein: "+instanz->get_Name());

 if(instanz->Id()==1) 
   { 
     liefer_kunde->setLabel("Kundennr","Kundenname");
     label_kundenauftrag->set_text("Kundenauftrag");
   }
 else 
   { 
     liefer_kunde->setLabel("Lieferantennr","Lieferantenname");
     label_kundenauftrag->set_text("Eigene Auftragsnummer");
   }
}

void auftrag_lieferschein::set_tree_daten_content(LieferscheinBase::ID lfrsid)
{
 tree_daten->clear();
 if(lfrsid!=LieferscheinBase::none_id)
   {
    try{ 
      cH_LieferscheinVoll LV=cH_LieferscheinVoll(instanz,lfrsid);
      std::vector<cH_RowDataBase> datavec;
      for(std::vector<LieferscheinEntry>::const_iterator i=LV->LsEntries().begin();i!=LV->LsEntries().end();++i)
         datavec.push_back(new Data_Lieferdaten(*i));
      tree_daten->setDataVec(datavec);
     }
    catch(SQLerror &e)
     { meldung->Show(e); return; }
  }
}


void auftrag_lieferschein::clear_input()
{
 liefermenge->set_value(0);
 anzahl->set_value(0);
 artikelbox->reset();
 auftragnr->set_text("");
}

void auftrag_lieferschein::on_Palette_activate()
{ gtk_spin_button_update(anzahl->gtkobj());
  gtk_spin_button_update(liefermenge->gtkobj());
  gtk_spin_button_update(Palette->gtkobj());
  
  ArtikelBase artikel = artikelbox->get_value();
  if(artikel.Id() == 0) return;

 try 
 {Transaction tr;
  if(instanz->Id()==ppsInstanz::INST_GARNEINKAUF)
   {
     int anzahl_kartons = anzahl->get_value_as_int();
     Lieferschein::mengen_t kg_pro_karton = liefermenge->get_value_as_float();
     Lager L(instanz->BestellungFuer());      
     L.rein_ins_lager(artikel,anzahl_kartons*kg_pro_karton);
   }
  if (!tree_offen->selection().size())
  {  // Menge verteilen
     lieferschein->push_back(artikel,anzahl->get_value_as_int(),
                  liefermenge->get_value_as_float(),Palette->get_value_as_int());
		
  }
  else
  {
    cH_Data_Lieferoffen dt=tree_offen->getSelectedRowDataBase_as<cH_Data_Lieferoffen>();
    AufEintragBase auftragentry=dt->getAufEintrag();
    Einheit e(artikel);
    
    lieferschein->push_back(auftragentry, artikel, anzahl->get_value_as_int(),
     		e!=Einheit::Stueck?liefermenge->get_value_as_float():0.0,
     		Palette->get_value_as_int());
  } 
  tr.commit();
  
  bool grab=!tree_offen->selection().size();
  set_tree_daten_content(lieferschein->Id());
  set_tree_offen_content();  // refresh display
  clear_input();
  if (grab)
  {  anzahl->select_region(0,anzahl->get_text_length());
     anzahl->grab_focus();
  }
 }
 catch(SQLerror &e) 
 { if(e.Code()==-400) liefernr->grab_focus(); //noch kein Liefrschein vorhanden 
   else meldung->Show(e); 
 }
 catch (const char *e)
 { meldung->Show(SQLerror(e));
 }
 catch(std::exception &e)
 { meldung->Show(SQLerror(e.what()));
 }
}

void auftrag_lieferschein::liefzeile_delete()
{
 try {
 if(rngnr->get_text()=="")
   if(lieferschein->Id()!=LieferscheinBase::none_id)
	if(deleteLiefEntry())
	  {  set_tree_daten_content(lieferschein->Id());
        set_tree_offen_content();
	  }
  }
 catch (SQLerror &e)
  {  meldung->Show(e); 
     return;
  }
}

bool auftrag_lieferschein::deleteLiefEntry()
{
 try{
   cH_Data_Lieferdaten dt(tree_daten->getSelectedRowDataBase_as<cH_Data_Lieferdaten>());
   LieferscheinEntry LE = dt->get_LieferscheinEntry();
   if (LE.Zeile()!=0)
    {
     LieferscheinVoll(instanz,LE.Id()).deleteRow(LE);
     return true;
    }
  } catch(...){}
 return false;
}

void auftrag_lieferschein::on_daten_leaf_selected(cH_RowDataBase d)
{
 const Data_Lieferdaten *dt=dynamic_cast<const Data_Lieferdaten*>(&*d);
 if(rngnr->get_text()=="" && 
    dt->get_Lieferschein_Id()!=LieferscheinBase::none_id)
   lieferzeile_delete->set_sensitive(true);
 else    
   lieferzeile_delete->set_sensitive(false);
// lieferscheinentry = dt->get_LieferscheinEntry();

}

void auftrag_lieferschein::on_artikelbox_activate()
{
 Einheit e(artikelbox->get_value());
 menge_einheit->set_text(e);
 if (e!=Einheit(Einheit::Stueck)) 
 { liefermenge->grab_focus();
   liefermenge->select_region(0,liefermenge->get_text_length());
   label_menge->hide();
   liefermenge->show();
 }
 else 
 { auftragnr->grab_focus();
   label_menge->hide();
   liefermenge->hide();
 }
}

void auftrag_lieferschein::on_button_liste_clicked()
{
  manage(new lieferscheinliste(instanz));
}

void auftrag_lieferschein::set_tree_offen_content()
{
 clear_input();
 tree_offen->clear();

 try {
   SQLFullAuftragSelector sel;
   if(instanz->Lieferant())
     sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Status(
                   instanz->Id(), (AufStatVal)OPEN,false));
   else
     sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Kunde_Status(
                   instanz->Id(), liefer_kunde->get_value(), (AufStatVal)OPEN));

   std::vector<cH_RowDataBase> datavec;
   SelectedFullAufList *offene_auftraege=new SelectedFullAufList(sel);
   for (SelectedFullAufList::const_iterator i=offene_auftraege->begin();i!=offene_auftraege->end();++i)
      datavec.push_back(new Data_Lieferoffen(*i));
   tree_offen->setDataVec(datavec);
  }catch (SQLerror &e) {std::cerr <<e<<'\n';}
}

