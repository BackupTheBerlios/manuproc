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

#include "auftrag_bearbeiten.hh"
#include "auftrag_main.hh"
#include <Aux/ctime_assert.h>
#include <Artikel/Einheiten.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/Artikelpreis.h>
#include <Gtk_OStream.h>
#include <Aux/Ausgabe_neu.h>
#include <Aux/dbconnect.h>
#include <unistd.h>
//#include <Auftrag/AufEintrag.h>

//#include <Auftrag/AuftragsEntryZuordnung.h>

#ifndef OLD
#include<Auftrag/selFullAufEntry.h>
#include "MyMessage.h"  
#include "AufEintrag.h" 
#include "auftragbase.h"
#endif

extern auftrag_main *auftragmain;
extern auftrag_bearbeiten *auftragbearbeiten;

extern MyMessage *meldung;

auftrag_bearbeiten::auftrag_bearbeiten(const cH_ppsInstanz& _instanz,const AufEintragBase *auftragbase)
: instanz(_instanz), kunde(Kunde::default_id),artikel_preisliste_geaendert(false)
{
 splitdialog=0;
 liefdatum_datewin->set_tab_pos(GTK_POS_TOP);
 table_auftragseintraege->hide();
 scrolledwindow_auftraege->hide();
 
#ifdef PETIG_EXTENSIONS
 table_preislisten->hide();
#endif

#ifdef MABELLA_EXTENSIONS
 WAufStat->set_history((AufStatVal)OPEN);
 _tooltips.set_tip(*button_drucken,"Linke Maustaste: 1 Original
Mittlere Maustaste: 1 Kopie","");

   std::string nurliefer(" and lieferadresse=true and coalesce(aktiv,true)=true");
   kundenbox->Einschraenkung(nurliefer);
   kundenbox->Einschraenken(true); 
#endif 

 zahlart->hide_int(true);
 
 zahlziel_datewin->setLabel(std::string("Zahlungsziel"));
 aufdatum_datewin->setLabel(std::string("Auftragsdatum"));

 newauftrag=false;
 
 aktaufeintrag = 0;
 selectedentry=-1;
 auftrag = 0;
 
 if(auftragbase)
   {
    loadAuftrag(*auftragbase);
    if(auftragbase->ZNr())
      {
       auftrag_clist->moveto(auftragbase->ZNr()-1,0,.5,0);
       auftrag_clist->row(auftragbase->ZNr()-1).select();
      }
   }
}

void auftrag_bearbeiten::onSelArtikel()
{
 assert(auftrag);
 assert(!aktaufeintrag);
 Einheit e(artikelbox->get_value());
 mengeeinheit->set_text((std::string)e);
 WPreis->set_Einheit((std::string)e);
 try {
    WPreis->reset();
#ifdef MABELLA_EXTENSIONS

    if(preislisten->get_value() == PreisListe::none_id)
      {
         Artikelpreis AP(kunde,artikelbox->get_value());
         if (AP.Gefunden())
            {artikel_preisliste=cH_PreisListe(AP.GefundenIn());
             preislisten->set_value(artikel_preisliste->Id());
            }
         else
            artikel_preisliste=PreisListe::none_id;
      }
    else
      artikel_preisliste=cH_PreisListe(preislisten->get_value());

    Artikelpreis ap(artikel_preisliste->Id(),artikelbox->get_value());
#else
    Artikelpreis ap(kunde->preisliste(),artikelbox->get_value());
#endif

    Preis p(ap.In(auftrag->getWaehrung()));
    WPreis->set_Waehrung(auftrag->getWaehrung());
    WPreis->set_all(p.Wert(),p.PreisMenge());
    artikel_preisliste_geaendert=false;
  } catch (SQLerror &e) 
  {  std::cerr << e <<'\n';  }
 stkmtr_spinbutton->grab_focus();
 stkmtr_spinbutton->select_region(0,stkmtr_spinbutton->get_text_length());
}

void auftrag_bearbeiten::on_auftrag_clist_select_row
				(gint row, gint column, GdkEvent *event)
{   
 assert(auftrag);
 aktaufeintrag = &auftrag->getAufEntry(row);
 try{artikelbox->set_value(ArtikelBase(aktaufeintrag->ArtId()));
 Einheit e(artikelbox->get_value());
 mengeeinheit->set_text((std::string)e);
 WPreis->set_Einheit((std::string)e);
 WPreis->set_Waehrung(auftrag->getWaehrung());
 }
 catch(SQLerror &e)
   {meldung->Show(e); return;}
 catch(ArtikelBoxErr &e)
   {meldung->Show(e.ErrMsg()); return;}
 setAufEntries();
 selectedentry=row;
 artikelbox->set_editable(false);

 WAufStat->set_History(aktaufeintrag->getAufStatus());
 aufentry_ok->set_sensitive(false);
 aufentry_abbruch->set_sensitive(false);
}

void auftrag_bearbeiten::on_auftrag_clist_unselect_row
				(gint row, gint column, GdkEvent *event)
{   
 aufentry_ok->set_sensitive(true);
 clearEntry();
}

void auftrag_bearbeiten::clearEntry()
{
 aktaufeintrag=0;
 artikelbox->reset();
 stkmtr_spinbutton->set_value(0);
 WPreis->reset();
 selectedentry=-1;
 artikelbox->set_editable(true);
#ifdef MABELLA_EXTENSIONS
 WAufEntryStat->set_history((AufStatVal)OPEN);
#else
 WAufEntryStat->set_history((AufStatVal)UNCOMMITED);
#endif 
 aufentry_ok->set_sensitive(true);
 aufentry_abbruch->set_sensitive(true);
}

gint auftrag_bearbeiten::on_aufrabatt_spinbutton_focus_out_event(GdkEventFocus *ev)
{
  if(auftrag)
   {
     aufrabatt_spinbutton->update();
     auftrag->setRabatt(aufrabatt_spinbutton->get_value_as_float());
   }
  return false;
}

void auftrag_bearbeiten::on_backtomain_button_clicked()
{splitdialog=0;
 destroy();
 auftragmain->show(); 
 auftragbearbeiten = NULL;
}

void auftrag_bearbeiten::on_newauftrag_button_clicked()
{on_clear_all();
 auftrag_ok->set_sensitive(true);
 auftrag_abbruch->set_sensitive(true);
 aufnr_scombo->set_sensitive(false);
 kundenbox->grab_focus();
}

void auftrag_bearbeiten::auftragstatus_geaendert()
{ 
 if (auftrag)
  { 
     auftrag->setStatusAuftragFull(WAufStat->get_Status(),int(getuid()));
  }
}

void auftrag_bearbeiten::waehrung_geaendert()
{
try { 
 if (auftrag)
   {  auftrag->setWaehrung(bea_WWaehrung->get_enum());
      WPreis->set_Waehrung(cP_Waehrung(bea_WWaehrung->get_enum()));
   }
 }
catch(SQLerror &e)
  {meldung->Show(e); return;}
}

void auftrag_bearbeiten::on_zahlziel_activate()
{   
try {   
 if(auftrag)
    auftrag->Zahlziel(zahlziel_datewin->get_value());
 }
catch(SQLerror &e)
  {meldung->Show(e); return;} 
}

void auftrag_bearbeiten::on_zahlziel_showkal_button_clicked()
{

}

void auftrag_bearbeiten::on_aufdat_showkal_button_clicked()
{   
}

void auftrag_bearbeiten::on_jahrgang_spinbutton_activate()
{   
 aufbemerkung_entry->grab_focus();
}

void auftrag_bearbeiten::on_stkmtr_spinbutton_activate()
{   
 assert(auftrag);
 if(aktaufeintrag)
      {
       gtk_spin_button_update(stkmtr_spinbutton->gtkobj());
       AuftragBase::mengen_t diffmenge=
       	AuftragBase::mengen_t(stkmtr_spinbutton->get_value_as_int())-
        aktaufeintrag->getStueck();
       			
       AuftragBase::mengen_t mt=aktaufeintrag->updateStkDiff__(getuid(),diffmenge,true);
       assert(mt==diffmenge);
       fillCList();
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
  if(stkmtr_spinbutton->get_value_as_int() > 0)
   { liefdatum_datewin->grab_focus();
   }
}

void auftrag_bearbeiten::on_lieferdatum_activate()
{   
  assert(auftrag);
  if(aktaufeintrag)
      {aktaufeintrag->updateLieferdatum(liefdatum_datewin->get_value(),getuid());
       fillCList();
	    auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
  aufentry_ok->grab_focus();
}


void auftrag_bearbeiten::on_aufentry_abbruch_clicked()
{   
 clearEntry();
 artikelbox->grab_focus();
}

void auftrag_bearbeiten::on_aufentry_ok_clicked()
{
 assert(auftrag);
 if (!aktaufeintrag)
   {
     stkmtr_spinbutton->update();
     cH_PreisListe artpreis=artikel_preisliste;
     if(artikel_preisliste_geaendert) 
      { artpreis=cH_PreisListe(PreisListe::none_id);
        artikel_preisliste_geaendert=false;
      }
//     int znr=
     auftrag->AuftragFull::insertNewEntry(
               stkmtr_spinbutton->get_value_as_int(),
               liefdatum_datewin->get_value(),
               artikelbox->get_value().Id(),
               WAufEntryStat->get_Status(),
               getuid(),
               WPreis->get_Preis(),
               rabattentry_spinbutton->get_value_as_float(),
               artpreis);
      fillCList();
      artikelbox->reset();
      artikelbox->grab_focus();
    }
}

void auftrag_bearbeiten::on_preis_changed()
{
 artikel_preisliste_geaendert=true;
}


void auftrag_bearbeiten::on_showkal_button_clicked()
{   
}

void auftrag_bearbeiten::on_rabattentry_spinbutton_activate()
{
 assert(auftrag);
 if (aktaufeintrag) 
   {
     gtk_spin_button_update(rabattentry_spinbutton->gtkobj());
     aktaufeintrag->updateRabatt(rabattentry_spinbutton->get_value_as_float());
       fillCList();
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
   {
    stkmtr_spinbutton->grab_focus();
    stkmtr_spinbutton->select_region(0,stkmtr_spinbutton->get_text_length()); 
   }
}

void auftrag_bearbeiten::on_aufentrystat_optionmenu_clicked()
{
  if (aktaufeintrag)
    {
       aktaufeintrag->setStatus(WAufEntryStat->get_Status(),int(getuid()));
       fillCList();
	    auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
       loadAuftrag(*auftrag);
    }
}


void auftrag_bearbeiten::loadAuftrag(const AuftragBase& auftragbase)
{
 try 
  { AuftragBase ab(auftragbase); // in case we got passed *auftrag ...
    if(auftrag) delete auftrag; 
    auftrag = new AuftragFull(ab);
  } catch(SQLerror &e)
  {
   meldung->Show(e); auftrag=NULL; return;}
 catch (Kalenderwoche::error &e)
 { std::cerr << "KW error\n";
 }

 artikelbox->reset();
 fillMask();
 aktaufeintrag = 0;
 stkmtr_spinbutton->set_value(0);
 rabattentry_spinbutton->set_value(0);
 WPreis->reset();
 liefdatum_datewin->set_value(ManuProC::Datum::today());
#ifdef MABELLA_EXTENSIONS
 WAufEntryStat->set_history((AufStatVal)OPEN);
#else
 WAufEntryStat->set_history((AufStatVal)UNCOMMITED);
#endif
 table_auftragseintraege->show();
 scrolledwindow_auftraege->show();
 zahlart->set_value(auftrag->Zahlart()->Id());
}

void auftrag_bearbeiten::fillMask() 
{if (!auftrag) return;
 kundenbox->set_value(auftrag->getKundennr());
 andererKunde(); // adjust Schema
 fillCList();
 WAufStat->set_history(auftrag->getStatus());
 aufnr_scombo->setContent(auftrag->getAuftragidToStr(),auftrag->Id());
 youraufnr_scombo->setContent(auftrag->getYourAufNr(),auftrag->Id());
 aufbemerkung_entry->set_text(auftrag->getBemerkung());
// jahrgang_spinbutton->set_value(auftrag->getJahrgang());
 aufdatum_datewin->set_value(auftrag->getDatum());
 bea_WWaehrung->set_History( auftrag->getWaehrung()->get_enum() );

 aufrabatt_spinbutton->set_value(auftrag->getAuftragsRabatt());
 zahlziel_datewin->set_value(auftrag->Zahlziel());
 auftrag_clist->thaw();
}

void auftrag_bearbeiten::Rabatt_setzen(const cH_Kunde &kunde)
{   if(kunde->zeilenrabatt())
     {
      aufrabatt_spinbutton->set_sensitive(false);
      aufrabatt_spinbutton->set_value(0);
      rabattentry_spinbutton->set_sensitive(true);
      rabattentry_spinbutton->set_value(kunde->rabatt());
     }          
    else
     {
      aufrabatt_spinbutton->set_sensitive(true);
      aufrabatt_spinbutton->set_value(kunde->rabatt());
      rabattentry_spinbutton->set_value(0);
     }          
}

void auftrag_bearbeiten::Rabatt_setzen(const cH_PreisListe &liste)
{if (!kunde->zeilenrabatt()) return;
 if (liste->festerRabatt())
    rabattentry_spinbutton->set_value(liste->getRabatt());
 else 
    rabattentry_spinbutton->set_value(kunde->rabatt());
}

void auftrag_bearbeiten::andererKunde()
{  
   kunde = kundenbox->get_value();
   Rabatt_setzen(kunde);
#ifdef MABELLA_EXTENSIONS
   std::string eins(" and exists (select prlsnr from ku_warenkorb"
   	" where prlsnr=ku_preisliste.prlsnr and kundennr=");
   eins+=itos(kunde->Id())+")";
   preislisten->Einschraenkung(eins);
   preislisten->set_value(kunde->preisliste());
   Rabatt_setzen(cH_PreisListe(kunde->preisliste()));
#endif
   artikelbox->setExtBezSchemaID(kunde->getSchemaId());

    bea_WWaehrung->set_History(kunde->getWaehrung()->get_enum());
    WPreis->set_Waehrung(kunde->getWaehrung()->get_enum() );

#ifdef MABELLA_EXTENSIONS
   artikelbox->NurWarenkorb(kunde->preisliste());
   artikelbox->Einschraenken_b(true);
#endif

   if(!kunde->isRechnungsadresse())
     {cH_Kunde krng(kunde->Rngan());
      zahlart->set_value(krng->zahlungsart()->Id());
     }
   else
      zahlart->set_value(kunde->zahlungsart()->Id());
}

void auftrag_bearbeiten::preisliste_reset()
{
 artikel_preisliste=PreisListe::none_id;
 artikelbox->reset();
 artikelbox->Einschraenken_b(false);
 artikelbox->grab_focus();
 
}


void auftrag_bearbeiten::on_aufnrscombo_activate()
{
 loadAuftrag(AuftragBase(instanz,aufnr_scombo->Content()));
}

int auftrag_bearbeiten::get_active_index(Gtk::Menu *om)
{
 return (int)(om->get_active()->get_user_data());
}


void auftrag_bearbeiten::on_youraufnrscombo_activate()
{ 
 if(newauftrag)
   jahrgang_spinbutton->grab_focus();
 else
  try{
   loadAuftrag(AuftragBase(instanz,youraufnr_scombo->Content()));
   } catch(SQLerror &e) {std::cerr << e<<'\n';}
}



auftrag_bearbeiten::~auftrag_bearbeiten()
{
 if(auftrag) delete(auftrag);
 auftrag=NULL;
}

void auftrag_bearbeiten::setAufEntries()
{
 assert(aktaufeintrag);
 stkmtr_spinbutton->set_value(aktaufeintrag->getStueck());
 liefdatum_datewin->set_value(aktaufeintrag->getLieferdatum());

 WPreis->set_Betrag(aktaufeintrag->EPreis().Wert());
 rabattentry_spinbutton->set_value(float(aktaufeintrag->Rabatt()));
 WPreis->set_Preismenge(aktaufeintrag->PreisMenge());
 
 WAufEntryStat->set_history(aktaufeintrag->getEntryStatus());
}

void auftrag_bearbeiten::on_splitten()
{
 if(splitdialog) splitdialog->destroy();
 splitdialog = manage(new termsplidial());
}

bool auftrag_bearbeiten::splitEntry()
{
 if(!splitdialog || !auftrag) return false;

 ManuProC::Datum newlief = splitdialog->getLiefDatum();
 int newmenge = splitdialog->getMenge();
 if(!newmenge) {meldung->Show(std::string("Menge nicht korrekt")); return false;}
 
 try{auftrag->split(getuid(),selectedentry, newlief, newmenge);}
 catch(SQLerror &e)
  {meldung->Show(e); return false;}    
 loadAuftrag(*auftrag);
 return true;
}

void auftrag_bearbeiten::on_clear_all()
{

 clearEntry();
 auftrag_clist->freeze();
 auftrag_clist->clear();
 auftrag_clist->thaw();
 if(auftrag) delete(auftrag);
 auftrag=0;
 aktaufeintrag=0; 
 kundenbox->reset();
 artikelbox->reset();		// ich denke das sollte hier auch rein
#ifdef MABELLA_EXTENSIONS 
  WAufStat->set_history((AufStatVal)OPEN);
#else
  WAufStat->set_history((AufStatVal)UNCOMMITED);
#endif
 aufnr_scombo->reset();
 youraufnr_scombo->reset();
 aufbemerkung_entry->set_text("");
 jahrgang_spinbutton->set_value(ManuProC::Datum::today().Jahr());
 aufdatum_datewin->set_value(ManuProC::Datum::today());
 aufrabatt_spinbutton->set_value(0);
 zahlziel_datewin->set_value(ManuProC::Datum::today());
 zahlart->reset();
 bea_WWaehrung->set_History(WaehrungID::EUR);
 preislisten->reset();
}


void auftrag_bearbeiten::on_auftrag_abbruch_clicked()
{   
 on_clear_all();
 auftrag_ok->set_sensitive(false);
 auftrag_abbruch->set_sensitive(false);
 aufnr_scombo->grab_focus();
 aufnr_scombo->set_sensitive(true);
}

void auftrag_bearbeiten::on_auftrag_ok_clicked()
{   
 try {
   auftrag = new AuftragFull(Auftrag::Anlegen(instanz->Id()),kundenbox->get_value());
 	auftrag->setBemerkung(aufbemerkung_entry->get_text());
 	auftrag->setYourAufNr(youraufnr_scombo->get_text());

     auftrag->Zahlart(zahlart->get_value());

      AuftragBase ab(*auftrag);

      cH_Kunde k(kundenbox->get_value());
      
      on_clear_all(); // careful this deletes auftrag
      loadAuftrag(ab);

// auf offen setzen
      auftrag->setStatusAuftragFull((AufStatVal)OPEN,getuid()); 
      WAufEntryStat->set_history((AufStatVal)OPEN); 
      WAufStat->set_history((AufStatVal)OPEN); 

 auftrag_ok->set_sensitive(false);
 auftrag_abbruch->set_sensitive(false);
 artikelbox->grab_focus();
 aufnr_scombo->set_sensitive(true);
      }
 catch(SQLerror &e)
   {meldung->Show(e);
    auftrag=NULL;
    return;
   }
}

void auftrag_bearbeiten::on_kunden_activate()
{   
 youraufnr_scombo->grab_focus();
 andererKunde();
}

void auftrag_bearbeiten::on_aufbemerkung_activate()
{   
 if(newauftrag)
   auftrag_ok->grab_focus();
 if(auftrag) auftrag->setBemerkung(aufbemerkung_entry->get_text());
}


void auftrag_bearbeiten::on_button_preview_clicked()
{  if (!auftrag) return;
   std::string art="Auftrag";
   if(instanz!=ppsInstanzID::Kundenauftraege) art="Extern";
   std::string command = "auftrag_drucken -a "+art+" -n "+itos(auftrag->Id())+
   		" -i " + itos(instanz->Id());
   system(command.c_str());
}  

gint auftrag_bearbeiten::on_button_drucken_clicked(GdkEventButton *ev)
{
   if (!auftrag) return false;

   std::string art="Auftrag";
   if(instanz!=ppsInstanzID::Kundenauftraege) art="Extern";

   if(ev->button==1)
   {
   std::string command = "auftrag_drucken -f -a "+art+" -n "+itos(auftrag->Id())+
   	" -p -i " + itos(instanz->Id());
   system(command.c_str());
   }

  if(ev->button==2)
   {
   std::string command = "auftrag_drucken  -a "+art+" -n "+itos(auftrag->Id())+
   	" -p -i " + itos(instanz->Id());
   system(command.c_str());
   }
 return false;
}


void auftrag_bearbeiten::on_activate_wpreis()
{
  assert(auftrag);
  if (aktaufeintrag)
   {
     Preis pr(WPreis->get_value());

     aktaufeintrag->updatePreis(pr);
     fillCList();
     auftrag_clist->grab_focus();
     auftrag_clist->moveto(selectedentry,0,.5,0);
   }
 else
  {
   rabattentry_spinbutton->grab_focus();
   rabattentry_spinbutton->select_region(0,rabattentry_spinbutton->get_text_length());
  }
}


void auftrag_bearbeiten::fillCList()
{
  auftrag_clist->freeze();
  auftrag_clist->clear();
  Gtk::OStream os(auftrag_clist);
  Preis psum;
  for(AuftragFull::const_iterator i = auftrag->begin();i!=auftrag->end();++i)
   {
     os << i->getStueck()<<'\t'
        << cH_ArtikelBezeichnung(ArtikelBase(i->ArtId()))->Bezeichnung()<<'\t'
        << '\t'
        << i->getRestStk()<<'\t'
        << i->EPreis().Wert()<<'\t'
        << Formatiere(i->Rabatt()) <<'\t'
        << i->GPreis().Wert()<<'\t'
        << i->getLieferdatum()<<'\t'
        << i->getEntryStatusStr()<<"\t"
        << i->LastEditDate()<<"\n";
     psum += i->GPreis();
//cout << i->GPreis() << ":" << psum << "\n";     
   } 

#ifdef PETIG_EXTENSIONS
  os << "\t\t\t\t-----------\n";
  os << "\t\t\tAuftragswert\t";
  os << Formatiere(psum.Wert()) << "\n";
#endif  

  auftragswert->set_text(Formatiere(psum.Wert()));

  for(guint i=0; i<auftrag_clist->columns().size();++i)
    auftrag_clist->set_column_auto_resize(i,true);
  auftrag_clist->thaw();
}

void auftrag_bearbeiten::on_auftrag_preislisten_activate()
{
#ifdef MABELLA_EXTENSIONS
 cH_PreisListe p(preislisten->get_value());
 artikelbox->reset();
 artikelbox->NurWarenkorb(p->Id());
 artikelbox->Einschraenken_b(true);
 artikelbox->grab_focus();
 Rabatt_setzen(p);
#endif
}

void auftrag_bearbeiten::on_zahlart_activate()
{   
try {   
 if(auftrag)
    auftrag->Zahlart(zahlart->get_value());
 }
catch(SQLerror &e)
  {meldung->Show(e); return;}  
}

void auftrag_bearbeiten::on_rueckstand_clicked()
{
 std::string command = "auftrag_drucken -a Auftrag -R -n "+itos(auftrag->Id())+
   	" -p -i " + itos(instanz->Id());
   system(command.c_str());  
}

