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
// newer (non customized) versions of this file go to auftragliste.cc_glade

// This file is for your program, I won't touch it again!

#include "auftragliste.hh"
#include <vector.h>
#include<Auftrag/selFullAufEntry.h>
#include "auftrag_bearbeiten.hh"
//#include<ExtBezSchema/ExtBezSchema.h>
#include "auftrag_main.hh"

extern auftrag_main *auftragmain;

auftragliste::auftragliste(cH_ppsInstanz _i)
: instanz(_i)
{
  set_column_titles();
  optionmenu_stil_deactivate();
}

void auftragliste::set_column_titles()
{
  vector<string> ct;
  ct.push_back("Kunde");
  ct.push_back("Auftragsnr.");
  ct.push_back("Auftr.Datum");
  ct.push_back("Kd.Auftr.Nr.");
  auftragsliste->setTitles(ct);
}

void auftragliste::fill_tree()
{
   AufStatVal stat=WAufStat->get_Status();
   SelectedFullAufList *allaufids;
   SQLFullAuftragSelector psel= SQLFullAuftragSelector::sel_Status(instanz->Id(),(AufStatVal)stat);
   allaufids = new SelectedFullAufList(psel);

 vector<cH_RowDataBase> datavec;

 for(vector<AufEintragBase>::iterator i = allaufids->aufidliste.begin();i!=allaufids->aufidliste.end();++i)
  {
   datavec.push_back(new Data_aliste(*i));
  } 

 auftragsliste->setDataVec(datavec);
 auftragsliste->leaf_selected.connect(SigC::slot(this,&auftragliste::on_leaf_selected));
}

void auftragliste::on_leaf_selected(cH_RowDataBase d)
{
 try {
 const Data_aliste *dt=dynamic_cast<const Data_aliste*>(&*d);
 selected_Auftrag=AufEintragBase2(instanz->Id(),dt->get_aid(),dt->get_zeilennr());
// selected_Artikel=dt->get_Artikel_ID();
 } catch (...)
 {  assert(!"d is Data_aliste");
 }
}

void auftragliste::on_button_erfassen_clicked()
{
// hide();
 if (!selected_Auftrag.Id()) return; 
 try
 { manage(new auftrag_bearbeiten(selected_Auftrag));
 } catch (SQLerror &e)
 {  cerr << e << '\n';
    show();
 } 
 selected_Auftrag=AufEintragBase2(instanz->Id());
 destroy();
}


void auftragliste::optionmenu_stil_deactivate()
{
  fill_tree();
}


void auftragliste::on_closebutton_clicked()
{   
 auftragmain->show();
 destroy();
}
