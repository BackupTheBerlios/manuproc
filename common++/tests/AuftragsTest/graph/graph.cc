// $Id: graph.cc,v 1.20 2003/01/08 09:46:58 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma  
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

                
//#include <fstream>
#include <iostream>
#include <string>
#include "dot_out.h"
#include "get_data.h"
#include <Misc/dbconnect.h>
#include <Misc/exception.h>
#include "getopt.h"
#include "../steuerprogramm.hh"

const static struct option options[]=
{{ "black",  no_argument, NULL, 'b' },
 { "colour",  no_argument, NULL, 'c' },
 { "referenz", no_argument, 0, 'r' },
 { "limit", required_argument, 0, 'l' },
 { NULL,      0,       NULL, 0 }
};       
        

void usage(std::string s)
{
  cout << "USAGE: " << s <<" [-bgr] <Dateisammlung z.B. M,P oder S, X=Legende>\n";
#if 0
       << "\t\t[P]lanung \n"
       << "\t\t[S]plit \n"
       << "\t\t[L]ager \n"
       << "\t\t[Z]wei Aufträge \n"
       << "\t\t[D]atum des zweiten Auftrags früher \n"
       << "\t\t[Ls] Lieferschein \n"
       << "\t\t[Lm] Lieferschein Mengenänderung\n"
       << "\t\t[LZ] Lieferschein mit Zustazeinträgen\n"
       << "\t\t[LA] Lieferschein mit Zwei Aufträgen\n"
       << "\t\t[LJ] Lieferschein für Jacek\n"
       << "\t\t[ZK] Zwei Kunden Zwei Aufträgen\n"
       << "\t\t[ZKM] Zwei Kunden Zwei Aufträgen, Menge freigeben\n"
       << "\t\t[MP] ManuProC\n"
       << "\t\t[Rg] Rep_Petig_0er_2er_gleichzeitig\n"
       << "\t\t[RK] Rep_Petig_Kunde\n"
       << "\t\t[RZ] Rep_Petig_Zuordung\n"
       << "\t\t[RKZ] Rep_Petig_Kunden_Zuordung\n"
       << "\t\t[X]  LEGENDE \n";
#endif       
  cout << "\t -b --black [default]\n"
       << "\t -c --colour\n"
       << "\t -r --referenz\tReferenz anzeigen\n"
       << "\t -l --limit <number>\tHöchstanzahl von angezeigten Schritten\n"
       << "\n";
}


int main(int argc, char *argv[])
{
  if(argc!=2 && argc!=3) {usage(argv[0]); exit(1);}

  dot_out::e_colour colour=dot_out::Black;
  int opt;
  while ((opt=getopt_long(argc,argv,"bcrl:",options,NULL))!=EOF)
   { switch (opt) {
      case 'b' : colour=dot_out::Black; break;
      case 'c' : colour=dot_out::Colour; break;
      case 'r' : graph_data_node::show_referenz=true; break;
      case 'l': graph_data_node::limit=atol(optarg); break;
     }
   }  

  std::string mode=argv[optind];
  Petig::PrintUncaughtExceptions();
  try{try{
#ifdef  MANU_PROC_TEST
   putenv("PGDATABASE=anleitungdb");
#elif defined  MABELLA_TEST
   putenv("PGDATABASE=mabella_test_db");
#else
   putenv("PGDATABASE=testdb");
#endif
   Petig::dbconnect();
   dot_out D(mode,colour);
   D.write();
  }catch(SQLerror &e){std::cout << e<<'\n';}
  }catch(std::exception  &e){std::cout << e.what()<<'\n';}
 return 0;
}
