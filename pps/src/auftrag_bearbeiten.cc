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
#include <Artikel/Artikelpreis.h>
#ifndef OLD
#include<Auftrag/selFullAufEntry.h>
#include "MyMessage.h"
#include "AufEintrag.h"
#include "auftragbase.h"
#endif

//typedef map<int ,AufEintrag *> X;
//extern X aufentrymap;

extern SelectedFullAufList *allaufids;
extern auftrag_main *auftragmain;
extern auftrag_bearbeiten *auftragbearbeiten;

#ifndef OLD
extern MyMessage *meldung;
#endif

// some assertions about constant equivalency
#warning ich hab die sechs Zeilen mal auskommentiert, ich habe eine Anhnung 
#warning wozu die sind, aber ich denke mit dem neuen Widget sind sie überflüssig.
//typedef ctime_assert<(AufStatVal(auftrag_bearbeiten::aufstat::unbest_tigt)==UNCOMMITED)>::_true failed;
//typedef ctime_assert<(AufStatVal(auftrag_bearbeiten::aufstat::offen)==OPEN)>::_true failed2;
//typedef ctime_assert<(AufStatVal(auftrag_bearbeiten::aufstat::fertig)==CLOSED)>::_true failed3;
//typedef ctime_assert<(AufStatVal(auftrag_bearbeiten::aufentrystat::unbest_tigt)==UNCOMMITED)>::_true failed4;
//typedef ctime_assert<(AufStatVal(auftrag_bearbeiten::aufentrystat::offen)==OPEN)>::_true failed5;
//typedef ctime_assert<(AufStatVal(auftrag_bearbeiten::aufentrystat::fertig)==CLOSED)>::_true failed6;

auftrag_bearbeiten::auftrag_bearbeiten(const AufEintragBase2& auftragbase)
: kunde(Kunde::default_id)
{
 instanz = auftragbase.Instanz();
// assert(instanz==1);
 splitdialog=0;
 table_auftragseintraege->hide();
 scrolledwindow_auftraege->hide();

 liefertermin->set_page(0);
 zahlziel_datewin->setLabel(string("Zahlungsziel"));
 aufdatum_datewin->setLabel(string("Auftragsdatum"));
 jahr_spinbutton->set_value(Petig::Datum::today().Jahr());
 newauftrag=false;
 
 aktaufeintrag = new aktAufEintrag();
 selectedentry=-1;
 auftrag=NULL;
 if(auftragbase.Id())
   {
    loadAuftrag(auftragbase);
    if(auftragbase.ZNr())
      {
       auftrag_clist->moveto(auftragbase.ZNr()-1,0,.5,0);
       auftrag_clist->row(auftragbase.ZNr()-1).select();
      }
   }
// warum nicht mit glade?
 artikelbox->activate.connect(SigC::slot(this,&auftrag_bearbeiten::onSelArtikel));
}

void auftrag_bearbeiten::onSelArtikel()
{aktaufeintrag->setArtikel(artikelbox->get_value());
 Einheit e(artikelbox->get_value());
 mengeeinheit->set_text((string)e);
 WPreis->set_Einheit((string)e);
 try {
    WPreis->reset();
    WPreis->set_Waehrung(auftrag->getWaehrung());
    Artikelpreis ap(kunde->Preisliste(),artikelbox->get_value());
    if (auftrag)
    {  Preis p(ap.In(auftrag->getWaehrung()));
       WPreis->set_all(p.Wert(),p.PreisMenge());
    }
    else
    {  
#warning soll das wirklich 0 sein für die preismenge und nicht ein?
#warning von WPreis->reset() wird es jetzt auf 1 gesetzt.
//       preismenge->set_value(0); // ALT
       WPreis->reset();
    }
 } catch (SQLerror &e)
 {  cerr << e <<'\n';
 }
 
 stkmtr_spinbutton->grab_focus();

#warning was macht 'select_region'??? Wenn es das macht, was ich glaube, dann 
#warning ist es überflüssig, oder? MAT
// stkmtr_spinbutton->select_region(0,preis_spinbutton->get_text().size());
 stkmtr_spinbutton->select_region(0,WPreis->get_text_length());
}

void auftrag_bearbeiten::on_auftrag_clist_select_row
				(gint row, gint column, GdkEvent *event)
{   
 if(!auftrag || !auftrag->existsAufEntry(row)) return;
 AufEintragBase &aufe=auftrag->getAufEntry(row);
 try{artikelbox->set_value(ArtikelBase(aufe.ArtikelID()));
 Einheit e(artikelbox->get_value());
 mengeeinheit->set_text((string)e);
 WPreis->set_Einheit((string)e);
 }
 catch(SQLerror &e)
   {meldung->Show(e); return;}
 catch(ArtikelBoxErr &e)
   {meldung->Show(e.ErrMsg()); return;}
 aktaufeintrag->fill(aufe);
 setAufEntries();
 selectedentry=row;
 artikelbox->set_editable(false);

 WAufStat->set_History(aufe.getAufStatus());
 aufentry_ok->set_sensitive(false);
 aufentry_abbruch->set_sensitive(false);
}

void auftrag_bearbeiten::on_auftrag_clist_unselect_row
				(gint row, gint column, GdkEvent *event)
{   
 clearEntry();
}

void auftrag_bearbeiten::clearEntry()
{
 aktaufeintrag->clear();
 artikelbox->reset();
 stkmtr_spinbutton->set_value(0);
// WPreis->set_Betrag(0); Alternative zu
 WPreis->reset();
 kw_spinbutton->set_value(1);
 jahr_spinbutton->set_value(Petig::Datum::today().Jahr());
// liefdatum_datewin->set_value(Petig::Datum::today());
 selectedentry=-1;
 artikelbox->set_editable(true);
#warning wozu ist das 'set_sensitive' an dieser Stelle gut? MAT
// aufentrystat->set_sensitive(true); //ALT
 WAufEntryStat->set_history((AufStatVal)UNCOMMITED);
 aufentry_ok->set_sensitive(true);
 aufentry_abbruch->set_sensitive(true);
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
{ if (auftrag)
  { // auftrag->setStatusAuftrag((AufStatVal)get_active_index(aufstat->get_menu())); // ALT
     auftrag->setStatusAuftrag(WAufStat->get_Status()); // NEU
     loadAuftrag(*auftrag);
  }
}

void auftrag_bearbeiten::waehrung_geaendert()
{ 
 if (auftrag)
   {  auftrag->setWaehrung(bea_WWaehrung->get_enum());
      WPreis->set_Waehrung(cP_Waehrung(bea_WWaehrung->get_enum()));
   }
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
 gtk_spin_button_update(stkmtr_spinbutton->gtkobj());
 if (!aktaufeintrag) return;

 if(aktaufeintrag->getZln()>0)
      {auftrag->updateStk(selectedentry,stkmtr_spinbutton->get_value_as_int());
       auftrag_clist->freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
  if(aktaufeintrag->setStk(stkmtr_spinbutton->get_value_as_int()))
   {kw_spinbutton->grab_focus();
    kw_spinbutton->select_region(0,kw_spinbutton->get_text().size());
   }
}

void auftrag_bearbeiten::on_lieferdatum_activate()
{   if (!aktaufeintrag) return;
 if(aktaufeintrag->getZln()>0)
      {auftrag->updateLieferdatum(selectedentry,
			liefdatum_datewin->get_value());
       auftrag_clist->freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
  if(aktaufeintrag->setLieferdatum(liefdatum_datewin->get_value()))
     aufentry_ok->grab_focus();
}


void auftrag_bearbeiten::on_kw_spinbutton_activate()
{if (!aktaufeintrag) return;
 gtk_spin_button_update(kw_spinbutton->gtkobj());

 if(aktaufeintrag->getZln()>0)
      {auftrag->updateLieferdatum(selectedentry,
		Petig::Datum(Kalenderwoche(kw_spinbutton->get_value_as_int(),
			      jahr_spinbutton->get_value_as_int())) );
       auftrag_clist->freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 jahr_spinbutton->grab_focus();
 jahr_spinbutton->select_region(0,jahr_spinbutton->get_text().size());
}

void auftrag_bearbeiten::on_jahr_spinbutton_activate()
{if (!aktaufeintrag) return;
 gtk_spin_button_update(jahr_spinbutton->gtkobj());

 if(aktaufeintrag->getZln()>0)
   on_kw_spinbutton_activate();
 else
   if(aktaufeintrag->setLieferdatum(Kalenderwoche(
	kw_spinbutton->get_value_as_int(),
	jahr_spinbutton->get_value_as_int())))
     aufentry_ok->grab_focus();
}

void auftrag_bearbeiten::on_aufentry_abbruch_clicked()
{   
 clearEntry();
 artikelbox->grab_focus();
}

void auftrag_bearbeiten::on_aufentry_ok_clicked()
{if (!aktaufeintrag) return;
 if(aktaufeintrag->allesOK())
   if(auftrag)
     {
      int znr=auftrag->insertNewEntry(*aktaufeintrag,artikelbox->getBezSchema());
      allaufids->insert(*auftrag,znr);
      // perhaps the user want's to preserve lieferdatum ?
#warning TODO: Lieferdatum erhalten      
      aktaufeintrag->clear(); 
// bei Gelegenheit in eigene Funktion
 auftrag_clist->freeze();
 auftrag_clist->clear();
 auftrag->fillCList(*auftrag_clist);
 auftrag_clist->thaw();
#warning die nächste Zeile sorgt dafür, daß nach dem OK für einen Artikel
#warning nicht wieder alles zurückgestellt wird (vor allem wg. des Liefer-
#warning datums, aber ich glaube, auf den Rest kann auch verzichtet werden, oder MAT
//      setAufEntries();
      artikelbox->reset();
       artikelbox->grab_focus();
     }
}



void auftrag_bearbeiten::on_showkal_button_clicked()
{   
}

void auftrag_bearbeiten::on_rabattentry_spinbutton_activate()
{ if (!aktaufeintrag) return;
 gtk_spin_button_update(rabattentry_spinbutton->gtkobj());

 if(aktaufeintrag->getZln()>0)
      {auftrag->updateRabatt(selectedentry,
		(int)((rabattentry_spinbutton->get_value_as_float()+.0005)*100));
       auftrag_clist-> freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
   {aktaufeintrag->setRabatt(
		(int)((rabattentry_spinbutton->get_value_as_float()+.0005)*100));  
    stkmtr_spinbutton->grab_focus();
    stkmtr_spinbutton->select_region(0,stkmtr_spinbutton->get_text().size()); 
   }

}

void auftrag_bearbeiten::on_aufentrystat_optionmenu_clicked()
{
if (!aktaufeintrag) return;
  if(selectedentry>=0)
      {
       auftrag->setStatusEntry(selectedentry,
         WAufEntryStat->get_Status());
       auftrag_clist->freeze();
       loadAuftrag(*auftrag); 
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	    auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
}

/*
void auftrag_bearbeiten::on_preismenge_activate()
{   if (!aktaufeintrag) return;
 if(aktaufeintrag->getZln()>0)
      {
//       auftrag->updatePreismenge(selectedentry,preismenge->get_value_as_int());
// was ist denn das alles? CP			
       auftrag_clist->freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
}
*/

#warning Diese freeze,loadAuftrag,thaw,reset,grab_focus,moveto Aktion sollte in eine eigene Unterfunktion!
#warning MAT: Immer noch? durch das neue WPreis-Widget ist diese Funktion jetzt
#warning überflüssig geworden, aber siehe unten !!!
/*
void auftrag_bearbeiten::on_preis_spinbutton_activate()
{if (!aktaufeintrag) return;
 gtk_spin_button_update(preis_spinbutton->gtkobj()); // ALT
// Preis pr(preis_spinbutton->get_value_as_float(),auftrag->getWaehrung()); // ALT
 Preis pr(WPreis->get_Betrag(),auftrag->getWaehrung());

 if(aktaufeintrag->getZln()>0)
      {
       auftrag->updatePreis(selectedentry,pr);
       auftrag_clist-> freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
  {
   aktaufeintrag->setPreis(pr);
//   aktaufeintrag->setPreismenge(preismenge->get_value_as_float());
   rabattentry_spinbutton->grab_focus();
   rabattentry_spinbutton->select_region(0,rabattentry_spinbutton->get_text().size());      
  }
}
*/

void auftrag_bearbeiten::loadAuftrag(const AuftragBase& auftragbase)
{
 try 
  { AuftragBase ab(auftragbase); // in case we got passed *auftrag ...
    if(auftrag) delete auftrag; 
//assert(ab.Instanz()==1);
    auftrag = new AuftragFull(ab);
  } catch(SQLerror &e)
  {
   meldung->Show(e); auftrag=NULL; return;}
 catch (Kalenderwoche::error &e)
 { cerr << "KW error\n";
 }
 fillMask();
 aktaufeintrag->clear();
 stkmtr_spinbutton->set_value(0);
 kw_spinbutton->set_value(0);
 jahr_spinbutton->set_value(Petig::Datum::today().Jahr());
 rabattentry_spinbutton->set_value(0);
// WPreis->set_Betrag(0); alternative zu
 WPreis->reset();
 artikelbox->reset();
 liefdatum_datewin->set_value(Petig::Datum::today());
 WAufEntryStat->set_history((AufStatVal)UNCOMMITED); //NEU

 table_auftragseintraege->show();
 scrolledwindow_auftraege->show();

}

void auftrag_bearbeiten::fillMask() 
{if (!auftrag) return;
 kundenbox->set_value(auftrag->getKundennr());
 andererKunde(); // adjust Schema
 auftrag_clist->freeze();
 auftrag_clist->clear();
 auftrag->fillCList(*auftrag_clist);
 WAufStat->set_history(auftrag->getStatus());
 aufnr_scombo->set_text(auftrag->getAuftragidToStr());
 youraufnr_scombo->set_text(auftrag->getYourAufNr());
 aufbemerkung_entry->set_text(auftrag->getBemerkung());
 jahrgang_spinbutton->set_value(auftrag->getJahrgang());
 aufdatum_datewin->set_value(auftrag->getDatum());
// Neues WWaehrungswidget
 bea_WWaehrung->set_History( auftrag->getWaehrung()->get_enum() );

 auftrag_clist->thaw();
}

// welcher Artikel? vieleicht gibt es ja dazu eine Idee
void auftrag_bearbeiten::andererKunde()
{  kunde = kundenbox->get_value();

   cH_ExtBezSchema ebsh(kunde->getSchema(ArtikelTyp::AufgemachtesBand));
#if 0
// try a meaningful Type - because of ArtikelBox's restriction
   if (!ebsh->size())
     ebsh=cH_ExtBezSchema(kunde->getSchema(ArtikelTyp::GewebtesBand));
#endif     

   artikelbox->setExtBezSchema(ebsh);
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
   loadAuftrag(AuftragBase(instanz,youraufnr_scombo->Content()));
}

void auftrag_bearbeiten::on_zahlzieldatewin_activate()
{   
}

auftrag_bearbeiten::~auftrag_bearbeiten()
{
 delete(aktaufeintrag);
 if(auftrag) delete(auftrag);
 auftrag=NULL;
}

void auftrag_bearbeiten::setAufEntries()
{
 aktAufEintrag *a=aktaufeintrag;
#warning fehlt hier nicht der Artikel?

 stkmtr_spinbutton->set_value(a->getStk());
 kw_spinbutton->set_value(a->getKW().Woche());
 jahr_spinbutton->set_value(a->getKW().Jahr());
 liefdatum_datewin->set_value(a->getLieferdatum());

 WPreis->set_Betrag(a->getPreis().Wert());
 rabattentry_spinbutton->set_value(a->Rabatt()/100.0);
 WPreis->set_Preismenge(a->Preismenge());
 
#warning set_sensitiv gibt es nicht für spinbutons ?
// preismenge->set_sensitiv(false);
 WAufEntryStat->set_history(a->getStatus()); //NEU
}

void auftrag_bearbeiten::on_splitten()
{
 if(splitdialog) splitdialog->destroy();
 splitdialog = manage(new termsplidial());
}

bool auftrag_bearbeiten::splitEntry()
{
 if(!splitdialog || !auftrag) return false;

 Petig::Datum newlief = splitdialog->getLiefDatum();
 int newmenge = splitdialog->getMenge();
 if(!newmenge) {meldung->Show(string("Menge nicht korrekt")); return false;}
 
 try{auftrag->split(selectedentry, newlief, newmenge);}
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
 auftrag=NULL;
 
 kundenbox->reset();
 WAufStat->set_history((AufStatVal)UNCOMMITED);
 aufnr_scombo->reset();
 youraufnr_scombo->reset();
 aufbemerkung_entry->set_text("");
 jahrgang_spinbutton->set_value(Petig::Datum::today().Jahr());
 aufdatum_datewin->set_value(Petig::Datum::today());
 aufrabatt_spinbutton->set_value(0);
 zahlziel_datewin->set_value(Petig::Datum::today());
 zahlart->set_history(0);
 bea_WWaehrung->set_history(0);
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
      auftrag = new AuftragFull(instanz,kundenbox->get_value(),
			jahrgang_spinbutton->get_value_as_int());
//NEU: Auftrag mit Auftrags ID 0 anlegen:
//      auftrag = new AuftragFull(AuftragBase(instanz,0),kundenbox->get_value(),
//			jahrgang_spinbutton->get_value_as_int());
 	auftrag->setBemerkung(aufbemerkung_entry->get_text());
 	auftrag->setYourAufNr(youraufnr_scombo->get_text());
      AuftragBase ab(*auftrag);
      on_clear_all(); // careful this deletes auftrag
 // eigentlich doch nur anzeigen oder? CP
      loadAuftrag(ab);

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
}

void auftrag_bearbeiten::on_aufbemerkung_activate()
{   
 if(newauftrag)
   auftrag_ok->grab_focus();
}


void auftrag_bearbeiten::on_button_preview_clicked()
{  if (!auftrag) return;
   string command = "auftrag_drucken Auftrag "+itos(auftrag->Id())+" Preview " + itos(instanz) ;
   system(command.c_str());
}  

void auftrag_bearbeiten::on_button_drucken_clicked()
{
   if (!auftrag) return;
   string command = "auftrag_drucken Auftrag "+itos(auftrag->Id())+" Plot " + itos(instanz);
   system(command.c_str());
}


void auftrag_bearbeiten::on_activate_wpreis()
{
  if (!aktaufeintrag) return;
  WPreis->update();
  Preis pr(WPreis->get_Betrag(),auftrag->getWaehrung(),WPreis->get_Preismenge());
  bea_WWaehrung->set_History(WPreis->get_Waehrung_enum());
  waehrung_geaendert();
  if(aktaufeintrag->getZln()>0)
      {
       auftrag->updatePreis(selectedentry,pr);
       auftrag_clist-> freeze();
       loadAuftrag(*auftrag);
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry,0,.5,0);
      }
 else
  {
#warning Dieser 'else' Teil war NICHT Bestandteil von 'on_preismenge_activate()'
#warning aber soweit ich das sehe, schadet es auch nicht ihn auszuführen.
#warning da durch diese neue Konstruktion einige Zeilen (und eine Funktion)
#warning wegfallen finde ich sie eleganter MAT
   aktaufeintrag->setPreis(pr);
//   aktaufeintrag->setPreismenge(preismenge->get_value_as_float());
   rabattentry_spinbutton->grab_focus();
   rabattentry_spinbutton->select_region(0,rabattentry_spinbutton->get_text().size());
  }
//  on_preis_spinbutton_activate(); // ALT MIT 'else'  
//  on_preismenge_activate();  // ALT OHNE 'else'
}
