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

#include "config.h"
#include "auftrag_rechnung.hh"
#include<Aux/Ausgabe_neu.h>
#include<Aux/itos.h>
#include <Artikel/Artikelpreis.h>
#include <gtk--/menu.h>
#include <tclistleaf.h>
#include "auftrag_rechnung_classes.h"
#include "MyMessage.h"
#include <Aux/dbconnect.h>
#include "ja_nein_frage.hh"
extern MyMessage *meldung;

#include <Aux/dbcapability.h>
extern DBCapability *dbcapability;
//extern ManuProC::Connection *Conn;
#include <Auftrag/AufEintrag.h>

#include <Lieferschein/RechnungVoll.h>

void auftrag_rechnung::on_rng_close()
{   
   timeout_connection.disconnect();
   destroy();
}

void auftrag_rechnung::on_rng_neu()
{   
 ja_nein_frage jnf("Wollen Sie wirklich jetzt eine neu Rechnung erstellen ?");

 jnf.set_transient_for(*this);

 int ret=jnf.run();

 if(ret==0)
  {
   if(lieferkunde->get_value()!=-1 && lieferkunde->get_value()!=0)
   {
    newRechnung(lieferkunde->get_value());
    rngnr->setContent(Formatiere(rechnung.Id(),0,6,"","",'0'),rechnung.Id());
    rngdatum->set_value(ManuProC::Datum::today());
    on_rngnr_activate();
   }
  else
   lieferkunde->grab_focus();
  }
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
   std::string optionen;
   if(checkbutton_ean_drucken->get_active()) optionen =" --ean ";
   
   std::string command = "auftrag_drucken -a Rechnung -n "
         +rngnr->get_text() 
         +" -i "+itos(instanz->Id()) 
         +optionen ;
   system(command.c_str());
}

#ifdef MABELLA_EXTENSIONS
#define STD_MABELLA(x,y) (y)
#else
#define STD_MABELLA(x,y) (x)
#endif

gint auftrag_rechnung::on_rng_print(GdkEventButton *ev)
{   
   if (rngnr->get_text()=="") return false;
//   std::string com="auftrag_drucken  -a Rechnung -n "+rngnr->get_text()+" -p -i "+itos(instanz->Id()) ;
   std::string optionen, opt_ean;
   if(checkbutton_firmenpapier->get_active()) optionen +=" --firma ";
   if(checkbutton_kopie->get_active())        optionen +=" --kopie " ;
   if(checkbutton_ean_drucken->get_active())  opt_ean +=" --ean ";

   std::string com="auftrag_drucken  -a Rechnung -n "
         +rngnr->get_text()+" -p -i "+itos(instanz->Id())+opt_ean ;

   if (ev->button==STD_MABELLA(1,3)) 
      system((com+optionen).c_str());
   if (ev->button==2)
    {
     system((com+" --kopie").c_str());
#ifndef MABELLA_EXTENSIONS
     system((com+" --firma").c_str());
#endif
    }
   if (ev->button==STD_MABELLA(3,1))
    {
     system((com+" --kopie").c_str());
     system((com+" --kopie").c_str());
     system((com+" --firma").c_str());
    }
 return false;
}

void auftrag_rechnung::rngzeile_delete()
{
 try{   
   if(!(rechnung.Bezahlt()))
	{
    try{
     cH_Data_Rechnung dt(rtree_daten->getSelectedRowDataBase_as<cH_Data_Rechnung>());
     RechnungEntry RE = dt->get_RechnungEntry();
     rechnung.deleteLieferschein(RE.Lfrs());
     } catch(...){}
	 on_rngnr_activate(); // neu anzeigen
   }
 }
 catch(SQLerror &e)
  {meldung->Show(e); return;}

 rngentry_del->set_sensitive(false); 
}

void auftrag_rechnung::on_rngdate_activate()
{   
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
  set_rtree_daten_content(rngnr->Content());
 }
 catch(SQLerror &e) {meldung->Show(e); return; }
 if(rechnung.rngArt()==Rechnung::RART_GUT)
   {
     frame_rechnung->set_label("Gutschrift");
     frame_rechnungsdaten->set_label("Gutschriftsdaten");
     set_title("Gutschrift");
     gutschrift->set_sensitive(false);
   }
 else if(rechnung.rngArt()==Rechnung::RART_RNG)
   {
     frame_rechnung->set_label("Rechnung");
     frame_rechnungsdaten->set_label("Rechnungsdaten");
     set_title("Rechnung");
     gutschrift->set_sensitive(true);     
   }
 else 
   {
std::cout <<rechnung.rngArt()<<'\n';
     assert(!"Never get here\n");
   }
}


void auftrag_rechnung::clear_rng()
{
 rngnr->reset();
 rechnung=Rechnung();
 rabatt_wert->set_value(0);
 rngdatum->set_value(ManuProC::Datum::today());
 zahlziel->set_value(ManuProC::Datum());
 optionmenu_zahlart->set_history(0);
 rtree_daten->clear();
}

void auftrag_rechnung::on_rngnr_activate()
{try{
 redisplay();
 lieferkunde->set_value(rechnung.KdNr());   
 set_rtree_offen_content(lieferkunde->get_value());
 rng_WWaehrung->set_value(rechnung.getWaehrung());

 Rechnung::rabatt_t rabatt=rechnung.Rabatt();
 if (rabatt<0.0) { rabatt=-rabatt; rabatt_typ->set_history(rabatt_typ::Zuschlag); }
 else rabatt_typ->set_history(rabatt_typ::Rabatt);
 rabatt_wert->set_value(rabatt.as_float());
 rngdatum->set_value(rechnung.getDatum());
 zahlziel->set_value(rechnung.getZahlziel());
 optionmenu_zahlart->set_history(rechnung.getZahlungsart()->Id());

 gint pos=0; 
 rng_notiz->delete_text(0,-1);
 rng_notiz_save->set_sensitive(false);
 rng_notiz->insert_text(rechnung.Notiz().c_str(),rechnung.Notiz().size(),&pos);
 rng_notiz_save->set_sensitive(false);

 bezahlt->set_active(rechnung.Bezahlt());

 rtree_daten->show();
// vbox_n_b_lieferscheine->show();
 frame_rechnungsdaten->show(); 
 }
 catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_rechnung::on_lieferkunde_activate()
{
 clear_rng();
 cH_Kunde k(lieferkunde->get_value());
 
#ifndef MABELLA_EXTENSIONS
 if(lieferkunde->get_value() != k->Rngan())
    lieferkunde->set_value(k->Rngan());
#else
   checkbutton_ean_drucken->set_active(k->showEAN());   
#endif

 rng_WWaehrung->set_value(cH_Kunde(lieferkunde->get_value())->getWaehrung());

 try{
  set_rtree_offen_content(lieferkunde->get_value()) ;
 }catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_rechnung::preis_activate()
{   
}

void auftrag_rechnung::lieferschein_uebernehmen()
{   
 try{
 if(rtree_offen->selection().size())
   {if (rechnung.Id()<1) 
     {
      on_rng_neu();
      if(rechnung.Id()<1)
        return;
     }
   if(!(rechnung.Bezahlt()))
	{
    try {
      cH_Data_RLieferoffen dt(rtree_offen->getSelectedRowDataBase_as<cH_Data_RLieferoffen>());

   	cH_Lieferschein chl(dt->get_Lieferschein()); 
      if(rechnung.Rabatt() != (chl->AufRabatt()) && 
         rechnung.Rabatt() != (Rechnung::rabatt_t)0)
           {meldung->Show("Aufträge mit unterschiedlichen Rabatten\n"
           	"(Rechnung "
           	+Formatiere_short(rechnung.Rabatt())+"% Lieferschein/Auftrag "
           	+Formatiere_short(chl->AufRabatt())+"%)\n"
           	"dürfen nicht auf eine Rechnung");
            return;
           }
       else
         rechnung.setze_Rabatt(chl->AufRabatt());

// Versuch unterschiedliche Zahlungsziele zu handeln
#ifdef MABELLA_EXTENSIONS         
   ManuProC::Datum d(chl->getMaxZahlziel());
       if(d.valid())
         {
          if(zahlziel->get_value().valid())
            {if(zahlziel->get_value()<d)
              rechnung.setze_Zahlziel(d);
            }
          else 
            {rechnung.setze_Zahlziel(d);            
             zahlziel->set_value(d);
            }
         }
       else {
         cH_Zahlungsart za(rechnung.getZahlungsart());
         if(za->getBankeinzug() || za->FristMonate())
          if(!zahlziel->get_value().valid())
             {rechnung.setze_Zahlziel(za->getZahlungstermin(rechnung.getDatum()));
              zahlziel->set_value(za->getZahlungstermin(rechnung.getDatum()));
             }

         }
#endif         
             
       rechnung.addLieferschein(*chl);
   	 on_rngnr_activate();
      }
    catch(std::exception &e) {std::cerr << e.what();}
	}
   }
 }
 catch(SQLerror &e)
 {meldung->Show(e);return;}

}

void auftrag_rechnung::on_rdaten_leaf_selected(cH_RowDataBase d)
{
try{
  const Data_Rechnung *dt=dynamic_cast<const Data_Rechnung*>(&*d);
  rngentry_del->set_sensitive(true);

  // Preise
  RechnungEntry RE=dt->get_RechnungEntry();

  cH_Kunde liefknd(lieferkunde->get_value());
  cH_Kunde rngkd(liefknd->Rngan());

  Artikelpreis::UnCache(rngkd->preisliste(),RE.Artikel());
  label_artikelpreis->set_text(Formatiere(Artikelpreis(rngkd->preisliste(),RE.Artikel(),RE.Stueck()).Wert()));
  spinbutton_preiseingabe->set_value(RE.getPreis().Wert().as_float());
  table_preisvergleich->show_all();
  try{
    label_auftragspreis->set_text(Formatiere(RE.getAuftragsPreis().Wert()));
   }catch(AufEintrag::NoAEB_Error &e) 
      // Wenn kein Auftrag zu dieser Rechnungszeile existiert
    {  radiobutton_auftragspreis->hide();
       label_auftragspreis->hide();
       radiobutton_artikelpreis->set_active(true);
    }
//#ifndef MABELLA_EXTENSIONS 
  button_pr->set_sensitive(true);
//#endif

}catch(std::exception &e) {std::cerr<<e.what();}
}

void auftrag_rechnung::on_unselectrow_rtree(int row, int col, GdkEvent* b)
{
 rngentry_del->set_sensitive(false); 
 button_pr->set_sensitive(false);
 table_preisvergleich->hide();
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

 if(!rechnung.Valid())
    lieferkunde->set_value(dt->get_Lieferschein()->getKunde()->Id());

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
 lieferscheindatum->set_value(ManuProC::Datum::today());  
 lief_uebernehmen->set_sensitive(false); 
}        

void auftrag_rechnung::Preis_setzen()
{  
  cH_Kunde liefknd(lieferkunde->get_value());
  cH_Kunde rngkd(liefknd->Rngan());

 try{
  const Data_Rechnung *dt=dynamic_cast<const Data_Rechnung*>(&*(rtree_daten->getSelectedRowDataBase()));
  RechnungEntry RE=dt->get_RechnungEntry();
  if(radiobutton_artikelpreis->get_active())
   {
     RE.setzePreis(Artikelpreis(rngkd->preisliste(),RE.Artikel(),RE.Stueck()));
   }
  else if(radiobutton_auftragspreis->get_active())
   {
     RE.setzePreis(RE.getAuftragsPreis());
   }  
  else if(radiobutton_preiseingabe->get_active())
   {
     spinbutton_preiseingabe->update();
     fixedpoint<2> p=fixedpoint<2>(spinbutton_preiseingabe->get_value_as_float());
     RE.setzePreis(Preis(p,rechnung.getWaehrung(),RE.getPreis().PreisMenge()));
   }  
  redisplay();
 }catch(std::exception &e) {std::cerr << e.what()<<'\n';}
}

void auftrag_rechnung::Preis_ergaenzen()
{
  cH_Kunde liefknd(lieferkunde->get_value());
  cH_Kunde rngkd(liefknd->Rngan());  

   if(!rechnung.Valid()) return;
   RechnungVoll rg=rechnung.Id();
   for (RechnungVoll::iterator i=rg.begin();i!=rg.end();++i)
   {  if (!(i->getPreis()))
      {  Artikelpreis p(rngkd->preisliste(),i->ArtikelID(),i->Stueck());
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
#ifdef MABELLA_EXTENSIONS
// preis_ergaenzen->hide();
 _tooltips.set_tip(*button27,"Linke Maustaste: 1 Orig. 2 Kopien. "
		"Mittlere Maustaste: 1 Kopie","");
  std::string nurliefer(" and lieferadresse=true  and coalesce(aktiv,true)=true");
  lieferkunde->Einschraenkung(nurliefer);
  lieferkunde->Einschraenken(true);      
  checkbutton_ean_drucken->show(); 
  button_lieferscheine_aufraumen->hide();
#endif
   set_tree_titles();
   fill_optionmenu_zahlungsart();
   optionmenu_zahlart->get_menu()->deactivate.connect(SigC::slot(static_cast<class auftrag_rechnung*>(this), &auftrag_rechnung::optionmenu_zahlart_deactivate));
   rngdatum->setLabel("");
   zahlziel->setLabel("Zahlungsziel");
   zahlziel->set_value(ManuProC::Datum());
   lieferscheindatum->setLabel("");
   rtree_daten->hide();
   on_button_allopen_clicked();
   storno->set_sensitive(false);   
   
// set GUI for DBCapabilities
 DBCapability::WhiteColumn wc("rechnung","bezahlt");
 bezahlt->set_sensitive(dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::UPDATE)) );
}

void auftrag_rechnung::set_tree_titles()
{
 std::vector<std::string> t1;
 t1.push_back("Zeile");
 t1.push_back("Lieferschein");
 t1.push_back("Artikel");   
 t1.push_back("Stueck");
 t1.push_back("Liefermenge");
 t1.push_back("E-Preis");
 t1.push_back("G-Preis");
 rtree_daten->setTitles(t1);  

 std::vector<std::string> t2;
 t2.push_back("Kunde");
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
    { 
      rechnungvoll = RechnungVoll(rngid);
      rechnung=Rechnung(rngid); 
    }
  else  rechnung=Rechnung(RechnungBase::none_id);
  }
 catch(SQLerror &e) { meldung->Show(e); return; }

 std::vector<cH_RowDataBase> datavec;
 for( RechnungVoll::const_iterator i=rechnungvoll.begin();i!=rechnungvoll.end();++i)
  {
   datavec.push_back(new Data_Rechnung(*i));
  }
 
 rtree_daten->clear();
 rtree_daten->setDataVec(datavec);
 
}

void auftrag_rechnung::optionmenu_zahlart_deactivate()
{
  int z = int(optionmenu_zahlart->get_menu()->get_active()->get_user_data());
  if (rechnung.Id()!=-1 && rechnung.Id()!=0)
     rechnung.setze_Zahlungsart(cH_Zahlungsart(z));
}


//void auftrag_rechnung::on_button_gutschrift_clicked()
//{
//  label_rechnung_ctl->set_text("Sicher? Diese Entscheidung kann nicht rückgängig gemacht werden.");
//  table_gutschrift_sicherheit->show();
//}

//void auftrag_rechnung::on_button_gutschrift_nein_clicked()
//{
//  label_rechnung_ctl->set_text("");
//  table_gutschrift_sicherheit->hide();
//}

//void auftrag_rechnung::on_button_gutschrift_ja_clicked()
//{
//
//}

void auftrag_rechnung::on_zahlziel_activate()
{
 try {
  if (rechnung.Id()!=-1 && rechnung.Id()!=0)
    rechnung.setze_Zahlziel(zahlziel->get_value());
  }
  
 catch(SQLerror &e) { meldung->Show(e); return; }    
}


void auftrag_rechnung::on_checkbutton_ean_drucken_clicked()
{
 cH_Kunde k(lieferkunde->get_value());
 
 if(k->Id() != Kunde::none_id)   
    {
     (const_cast<Kunde*>(&*k))->
     	showEAN(checkbutton_ean_drucken->get_active());
    }
}



void auftrag_rechnung::on_button_allopen_clicked()
{
  rechnung=Rechnung();
  lieferkunde->reset();
  frame_rechnungsdaten->hide();
  rngnr->reset();
  set_rtree_offen_content(Rechnung::none_id);
}


void auftrag_rechnung::on_button_lieferscheine_aufraumen_clicked()
{
  try{
     Lieferschein::aufraumen();
     on_button_allopen_clicked();
  }catch(SQLerror &e) {meldung->Show(e);}
}



void auftrag_rechnung::on_notiz_changed()
{
  rng_notiz_save->set_sensitive(true);
}

void auftrag_rechnung::on_notiz_save_clicked()
{   
  if(rechnung.Id()!=Rechnung::none_id) 
    rechnung.Notiz(rng_notiz->get_chars(0,rng_notiz->get_length()));
  rng_notiz_save->set_sensitive(false);
}

gint auftrag_rechnung::on_bezahlt_toggled(GdkEventButton *ev)
{ 
 DBCapability::WhiteColumn wc("rechnung","bezahlt");
 if(dbcapability->isWhite(DBCapability::ColAct(wc,DBCapability::UPDATE)) )
   {
    try{
    if(rechnung.Id()!=Rechnung::none_id)
      rechnung.setBezahlt(bezahlt->get_active());
    }
    catch(SQLerror &e){meldung->Show(e);
      bezahlt->set_active(bezahlt->get_active());
      }
   }   
 else
  {meldung->Show("keine Berechtigung");  
   bezahlt->set_active(bezahlt->get_active());
  }

 return false;
}

void auftrag_rechnung::on_gutschrift_activate()
{
 ja_nein_frage d("Wollen Sie wirklich ein Gutschrift erstellen ?");
 
 d.set_transient_for(*this);
 
 gint ret=d.run();
 
 if(ret==0)
   {
    try{rechnung.setRngArt(Rechnung::RART_GUT);}
    catch(SQLerror &e) 
      {meldung->Show(e);
       redisplay();
      }
    redisplay();
   }
}


void auftrag_rechnung::on_storno_activate()
{  
}


