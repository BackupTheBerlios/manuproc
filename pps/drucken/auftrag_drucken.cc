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

#include "drucken.hh"

int main (int argc, char *argv[])
{

  if (argc!=4) {cerr<< "FEHLER: 3 Argumente nötig für "<< argv[0]<<"\n"; return 1;}
  string s = argv[1];
  if (s!="Rechnung" && s!= "Lieferschein" && s!="Auftrag" )
    {cerr<<"FEHLER: 1. Argument von "<<argv[0]<<" muß 'Rechnung' oder 'Lieferschein' oder 'Auftrag' sein und nicht ´"<<s<<"´\n"; return 1;}
  unsigned int auftragsnr = atoi(argv[2]);
  string p = argv[3];
  if (p!="Preview" && p!= "Plot")
    {cerr<<"FEHLER: 3. Argument von "<<argv[0]<<" muß 'Preview' oder 'Plot' sein und nicht '"<<p<<"'\n"; return 1;}

  try {
      Petig::Connection conn;
      conn.setDbase("petigdb");
      Petig::dbconnect(conn);  

//      LR_drucken l("Lieferschein",10025,"Preview");
//      LR_drucken l("Rechnung",10008,"Preview");
//      LR_drucken l("Auftrag",10040,"Preview");

      LR_drucken l(s,auftragsnr,p);      
         
      Petig::dbdisconnect();
   } catch (SQLerror &e)
   {  cerr << e << '\n';
      return 1;
   }
   return 0;
}
