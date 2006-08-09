/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
 *  Copyright (C) 2006 Christof Petig
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
#include<Misc/Ausgabe_neu.h>
#include<Lieferschein/LieferscheinEntry.h>
#include <Auftrag/selFullAufEntry.h>
#include<typeinfo>
#include "lieferscheinliste.hh"
#include <Lager/RohwarenLager.h>
#include <SearchComboContent.h>
#include <Auftrag/AuftragFull.h>
//#include "auftrag_main.hh"
#include <unistd.h>
#include <Misc/Trace.h>
#include "buchen_dialog.hh"
#include "info_dialog.hh"
#include <expander_hook.h>
#include <Misc/i18n.h>
#include <Misc/ucompose.hpp>
#include <DynamicEnums/DynamicConfig.h>

extern MyMessage *meldung;
extern auftrag_main *auftragmain;

auftrag_lieferschein *auftraglieferschein;

typedef std::vector<cH_RowDataBase>::iterator DVI;
typedef std::vector<cH_RowDataBase>::const_iterator cDVI;

bool auftrag_lieferschein::on_liefer_delete(GdkEventAny *ev)
{
 on_liefer_close();
 return 0;
}

void auftrag_lieferschein::on_liefer_close()
{ const std::vector<cH_RowDataBase> &datavec_liefdata
          =tree_daten->getModel().getDataVec();
 if(lieferschein->Id()!=LieferscheinBase::none_id)
   {
    cDVI j = find_if(datavec_liefdata.begin(),datavec_liefdata.end(),
		LiefHasStatus((AufStatVal)UNCOMMITED));
    if(j!=datavec_liefdata.end())
      on_lager_buchen_clicked();
    else
      delete this;	
   }
 else
   delete this;
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
 liefernr->setContent(AuftragBase::ID2string(lieferschein->Id()),lieferschein->Id());
 vbox_eingabe->show();
 tree_daten->show();

 rngnr->set_text(std::string());
 lager_buchen->set_sensitive(true);
 spinbutton_paeckchen->set_value(1);
 spinbutton_pakete->set_value(1);
 spinbutton_brutto->set_value(0);
 spinbutton_netto->set_value(0);
 entry_dpdnr->set_text(std::string());
 frame_eingabe->set_sensitive(true);
 frame_zeilen->set_sensitive(true);
}

void auftrag_lieferschein::on_lief_save()
{   
}

void auftrag_lieferschein::on_lief_preview()
{  
   if (liefernr->get_text().empty()) return;
   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean ";
   std::string command = "auftrag_drucken -G -aLieferschein -n"
      +liefernr->get_text()+" -i"+ itos(instanz->Id())+optionen;
   system(command.c_str());
}

void auftrag_lieferschein::on_lief_print()
{  
   if (liefernr->get_text().empty()) return;
   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean";
   std::string command = "auftrag_drucken -aLieferschein -n"
      +liefernr->get_text()+" -Y0,0,1 -i"+ itos(instanz->Id())+optionen;
   system(command.c_str()); 
}

void auftrag_lieferschein::display(int lfrsid)
{
 set_tree_daten_content(lfrsid);
 display2(lieferschein->KdNr());
 int rng = lieferschein->RngNr();
 if (rng!=-1)
    rngnr->set_text(AuftragBase::ID2string(rng));
 else rngnr->set_text(std::string());
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
 frame_notiz->set_label(_("Notiz zum Lieferschein"));
 lief_notiz->get_buffer()->set_text(lieferschein->Notiz());
 liefnotiz_save->set_sensitive(false);
}

void auftrag_lieferschein::display2(int kdnr)
{
try{
 liefer_kunde->set_value(kdnr);
// set_tree_offen_content();
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
   lieferschein = new Lieferschein(instanz,liefernr->Content());

   if(lieferschein->KdNr()!=liefer_kunde->get_value())
     {liefer_kunde->set_value(lieferschein->KdNr());
      datavec_liefoff.clear();
     }

   display(liefernr->Content());
   anzahl->grab_focus();
   lager_buchen->set_sensitive(true); 
   frame_eingabe->set_sensitive(true);
   frame_zeilen->set_sensitive(true);
 }catch(SearchComboContent<int>::ContentError &e)
 {}
}

void auftrag_lieferschein::on_lieferkunde_activate()
{
try{display2(liefer_kunde->get_value());
 liefernr->reset();
 set_tree_offen_content();
 } catch(SQLerror &e) {meldung->Show(e);}
#ifdef EAN_AUSDRUCKEN_IN_PPS
 cH_Kunde k(liefer_kunde->get_value());
 checkbutton_ean_drucken->set_active(k->showEAN());   
#endif 
}

void auftrag_lieferschein::on_liefdate_activate()
{ 
try{  
   lieferschein->setDatum(liefdate->get_value());
  } catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_lieferschein::on_button_rng_erledigt_clicked()
{  
 try {
 if(lieferschein->Id() != Lieferschein::none_id)
   if(lieferschein->RngNr() == ManuProcEntity<>::none_id)
   {lieferschein->closeLfrs();
    rngnr->set_text("000000");
   }
 }
 catch(SQLerror &e) {meldung->Show(e);}
 
}

#ifdef DPD_LIEFERSCHEINE

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

bool auftrag_lieferschein::on_spinbutton_paeckchen_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_paeckchen->update();
    lieferschein->setPaeckchen(spinbutton_paeckchen->get_value_as_int());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

bool auftrag_lieferschein::on_spinbutton_pakete_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_pakete->update();
    lieferschein->setPakete(spinbutton_pakete->get_value_as_int());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

bool auftrag_lieferschein::on_spinbutton_brutto_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_brutto->update();
    lieferschein->setGewichtBrutto(spinbutton_brutto->get_value());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

bool auftrag_lieferschein::on_spinbutton_netto_focus_out_event(GdkEventFocus *ev)
{
 try{  
    spinbutton_netto->update();
    lieferschein->setGewichtNetto(spinbutton_netto->get_value());
  } catch(SQLerror &e) {meldung->Show(e);}
 return false;
}

#endif

// ist eigentlich anzahl oder?
void auftrag_lieferschein::on_liefermenge_activate()
{
if (!tree_offen->get_selection()->count_selected_rows()) 
   artikelbox->grab_focus();

}

void auftrag_lieferschein::on_offen_leaf_selected(cH_RowDataBase d)
{
 tree_daten->get_selection()->unselect_all();
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
#ifndef MABELLA_EXTENSIONS
  Lieferschein::StandardLaenge(AE.Artikel()).as_int(),
#else
  0,
#endif
  false
  );
}
void auftrag_lieferschein::fill_input(const AufEintrag& AE,const LieferscheinEntry& LE)
{
  // Zusatzinfos dürfen nicht geändert werden:
  fill_with(AE,Einheit(LE.Artikel()),LE.Stueck(),LE.Menge().as_float(),false);
  if (!LE.getRefOrder().empty())
  { auftragnr->set_text(LE.getRefOrder());
    auftragnr->set_sensitive(true);
  }
  Palette->set_value(LE.Palette());
 frame_notiz->set_label(String::ucompose(_("Notiz zu Zeile %1"),LE.Zeile()));
 lief_notiz->get_buffer()->set_text(LE.Text());
 liefnotiz_save->set_sensitive(false);
}

void auftrag_lieferschein::fill_with(const AufEintrag& AE,const Einheit& E,
         int stueck,double menge,bool check_bestand)
{
  artikelbox->set_value(AE.Artikel());
  if (AE.valid()) auftragnr->set_text(AuftragBase::ID2string(AE.Id()));
  else auftragnr->set_text(std::string());

  if(check_bestand)
    {int bestand(AE.getAmLager().as_int());
     anzahl->set_value(stueck<bestand ? stueck : (bestand>0?bestand:0));
    }
  else
     anzahl->set_value(stueck);

  menge_einheit->set_text(E.StueckEinheit());
  label_stueck->set_text(E.StueckEinheit());
  if (E.hatMenge())
   {
     table_menge->show();
     liefermenge->set_value(menge);
     menge_einheit->set_text(E.MengenEinheit());
   }
  else
   {
     table_menge->hide();
//     liefermenge->set_value(0.0);
   } 
  artikelbox->set_sensitive(false);
  auftragnr->set_sensitive(!AE.valid());
}


void auftrag_lieferschein::on_unselectrow_offauf()
{
 clear_input();   
 button_zeile_uebernehmen->set_sensitive(false);
 button_kompletter_auftrag->set_sensitive(false);
}

void auftrag_lieferschein::init()
{
  gtk_expander_hook_size_to_parent(frame_zeilen->gobj());
  gtk_expander_hook_size_to_parent(frame_notiz->gobj());
//  gtk_expander_hook_size_to_parent(expander_ls->gobj());
//  gtk_expander_hook_size_to_parent(frame_eingabe->gobj());
  gtk_expander_hook_size_to_parent(expander_order->gobj());
  liefernr->get_entry()->set_width_chars(5);
  
#ifndef DPD_LIEFERSCHEINE
 table_DPD->hide();
#endif
#ifdef EAN_AUSDRUCKEN_IN_PPS
  checkbutton_ean_drucken->show();
#endif 

#ifndef PETIG_EXTENSIONS
  std::string nuraktiv("and coalesce(aktiv,true)=true");
  liefer_kunde->Einschraenkung(nuraktiv,true);

  if(instanz->Id()==ppsInstanzID::Kundenauftraege)
#ifdef HAS_ADDR_GROUP_Auftragsadresse
     liefer_kunde->EinschraenkenKdGr(KundengruppeID::Auftragsadresse);
#else
     ;
#endif
  else
     if(instanz->ExterneBestellung())
#ifdef HAS_ADDR_GROUP_Lieferanten
       liefer_kunde->EinschraenkenKdGr(KundengruppeID::Lieferanten);
#else
       ;
#endif
#endif
  masseneingabe2->set_active(true);
  expander_order->set_expanded(false);

  liefer_kunde->setExpandStr1(true);
  liefer_kunde->setExpandStr2(true);

#ifdef MABELLA_EXTENSIONS
 button_zeile_uebernehmen->set_sensitive(false);
 button_kompletter_auftrag->set_sensitive(false);
 masseneingabe2->set_active(false);
#else
 lagerwahl->hide();
#endif

 set_tree_titles();
 liefernr->set_value_in_list(false,false);
 liefernr->set_always_fill(false);

// tree_daten->hide();
// vbox_eingabe->hide();
 liefdate->setLabel(std::string());
#ifdef LIEFERSCHEINE_IMMER_BESTAETIGT
 lager_buchen->hide();
 tree_daten->set_tree_column_visibility(
	tree_daten->getStore()->IndexFromColumn(Data_Lieferdaten::VOMLAGER_SEQ),false);
#endif

 auftraglieferschein=this;
}

auftrag_lieferschein::auftrag_lieferschein(LieferscheinEntryBase const& toload)
 : instanz(toload.Instanz())
{ init();
  liefernr->set_value(itos(toload.Id()),toload.Id());
  on_liefnr_activate();
}

auftrag_lieferschein::auftrag_lieferschein(cH_ppsInstanz _instanz)
:instanz(_instanz) //, show_offen(true)
{
  init();
}

void auftrag_lieferschein::set_tree_titles()
{
 std::vector<std::string> t1;
 t1.push_back(_("Lief.Zeile"));
 t1.push_back(_("Artikel"));
 t1.push_back(_("Auftrag"));
 t1.push_back(_("Palette"));
 t1.push_back(_("Liefermenge"));
 t1.push_back(_("% bestellt"));
 t1.push_back(_("vom Lager"));
 t1.push_back(_("Bemerkung"));
 tree_daten->setTitles(t1); 
 std::vector<float> a1(t1.size());
 a1[Data_Lieferdaten::VOMLAGER_SEQ]=1;
 tree_daten->setAlignment(a1);

 std::vector<std::string> t2;
 t2.push_back(_("Auftrag"));
 t2.push_back(_("Artikel"));
 t2.push_back(_("Lieferdatum"));
 t2.push_back(_("offen"));
 t2.push_back(_("geliefert"));
 t2.push_back(_("im Lager"));
 tree_offen->setTitles(t2); 
 std::vector<float> a2(t2.size());
 a2[Data_Lieferoffen::IMLAGER_SEQ]=1;
 tree_offen->setAlignment(a2);

 set_title(String::ucompose(_("Lieferschein: %1"),instanz->get_Name()));

 if(instanz->Id()==ppsInstanzID::Kundenauftraege) 
   { 
     liefer_kunde->setLabel(_("Kundennr"),_("Kundenname"));
     label_kundenauftrag->set_text(_("Kundenauftrag"));
   }
 else 
   { 
     liefer_kunde->setLabel(_("Lieferantennr"),_("Lieferantenname"));
     label_kundenauftrag->set_text(_("Eigene Auftragsnummer"));
   }
}

static void push_line(std::vector<cH_RowDataBase> &vec, const LieferscheinEntry &entry)
{ vec.push_back(new Data_Lieferdaten(entry));
  if(entry.getZusatzInfos().size()>1)
  { std::vector<LieferscheinEntry::st_zusatz> VZ=entry.getZusatzInfos();
    std::string zeile=itos(entry.Zeile());
    char z='a';
    for(std::vector<LieferscheinEntry::st_zusatz>::const_iterator j=VZ.begin();j!=VZ.end();++j)
    vec.push_back(new Data_Lieferdaten(zeile+(z++),entry,j->aeb,j->menge)); 
  }
}

void auftrag_lieferschein::set_tree_daten_content(LieferscheinBase::ID lfrsid)
{
 tree_daten->clear();

 if(lfrsid!=LieferscheinBase::none_id)
   {
    try{ 
      std::vector<cH_RowDataBase> datavec_liefdata;
      cH_LieferscheinVoll LV=cH_LieferscheinVoll(instanz,lfrsid);
      for(std::vector<LieferscheinEntry>::const_iterator i=LV->LsEntries().begin();
                i!=LV->LsEntries().end();++i)
         push_line(datavec_liefdata,*i);
      tree_daten->setDataVec(datavec_liefdata);
      // letzte Zeile anzeigen ...
      if (!datavec_liefdata.empty()) 
        tree_daten->scroll_to(datavec_liefdata.back());
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
    auftragnr->set_text(std::string());
 }
 artikelbox->set_sensitive(true);
 auftragnr->set_sensitive(true);
}

// this logic really should go into common++ and get covered by a test!
void auftrag_lieferschein::on_Palette_activate()
{ 
// ManuProC::Tracer::Enable(AuftragBase::trace_channel);
 if(lieferschein->Id()==LieferscheinBase::none_id) return;

 if(lieferschein->RngNr() != ManuProcEntity<>::none_id)
   {
    meldung->Show(_("Berechnete Lieferscheine können nicht mehr geändert werden"));
    return;
   }

// lieferschein->lagerid=int(lagerwahl->get_menu()->get_active()->get_user_data());

  anzahl->update();

  if(anzahl->get_value_as_int() == 0)
   {
    meldung->Show(_("Anzahl darf nicht 0 sein"));
    return;
   }

  liefermenge->update();
  Palette->update();
  
  ArtikelBase artikel = artikelbox->get_value();
  if(artikel.Id() == 0) return;

  Einheit e(artikel);
  Lieferschein::mengen_t menge=e.hatMenge()?liefermenge->get_value():0.0;
 try 
 {Transaction tr;
  int neue_zeile=-1;
  if (!tree_offen->get_selection()->count_selected_rows())
  { if(!checkVerkConsist())
      return;
      
    // in case an order no is given only try to satisfy this one
    if (!auftragnr->get_text().empty())
      try 
      { Auftrag::ID aid=Auftrag::getIdFromYourAufId(lieferschein->Instanz()->Id(),
                auftragnr->get_text(),lieferschein->KdNr(),true);
        AuftragBase order(lieferschein->Instanz(),aid);
        neue_zeile=lieferschein->push_back(order,artikel,anzahl->get_value_as_int(),
                  menge, Palette->get_value_as_int());
      }
      catch (...)
      { neue_zeile=lieferschein->push_back(artikel,anzahl->get_value_as_int(),
                  menge, Palette->get_value_as_int());
        LieferscheinEntry(make_value(LieferscheinEntryBase(*lieferschein,neue_zeile)))
           .setRefOrder(auftragnr->get_text());
      }
    else
    // Menge verteilen
    { neue_zeile=lieferschein->push_back(artikel,anzahl->get_value_as_int(),
                  menge, Palette->get_value_as_int());
    }
  }
  else
  {
    cH_Data_Lieferoffen dt=tree_offen->getSelectedRowDataBase_as<cH_Data_Lieferoffen>();
    AufEintrag auftragentry(dt->getAufEintrag());
    if(!checkVerkConsist(auftragentry))
      return;
    neue_zeile=lieferschein->push_back(auftragentry, artikel, anzahl->get_value_as_int(),
     		menge, Palette->get_value_as_int());
    dt->getAufEintrag().tmp_geliefert+=anzahl->get_value_as_int();
  } 

  bool grab=!tree_offen->get_selection()->count_selected_rows();
  if (neue_zeile!=-1)
  { std::vector<cH_RowDataBase> vec;
    push_line(vec,LieferscheinEntry(LieferscheinEntryBase(*lieferschein,neue_zeile)));
    for (std::vector<cH_RowDataBase>::const_iterator i=vec.begin();i!=vec.end();++i)
      tree_daten->getModel().append_line(*i);
    tree_daten->scroll_to(vec.back());
  }
  else set_tree_daten_content(lieferschein->Id());
  set_tree_offen_content();  // refresh display
  clear_input();

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
 if((lieferschein->getVerknr()!=Kunde::none_id) &&
	(ae.Id()!=AuftragBase::none_id) )
   {if(lieferschein->getVerknr()!=ae.getVerknr())
     { meldung->Show(_("Aufträge von verschiedenen Verkäufern dürfen nicht "
		"auf einen Lieferschein; Bitte getrennte Lieferscheine erstellen"));
       return false;
     }
   }
 else 
   if(ae.Id()!=AuftragBase::none_id) 
     {if(ae.getVerknr()!=Kunde::none_id)
        lieferschein->setVerknr(ae.getVerknr());
     }
   else
     {cH_Kunde k(lieferschein->KdNr());
      lieferschein->setVerknr(k->VerkNr());
     }
#endif
 return true;
}

void auftrag_lieferschein::on_newlieferentryall_ok()
{   
 if(lieferschein->Id()==LieferscheinBase::none_id) return;
 
  if (!tree_offen->get_selection()->count_selected_rows()) 
   { 
     meldung->Show(_("Keine Zeile selektiert"));
     return;
   }
 cH_Data_Lieferoffen dt=tree_offen->getSelectedRowDataBase_as<cH_Data_Lieferoffen>();
 AufEintragBase auftragentry=dt->getAufEintrag();
// AuftragFull AF(auftragentry);

 if(!checkVerkConsist(auftragentry))
   return;

 typedef std::vector<cH_RowDataBase>::iterator DVI;

 for(DVI i=datavec_liefoff.begin(); i!=datavec_liefoff.end(); ++i)
   {
    Handle<const Data_Lieferoffen> h_lo=
			(*i).cast_dynamic<const Data_Lieferoffen>();
     if(h_lo->getAufEintrag().Id()!=auftragentry.Id()) continue;
     auftragzeile_zeile_uebernehmen(h_lo->getAufEintrag());   
   }
  set_tree_daten_content(lieferschein->Id());
  set_tree_offen_content();
}




void auftrag_lieferschein::auftragzeile_zeile_uebernehmen(const AufEintrag &AE)
{
 if(lieferschein->Id()==LieferscheinBase::none_id) return;
 
   Einheit e(AE.Artikel());
   AufEintrag ae(AE);
//   lieferschein->lagerid=
//		int(lagerwahl->get_menu()->get_active()->get_user_data());
      
   int stueck=AE.getRestStk().as_int();
//   int bestand=AE.getAmLager().as_int();

//   int stk=stueck < bestand ? stueck : (bestand > 0 ? bestand:0);

// Zu erst den Lieferschein komplett übernehmen. 
// Wenn gebucht wird, dann gegen das Lager checken; passier in buchen.

//   if(stk>0 
//#ifdef MABELLA_EXTENSIONS   
//   	|| ArtikelTyp(AE.Artikel())==ArtikelTypID::Musterkarten
//#endif   	
//   	)
//     {
      lieferschein->push_back(ae,AE.Artikel(), stueck,
     		e.hatMenge()?liefermenge->get_value():0.0,
     		Palette->get_value_as_int());
      AE.tmp_geliefert+=stueck;
//     }
}



void auftrag_lieferschein::liefzeile_delete()
{
 try {
 if(rngnr->get_text().empty())
   if(lieferschein->Id()!=LieferscheinBase::none_id)
      if(tree_daten->get_selection()->count_selected_rows()) 
	if(deleteLiefEntry())
	  {set_tree_daten_content(lieferschein->Id());
           set_tree_offen_content();
	  }
  }
  catch (SQLerror &e)
  {  meldung->Show(e); 
     return;
  }
}


void auftrag_lieferschein::adjustOffAufEntry(cH_Data_Lieferdaten dt,
					     int deltaMenge)
{
 DVI j = find_if(datavec_liefoff.begin(),datavec_liefoff.end(),
	Auftrag_ref_Lief(dt->getAufEintragBase()));

 if(j!=datavec_liefoff.end())
       {
        Handle<const Data_Lieferoffen> h_lo=
		(*j).cast_dynamic<const Data_Lieferoffen>();
        h_lo->getAufEintrag().tmp_geliefert+=deltaMenge;

        h_lo->getAufEintrag().tmp_geliefert=
			h_lo->getAufEintrag().tmp_geliefert<0?0
			:h_lo->getAufEintrag().tmp_geliefert;
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

     if(LE.Status()==(AufStatVal)UNCOMMITED)
       {
        int deltaMenge(-LE.Stueck());
        lv.deleteRow(LE);
        adjustOffAufEntry(dt,deltaMenge);
       }
     else
       if(LE.Status()==(AufStatVal)OPEN)
	 {
         LE.changeStatus((AufStatVal)STORNO,
#ifdef MABELLA_EXTENSIONS
				true
#else
				false
#endif
	      );
	 datavec_liefoff.clear();
	}
     else
 	return false;

#ifdef MABELLA_EXTENSIONS     
     if(lv.size()==0) // Verkäufer auf NONE setzten
       lieferschein->setVerknr(Kunde::none_id);
#endif       
     return true;
    }
#ifdef MABELLA_EXTENSIONS     
  } catch(LagerError &e){meldung->Show(e); return false;}
#else
  } catch(...){}
#endif

 return false;
}

void auftrag_lieferschein::on_daten_leaf_selected(cH_RowDataBase d)
{
 tree_offen->get_selection()->unselect_all();

 try{

 const Data_Lieferdaten *dt=dynamic_cast<const Data_Lieferdaten*>(&*d);

 bool permit_change((dt->get_LieferscheinEntry().Status()==UNCOMMITED ||
       		     dt->get_LieferscheinEntry().Status()==OPEN) &&
		     lieferschein->RngNr() == ManuProcEntity<>::none_id && 
		     dt->get_Lieferschein_Id()!=LieferscheinBase::none_id);

 lieferzeile_delete->set_sensitive(permit_change);

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

void auftrag_lieferschein::on_daten_unselect_row()
{
  clear_input();
  button_zeile_modifizieren->set_sensitive(false);
 frame_notiz->set_label(_("Notiz zum Lieferschein"));
 try {
 lief_notiz->get_buffer()->set_text(lieferschein->Notiz());
 } catch(SQLerror&e) { lief_notiz->get_buffer()->set_text(Glib::ustring()); }
 liefnotiz_save->set_sensitive(false);
}


void auftrag_lieferschein::on_artikelbox_activate()
{
 Einheit e(artikelbox->get_value());
 menge_einheit->set_text(e.StueckEinheit());

 label_stueck->set_text(e.StueckEinheit());

 if (e.hatMenge()) 
 { table_menge->show();
   menge_einheit->set_text(e.MengenEinheit());
//     liefermenge->set_value(Lieferschein::StandardLaenge(AE.Artikel()).as_int());
   liefermenge->grab_focus();
   liefermenge->select_region(0,liefermenge->get_text_length());
 }
 else 
 { auftragnr->grab_focus();
   table_menge->hide();
 }
}

void auftrag_lieferschein::on_button_liste_clicked()
{
  new lieferscheinliste(instanz);
}

void auftrag_lieferschein::set_tree_offen_content()
{
 clear_input();
 tree_offen->clear();

 try{
   if(datavec_liefoff.empty())
     {
      SQLFullAuftragSelector sel;
#ifdef MABELLA_EXTENSIONS

      sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Kunde_Status_Lager(
                   instanz->Id(), liefer_kunde->get_value(), 
			(AufStatVal)OPEN, 
		int(long(lagerwahl->get_menu()->get_active()->get_data("user_data")))));
#else
      sel=SQLFullAuftragSelector(SQLFullAuftragSelector::sel_Kunde_Status(
                   instanz->Id(), liefer_kunde->get_value(), (AufStatVal)OPEN));
#endif
      SelectedFullAufList *offene_auftraege=new SelectedFullAufList(sel);
      for (SelectedFullAufList::const_iterator i=offene_auftraege->begin();i!=offene_auftraege->end();++i)
        datavec_liefoff.push_back(new Data_Lieferoffen(*i,auftragmain));
     }

   std::vector<cH_RowDataBase> datavec;
   typedef std::vector<cH_RowDataBase>::const_iterator DVI;
   
   for(DVI i=datavec_liefoff.begin(); i!=datavec_liefoff.end();++i)
     {
      Handle<const Data_Lieferoffen> h_lo=
			(*i).cast_dynamic<const Data_Lieferoffen>();
      const std::vector<cH_RowDataBase> &datavec_liefdata
          =tree_daten->getModel().getDataVec();
      DVI j = find_if(datavec_liefdata.begin(),datavec_liefdata.end(),
		Lief_ref_Auftrag(h_lo->getAufEintrag()));

      if(j==datavec_liefdata.end())
	{
	 Handle<const Data_Lieferoffen> h_lo=
		(*i).cast_dynamic<const Data_Lieferoffen>();
	 if(h_lo->getAufEintrag().getRestStk().as_int()>0)
           datavec.push_back(*i);
	}
      else
        {Handle<const Data_Lieferdaten> h_ld=
				(*j).cast_dynamic<const Data_Lieferdaten>();
	 int rest=h_lo->getAufEintrag().getRestStk().as_int();
	 if(rest>0)
	   datavec.push_back(*i);
	}
     }
   tree_offen->setDataVec(datavec);
   // scroll to bottom (put this into SimpleTree?)
   Gtk::Adjustment *adj=tree_offen->get_vadjustment();
   if (adj)
   { if (adj->get_upper()>adj->get_page_size())
       adj->set_value(adj->get_upper()-adj->get_page_size());
//     else
//       adj->set_value(0);
   }
  }catch (SQLerror &e) {std::cerr <<e<<'\n';}
}

void auftrag_lieferschein::on_button_zeile_modifizieren_clicked()
{
 if(lieferschein->RngNr() != ManuProcEntity<>::none_id)
   {
    meldung->Show(_("Berechnete Lieferscheine können nicht mehr geändert werden"));
    return;
   }

 try{
   cH_Data_Lieferdaten dt(tree_daten->getSelectedRowDataBase_as<cH_Data_Lieferdaten>());
   LieferscheinEntry LE = dt->get_LieferscheinEntry();
   
   Palette->update();
   liefermenge->update();
   anzahl->update();


   if(Palette->get_value_as_int() != LE.Palette())
      LE.setPalette(Palette->get_value_as_int());
   if (auftragnr->get_text()!=LE.getRefOrder() 
       && (!dt->getAufEintragBase().valid() ||
           auftragnr->get_text()!=AuftragBase::ID2string(dt->getAufEintragBase().Id())))
   { LE.setRefOrder(auftragnr->get_text());
   }
   if(LieferscheinBase::mengen_t(liefermenge->get_value()) != LE.Menge() ||
            anzahl->get_value_as_int() != LE.Stueck())
    {
     if(LE.Status()==(AufStatVal)UNCOMMITED ||
	LE.Status()==(AufStatVal)OPEN)
       {
        int deltaMenge(anzahl->get_value_as_int() - LE.Stueck());

        LE.changeMenge(anzahl->get_value_as_int(),liefermenge->get_value(),
#ifdef MABELLA_EXTENSIONS
				true
#else
				false
#endif
	);
	if(LE.Status()==(AufStatVal)UNCOMMITED)
	  adjustOffAufEntry(dt,deltaMenge);
	else
	  datavec_liefoff.clear();
       }
    }

   set_tree_daten_content(lieferschein->Id());
   set_tree_offen_content();
  } 
 catch(SQLerror &e)
   {meldung->Show(e); return;}
 catch(std::exception &e) {std::cerr << e.what();}
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
   {  s=Formatiere((unsigned long)stueck)
        + Einheit(artikel).StueckEinheit();
   }
  if (menge.Scaled()!=0)
   {  if (stueck!=1) smenge+="*";
      s+=Formatiere_short(menge)
         + Einheit(artikel).MengenEinheit();
   }
  if (stueck==1 && menge.Scaled()==0)
      s=Formatiere((unsigned long)stueck)+Einheit(artikel).MengenEinheit();
 return s;
}

void auftrag_lieferschein::on_liefnotiz_changed()
{  
 liefnotiz_save->set_sensitive(true); 
}

void auftrag_lieferschein::on_liefnotiz_save_clicked()
{  
 if(lieferschein) 
 { if (!tree_daten->get_selection()->count_selected_rows())
     lieferschein->Notiz(lief_notiz->get_buffer()->get_text());
   else
   { cH_Data_Lieferdaten dt(tree_daten->getSelectedRowDataBase_as<cH_Data_Lieferdaten>());
     LieferscheinEntry LE = dt->get_LieferscheinEntry();
     LE.Text(lief_notiz->get_buffer()->get_text());
     // redisplay
     set_tree_daten_content(lieferschein->Id());
   }
 }
 liefnotiz_save->set_sensitive(false);
}


void auftrag_lieferschein::on_lagerwahl_changed()
{
 try{
  datavec_liefoff.clear();
  set_tree_offen_content();
 } catch(SQLerror &e) {meldung->Show(e);}  
}

void auftrag_lieferschein::on_lieferkunde_reset()
{
 clear_input();
 tree_daten->clear();
 tree_offen->clear();
 datavec_liefoff.clear();
 liefernr->reset();
std::cout << "KUNDE reset\n";
}


void auftrag_lieferschein::erledigt_markieren()
{
 if(lieferschein)
   {
    Query("update lieferscheinentry set lagerid=0 where "
	"(lfrsid,instanz)=(?,?) and status=?"
	" and lagerid is null") 
	<< lieferschein->Id() 
	<< lieferschein->Instanz()->Id()
	<< (AufStatVal)UNCOMMITED;
    SQLerror::test(__FILELINE__,100);
   }
}

void auftrag_lieferschein::on_lager_buchen_clicked()
{
   int ret=0;
   { buchen_dialog bd(!instanz->ExterneBestellung());
     bd.set_transient_for(*this);
     ret=bd.run();
   }

   if(ret==Gtk::RESPONSE_YES)
     {
      bool exists_ungebucht=false;

      const std::vector<cH_RowDataBase> &datavec_liefdata
          =tree_daten->getModel().getDataVec();
      if(datavec_liefdata.empty()) return;
      std::vector<cH_RowDataBase>::const_iterator i=datavec_liefdata.begin();
      for(;i!=datavec_liefdata.end();++i)
	{
	 Handle<const Data_Lieferdaten> ld=
			(*i).cast_dynamic<const Data_Lieferdaten>();
         LieferscheinEntry LE = ld->get_LieferscheinEntry();
	 if(ld->get_LieferscheinEntry().Status()==(AufStatVal)UNCOMMITED)
	   {
	    LE.lagerid=long(lagerwahl->get_menu()->
				get_active()->get_data("user_data"));
            Transaction tr;
	    try {
	    LE.changeStatus((AufStatVal)OPEN,true);
	    LE.setLagerid(long(lagerwahl->get_menu()->
			      get_active()->get_data("user_data")) );
	
	    // Wenn schon fakturiert, dann direkt schliessen		      
	    if(lieferschein->RngNr()!=ManuProcEntity<>::none_id)
	      LE.changeStatus((AufStatVal)CLOSED,false);
	      
            tr.commit();
	    }
            catch(SQLerror &e) {meldung->Show(e); tr.rollback(); return;}
	    catch(LagerError &l) 
		{std::cerr << l << "\n"; 
 		 tr.rollback(); 
		 exists_ungebucht=true;
		 continue;
		}
	   }
	}

    if(exists_ungebucht)
      {
       info_dialog id(_("Einige Zeilen konnten nicht"
	 " vom Lager abgebucht werden, da der Lagerbestand nicht ausreicht"));
       id.set_transient_for(*this);
       id.run();
      }
    

      datavec_liefoff.clear();
      on_liefnr_activate();     
//      lager_buchen->set_sensitive(false);
     }
   else
   if(ret==Gtk::RESPONSE_NO)
     delete this;
   if(ret==Gtk::RESPONSE_REJECT)
     {erledigt_markieren();
      delete this;
     }
   else
     return;
}



void auftrag_lieferschein::on_ungeb_lief_show_activate()
{  
 std::string cmd("ungebuchte_lfrs V");
 system(cmd.c_str());
}

void auftrag_lieferschein::on_ungeb_lief_print_activate()
{  
 std::string cmd("ungebuchte_lfrs P");
 system(cmd.c_str());
}

#include "petig_we.hh"

void auftrag_lieferschein::on_petig_we_activate()
{
 if(!instanz->ExterneBestellung()) return;

 if(lieferschein->Id()==ManuProcEntity<>::none_id)
   {
    meldung->Show(_("Bitte einen Lieferschein erstellen oder laden"));
    return;
   }
   
 if(lieferschein->RngNr()!=ManuProcEntity<>::none_id)
   {
    meldung->Show(_("Lieferschein ist fakturiert und darf nicht mehr gänder werden"));
    return;
   }   

 petig_we pwe;  
 pwe.set_transient_for(*this);

 pwe.run();

}

