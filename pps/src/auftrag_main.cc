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

// generated 2000/11/22 20:43:27 MET by jacek@mimi.
// using glademm V0.5.10
//
// newer (non customized) versions of this file go to auftrag_main.cc_glade

// This file is for your program, I won't touch it again!

#include "auftrag_main.hh"
#include "auftrag_bearbeiten.hh"
#include"auftragbase.h"
#include<Auftrag/selFullAufEntry.h>
#include<Auftrag/AufEintragBase2.h>
#include<Auftrag/auftrag_status.h>
#include"auftrag_lieferschein.hh"
#include"auftrag_rechnung.hh"

extern AUFENTRYMAP aufentrymap;
extern MyMessage *meldung;

SelectedFullAufList *allaufids;
extern auftrag_main *auftragmain;
extern auftrag_bearbeiten *auftragbearbeiten;



bool stuecksumme;/* ob stueck summiert werden sollen */
bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
                        /* gespeichert werden soll */


void auftrag_main::on_beenden_activate()
{   
 maintree->detach_from_clist();
 Gtk::Main::instance()->quit();
}

void auftrag_main::on_erfassen_activate()
{   
 hide();
 try
 {  auftragbearbeiten = new auftrag_bearbeiten(selectedaufid,selectedaufzeile);
 } catch (SQLerror &e)
 {  cerr << e << '\n';
    show();
 } 
 selectedaufid=selectedaufzeile=0;
}

void auftrag_main::on_neuladen_activate()
{
#warning Diese Variable ist unnötig ?? (CP)   
// SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Status(OPEN));

 if(allaufids) { delete(allaufids); allaufids=NULL; }
 aufentrymap.clear();
 showtree();

}

void auftrag_main::on_main_drucken_activate()
{   
}


void auftrag_main::on_abschreiben_activate()
{   
 manage (new auftrag_lieferschein());
}

void auftrag_main::on_rechnung_activate()
{   
 manage (new auftrag_rechnung());
}

void auftrag_main::on_main_kndbez_activate()
{
 interne_namen = false;

 for(vector<AufEintragBase>::iterator j = allaufids->aufidliste.begin();
        j!=allaufids->aufidliste.end(); ++j)
     {
      AufEintrag &af = aufentrymap[(*j).mapKey()];
      cH_ExtBezSchema cs(af.getKdNr(),ExtBezSchema::default_Typ);
      af.setArtikelBezeichnung(cs);
     }

 showtree();
}

void auftrag_main::on_main_intbez_activate()
{   
 interne_namen = true;

 cH_ExtBezSchema cs(1,ExtBezSchema::default_Typ);
 for(vector<AufEintragBase>::iterator j = allaufids->aufidliste.begin();
        j!=allaufids->aufidliste.end(); ++j)
  {
   AufEintrag &af = aufentrymap[(*j).mapKey()];
   af.setArtikelBezeichnung(cs);
  }

 showtree();
}

void auftrag_main::on_mainprint_button_clicked()
{   
}

void auftrag_main::on_main_showtreebutton_clicked()
{   
 showtree();
}

void auftrag_main::on_main_defaultattrbutton_clicked()
{   
 maintree->setDefaultSeq();
}

auftrag_main::auftrag_main()
{
 auftragmain=this;

 selectedaufid=0;
 selectedaufzeile=0;
 selectedmyrow=0;
 allaufids=0;
 interne_namen=true;
 showdeep=0;

 showtree();

 maintree->select_row.connect(SigC::slot(this,&auftrag_main::onRowSelect));
 maintree->unselect_row.connect(SigC::slot(this,&auftrag_main::onRowUnselect));

}



void auftrag_main::showtree()
{
 maintree->detach_from_clist();
 maintree->clear();
 maintree->setShowdeep(showdeep);

 if(!allaufids) 
   { SQLFullAuftragSelector psel((SQLFullAuftragSelector::sel_Status)((AufStatVal)OPEN));
     if(interne_namen)
       allaufids = new SelectedFullAufList(psel,
			cH_ExtBezSchema(1,ExtBezSchema::default_Typ));
     else allaufids = new SelectedFullAufList(psel);
   }

 maintree->fill(*allaufids, showdeep);
 maintree->stutzeBaum();
 maintree->updateSummen();
 maintree->attach_to_clist();
}	 


void auftrag_main::onRowUnselect(int row, int col, GdkEvent* b)
{
 if(selectedmyrow)
   if(selectedmyrow->Leaf())
      prozlist_scombo->reset();
 selectedaufid=selectedaufzeile=0;
 selectedmyrow=0;
}

void auftrag_main::onRowSelect(int row, int col, GdkEvent* b)
{
 TCListRow *tclr=(TCListRow*)(maintree->get_row_data(row));
 selectedmyrow = (MyRow*)(*tclr).get_user_data();
 selectedaufid = selectedmyrow->getAuftragid();
 selectedaufzeile = selectedmyrow->getZeilennr();
 prozlist_scombo->reset();
 if(selectedmyrow->Leaf())   
   prozlist_scombo->set_text(selectedmyrow->ProzessText());
}

void auftrag_main::on_prozlistscombo_search(int *cont, GtkSCContext newsearch)
{if (newsearch!=GTK_SEARCH_OPEN) return;
 if(selectedmyrow)
   if(selectedmyrow->Leaf())     
     {  ArtikelBaum artb(selectedmyrow->getArtID());
        cH_Prozess p(Prozess::default_id);
        ArtikelBase::ID tmpid;   

        prozlist.erase(prozlist.begin(),prozlist.end());

        while((p=artb.getErzeugendenProzess())->Id()!=Prozess::default_id)
         {
          prozlist_scombo->add_item(
          		itos(p->getProzessID())+":"+p->getTyp()+" "+p->getText(),
					p->getProzessID());
          prozlist.push_back(p);
          tmpid=artb.ParentArtikelID();
          if(tmpid)
            artb=ArtikelBaum(tmpid);
          else break;
        }
     }
 *cont=false;
}


void auftrag_main::on_prozlistscombo_activate()
{
 if(selectedmyrow)
   if(selectedmyrow->Leaf())
     {
      int aufid=selectedmyrow->getAuftragid();
      int znr = selectedmyrow->getZeilennr();
      AufEintragBase2 aufbase(aufid,znr);
      int pid=atoi(prozlist_scombo->get_text().c_str());
      AufEintrag &af=aufentrymap[aufbase.mapKey()];
      try{af.setVerarbeitung(cH_Prozess(pid));}
      catch(SQLerror &e)
        {meldung->Show(e); return;}
      foot_statusbar->push(0,selectedmyrow->Description(
                                cH_ExtBezSchema(ExtBezSchema::default_ID)));

      EntryValue ev((cH_Prozess)(pid),SUBID_SPAWN,Petig::Datum().today());
      vector<int> buf = maintree->getCurrSeq();

      size_t i;
      for(i=0; (i<buf.size()) && (buf[i]!=PROZ_SEQ); i++);

      selectedmyrow->setText(i,ev.getStrProzVal());
     }
}

