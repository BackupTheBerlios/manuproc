/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef _AUFTRAGLISTE_HH
#  include "auftragliste_glade.hh"
#  define _AUFTRAGLISTE_HH

#include <Auftrag/AufEintragBase.h>
#include<Auftrag/auftrag_status.h>
#include <Aux/EntryValueDatum.h>


class auftragliste : public auftragliste_glade
{   
        ppsInstanz instanz;
        AufEintragBase2 selected_Auftrag;
               
        friend class auftragliste_glade;
        void optionmenu_stil_deactivate();
        void on_closebutton_clicked();
        void on_button_erfassen_clicked();
        void set_column_titles();
        void fill_tree();
        void on_leaf_selected(cH_RowDataBase d);
   public:
       auftragliste(ppsInstanz _i);
};

class Data_aliste : public RowDataBase
{

   const AufEintragBase &AB ;
   enum {KUNDE,AUFTRAGS_NR,AUFTRAGS_DATUM,KD_AUFTR_NR};
 public:
   Data_aliste(const AufEintragBase& ab) : AB(ab) {}
   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
       switch (seqnr) {
         case KUNDE : return  cH_EntryValueIntString(AB.getKdNr());
         case AUFTRAGS_NR : return cH_EntryValueIntString(AB.getAuftragid());
#warning Ist das Lieferdatum hier richtig? MAT
         case AUFTRAGS_DATUM : return cH_EntryValueDatum(AB.getLieferdatum());
         case KD_AUFTR_NR : return cH_EntryValueIntString(AB.getYourAufNr());
         }   
      return cH_EntryValueIntString("?");
    }

  int get_aid() const {return AB.getAuftragid();}
  int get_zeilennr() const {return AB.getZnr();}
};
#endif
