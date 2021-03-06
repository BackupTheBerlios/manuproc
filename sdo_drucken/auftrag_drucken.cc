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
#include "Configuration.h"
#include <Misc/Ausgabe_neu.h>
#include <TeX.h>


struct Configuration Configuration;

enum {EAN=256, COMBINE};

const static struct option options[]=
{
//{ "firma",  no_argument, NULL, 'f' },
// { "kopie",  no_argument, NULL, 'k' },  
 { "toTeX",  required_argument, NULL, 't' },  
 { "batch",  required_argument, NULL, 'B' },
 { "ean",  no_argument, NULL, EAN },  
 { "art",     required_argument,      NULL, 'a' }, 
 { "nr",     required_argument,      NULL, 'n' }, 
// { "print",     no_argument,      NULL, 'p' }, 
 { "instanz", required_argument,      NULL, 'i' }, 
 { "database", required_argument,      NULL, 'd' }, 
 { "dbhost", required_argument,      NULL, 'h' }, 
 { "combine-names", no_argument, NULL, COMBINE },
 { "copies",   required_argument, NULL, 'Y' },
 { "order_clause",   required_argument, NULL, 'O' },
 { "preview",  no_argument, NULL, 'G' },
 { NULL,      0,       NULL, 0 }
};       


void usage(std::string n, ppsInstanz::ID instanz,
   	std::string database,std::string dbhost)
{
   std::cout << "$Id: auftrag_drucken.cc,v 1.4 2006/02/23 14:47:41 christof Exp $\n\n"
              "USAGE:" << n << " -n <Nr> [-a <Typ>] [-kft] [-i <Instanz>] [-d <Datenbank>]\n"
		"\n\t-t<file>\t nur TeX file erzeugen; in file speichern("<< (Configuration.toTeX?"an":"aus")<< ")\n"
	        "\n\t-B<printer>\t batch mode on <printer>; kein GUI ("<< (Configuration.batch?"an":"aus")<< ")\n"
		"\t-a\t Aufrag(*), Rechnung, Lieferschein, Intern, Extern\n"
		"\t-n\t (A./R./L.)-Nummer (wichtig!)\n"
		"\t-i\t Instanz ausw�hlen ("<< instanz<< ")\n"
		"\t-d\t Datenbank ("<< database<< ")\n"
		"\t-h\t DbHost ("<< dbhost<< ")\n"
		"\t-Y <1,2,3>\t Originale, Kopien auf Firmenpapier, Kop. auf wei�\n"
		"\t-R\t R�ckstand zum Auftrag\n"
		"\t-G\t Direkt anzeigen (kann nicht gedruckt werden)\n"
		"\t--EAN\t Mit EAN-Code\n"
		"\t-O<order clausel>\t mit dieser Klausel Auftrag sortieren\n"
		"\t-Z\t Rechnung sortieren nach Zeilennr\n";
            exit(1);

}

bool sort_by_rownr=false;

int main (int argc, char *argv[])
{
 LR_Base::typ was=LR_Base::Auftrag;
 bool rueckstand=false;
 bool ean_code=false;
 unsigned int auftragsnr = 0; 
 ppsInstanz::ID instanz = ppsInstanzID::Kundenauftraege;
 std::string database;
 std::string dbhost;

 int opt;

 if(argc==1) usage(argv[0],instanz,database,dbhost);

 while((opt=getopt_long(argc,argv,"Ga:n:pi:d:RZY:t:B:O:",options,NULL))!=EOF)
  { switch (opt)
    { case 'a' : if(std::string("Rechnung")==optarg) was=LR_Base::Rechnung;
		  else if(std::string("Lieferschein")==optarg) was=LR_Base::Lieferschein;
		  else if(std::string("Auftrag")==optarg) was=LR_Base::Auftrag;
		  else if(std::string("Intern")==optarg) was=LR_Base::Intern;
		  else if(std::string("Extern")==optarg) was=LR_Base::Extern;
		  else was=LR_Base::NICHTS;
		break;
       case 'n' : auftragsnr=atoi(optarg);break;
       case 'i' : instanz=(ppsInstanz::ID)atoi(optarg);break;
	case 'd' : database=optarg;break; 
	case 'h' : dbhost=optarg;break; 
	case 'R' : rueckstand=true; break;
	case EAN : ean_code=true; break;
	case COMBINE: Configuration.combine=true; break;
	case 'Y' : Configuration.copies=optarg; break;
	case 'Z' : sort_by_rownr=true; break;
	case 'O' : Configuration.order_clausel=optarg; break;
	 case 't' : Configuration.toTeX=true; 
                  Configuration.texfile=optarg;
               break;
        case 'B' : Configuration.batch=true;
                  Configuration.printer=optarg;
                break;
	case 'G': Configuration.preview_only=true; break;
	case '?': usage(argv[0],instanz,database,dbhost); break;
    }
  }                 
  try {
      ManuProC::Connection conn;
      conn.setDbase(database);
      conn.setHost(dbhost);

      Ausgabe_neu::TeX_uses_UTF8=false;
      TeX::TeX_uses_UTF8=false;

      ManuProC::dbconnect(conn);  

//      Query ("set names 'utf-8'");

#warning: must be at properly place
Configuration.zaehle_spalten=true;

      LR_drucken l(was,auftragsnr, cH_ppsInstanz(instanz),
		rueckstand,ean_code);
         
      ManuProC::dbdisconnect();
   } catch (SQLerror &e)
   {  std::cerr << e << '\n';
      return 1;
   }
   return 0;
}
