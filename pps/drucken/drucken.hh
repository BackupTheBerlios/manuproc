/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <Aux/dbconnect.h>
#include <Aux/itos.h>
#include <string>
#include <fstream>
#include <Kunde/Kunde.h>
#include <Artikel/ArtikelBezeichnung.h>

class LR_drucken 
{
      unsigned int auftragsnr;
      const string RL;
      string kopie;
   private:
//      const static float DM_EURO=1.95583;
      void drucken(string view_plot);
//      void drucken_header(ofstream &os);
//      unsigned int sql_kundennr(const string& RL,unsigned int auftragsnr);
//      void drucken_footer(ofstream &os);
//        void drucken_table(ofstream &os,unsigned int kunden_id,string RL);
//      void drucken_table_lieferschein(ofstream &os,unsigned int kunden_id);
//      void drucken_table_rechnung(ofstream &os,unsigned int kunden_id);
//      fixedpoint<2> drucken_table_rechnung_preissum_warengruppe(ofstream &os,
//         vector<fixedpoint<2> >& preissum_zeile, vector<fixedpoint<2> >& preissum_warengruppe,
//         unsigned int spaltenzahl,string text="");
//      void drucken_table_new_page_header(ofstream& os,unsigned int page_count);
//      unsigned int drucken_table_rechnung_header(ofstream &os,
//         cH_ExtBezSchema& schema,unsigned int signifikanz,
//         bool menge_bool, bool rabatt_bool,float preismenge,string waehrung,
//         string einheit);
//      unsigned int sql_artid();
//     unsigned int sql_warengruppe(unsigned int artikel_id);
   public:
      LR_drucken(string RL, unsigned int auftragsnr,string view_plot);
};
