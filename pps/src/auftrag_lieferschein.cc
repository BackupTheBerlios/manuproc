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
#include<typeinfo>
#include <tclistleaf.h>
#include "lieferscheinliste.hh"
#include "auftrag_lieferschein_classes.h" // erforderlich?
#include <Lager/RohwarenLager.h>
#include <SearchComboContent.h>
#include <Auftrag/AuftragFull.h>
//#include "auftrag_main.hh"
#include <unistd.h>
#include <Aux/Trace.h>

extern MyMessage *meldung;
extern auftrag_main *auftragmain;

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
 try{
 lieferschein = new Lieferschein(instanz,cH_Kunde(liefer_kunde->get_value()));
 }
 catch(SQLerror &e) {meldung->Show(e); return;}
 
 liefdate->set_value(ManuProC::Datum::today());

 tree_daten->clear();
 liefernr->setContent(Formatiere(lieferschein->Id(),0,6,"","",'0'),lieferschein->Id());
 vbox_eingabe->show();
 tree_daten->show();

 rngnr->set_text("");
 spinbutton_paeckchen->set_value(1);
 spinbutton_pakete->set_value(1);
 spinbutton_brutto->set_value(0);
 spinbutton_netto->set_value(0);
 entry_dpdnr->set_text("");
  
}

void auftrag_lieferschein::on_lief_save()
{   
}

void auftrag_lieferschein::on_lief_preview()
{  
   if (liefernr->get_text()=="") return;
   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean ";
   std::string command = "auftrag_drucken -a Lieferschein -n "
      +liefernr->get_text()+" -i "+ itos(instanz->Id())+optionen;
   system(command.c_str());
}

void auftrag_lieferschein::on_lief_print()
{  
   if (liefernr->get_text()=="") return;
   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean ";
   std::string command = "auftrag_drucken -a Lieferschein -n "
      +liefernr->get_text()+" -p -i "+ itos(instanz->Id())+optionen;
   system(command.c_str()); 
}

void auftrag_lieferschein::display(int lfrsid)
{
 set_tree_daten_content(lfrsid);
 display2(lieferschein->KdNr());
 int rng = lieferschein->RngNr();
 if (rng!=-1)
    rngnr->set_text(Formatiere(rng,0,6,"","",'0'));
 else rngnr->set_text("");
 vbox_eingabe->show();
 tree_daten->show();
 liefdate->set_value(lieferschein->getDatum());
#ifdef DPD_LIEFERSCHEINE
 entry_dpdnr->set_text(itos0p(lieferschein->getDPDlnr(),Lieferschein::Offen));
 spinbutton_pakete->set_value(lieferschein->Pakete());
 spinbutton_paeckchen->set_value(lieferschein->Paeckchen());
 spinbutton_brutto->set_value(lieferschein->GewichtBrutto().as_float());
 spinbutton_netto->set_value(lieferschein->GewichtNetto().as_float());
#endif
}

void auftrag_lieferschein::display2(int kdnr)
{
try{
 liefer_kunde->set_value(kdnr);
 set_tree_offen_content();
 cH_Kunde k(kdnr);
 vbox_eingabe->show();

#ifdef PETIG_EXTENSIONS 
   if(instanz->EinlagernIn()!=ppsInstanzID::None)
    { artikelbox->setExtBezSchema(cH_Kunde(Kunde::default_id)->getSchema(ArtikelTypID::Garn)); }
   else if (artikelbox->getBezSchema()->Id() != k->getSchemaId())
    { artikelbox->setExtBezSchema(k->getSchema(ArtikelTypID::Band)); }
#else
   artikelbox->setExtBezSchema(k->getSchema(ManuProC::DefaultValues::ArtikelTyp));
#endif
   artikelbox->setExtBezSchemaID(k->getSchemaId());
 }
 catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_lieferschein::on_liefnr_activate()
{
 try{
 try{
   lieferschein = new Lieferschein(instanz,liefernr->Content());
   display(liefernr->Content());
 }catch(SearchComboContent<int>::ContentError &e)
 { display(atoi(liefernr->get_text().c_str()));
   spinbutton_paeckchen->grab_focus();
 }
 }catch(std::exception &e) {std::cerr<<e.what();}
}

void auftrag_lieferschein::on_lieferkunde_activate()
{
try{display2(liefer_kunde->get_value());
 liefernr->reset();
 } catch(SQLerror &e) {meldung->Show(e);}
#ifdef EAN_AUSDRUCKEN_IN_PPS
 cH_Kunde k(liefer_kunde->get_value());
 checkbutton_ean_drucken->set_active(k->showEAN());   
#endif 
// set_tree_offen_content();
}

void auftrag_lieferschein::on_liefdate_activate()
{ 
try{  
   lieferschein->setDatum(liefdate->get_value());
  } catch(SQLerror &e) {meldung->Show(e);}
}


#ifdef DPD_LIEFERSCHEINE

void auftrag_lieferschein::on_button_rng_erledigt_clicked()
{  
 try {
 if(lieferschein->Id() != Lieferschein::none_id)
   if(lieferschein->RngNr() == Lieferschein::none_id)
   {lieferschein->closeLfrs();
    rngnr->set_text("000000");
   }
 }
 catch(SQLerror &e) {meldung->Show(e);}
 
}

void auftrag_lieferschein::on_button_erledigt_clicked()
{
  try{
    if(lieferschein->getDPDlnr() == Lieferschein::Offen)
   {
      lieferschein->setDPDlnr(Lieferschein::Fertig);
      entry_dpdnr->set_text("0");
   }
   } catch(SQLerror &e) {meldung->Show(e);}
}


void auftrag_lieferschein::on_spinbutton_paeckchen_activate()
{
  spinbutton_pakete->grab_focus();
}

void auftrag_lieferschein::on_spinbutton_pakete_activate()
{
  spinbutton_brutto->grab_focus();
}

void auftrag_lieferschein::on_spinbutton_brutto_activate()
{
  spinbutton_netto->grab_focus();
}

void auftrag_lieferschein::on_spinbutton_netto_activate()
{
  liefdate->grab_focus();
}

gint auftrag_lieferschein::on_spinbutton_paeckchen_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_paeckchen->update();
    lieferschein->setPaeckchen(spinbutton_paeckchen->get_value_as_int());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

gint auftrag_lieferschein::on_spinbutton_pakete_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_pakete->update();
    lieferschein->setPakete(spinbutton_pakete->get_value_as_int());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

gint auftrag_lieferschein::on_spinbutton_brutto_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_brutto->update();
    lieferschein->setGewichtBrutto(spinbutton_brutto->get_value_as_float());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

gint auftrag_lieferschein::on_spinbutton_netto_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_netto->update();
    lieferschein->setGewichtNetto(spinbutton_netto->get_value_as_float());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

#endif

// ist eigentlich anzahl oder?
void auftrag_lieferschein::on_liefermenge_activate()
{
if (!tree_offen->selection().size()) 
   artikelbox->grab_focus();

}

void auftrag_lieferschein::on_offen_leaf_selected(cH_RowDataBase d)
{
 tree_daten->unselect_all();
 const Data_Lieferoffen *dt=dynamic_cast<const Data_Lieferoffen*>(&*d);
 if(dt->getAufEintrag().getRestStk()<=0) return;
 fill_input(dt->getAufEintrag());
 if(lieferschein->RngNr() == ManuProcEntity<>::none_id)
   {button_zeile_uebernehmen->set_sensitive(true);
    button_kompletter_auftrag->set_sensitive(true);
   }
 else
   {button_zeile_uebernehmen->set_sensitive(false);
    button_kompletter_auftrag->set_sensitive(false);
   } 
}



void auftrag_lieferschein::fill_input(const AufEintrag& AE)
{
  fill_with(AE,Einheit(AE.Artikel()),AE.getRestStk().as_int(),
  Lieferschein::StandardLaenge(AE.Artikel()).as_int()
  );
}
void auftrag_lieferschein::fill_input(const AufEintrag& AE,const LieferscheinEntry& LE)
{
  // Zusatzinfos dürfen nicht geändert werden:
  fill_with(AE,Einheit(LE.Artikel()),LE.Stueck(),LE.Menge().as_float());
  Palette->set_value(LE.Palette());
}


void auftrag_lieferschein::fill_with(const AufEintrag& AE,const Einheit& E,
         int stueck,double menge)
{
  artikelbox->set_value(AE.Artikel());
  auftragnr->set_text(Formatiere(AE.Id()));
  menge_einheit->set_text(E);
  if (E.hatMenge())
   {
     label_menge->show();
//     liefermenge->set_value(Lieferschein::StandardLaenge(AE.Artikel()).as_int());
     liefermenge->show();
     liefermenge->set_value(menge);
     anzahl->set_value(stueck);
   }
  else
   {
     label_menge->hide();
     liefermenge->hide();
     liefermenge->set_value(0.0);
     anzahl->set_value(stueck);
   } 
  artikelbox->set_sensitive(false);
  auftragnr->set_sensitive(false);
}


void auftrag_lieferschein::on_unselectrow_offauf(int row, int col, GdkEvent* b)
{
 clear_input();   
 button_zeile_uebernehmen->set_sensitive(false);
 button_kompletter_auftrag->set_sensitive(false);
}

auftrag_lieferschein::auftrag_lieferschein(cH_ppsInstanz _instanz)
:instanz(_instanz) //, show_offen(true)
{
#ifndef DPD_LIEFERSCHEINE
 table_DPD->hide();
#endif
#ifdef EAN_AUSDRUCKEN_IN_PPS
  checkbutton_ean_drucken->show();
#endif 

#ifndef PETIG_EXTENSIONS
  std::string nuraktiv("and coalesce(aktiv,true)=true");
  liefer_kunde->Einschraenkung(nuraktiv,true);
  liefer_kunde->EinschraenkenKdGr(KundengruppeID::Auftragsadresse);
#endif

  liefer_kunde->setExpandStr1(true);
  liefer_kunde->setExpandStr2(true);

#ifdef MABELLA_EXTENSIONS
 button_zeile_uebernehmen->set_sensitive(false);
 button_kompletter_auftrag->set_sensitive(false);
#endif

 set_tree_titles();
 liefernr->set_value_in_list(false,false);
 liefernr->set_always_fill(false);

 tree_daten->hide();
 vbox_eingabe->hide();
 liefdate->setLabel("");
}

void auftrag_lieferschein::set_tree_titles()
{
 std::vector<std::string> t1;
 t1.push_back("Lief.Zeile");
 t1.push_back("Artikel");
 t1.push_back("Auftrag");
 t1.push_back("Palette");
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
       {
         datavec.push_back(new Data_Lieferdaten(*i));
         if(i->getZusatzInfos().size()>1)
          {
            std::vector<LieferscheinEntry::st_zusatz> VZ=i->getZusatzInfos();
            std::string zeile=itos(i->Zeile());
            char z='a';
            for(std::vector<LieferscheinEntry::st_zusatz>::const_iterator j=VZ.begin();j!=VZ.end();++j)
             {
               datavec.push_back(new Data_Lieferdaten(zeile+(z++),*i,j->aeb,j->menge)); 
             }
          }
       }
      tree_daten->setDataVec(datavec);
#warning warum geht das moveto nicht? MAT
      tree_daten->cell(datavec.size()-1,0).moveto();
//      tree_daten->moveto(datavec.size(),0,0.5,0);
     }
    catch(SQLerror &e)
     { meldung->Show(e); return; }
  }
}


void auftrag_lieferschein::clear_input()
{
 if (!masseneingabe2->get_active())
 {  liefermenge->set_value(0);
    anzahl->set_value(0);
// Palette->set_value(1);
    artikelbox->reset();
    auftragnr->set_text("");
 }
 artikelbox->set_sensitive(true);
 auftragnr->set_sensitive(true);
}


void auftrag_lieferschein::on_Palette_activate()
{ 
 ManuProC::Tracer::Enable(AuftragBase::trace_channel);
 if(lieferschein->Id()==LieferscheinBase::none_id) return;

  anzahl->update();
  liefermenge->update();
  Palette->update();
  
  ArtikelBase artikel = artikelbox->get_value();
  if(artikel.Id() == 0) return;

 try 
 {Transaction tr;
  if (!tree_offen->selection().size())
  {  // Menge verteilen
     lieferschein->push_back(artikel,anzahl->get_value_as_int(),
                  liefermenge->get_value_as_float(),Palette->get_value_as_int());
  }
  else
  {
    cH_Data_Lieferoffen dt=tree_offen->getSelectedRowDataBase_as<cH_Data_Lieferoffen>();
    AufEintrag auftragentry(dt->getAufEintrag());
    Einheit e(artikel);
    if(!checkVerkConsist(auftragentry))
      return;
    lieferschein->push_back(auftragentry, artikel, anzahl->get_value_as_int(),
     		e.hatMenge()?liefermenge->get_value_as_float():0.0,
     		Palette->get_value_as_int());
  } 


  
  bool grab=!tree_offen->selection().size();
  set_tree_daten_content(lieferschein->Id());
  set_tree_offen_content();  // refresh display
  clear_input();
//ManuProC::Tracer::Enable(ManuProC::Tracer::Artikel,false);
  if (grab)
  {  anzahl->select_region(0,anzahl->get_text_length());
     anzahl->grab_focus();
  }
  tr.commit();

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


bool auftrag_lieferschein::checkVerkConsist(const AufEintragBase &ae)
{
#ifdef MABELLA_EXTENSIONS
 if(lieferschein->getVerknr()!=Kunde::none_id)
   {if(lieferschein->getVerknr()!=ae.getVerknr())
     { meldung->Show("Aufträge von verschiedenen Verkäufern dürfen nicht "
		"auf einen Lieferschein; Bitte getrennte Lieferscheine erstellen");
       return false;
     }
   }
 else
  lieferschein->setVerknr(ae.getVerknr());
#endif
 return true;
}

void auftrag_lieferschein::on_newlieferentryall_ok()
{   
 if(lieferschein->Id()==LieferscheinBase::none_id) return;
 
  if (!tree_offen->selection().size()) 
   { 
     meldung->Show("Keine Zeile selektiert");
     return;
   }
 cH_Data_Lieferoffen dt=tree_offen->getSelectedRowDataBase_as<cH_Data_Lieferoffen>();
 AufEintragBase auftragentry=dt->getAufEintrag();
 AuftragFull AF(auftragentry);

 if(!checkVerkConsist(auftragentry))
   return;

 for(AuftragFull::const_iterator i=AF.begin();i!=AF.end();++i)
   {
     if(i->getEntryStatus()!=AufStatVal(OPEN)) continue;
     if(i->getGeliefert()>=i->getStueck()) continue;
     auftragzeile_zeile_uebernehmen(*i);   
   }
  set_tree_daten_content(lieferschein->Id());
  set_tree_offen_content();
}


void auftrag_lieferschein::auftragzeile_zeile_uebernehmen(const AufEintrag &AE)
{
 if(lieferschein->Id()==LieferscheinBase::none_id) return;
 
   Einheit e(AE.Artikel());
   AufEintrag ae(AE);
   lieferschein->push_back(ae,AE.Artikel(), AE.getRestStk().as_int(),
     		e.hatMenge()?liefermenge->get_value_as_float():0.0,
     		Palette->get_value_as_int());
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
     LieferscheinVoll lv(instanz,LE.Id());
     lv.deleteRow(LE);
#ifdef MABELLA_EXTENSIONS     
     if(lv.size()==0) // Verkäufer auf NONE setzten
       lieferschein->setVerknr(Kunde::none_id);
#endif       
     return true;
    }
  } catch(...){}
 return false;
}

void auftrag_lieferschein::on_daten_leaf_selected(cH_RowDataBase d)
{
 tree_offen->unselect_all();
 try{
 const Data_Lieferdaten *dt=dynamic_cast<const Data_Lieferdaten*>(&*d);
 if(lieferschein->RngNr() == ManuProcEntity<>::none_id && 
    dt->get_Lieferschein_Id()!=LieferscheinBase::none_id)
   {
    lieferzeile_delete->set_sensitive(true);
   }
 else    
    lieferzeile_delete->set_sensitive(false);
 AufEintrag AE;
 if (!!dt->getAufEintragBase())
 {  try{ AE=dt->getAufEintragBase();}
    catch(AufEintrag::NoAEB_Error &e){}
 }
 if(dt->istZusatzinfo()) button_zeile_modifizieren->set_sensitive(false);
 else                    button_zeile_modifizieren->set_sensitive(true);
 fill_input(AE,dt->get_LieferscheinEntry());
 }catch(std::exception &e) {std::cerr << e.what()<<'\n';}
}

void auftrag_lieferschein::on_daten_unselect_row(int row, int col, GdkEvent* b)
{
  clear_input();
  button_zeile_modifizieren->set_sensitive(false);
}


void auftrag_lieferschein::on_artikelbox_activate()
{
 Einheit e(artikelbox->get_value());
 menge_einheit->set_text(e);
 if (e.hatMenge()) 
 { liefermenge->grab_focus();
   liefermenge->select_region(0,liefermenge->get_text_length());
   label_menge->show();
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
//return;
 clear_input();
 tree_offen->clear();
// if (!show_offen) return; 

 try {
   SQLFullAuftragSelector sel;
     sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Kunde_Status(
                   instanz->Id(), liefer_kunde->get_value(), (AufStatVal)OPEN));

   std::vector<cH_RowDataBase> datavec;
   SelectedFullAufList *offene_auftraege=new SelectedFullAufList(sel);
   for (SelectedFullAufList::const_iterator i=offene_auftraege->begin();i!=offene_auftraege->end();++i)
      datavec.push_back(new Data_Lieferoffen(*i,auftragmain));
   tree_offen->setDataVec(datavec);
  }catch (SQLerror &e) {std::cerr <<e<<'\n';}
}

void auftrag_lieferschein::on_button_zeile_modifizieren_clicked()
{
 try{
   cH_Data_Lieferdaten dt(tree_daten->getSelectedRowDataBase_as<cH_Data_Lieferdaten>());
   LieferscheinEntry LE = dt->get_LieferscheinEntry();
   
   Palette->update();
   liefermenge->update();
   anzahl->update();

   if(Palette->get_value_as_int() != LE.Palette())
      LE.setPalette(Palette->get_value_as_int());
   if(LieferscheinBase::mengen_t(liefermenge->get_value_as_float()) != LE.Menge() ||
            anzahl->get_value_as_int() != LE.Stueck())
    {
       LE.changeMenge(anzahl->get_value_as_int(),liefermenge->get_value_as_float(),
       		*lieferschein,
#ifdef MABELLA_EXTENSIONS
				true
#else
				false
#endif
		       			);
       set_tree_offen_content();
    }
   set_tree_daten_content(lieferschein->Id());
  } catch(std::exception &e) {std::cerr << e.what();}
}

void auftrag_lieferschein::on_checkbutton_ean_drucken_clicked()
{
 cH_Kunde k(liefer_kunde->get_value());
 
 if(k->Id() != Kunde::none_id)   
    {
     (const_cast<Kunde*>(&*k))->
     showEAN(checkbutton_ean_drucken->get_active());
    }  
}


std::string Data_Lieferdaten::FormatiereMenge(ArtikelBase artikel, int stueck, AuftragBase::mengen_t menge)
{
 return  FormatiereMenge(artikel,stueck,LieferscheinBase::mengen_t(menge));
}



std::string Data_Lieferdaten::FormatiereMenge(ArtikelBase artikel, int stueck, LieferscheinBase::mengen_t menge)
{
  std::string s;
  if (stueck!=1)
   {  s=Formatiere(stueck)
        + Einheit(artikel).StueckEinheit();
   }
  if (menge.Scaled()!=0)
   {  if (stueck!=1) smenge+="*";
      s+=Formatiere_short(menge)
         + Einheit(artikel).MengenEinheit();
   }
  if (stueck==1 && menge.Scaled()==0)
      s=Formatiere(stueck)+Einheit(artikel).MengenEinheit();
 return s;
}

void auftrag_lieferschein::on_liefnotiz_changed()
{  
}

void auftrag_lieferschein::on_liefnotiz_save_clicked()
{  
}
