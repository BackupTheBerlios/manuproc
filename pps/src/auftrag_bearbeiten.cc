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
#include <Misc/ctime_assert.h>
#include <Artikel/Einheiten.h>
#include <Artikel/ArtikelStamm.h>
#include <Artikel/Artikelpreis.h>
#include <SearchComboContent.h>
#include <Gtk_OStream.h>
#include <Misc/Ausgabe_neu.h>
#include <Misc/dbconnect.h>
#include <unistd.h>
//#include <Auftrag/ppsInstanzProduziert.h>
#include "auftrag_copy.hh"
#include "auftrag_provision.hh"
#include "ja_nein_frage.hh"
#include "Configuration.h"

#ifndef OLD
#include<Auftrag/selFullAufEntry.h>
#include "MyMessage.h"  
#include "auftragbase.h"
#endif
#include <MyWindow.hh>


extern auftrag_main *auftragmain;
auftrag_bearbeiten *auftragbearbeiten;

extern MyMessage *meldung;

int RC_OffenMenge(cH_ArtikelBezeichnung b, ManuProC::Datum &d);

auftrag_bearbeiten::auftrag_bearbeiten(const cH_ppsInstanz& _instanz,const AufEintragBase *auftragbase)
: instanz(_instanz), auftrag(), aktaufeintrag(), kunde(Kunde::default_id),
	aufentrystat_chid(), aufstat_chid(), selectedentry(-1),
	artikel_preisliste_geaendert(), newauftrag(),
	splitdialog(),
	new_aufid_from_copy(AuftragBase::none_id)
{
 MyWindow::setPositionSize(*this,"pps");
 set_title("Auftragserfassung: "+instanz->get_Name());
 liefdatum_datewin->set_tab_pos(GTK_POS_TOP);
// aufeintrag_box->hide(); // beinhaltet beide oben
 
#ifdef HIDE_PREISAUSWAHL
 table_preislisten->hide();
#endif
#ifdef LIFERDATUM_STANDARD
 liefdatum_datewin->set_page(LIFERDATUM_STANDARD);
#endif

#ifdef MASSENEINGABE_AUS
 masseneingabe1->set_active(false);
#endif

 if(instanz->ExterneBestellung())
   WAufStat->set_history((AufStatVal)UNCOMMITED);
 else
   WAufStat->set_history((AufStatVal)OPEN);

#ifdef MABELLA_EXTENSIONS
 table_vorraetige_menge->show();
 _tooltips.set_tip(*button_drucken,"Linke Maustaste: 1 Original"
"Mittlere Maustaste: 1 Kopie","");

//   std::string nuraktiv(" and coalesce(aktiv,true)=true");
//   kundenbox->Einschraenkung(nuraktiv,true);
   if(instanz->Id()==ppsInstanzID::Kundenauftraege)
     kundenbox->EinschraenkenKdGr(KundengruppeID::Auftragsadresse);
   else
     if(instanz->ExterneBestellung())
       kundenbox->EinschraenkenKdGr(KundengruppeID::Lieferanten);

   kundenbox->setExpandStr1(true);
   kundenbox->setExpandStr2(true);
   checkbutton_ean_drucken->show();
   youraufnr_scombo->set_value_in_list(false,false);
   artikelbox->set_always_fill(true);
#endif 

 zahlart->hide_int(true);
 zahlart->setLabel("","");
 
 zahlziel_datewin->setLabel(std::string(""));
 zahlziel_datewin->set_value(ManuProC::Datum());
 aufdatum_datewin->setLabel("");

 if(auftragbase && auftragbase->editierbar())
   {
    loadAuftrag(*auftragbase);
    if(auftragbase->ZNr())
      {
       auftrag_clist->moveto(auftragbase->ZNr()-1,0,.5,0);
       auftrag_clist->row(auftragbase->ZNr()-1).select();
      }
   }
 auftragbearbeiten=this;  
 
 auftrag_label->set_value(0);
}


void auftrag_bearbeiten::showBestandData()
{
    int offen=0,bestand=0,verfuegbar=0;
    try{
    ArtBestandData(artikelbox->get_value().Id(),offen,bestand,verfuegbar);
    }
    catch(SQLerror &e)
      {if(e.Code()!=100) 
         meldung->Show(e);
       bestellt_label->set_text("--");
       lagerbest_label->set_text("--");
       verfuegbar_label->set_text("--");
       return;
      }

 bestellt_label->set_text(itos(offen));
 lagerbest_label->set_text(itos(bestand));
 verfuegbar_label->set_text(itos(verfuegbar));
}


void auftrag_bearbeiten::onSelArtikel()
{
 if(!auftrag) return;
 if(auftrag->Id()==AuftragBase::none_id) return;

 assert(!aktaufeintrag);

 Einheit e(artikelbox->get_value());
 mengeeinheit->set_text((std::string)e);
 WPreis->set_Einheit(e);
 cH_PreisListe artikel_preisliste;

 try {
    WPreis->reset();
#ifndef HIDE_PREISAUSWAHL
    cH_Kunde kndrng(kunde->Rngan());
 
    if(preisautomatik->get_active())
      {
         Artikelpreis AP(kndrng,artikelbox->get_value(),
			stkmtr_spinbutton->get_value_as_int());
         if (AP.Gefunden())
            {artikel_preisliste=cH_PreisListe(AP.GefundenIn());
             preislisten->set_value(artikel_preisliste->Id());
            }
         else
            artikel_preisliste=PreisListe::none_id;
      }
    else
      artikel_preisliste=cH_PreisListe(preislisten->get_value());

    Artikelpreis ap(artikel_preisliste->Id(),artikelbox->get_value(),
			stkmtr_spinbutton->get_value_as_int());		
    Rabatt_setzen(artikel_preisliste);

    showBestandData();
    lieferant_offen->set_text("(0)");

#else
    // hmmm die Menge beeinflusst den Preis, also vielleicht später nochmal 
    // nachsehen
    Artikelpreis ap(kunde->preisliste(),artikelbox->get_value());
#endif

    Preis p(ap.In(auftrag->getWaehrung(),ap.PreisMenge() ));
    WPreis->set_value(p,ap.MindMenge());
    artikel_preisliste_geaendert=false;
    if (!p.Wert()) // Wenn kein Preis gefunden eingeben
    {  WPreis->grab_focus();
       return;
    }
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
 try {
  artikelbox->set_value(aktaufeintrag->Artikel());
  Einheit e(artikelbox->get_value());
  mengeeinheit->set_text(std::string(e));
  WPreis->set_Einheit(e);
  WPreis->set_Waehrung(auftrag->getWaehrung());
  preislisten->set_value(aktaufeintrag->getPreisliste()->Id());  
 }
 catch(SQLerror &e)
   {meldung->Show(e); return;}
 catch(ArtikelBoxErr &e)
   {meldung->Show(e.ErrMsg()); return;}
 setAufEntries();
 selectedentry=row;
 artikelbox->set_editable(false);

#ifdef MABELLA_EXTENSIONS
 showBestandData();
 lieferant_offen->set_text("(0)");
#endif

 WAufStat->set_History(aktaufeintrag->getAufStatus());
 aufentry_ok->set_sensitive(false);
 aufentry_abbruch->set_sensitive(false);
}

void auftrag_bearbeiten::on_auftrag_clist_unselect_row
				(gint row, gint column, GdkEvent *event)
{   
 aufentry_ok->set_sensitive(true);
 clearEntry();
 cH_Kunde rngkd(kunde->Rngan());
 preislisten->set_value(rngkd->preisliste());
}

void auftrag_bearbeiten::clearEntry()
{
 aktaufeintrag=0;
 artikelbox->reset();
 on_preisautomatik_clicked();
 stkmtr_spinbutton->set_value(1);
 WPreis->reset();
 selectedentry=-1;
 artikelbox->set_editable(true);
 if(instanz->ExterneBestellung())
   WAufStat->set_history((AufStatVal)UNCOMMITED);
 else
   WAufStat->set_history((AufStatVal)OPEN);
 aufentry_ok->set_sensitive(true);
 aufentry_abbruch->set_sensitive(true);
 bestellt_label->set_text("0");
 lagerbest_label->set_text("0");
 verfuegbar_label->set_text("0");
 lieferant_offen->set_text("(0)");
 auftrag_label->set_value(0);
 notiz->delete_text(0,-1);
 notiz_save->set_sensitive(false);
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
{if (splitdialog) { splitdialog->destroy(); splitdialog=0; }
 if(auftrag) { delete(auftrag); auftrag=0; }
 MyWindow::setPositionSize(*auftragmain,"pps");
 destroy();
}

void auftrag_bearbeiten::on_newauftrag_button_clicked()
{Kunde::ID k=kundenbox->get_value();
 on_clear_all();
 auftrag_ok->set_sensitive(true);
 auftrag_abbruch->set_sensitive(true);
 aufnr_scombo->set_sensitive(false);

 if (k!=Kunde::none_id) 
 {  kundenbox->set_value(k);
    andererKunde();
    youraufnr_scombo->grab_focus();
 }
 else
 kundenbox->grab_focus();
 newauftrag=true;
}

void auftrag_bearbeiten::auftragstatus_geaendert()
{ 
 int oldstat=WAufStat->get_Status();
 if (auftrag)
  { try{
     auftrag->setStatusAuftragFull(WAufStat->get_Status());}
     catch(SQLerror &e)
      {meldung->Show(e); 
       WAufStat->set_history((AufStatVal)oldstat);
       return;}    
  }
}

void auftrag_bearbeiten::waehrung_geaendert()
{
try { 
 if (auftrag)
   {
      auftrag->setWaehrung(bea_WWaehrung->get_enum());
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


void auftrag_bearbeiten::on_stkmtr_spinbutton_activate()
{   
 assert(auftrag);

 gtk_spin_button_update(stkmtr_spinbutton->gtkobj());

// hier zuerst den Preis noch mal setzten, da abhängig von der bestellten Menge
    if(preislisten->get_value() != PreisListe::none_id)
      {
       WPreis->reset();
#ifndef HIDE_PREISAUSWAHL
       if(preisautomatik->get_active())
         {
          cH_Kunde kndrng(kunde->Rngan());
          Artikelpreis AP(kndrng,artikelbox->get_value(),
			stkmtr_spinbutton->get_value_as_int());
          if (AP.Gefunden())
             preislisten->set_value(AP.GefundenIn());
	 }

       Artikelpreis ap(preislisten->get_value(),artikelbox->get_value(),
			stkmtr_spinbutton->get_value_as_int());	

       Rabatt_setzen(cH_PreisListe(preislisten->get_value()));
#else
       Artikelpreis ap(kunde->preisliste(),artikelbox->get_value(),
			stkmtr_spinbutton->get_value_as_int());	
#endif

       Preis p(ap.In(auftrag->getWaehrung(),ap.PreisMenge() ));
       WPreis->set_value(p,ap.MindMenge());
       artikel_preisliste_geaendert=false;
       on_activate_wpreis();
      }

 if(aktaufeintrag && ppsInstanzID::Kundenauftraege)
      {
       AuftragBase::mengen_t diffmenge=
       	AuftragBase::mengen_t(stkmtr_spinbutton->get_value_as_int())-
        aktaufeintrag->getStueck();
       			
       AuftragBase::mengen_t mt=aktaufeintrag->MengeAendern(diffmenge);
       assert(mt==diffmenge);
       int sel_row=selectedentry;
       fillCList();
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(sel_row,0,.5,0);
     }
 else
  if(stkmtr_spinbutton->get_value_as_int() > 0)
   { liefdatum_datewin->grab_focus();
   }


}

void auftrag_bearbeiten::on_lieferdatum_activate()
{   
  assert(auftrag);
  if(aktaufeintrag  && ppsInstanzID::Kundenauftraege)
      {
       int sel_row=selectedentry;
       
       Transaction tr;

       if(auftrag_clist->selection().size()==1)
	  {aktaufeintrag->updateLieferdatum(liefdatum_datewin->get_value());
	   std::cout << "Lieferdatum changed\n";
	  }
       else if(!auftrag_clist->selection().empty())
         {
	  std::string txt="Das Lieferdatum in ";
	  txt+=itos(auftrag_clist->selection().size())+
		". Zeile(n) auf "+
		liefdatum_datewin->get_value().write_euro()+
		" setzen."+
		" Wollen Sie fortfahren ?";
          ja_nein_frage jnf(txt);
          jnf.set_transient_for(*this);
          int ret=jnf.run();

          if(ret!=0) 
             return;

	  for(Gtk::CList::SelectionList::iterator s=auftrag_clist->selection().begin();
	      s!=auftrag_clist->selection().end(); ++s)
	    {
	     int znr(atoi((*s)->begin()->get_text().c_str())-1);
	     AufEintrag *aentry=&auftrag->getAufEntry(znr);
	     aentry->updateLieferdatum(liefdatum_datewin->get_value());	     
	    }

	 }
       else return;

       fillCList();
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(sel_row,0,.5,0);
       
       tr.commit();       
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
 try{
 if(!auftrag)
   {meldung->Show("Bitte zuerst einen Auftrag auswählen oder einen neuen Auftrag erzeugen");
    return;
   }
   
 if (!aktaufeintrag)
   {
     stkmtr_spinbutton->update();
     cH_PreisListe artpreis(preislisten->get_value());
     if(artikel_preisliste_geaendert) 
      { artpreis=cH_PreisListe(PreisListe::none_id);
        artikel_preisliste_geaendert=false;
      }

#ifdef MABELLA_EXTENSIONS // auf keinen Fall Preis=0.0 akzeptieren
 if(!(WPreis->get_Preis()!=Preis(0.0,WPreis->get_Preis().getWaehrung())) )
  {
   ArtikelTyp at(artikelbox->get_value());
   if(ArtikelTyp::hasAttribute(at.Id(),ArtikelTypAttr::null_preis_check)
	&& instanz->Id()==ppsInstanzID::Kundenauftraege)
   {meldung->Show("Preis 0,00 ist nicht erlaubt");
    return;
   }
  }
 

  if(instanz->ExterneBestellung())
    {if(  auftrag->existsEntry(artikelbox->get_value().Id(),
  		WAufEntryStat->get_Status(), liefdatum_datewin->get_value()) )
     {meldung->Show("Der Artikel ist in diesem Auftrag bereits erfaßt");
      return;
     }
    }
  else
    {
     if(  auftrag->existsEntry(artikelbox->get_value().Id(),
  		       WAufEntryStat->get_Status()) )
     {meldung->Show("Der Artikel ist in diesem Auftrag bereits erfaßt");
      return;
     }
    }	           
   
#endif
      if(instanz==ppsInstanzID::Kundenauftraege || instanz->ExterneBestellung())
       {
         if(instanz->ExterneBestellung())
            WAufEntryStat->set_history((AufStatVal)UNCOMMITED); 
         else
            WAufEntryStat->set_history((AufStatVal)OPEN); 

         auftrag->push_back(
               stkmtr_spinbutton->get_value_as_int(),
               liefdatum_datewin->get_value(),
               artikelbox->get_value(),
               WAufEntryStat->get_Status(),
               WPreis->get_Preis(),
               rabattentry_spinbutton->get_value_as_float(),
               artpreis);
       }
      else 
      { meldung->Show("In dieser Produktionsebene können keine Aufträge geschrieben werden");
        return;
      }
      fillCList();
      if (!masseneingabe1->get_active())
      {  artikelbox->reset();
         on_preisautomatik_clicked();
         artikelbox->grab_focus();
      }
#ifdef MABELLA_EXTENSIONS
      else
      {  
	cH_ExtBezSchema es(artikelbox->getBezSchema());
	int sigfkz=1;
	if(es->Id()==ExtBezSchema::default_ID)
	  {if(es->Typ()==ArtikelTyp::default_ID)
	    artikelbox->set_focus(sigfkz-1,es->sigsize(sigfkz)-2);
	  else
	    artikelbox->set_focus(sigfkz-1,es->sigsize(sigfkz)-1);
	  }
	else
	 {artikelbox->reset();
          on_preisautomatik_clicked();
          artikelbox->grab_focus();
	 }
      }
#else      
      else // Masseneingabe
      {  // stkmtr_spinbutton->grab_focus();
         artikelbox->grab_focus();
         // stkmtr_spinbutton->select_region(0,-1);
      }
#endif
    }
 }catch(SQLerror &e) {meldung->Show(e);}
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
       int sel_row=selectedentry;
       fillCList();
       auftrag_clist->grab_focus();
       auftrag_clist->moveto(sel_row,0,.5,0);
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
       aktaufeintrag->setStatus(WAufEntryStat->get_Status());
       int sel_row=selectedentry;
       fillCList();
	    auftrag_clist->grab_focus();
       auftrag_clist->moveto(sel_row,0,.5,0);
       loadAuftrag(*auftrag);
       if(instanz->ExterneBestellung())
         WAufStat->set_history((AufStatVal)UNCOMMITED);
       else
         WAufStat->set_history((AufStatVal)OPEN);
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
   meldung->Show(e); auftrag=NULL; 
   aufeintrag_box->hide();
   return;}
 catch (Kalenderwoche::error &e)
 { std::cerr << "KW error\n";
 }

 artikelbox->reset();
 on_preisautomatik_clicked(); 
 fillMask();
 aktaufeintrag = 0;
 stkmtr_spinbutton->set_value(1);
// rabattentry_spinbutton->set_value(0);
 Rabatt_setzen(kunde,auftrag);
 WPreis->reset();
 liefdatum_datewin->set_value(ManuProC::Datum::today());
 if(instanz->ExterneBestellung())
   WAufEntryStat->set_history((AufStatVal)UNCOMMITED);
 else
   WAufEntryStat->set_history((AufStatVal)OPEN);
// table_auftragseintraege->show();
// scrolledwindow_auftraege->show();
 aufeintrag_box->show();
 zahlart->set_value(auftrag->Zahlart()->Id());
}

void auftrag_bearbeiten::fillMask() 
{if (!auftrag) return;

 try {
 kundenbox->set_value(auftrag->getKundennr());
 andererKunde(); // adjust Schema
 fillCList();
 WAufStat->set_history(auftrag->getStatus());
 aufnr_scombo->setContent(auftrag->getAuftragidToStr(),auftrag->Id());
 youraufnr_scombo->setContent(auftrag->getYourAufNr(),auftrag->Id());
 aufbemerkung_entry->set_text(auftrag->getBemerkung());
 aufdatum_datewin->set_value(auftrag->getDatum());
 bea_WWaehrung->set_History( auftrag->getWaehrung()->get_enum() );
 
 gint pos=0; 
 notiz->insert_text(auftrag->Notiz().c_str(),auftrag->Notiz().size(),&pos);
 notiz_save->set_sensitive(false);

 aufrabatt_spinbutton->set_value(auftrag->getAuftragsRabatt().as_float());
 zahlziel_datewin->set_value(auftrag->Zahlziel());

 auftrag_label->set_value(auftrag->Label());
 
 auftrag_clist->thaw();
 }
 catch(std::exception &e) 
   {meldung->Show(e.what());
    return;
   }	            
}

void auftrag_bearbeiten::Rabatt_setzen(const cH_Kunde &kunde,AuftragFull *a)
{
 cH_Kunde rngk(kunde->isInGrp(KundengruppeID::Rechnungsadresse) ? kunde->Id() : kunde->Rngan());

    if(rngk->zeilenrabatt())
     {
      aufrabatt_spinbutton->set_sensitive(true);
      rabattentry_spinbutton->set_sensitive(true);
      rabattentry_spinbutton->set_value(rngk->rabatt().as_float());
      if(auftrag)
        aufrabatt_spinbutton->set_value(auftrag->getAuftragsRabatt().as_float());      
     }          
    else
     {
      aufrabatt_spinbutton->set_sensitive(true);
      if(auftrag)
        aufrabatt_spinbutton->set_value(auftrag->getAuftragsRabatt().as_float());
      else
        aufrabatt_spinbutton->set_value(rngk->rabatt().as_float());
      rabattentry_spinbutton->set_value(0);
     }          
}

void auftrag_bearbeiten::Rabatt_setzen(const cH_PreisListe &liste)
{
 cH_Kunde rngk(kunde->isInGrp(KundengruppeID::Rechnungsadresse) ? kunde->Id() : kunde->Rngan());

 if (!rngk->zeilenrabatt()) return;
 if (liste->festerRabatt())
    rabattentry_spinbutton->set_value(liste->getRabatt().as_float());
 else 
    rabattentry_spinbutton->set_value(rngk->rabatt().as_float());
}

void auftrag_bearbeiten::andererKunde()
{  
   kunde = kundenbox->get_value();
   cH_Kunde rngkd(kunde->Rngan());
 
   aufnr_scombo->reset();
   youraufnr_scombo->reset();
 

#ifdef MABELLA_EXTENSIONS
   Rabatt_setzen(rngkd);
   std::string eins_prlist(" and exists (select prlsnr from ku_warenkorb"
   	" where prlsnr=ku_preisliste.prlsnr and kundennr=");
   eins_prlist+=itos(kunde->Rngan())+")";

   std::string EK_VK;
   if(!instanz->ExterneBestellung())
     EK_VK=" and art='V' ";
   else EK_VK=" and art='E' ";

   eins_prlist+=EK_VK;

   preislisten->Einschraenkung(eins_prlist);
   preislisten->set_value(rngkd->preisliste());
   Rabatt_setzen(cH_PreisListe(rngkd->preisliste()));

   artikelbox->setExtBezSchemaID(rngkd->getSchemaId());

    bea_WWaehrung->set_History(rngkd->getWaehrung()->get_enum());
    WPreis->set_Waehrung(rngkd->getWaehrung()->get_enum() );
#else
   Rabatt_setzen(kunde);
   artikelbox->setExtBezSchemaID(kunde->getSchemaId());

    bea_WWaehrung->set_History(kunde->getWaehrung()->get_enum());
    WPreis->set_Waehrung(kunde->getWaehrung()->get_enum() );
#endif

    preisautomatik->set_active(rngkd->Preisautomatik());
    
#ifndef HIDE_PREISAUSWAHL
   if(preisautomatik->get_active())
     artikelbox->AlleWarenkorb(rngkd->Id());
   else
     artikelbox->EinWarenkorb(rngkd->preisliste());   
   artikelbox->Einschraenken_b(true);
   checkbutton_ean_drucken->set_active(kunde->showEAN());
   newauftrag_button->set_sensitive(kunde->Aktiv());
   if(kunde->Aktiv())
      _tooltips.set_tip(*kundenbox,NULL,"");
   else
     {_tooltips.set_tip(*kundenbox,"Der Kunde ist gesperrt","");
      auftrag_ok->set_sensitive(false);
     }
#endif

   zahlart->set_value(rngkd->zahlungsart()->Id());
}

void auftrag_bearbeiten::preisliste_reset()
{
 artikelbox->reset();
 artikelbox->Einschraenken_b(false);
 artikelbox->grab_focus();
}


void auftrag_bearbeiten::on_aufnrscombo_activate()
{
 clearEntry();
 loadAuftrag(AuftragBase(instanz,aufnr_scombo->Content()));
}

int auftrag_bearbeiten::get_active_index(Gtk::Menu *om)
{
 return (int)(om->get_active()->get_user_data());
}


void auftrag_bearbeiten::on_youraufnrscombo_activate()
{ 
 if(newauftrag)
   aufbemerkung_entry->grab_focus();
 else if(auftrag)
  {
   ja_nein_frage jnf("Sie verändern gerade die Auftragsnummer des Kunde. "
			" Wollen Sie wirklich fortfahren ?");
   jnf.set_transient_for(*this);
   int ret=jnf.run();

   if(ret==0)
     {
      try{auftrag->setYourAufNr(youraufnr_scombo->get_text());}
      catch(SQLerror &e)
      {meldung->Show(e); 
       youraufnr_scombo->setContent(auftrag->getYourAufNr(),auftrag->Id());
       return;}    
     }
   else
     youraufnr_scombo->setContent(auftrag->getYourAufNr(),auftrag->Id());

   return;
  }
 else
  try{
   loadAuftrag(AuftragBase(instanz,youraufnr_scombo->Content()));
   } 
   catch(SQLerror &e) {std::cerr << e<<'\n';}
   catch(SearchComboContent<int>::ContentError &c)
   { meldung->Show("Auftrag mit dieser Nummer nicht vorhanden"); 
    return;}
}



void auftrag_bearbeiten::setAufEntries()
{
 assert(aktaufeintrag);
 stkmtr_spinbutton->set_value(aktaufeintrag->getStueck().as_int());
 liefdatum_datewin->set_value(aktaufeintrag->getLieferdatum());

 WPreis->set_Betrag(aktaufeintrag->EPreis().Wert());
 rabattentry_spinbutton->set_value(aktaufeintrag->Rabatt().as_float());
 WPreis->set_Preismenge(aktaufeintrag->PreisMenge());
 
 WAufEntryStat->set_history(aktaufeintrag->getEntryStatus());
}

void auftrag_bearbeiten::on_splitten()
{
 if(splitdialog) { splitdialog->destroy(); splitdialog=0; }
 if (!auftrag || !aktaufeintrag) return;
 splitdialog = new termsplidial(this,aktaufeintrag->getLieferdatum());
}

bool auftrag_bearbeiten::splitEntry()
{
 if(!splitdialog || !auftrag || !aktaufeintrag) return false;

 ManuProC::Datum newlief = splitdialog->getLiefDatum();
 int newmenge = splitdialog->getMenge();
 if(!newmenge) {meldung->Show(std::string("Menge nicht korrekt")); return false;}
 
 // Malte: welchen Wert muss dispoplanung haben?
 try{aktaufeintrag->split(newmenge, newlief);}
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
 aufeintrag_box->hide();
 aktaufeintrag=0; 
 kundenbox->reset();
 newauftrag_button->set_sensitive(true);
 artikelbox->reset();		// ich denke das sollte hier auch rein
 if(instanz->ExterneBestellung())
   WAufStat->set_history((AufStatVal)UNCOMMITED);
 else
   WAufStat->set_history((AufStatVal)OPEN);
 aufnr_scombo->reset();
 youraufnr_scombo->reset();
 aufbemerkung_entry->set_text("");
 aufdatum_datewin->set_value(ManuProC::Datum::today());
 aufrabatt_spinbutton->set_value(0);
 zahlziel_datewin->set_value(ManuProC::Datum());
 zahlart->reset();
 bea_WWaehrung->set_History(WaehrungID::EUR);
 preislisten->reset();
 notiz->delete_text(0,-1);
 notiz_save->set_sensitive(false);
 kundenbox->grab_focus();
}


void auftrag_bearbeiten::on_auftrag_abbruch_clicked()
{   
 on_clear_all();
 auftrag_ok->set_sensitive(false);
 auftrag_abbruch->set_sensitive(false);
 aufnr_scombo->grab_focus();
 aufnr_scombo->set_sensitive(true);
 newauftrag=false;
}

void auftrag_bearbeiten::on_auftrag_ok_clicked()
{   
 try {
   auftrag = new AuftragFull(Auftrag::Anlegen(instanz->Id()),kundenbox->get_value());
 	auftrag->setBemerkung(aufbemerkung_entry->get_text());
 	auftrag->setYourAufNr(youraufnr_scombo->get_text());
     auftrag->setWaehrung(bea_WWaehrung->get_enum());
     
     auftrag->Zahlart(zahlart->get_value());
     auftrag->setRabatt(aufrabatt_spinbutton->get_value_as_float());

      AuftragBase ab(*auftrag);

      cH_Kunde k(kundenbox->get_value());
      
      on_clear_all(); // careful this deletes auftrag
      loadAuftrag(ab);

// auf default Status setzten
      if(instanz->ExterneBestellung())
        {WAufStat->set_history((AufStatVal)UNCOMMITED);
         WAufEntryStat->set_history((AufStatVal)UNCOMMITED); 
	}
      else
        {WAufStat->set_history((AufStatVal)OPEN);
         WAufEntryStat->set_history((AufStatVal)OPEN); 
	}
      auftrag->setStatusAuftragFull(WAufStat->get_Status()); 

 auftrag_ok->set_sensitive(false);
 auftrag_abbruch->set_sensitive(false);
 artikelbox->grab_focus();
 aufnr_scombo->set_sensitive(true);
 newauftrag=false;
      }
 catch(SQLerror &e)
   {meldung->Show(e);
    auftrag=0;
    aufeintrag_box->hide();
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
   if(instanz->ExterneBestellung()) art="Extern";

   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean ";

   std::string command = "auftrag_drucken -G -a"+art+" -n"+itos(auftrag->Id())+
   		" -i" + itos(instanz->Id())+optionen;
   system(command.c_str());
}  

gint auftrag_bearbeiten::on_button_drucken_clicked(GdkEventButton *ev)
{
   if (!auftrag) return false;

   std::string art="Auftrag";
   if(instanz->ExterneBestellung()) art="Extern";

   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean ";

   if(ev->button==1)
   {
   std::string command = "auftrag_drucken -Y1,0,0 -a"+art+" -n"+itos(auftrag->Id())+
   	" -i" + itos(instanz->Id())+optionen;
   system(command.c_str());
   }

  else if(ev->button==2)
   {
   std::string command = "auftrag_drucken -Y0,0,1 -a"+art+" -n"+itos(auftrag->Id())+
   	" -i" + itos(instanz->Id())+optionen;
   system(command.c_str());
   }
 return false;
}


void auftrag_bearbeiten::on_activate_wpreis()
{
 if(!auftrag)
   {meldung->Show("Bitte zuerst einen Auftrag auswählen oder einen neuen Auftrag erzeugen");
    return;
   }

//  assert(auftrag);

  if (aktaufeintrag)
   {
     Preis pr(WPreis->get_value());

#ifdef MABELLA_EXTENSIONS // auf keinen Fall Preis=0.0 akzeptieren
 if(!(WPreis->get_Preis()!=Preis(0.0,WPreis->get_Preis().getWaehrung())))
   {meldung->Show("Preis 0,00 ist nicht erlaubt");
    return;
   }
#endif

     aktaufeintrag->updatePreis(pr);
     int sel_row=selectedentry;
     fillCList();
     auftrag_clist->grab_focus();
     auftrag_clist->moveto(sel_row,0,.5,0);
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
  Preis::geldbetrag_out psum;
  int znr=1;
  for(AuftragFull::const_iterator i = auftrag->begin();i!=auftrag->end();++i)
   {
     os << znr++ <<'\t'
     	<< i->getStueck()<<'\t'
        << cH_ArtikelBezeichnung(i->Artikel())->Bezeichnung()<<'\t'
        << '\t'
        << i->getRestStk()<<'\t'
        << Formatiere(i->EPreis().Wert())<<'\t'
        << Formatiere(i->Rabatt()) <<'\t'
        << Formatiere(i->GPreis().Wert())<<'\t'
        << i->getLieferdatum()<<'\t'
        << i->getEntryStatusStr()<<"\t"
        << i->LastEditDate()<<"\n";
     if(i->getEntryStatus()!=STORNO)
       psum += i->GPreis().Wert();
//cout << i->GPreis() << ":" << psum << "\n";     
   } 

  auftragswert->set_text(Formatiere(psum));

  for(guint i=0; i<auftrag_clist->columns().size();++i)
    auftrag_clist->set_column_auto_resize(i,true);
  auftrag_clist->thaw();
}

void auftrag_bearbeiten::on_auftrag_preislisten_activate()
{
#ifndef HIDE_PREISAUSWAHL
 cH_PreisListe p(preislisten->get_value());
 artikelbox->reset();
 if(preisautomatik->get_active())
     artikelbox->AlleWarenkorb(kunde->Rngan());
 else
     artikelbox->EinWarenkorb(p->Id());   
 artikelbox->Einschraenken_b(p->Id()!=PreisListe::none_id);
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
 std::string typ;
 if(instanz->ExterneBestellung()) typ="Extern";
 else typ="Auftrag";

 std::string command = std::string("auftrag_drucken -Y0,0,1 -a")+typ+
	" -R -n"+itos(auftrag->Id())+
   	" -i" + itos(instanz->Id());
   system(command.c_str());  
}

void auftrag_bearbeiten::on_notiz_changed()
{
  notiz_save->set_sensitive(true);
}

void auftrag_bearbeiten::on_notiz_save_clicked()
{   
  if(auftrag) auftrag->Notiz(notiz->get_chars(0,notiz->get_length()));
  notiz_save->set_sensitive(false);
}

void auftrag_bearbeiten::on_preisautomatik_clicked()
{
#ifndef HIDE_PREISAUSWAHL
 if(preisautomatik->get_active())
     {artikelbox->AlleWarenkorb(kunde->Rngan());
      preislisten->set_sensitive(false);
      artikelbox->Einschraenken_b(true);   
     }
 else
     {artikelbox->EinWarenkorb(preislisten->get_value());
      preislisten->set_sensitive(true);
      artikelbox->Einschraenken_b(preislisten->get_value()!=PreisListe::none_id);
     }

#endif    
}

void auftrag_bearbeiten::on_checkbutton_ean_drucken_clicked()
{
 if(kunde->Id() != Kunde::none_id)
   {  
    (const_cast<Kunde*>(&*kunde))->showEAN(checkbutton_ean_drucken->get_active());
   }
}


void auftrag_bearbeiten::on_auftrag_kopieren_activate()
{  
 if(!auftrag) return;
 
 auftrag_copy ac(auftrag);
 
 ac.set_transient_for(*this);
 
 gint ret;
 ret=ac.run();

 if(ret==0)
   if(new_aufid_from_copy!=AuftragBase::none_id)
     loadAuftrag(AuftragBase(instanz->Id(),new_aufid_from_copy));     
 
}

void auftrag_bearbeiten::on_provisionierung_activate()
{
 if(!auftrag) return;
 
 auftrag_provision ap(auftrag);
 
 ap.set_transient_for(*this);
 
 gint ret;
 ret=ap.run();
}

void auftrag_bearbeiten::on_aufrabatt_spinbutton_activate()
{  
 aufnr_scombo->grab_focus();  
}



void auftrag_bearbeiten::on_offen_bei_lieferant_clicked()
{  
 ManuProC::Datum d;

 if(artikelbox->get_value().Id() == ArtikelBase::none_id) return;

 int offen=
	RC_OffenMenge(cH_ArtikelBezeichnung(artikelbox->get_value()),d);
 if(offen==-1)
   lieferant_offen->set_text("--");
 else
   {
   std::string kwstr;
   if(d.valid() && offen)
      {
      Kalenderwoche kw=d.KW();
      kwstr=" "+itos(kw.Woche())+"'"+itos(kw.Jahr()%100);
      }
   lieferant_offen->set_text("("+itos(offen)+")"+kwstr);
   }

}

void auftrag_bearbeiten::auftraglabel_geaendert()
{
 if(!auftrag) return;   
 
 try{
 auftrag->Label(auftrag_label->get_value());}
 catch(SQLerror &e)
  {meldung->Show(e); return;}  
  
}

