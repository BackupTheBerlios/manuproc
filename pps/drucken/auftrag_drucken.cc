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
#include "drucken.hh"
#include "getopt.h"
#include <Aux/itos.h>

//struct Configuration Configuration;

const static struct option options[]=
{{ "firma",  no_argument, NULL, 'f' },
 { "kopie",  no_argument, NULL, 'k' },  
 { "art",     required_argument,      NULL, 'a' }, 
 { "nr",     required_argument,      NULL, 'n' }, 
  // ich mag den Namen nicht, schlieﬂlich ist das kein Plotter CP; genau JJ:-)
 { "plot",     no_argument,      NULL, 'p' },
 { "print",     no_argument,      NULL, 'p' }, 
 { "instanz", required_argument,      NULL, 'i' }, 
 { "database", required_argument,      NULL, 'd' }, 
 { "dbhost", required_argument,      NULL, 'h' }, 
 { NULL,      0,       NULL, 0 }
};       

int main (int argc, char *argv[])
{
 bool firmenpapier=false;
 bool kopie=false;
 LR_Base::typ was=LR_Base::Auftrag;
 bool plot=false;
 bool toTeX=false;
 unsigned int auftragsnr = 0; 
 ppsInstanz::ppsInstId instanz = ppsInstanz::INST_KNDAUF;
 std::string database="";
 std::string dbhost="";
 
 int opt;

 if(argc==1) exit(1);

 while ((opt=getopt_long(argc,argv,"ftka:n:pi:d:",options,NULL))!=EOF)
  { switch (opt)
    {  case 'f' : firmenpapier=true; break;
       case 'k' : kopie=true; break;
       case 'a' : if(std::string("Rechnung")==optarg) was=LR_Base::Rechnung;
		  else if(std::string("Lieferschein")==optarg) was=LR_Base::Lieferschein;
		  else if(std::string("Auftrag")==optarg) was=LR_Base::Auftrag;
		  else if(std::string("Intern")==optarg) was=LR_Base::Intern;
		  else if(std::string("Extern")==optarg) was=LR_Base::Extern;
		  else was=LR_Base::NICHTS;
		break;
       case 'n' : auftragsnr=atoi(optarg);break;
       case 'i' : instanz=(ppsInstanz::ppsInstId)atoi(optarg);break;
       case 'p' : plot=true;break;
	case 'd' : database=optarg;break; 
	case 'h' : dbhost=optarg;break; 
	case 't' : toTeX=true;break; 
	case '?':
            std::cout << "$Id: auftrag_drucken.cc,v 1.11 2002/03/20 08:03:36 christof Exp $\n\n"
                   "USAGE:" << argv[0] << " -n <Nr> [-a <Auftrag|Rechung|Lieferschein|Intern|Extern>] [-kft] [-i <Instanz>] [-d <Datenbank>]\n"
		"\n\t-t\t nur TeX file erzeugen ("<< (toTeX?"an":"aus")<< ")\n"
		"\t-p\t drucken ("<< (plot?"an":"aus")<< ")\n"
		"\t-a\t Aufrag(*), Rechnung, Lieferschein, Intern, Extern\n"
		"\t-n\t (A./R./L.)-Nummer (wichtig!)\n"
		"\t-f\t auf Firmenpapier ("<< (firmenpapier?"an":"aus")<< ")\n"
		"\t-f\t Kopien ("<< (kopie?"an":"aus")<< ")\n"
		"\t-i\t Instanz ausw‰hlen ("<< instanz<< ")\n"
		"\t-d\t Datenbank ("<< database<< ")\n"
		"\t-d\t DbHost ("<< dbhost<< ")\n";
            exit(1);
    }
  }                 
  try {
      Petig::Connection conn;
      conn.setDbase(database);
      conn.setHost(dbhost);
      Petig::dbconnect(conn);  

      LR_drucken l(was,auftragsnr,plot,firmenpapier,
			kopie,cH_ppsInstanz(instanz),toTeX);      
         
      Petig::dbdisconnect();
   } catch (SQLerror &e)
   {  std::cerr << e << '\n';
      return 1;
   }
   return 0;
}
