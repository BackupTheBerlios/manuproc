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
#include "getopt.h"

//struct Configuration Configuration;

const static struct option options[]=
{{ "firma",  no_argument, NULL, 'f' },
 { "kopie",  no_argument, NULL, 'k' },  
 { "art",     required_argument,      NULL, 'a' }, 
 { "nr",     required_argument,      NULL, 'n' }, 
 { "plot",     no_argument,      NULL, 'p' }, 
 { "instanz", required_argument,      NULL, 'i' }, 
 { NULL,      0,       NULL, 0 }
};       

int main (int argc, char *argv[])
{
 bool firmenpapier=false;
 bool kopie=false;
 std::string was;
 bool plot=false;
 unsigned int auftragsnr = 0; 
 ppsInstanz::ppsInstId instanz = ppsInstanz::INST_KNDAUF;
 
 
 int opt;
 while ((opt=getopt_long(argc,argv,"fka:n:pi:",options,NULL))!=EOF)
  { switch (opt)
    {  case 'f' : firmenpapier=true; break;
       case 'k' : kopie=true; break;
       case 'a' : was=optarg;break;
       case 'n' : auftragsnr=atoi(optarg);break;
       case 'i' : instanz=(ppsInstanz::ppsInstId)atoi(optarg);break;
       case 'p' : plot=true;break;
    }
  }                 
 if (was != "Rechnung" && was!= "Lieferschein" && was!="Auftrag" && was !="Intern" )
    {cerr<<"FEHLER: -a (--art) Argument von "<<argv[0]<<" muß 'Rechnung' oder 'Lieferschein' oder 'Auftrag' oder 'Intern' sein \n"; return 1;}
 if (!auftragsnr) 
    {cerr<<"FEHLER: -n (--nr) Argument von "<<argv[0]<<" muß Nummer der Rechnung, des Lieferscheins oder des Auftrags sein\n"; return 1;}

  try {
      Petig::Connection conn;
      conn.setDbase("petigdb");
      Petig::dbconnect(conn);  

//      LR_drucken l("Lieferschein",10025,"Preview",instanz);
//      LR_drucken l("Rechnung",10023,"Preview",instanz);
//      LR_drucken l("Auftrag",10040,"Preview");

      std::string p="Preview";
      if(plot) p="Plot";
      LR_drucken l(was,auftragsnr,p,firmenpapier,kopie,cH_ppsInstanz(instanz));      
         
      Petig::dbdisconnect();
   } catch (SQLerror &e)
   {  cerr << e << '\n';
      return 1;
   }
   return 0;
}
