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

typedef map<int ,AufEintrag *> X;
extern X aufentrymap;

extern SelectedFullAufList *allaufids;
extern auftrag_main *auftragmain;
extern auftrag_bearbeiten *auftragbearbeiten;

auftrag_bearbeiten::auftrag_bearbeiten(int aufid, int znr)
: kunde(Kunde::default_id)
{
// perhaps compilation time asserts
 assert(aufstat::unbest_tigt==UNCOMMITED);
 assert(aufstat::offen==OPEN);
 assert(aufstat::fertig==CLOSED);
 assert(aufentrystat::unbest_tigt==UNCOMMITED);
 assert(aufentrystat::offen==OPEN);
 assert(aufentrystat::fertig==CLOSED);
// -------

 splitdialog=0;

 liefertermin->set_page(0);
 zahlziel_datewin->setLabel(string("Zahlungsziel"));
 aufdatum_datewin->setLabel(string("Auftragsdatum"));

 newauftrag=false;
 
 aktaufeintrag = new aktAufEintrag();
 selectedentry=0;
 auftrag=NULL;
 if(aufid)
   {
    loadAuftrag(aufid);
    if(znr)
      {
       auftrag_clist->moveto(znr-1,0,.5,0);
       auftrag_clist->row(znr-1).select();
      }
   }

 artikelbox->activate.connect(SigC::slot(this,&auftrag_bearbeiten::onSelArtikel));
}

void auftrag_bearbeiten::onSelArtikel()
{
 aktaufeintrag->setArtikel(artikelbox->get_value());
 preis_spinbutton->grab_focus();
 preis_spinbutton->select_region(0,preis_spinbutton->get_text().size());
}

void auftrag_bearbeiten::on_auftrag_clist_select_row
				(gint row, gint column, GdkEvent *event)
{   
 if(!auftrag->existsAufEntry(row)) return;
 AufEintrag &aufe=auftrag->getAufEntry(row);
 try{artikelbox->set_value(ArtikelBase(aufe.ArtikelID()));}
 catch(SQLerror &e)
   {meldung->Show(e); return;}
 catch(ArtikelBoxErr &e)
   {meldung->Show(e.ErrMsg()); return;}
 aktaufeintrag->fill(aufe);
 setAufEntries();
 selectedentry=row;
 artikelbox->set_editable(false);

 int aid=aufe.getAuftragid();
 int znr=aufe.getZnr();
 X::iterator m=aufentrymap.find(MAPKEY(aid,znr));
 if(m!=aufentrymap.end())
   aufentrymap.erase(m);
 aufstat->set_history(aufe.getAufStatus()); 
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
 preis_spinbutton->set_value(0);
 kw_spinbutton->set_value(1);
 jahr_spinbutton->set_value(Petig::Datum::today().Jahr());
 liefdatum_datewin->set_value(Petig::Datum::today());
 selectedentry=0;
 artikelbox->set_editable(true);
 aufentrystat->set_history((AufStatVal)UNCOMMITED); 
 aufentrystat->set_sensitive(true);
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

void auftrag_bearbeiten::on_aufstatoptionmenu_clicked()
{   
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

 if(aktaufeintrag->getZln()>0)
      {auftrag->updateStk(selectedentry,stkmtr_spinbutton->get_value_as_int());
       auftrag_clist->freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
      }
 else
  if(aktaufeintrag->setStk(stkmtr_spinbutton->get_value_as_int()))
   {kw_spinbutton->grab_focus();
    kw_spinbutton->select_region(0,kw_spinbutton->get_text().size());
   }
}

void auftrag_bearbeiten::on_lieferdatum_activate()
{   
 if(aktaufeintrag->getZln()>0)
      {auftrag->updateLieferdatum(selectedentry,
			liefdatum_datewin->get_value());
       auftrag_clist->freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
      }
 else
  if(aktaufeintrag->setLieferdatum(liefdatum_datewin->get_value()))
     aufentry_ok->grab_focus();
}


void auftrag_bearbeiten::on_kw_spinbutton_activate()
{
 gtk_spin_button_update(kw_spinbutton->gtkobj());

 if(aktaufeintrag->getZln()>0)
      {auftrag->updateLieferdatum(selectedentry,
		Petig::Datum(Kalenderwoche(kw_spinbutton->get_value_as_int(),
			      jahr_spinbutton->get_value_as_int())) );
       auftrag_clist->freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
      }
 jahr_spinbutton->grab_focus();
 jahr_spinbutton->select_region(0,jahr_spinbutton->get_text().size());
}

void auftrag_bearbeiten::on_jahr_spinbutton_activate()
{
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
{
 if(aktaufeintrag->allesOK())
   if(auftrag)
     {
      int znr=auftrag->insertNewEntry(*aktaufeintrag,artikelbox->getBezSchema());
      allaufids->insert(auftrag->getAuftragid(),znr);
      aktaufeintrag->clear();
      fillMask();
      setAufEntries();
      artikelbox->reset();
       artikelbox->grab_focus();
     }
}



void auftrag_bearbeiten::on_showkal_button_clicked()
{   
}

void auftrag_bearbeiten::on_rabattentry_spinbutton_activate()
{ 
 gtk_spin_button_update(rabattentry_spinbutton->gtkobj());

 if(aktaufeintrag->getZln()>0)
      {auftrag->updateRabatt(selectedentry,
		(int)((rabattentry_spinbutton->get_value_as_float()+.0005)*100));
       auftrag_clist-> freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
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
#warning muesste hier nicht eigentlich nur selectedentry verwendet werden?
 if(aktaufeintrag->getZln()>0 && selectedentry)
      {auftrag->setStatusEntry(selectedentry,
			(AufStatVal)get_active_index(aufentrystat->get_menu()));
       auftrag_clist->freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
      }
}

void auftrag_bearbeiten::on_preismenge_activate()
{   
 if(aktaufeintrag->getZln()>0)
      {gtk_spin_button_update(preismenge->gtkobj());
//       auftrag->updatePreismenge(selectedentry,preismenge->get_value_as_int());
// was ist denn das alles? CP			
       auftrag_clist->freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
	auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
      }
}

void auftrag_bearbeiten::on_preis_spinbutton_activate()
{
 gtk_spin_button_update(preis_spinbutton->gtkobj());
 Preis pr(preis_spinbutton->get_value_as_float(),auftrag->getWaehrung());

 if(aktaufeintrag->getZln()>0)
      {
       auftrag->updatePreis(selectedentry,pr);
       auftrag_clist-> freeze();
       loadAuftrag(auftrag->getAuftragid());
       auftrag_clist->thaw();
       artikelbox->reset();
//       aufentrystat->set_sensitive(false);
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(selectedentry-1,0,.5,0);
      }
 else
  {
   aktaufeintrag->setPreis(pr);
//   aktaufeintrag->setPreismenge(preismenge->get_value_as_float());
   rabattentry_spinbutton->grab_focus();
   rabattentry_spinbutton->select_region(0,rabattentry_spinbutton->get_text().size());      
  }
}

void auftrag_bearbeiten::loadAuftrag(int aid)
{
// int aufid = aid ? aid : atoi(aufnrentry.get_text().c_str());
 
 try { if(auftrag) delete(auftrag); auftrag = new Auftrag(aid);}
 catch(SQLerror &e)
  {
   meldung->Show(e); auftrag=NULL; return;}
 catch (Kalenderwoche::error &e)
 { cerr << "KW error\n";
 }
 fillMask();
 aktaufeintrag->clear();
 stkmtr_spinbutton->set_value(0);
 kw_spinbutton->set_value(0);
 jahr_spinbutton->set_value(0);
 rabattentry_spinbutton->set_value(0);
 preis_spinbutton->set_value(0);
 artikelbox->reset();
 liefdatum_datewin->set_value(Petig::Datum::today());
 aufentrystat->set_history((AufStatVal)UNCOMMITED);
}

void auftrag_bearbeiten::fillMask() 
{
 kundenbox->set_value(auftrag->getKundennr());
 andererKunde(); // adjust Schema
 auftrag_clist->freeze();
 auftrag_clist->clear();
 auftrag->fillCList(*auftrag_clist);
 aufstat->set_history(auftrag->getStatus());
 aufnr_scombo->set_text(auftrag->getAuftragidToStr());
 youraufnr_scombo->set_text(auftrag->getYourAufNr());
 aufbemerkung_entry->set_text(auftrag->getBemerkung());
 jahrgang_spinbutton->set_value(auftrag->getJahrgang());
 aufdatum_datewin->set_value(auftrag->getDatum());
 auftrag_clist->thaw();
}


void auftrag_bearbeiten::andererKunde()
{  kunde = kundenbox->get_value();

   cH_ExtBezSchema ebsh(kunde->getSchema(ArtikelTyp::AufgemachtesBand));

// try a meaningful Type - because of ArtikelBox's restriction
   if (!ebsh->size())
     ebsh=cH_ExtBezSchema(kunde->getSchema(ArtikelTyp::GewebtesBand));

   artikelbox->setExtBezSchema(ebsh);
}

void auftrag_bearbeiten::on_aufnrscombo_activate()
{
 loadAuftrag(aufnr_scombo->Content());
 
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
   loadAuftrag(youraufnr_scombo->Content());
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

 stkmtr_spinbutton->set_value(a->getStk());
 kw_spinbutton->set_value(a->getKW().Woche());
 jahr_spinbutton->set_value(a->getKW().Jahr());
 liefdatum_datewin->set_value(a->getLieferdatum());
 preis_spinbutton->set_value(a->getPreis().Wert());
 rabattentry_spinbutton->set_value(a->Rabatt()/100.0);
 preismenge->set_value(a->Preismenge());
#warning set_sensitiv gibt es nicht für spinbutons ?
// preismenge->set_sensitiv(false);
 aufentrystat->set_history(a->getStatus());
}

void auftrag_bearbeiten::on_splitten()
{
 if(splitdialog) splitdialog->destroy();
 splitdialog = manage(new termsplidial());
}

bool auftrag_bearbeiten::splitEntry()
{
 if(!splitdialog) return false;

 Petig::Datum newlief = splitdialog->getLiefDatum();
 int newmenge = splitdialog->getMenge();
 if(!newmenge) {meldung->Show(string("Menge nicht korrekt")); return false;}
 
 try{auftrag->split(selectedentry, newlief, newmenge);}
 catch(SQLerror &e)
  {meldung->Show(e); return false;}    
 loadAuftrag(auftrag->getAuftragid());
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
 aufstat->set_history((AufStatVal)UNCOMMITED);
 aufnr_scombo->reset();
 youraufnr_scombo->reset();
 aufbemerkung_entry->set_text("");
 jahrgang_spinbutton->set_value(Petig::Datum::today().Jahr());
 aufdatum_datewin->set_value(Petig::Datum::today());
 aufrabatt_spinbutton->set_value(0);
 zahlziel_datewin->set_value(Petig::Datum::today());
 zahlart->set_history(0);
 waehrung->set_history(0);   
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
      auftrag = new Auftrag(kundenbox->get_value(),
			jahrgang_spinbutton->get_value_as_int());
     }

 catch(SQLerror &e)
 {
  meldung->Show(e);
  auftrag=NULL;
  return;
 }

 int aid;
 try {
	aid=auftrag->getAuftragid();
 	auftrag->setBemerkung(aufbemerkung_entry->get_text());
 	auftrag->setYourAufNr(youraufnr_scombo->get_text());
      }
 catch(SQLerror &e)
   {meldung->Show(e);
    auftrag=NULL;
    return;
   }

 on_clear_all();
 loadAuftrag(aid);

 auftrag_ok->set_sensitive(false);
 auftrag_abbruch->set_sensitive(false);
 artikelbox->grab_focus();
 aufnr_scombo->set_sensitive(true);
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
