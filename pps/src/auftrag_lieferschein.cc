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

// generated 2001/1/26 10:08:28 CET by jacek@mimi.(none)
// using glademm V0.5_11a
//
// newer (non customized) versions of this file go to auftrag_lieferschein.cc_glade

// This file is for your program, I won't touch it again!

#include "auftrag_lieferschein.hh"
#include"MyMessage.h"
#include<Kunde/Kunde.h>
//#include"OffAuf_RowData.h"
//#include"OffAuf_Leaf.h"
#include<Aux/Ausgabe_neu.h>
#include<Lieferschein/LieferscheinEntry.h>
#include <Auftrag/selFullAufEntry.h>
#include <Artikel/Einheiten.h>
#include<typeinfo>
#include <tclistleaf.h>
#include "lieferscheinliste.hh"
#include "auftrag_lieferschein_classes.h"

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
 lieferschein = new Lieferschein(cH_Kunde(liefer_kunde->get_value()));
 set_tree_daten_content(lieferschein->Id());
 
// lieferschein_liste->newLieferschein(liefer_kunde->get_value());
//cout << "liefID" << lieferschein_liste->getLieferschein().Id() << "\n";
 liefernr->set_text(
//	Formatiere(lieferschein_liste->getLieferschein().Id(),0,6,"","",'0'));
 Formatiere(lieferschein->Id(),0,6,"","",'0'));
 on_liefnr_activate();
}

void auftrag_lieferschein::on_lief_save()
{   
}

void auftrag_lieferschein::on_lief_preview()
{  
   if (liefernr->get_text()=="") return;
   string command = "auftrag_drucken Lieferschein "+liefernr->get_text()+" Preview "+ itos(instanz->Id());
   system(command.c_str());
}

void auftrag_lieferschein::on_lief_print()
{  
   if (liefernr->get_text()=="") return;
   string command = "auftrag_drucken Lieferschein "+liefernr->get_text()+" Plot "+ itos(instanz->Id());
   system(command.c_str()); 
}

void auftrag_lieferschein::display(int lfrsid)
{
 set_tree_daten_content(lfrsid);
 display2(lieferschein->KdNr());
// display2(lieferschein_liste->getLieferschein().KdNr());
 int rng = lieferschein->RngNr();
 if (rng)
    rngnr->set_text(Formatiere(rng,0,6,"","",'0'));
 else rngnr->set_text("");
 vbox_eingabe->show();
 liefdate->set_value(lieferschein->getDatum());
 
/*
 lieferschein_liste->clear();
 lieferschein_liste->showLieferschein(lfrsid);

 display2(lieferschein_liste->getLieferschein().KdNr());
 if (lieferschein_liste->getLieferschein().RngNr())
    rngnr->set_text(Formatiere(lieferschein_liste->getLieferschein().RngNr(),0,6,"","",'0'));
 else rngnr->set_text("");
 lieferschein_liste->show();
 vbox_eingabe->show();
 liefdate->set_value(lieferschein_liste->getLieferschein().getDatum());
*/
}

void auftrag_lieferschein::display2(int kdnr)
{
try{
 liefer_kunde->set_value(kdnr);
// offene_auftraege->setKdNr(kdnr);
 display2();
   cH_Kunde k(kdnr);
   if (artikelbox->getBezSchema()->Id()!=k->getSchema()->Id())
   { artikelbox->setExtBezSchema(k->getSchema(ArtikelTyp::AufgemachtesBand)); }
 }
 catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_lieferschein::on_liefnr_activate()
{
 lieferschein = new Lieferschein(atoi(liefernr->get_text().c_str()));
 display(atoi(liefernr->get_text().c_str()));
}

void auftrag_lieferschein::on_lieferkunde_activate()
{
try{display2(liefer_kunde->get_value());
// lieferschein_liste->setLieferschein(LieferscheinBase::none_id);
 lieferschein = new Lieferschein(LieferscheinBase::none_id);
 liefernr->reset();
// lieferschein_liste->clear();
 } catch(SQLerror &e) {meldung->Show(e);}
}

void auftrag_lieferschein::on_liefdate_activate()
{ 
try{  
//  lieferschein_liste->getLieferschein().setDatum(liefdate->get_value());
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
 if(dt->Offen()<=0) return;

 Einheit e(dt->get_Artikel_Id());
 if (e==EINH_STUECK) 
 {  anzahl->set_value(dt->Offen());
    liefermenge->set_value(0.0);
 }
 else 
 {  liefermenge->set_value(dt->Offen());
    anzahl->set_value(1);
 }
 artikelbox->set_value(dt->get_Artikel_Id());
 auftragnr->set_text(Formatiere(dt->get_Auftrag_Id()));
// auftragentry = dt->AuftragEntry();
// artikel = dt->get_Artikel_Id();
}


/*
void auftrag_lieferschein::on_selectrow_offauf(int row, int col, GdkEvent* b)
{   
 TCListRow_API *tclapi=(TCListRow_API*)(offene_auftraege->get_row_data(row));
 selectedrow=(TCListRowData*)(*tclapi).get_user_data();

 if(!selectedrow->Leaf()) return;

// es ging nicht mit dynamic_cast auf OffAuf_Leaf ??!!
 cH_OffAuf_RowData entry((dynamic_cast<TCListLeaf*>(selectedrow))->LeafData());

 if(entry->Offen()<=0) return;

 Einheit e(entry->Artikel());
 if (e==EINH_STUECK) 
 {  anzahl->set_value(entry->Offen());
    liefermenge->set_value(0.0);
 }
 else 
 {  liefermenge->set_value(entry->Offen());
    anzahl->set_value(1);
 }
 artikelbox->set_value(entry->ArtikelID());
 auftragnr->set_text(Formatiere(entry->AufId()));
 auftragentry = entry->AuftragEntry();
 artikel = entry->Artikel();
}
*/

void auftrag_lieferschein::on_unselectrow_offauf(int row, int col, GdkEvent* b)
{
 clear_offauf();   
}

auftrag_lieferschein::auftrag_lieferschein(cH_ppsInstanz _instanz)
:instanz(_instanz) 
{
// selectedrow=NULL;
 set_tree_titles();
 liefernr->set_value_in_list(false,false);
 liefernr->set_always_fill(false);

// lieferschein_liste->hide();
 tree_daten->hide();
 vbox_eingabe->hide();
}

void auftrag_lieferschein::set_tree_titles()
{
 vector<std::string> t1;
 t1.push_back("Lief.Zeile");
 t1.push_back("Artikel");
 t1.push_back("Auftrag");
 t1.push_back("Liefermenge");
 tree_daten->setTitles(t1); 

 vector<std::string> t2;
 t2.push_back("Auftrag");
 t2.push_back("Artikel");
 t2.push_back("Lieferdatum");
 t2.push_back("offen");
 t2.push_back("geliefert");
 tree_offen->setTitles(t2); 
}

void auftrag_lieferschein::set_tree_daten_content(LieferscheinBase::ID lfrsid)
{
 tree_daten->clear();
 cH_LieferscheinVoll LV=cH_LieferscheinVoll(LieferscheinBase::none_id);
 if(lfrsid!=LieferscheinBase::none_id)
   {try{ LV=cH_LieferscheinVoll(lfrsid); }
    catch(SQLerror &e)
   { meldung->Show(e); return; }
   }
// ExtBezSchema::ID ebzid=LV.getSchema();
 vector<cH_RowDataBase> datavec;

 for(std::vector<LieferscheinEntry>::const_iterator i=LV->LsEntries().begin();i!=LV->LsEntries().end();++i)
   datavec.push_back(new Data_Lieferdaten(*i));

 tree_daten->setDataVec(datavec);
}


/* sihe sql File
void auftrag_lieferschein::set_tree_eingabe_content()
{
}
*/

void auftrag_lieferschein::clear_offauf()
{
 liefermenge->set_value(0);
 anzahl->set_value(0);
 artikelbox->reset();
 auftragnr->set_text("");
// auftragentry = AufEintragBase2();
// artikel = ArtikelBase();
}

void auftrag_lieferschein::on_Palette_activate()
{ gtk_spin_button_update(anzahl->gtkobj());
  gtk_spin_button_update(liefermenge->gtkobj());
  gtk_spin_button_update(Palette->gtkobj());
  
  if (!tree_offen->selection().size())
  {  // offenene Aufträge zum Artikel suchen, nach Faelligkeit sortiert
     // Menge verteilen
  
     if (!auftragnr->get_text().size() && artikelbox->get_value().Id())
     {  SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Kunde_Artikel
     		(instanz->Id(),liefer_kunde->get_value(),artikelbox->get_value().Id()));
     	SelectedFullAufList list(psel,cH_ExtBezSchema(1,ExtBezSchema::default_Typ));
     	Transaction tr;
     	int _anzahl=anzahl->get_value_as_int();
     	float menge(_anzahl);
     	Einheit e(artikelbox->get_value());
     	// war e!=EINH_STUECK
     	float mengeneinheit(liefermenge->get_value_as_float());
     	if (mengeneinheit==0) mengeneinheit=1;
     	else menge*=mengeneinheit;
     	
       try{
        if (list.aufidliste.begin()==list.aufidliste.end())
        // kann nicht abschreiben
        {  LieferscheinEntry le(*lieferschein,
     	                        artikelbox->get_value(),
     	                        _anzahl,
     	                        e==EINH_STUECK?0.0:mengeneinheit,
     	                        Palette->get_value_as_int(),false);
        }
        else if (menge<=list.aufidliste.begin()->getRestStk())
        // kann in einem Stueck abschreiben
        {  vector<AufEintragBase>::iterator i=list.aufidliste.begin();
           LieferscheinEntry le(*lieferschein,
     	                        *i,artikelbox->get_value(),
     	                        _anzahl,
     	                        e==EINH_STUECK?0.0:mengeneinheit,
     	                        Palette->get_value_as_int(),false);
//     	   OffAuf_RowData::abschreiben(*i,menge);
     	   Data_Lieferoffen::abschreiben(*i,menge);
        }
        else
        // stueckeln (1*Lieferung, dann Zuordnung)
        {  LieferscheinEntry le(*lieferschein,
     	                        artikelbox->get_value(),
     	                        _anzahl,
     	                        e==EINH_STUECK?0.0:mengeneinheit,
     	                        Palette->get_value_as_int(),false);
           for (vector<AufEintragBase>::iterator i=list.aufidliste.begin();
        	        menge && i!=list.aufidliste.end(); ++i)
           {  float abmenge(menge);
              if (i->getRestStk()<abmenge) abmenge=i->getRestStk();
              LieferscheinEntry le(*lieferschein,
     	                        *i,artikelbox->get_value(),
     	                        e==EINH_STUECK?abmenge:1,
     	                        e==EINH_STUECK?0.0:abmenge,
     	                        0,true);
//              OffAuf_RowData::abschreiben(*i,abmenge);
              Data_Lieferoffen::abschreiben(*i,abmenge);
              menge-=abmenge;
       	   }
           if (menge)
           {  LieferscheinEntry le(*lieferschein,
     	                        artikelbox->get_value(),
     	                        e==EINH_STUECK?menge:1,
     	                        e==EINH_STUECK?0.0:menge,
     	                        0,true);
           }
     	}
     	tr.commit();

	clear_offauf();
//	lieferschein_liste->clear();
//	lieferschein_liste->showLieferschein(lieferschein_liste->getLieferschein().Id());
   set_tree_daten_content(lieferschein->Id());
	anzahl->select_region(0,liefermenge->get_text().size());
	anzahl->grab_focus();
       }
       catch (SQLerror &e)
        {  meldung->Show(e); 
           tr.rollback();
           return;
        }
       catch (const char *e)
        {  meldung->Show(SQLerror(e));
           tr.rollback();
           return;
        }
     }
  }
  else
  {
   try {
    cH_Data_Lieferoffen dt(tree_daten->getSelectedRowDataBase_as<cH_Data_Lieferoffen>());
    ArtikelBase artikel = dt->get_Artikel_Id();
    if(artikel.Id() == 0) return;

    AufEintragBase2 auftragentry=dt->AuftragEntry();
    Transaction tr;
    LieferscheinEntry le = LieferscheinEntry(*lieferschein,
		auftragentry, artikel, anzahl->get_value_as_int(), 
		liefermenge->get_value_as_float(),Palette->get_value_as_int());

//    cH_OffAuf_RowData entry((dynamic_cast<TCListLeaf*>(selectedrow))->LeafData()); 
    Einheit e(artikel);
    if (e!=EINH_STUECK)
//     (const_cast<OffAuf_RowData*>(&*entry))->abschreiben(anzahl->get_value_as_int()*liefermenge->get_value_as_float());
       (const_cast<Data_Lieferoffen*>(&*dt))->abschreiben(anzahl->get_value_as_int()*liefermenge->get_value_as_float());
    else    
//     (const_cast<OffAuf_RowData*>(&*entry))->abschreiben(anzahl->get_value_as_int());
     (const_cast<Data_Lieferoffen*>(&*dt))->abschreiben(anzahl->get_value_as_int());

    tr.commit();
 
    clear_offauf();

// lieferschein_liste->clear();
// lieferschein_liste->showLieferschein(le.Id());
   set_tree_daten_content(le.Id());

// offene_auftraege->clear();
// offene_auftraege->showOffAuf();
    set_tree_offen_content();
  }
 catch(SQLerror &e) 
	{ if(e.Code()==-400) liefernr->grab_focus(); //noch kein Liefrschein vorhanden 
	  else meldung->Show(e); 
	  return; 
	}
 catch(...){}
  }
}

void auftrag_lieferschein::liefzeile_delete()
{
 try {
 if(rngnr->get_text()=="")
   if(lieferschein->Id()!=LieferscheinBase::none_id)
	if(deleteLiefEntry())
	  { display(); display2();
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
     LieferscheinVoll(LE.Id()).deleteRow(LE);
     return true;
    }
  } catch(...){}
 return false;
}

void auftrag_lieferschein::display()
{//  lieferschein_liste->clear();
 //  lieferschein_liste->showLieferschein(lieferschein_liste->getLieferschein().Id());
 set_tree_daten_content(lieferschein->Id());
}

void auftrag_lieferschein::display2()
{  clear_offauf();
//   offene_auftraege->clear();
//   offene_auftraege->showOffAuf();
  set_tree_offen_content();
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
 if (e!=Einheit(EINH_STUECK)) 
 { liefermenge->grab_focus();
   liefermenge->select_region(0,liefermenge->get_text().size());
 }
 else 
 { auftragnr->grab_focus();
 }
}

void auftrag_lieferschein::on_button_liste_clicked()
{
  manage(new lieferscheinliste());
}
