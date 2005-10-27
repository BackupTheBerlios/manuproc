// $Id: graph.cc,v 1.29 2005/10/27 13:10:58 christof Exp $
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
 { "start", required_argument, 0, 's' },
 { "only", required_argument, 0, 'o' },
 { "dont-hide", no_argument, 0, 'h' },
 { "art", required_argument, 0, 'A' },
 { "live", no_argument, 0, 'L' },
 { NULL,      0,       NULL, 0 }
};       
        

void usage(std::string s)
{
  std::cout << "USAGE: " << s <<" [-bgr] <Dateisammlung z.B. M,P oder S, X=Legende>\n";
  std::cout << "\t -b --black [default]\n"
       << "\t -c --colour\n"
       << "\t -r --referenz\tReferenz anzeigen\n"
       << "\t -s --start <number>\tErster angezeigter Schritt\n"
       << "\t -l --limit <number>\tLetzter angezeigter Schritt\n"
       << "\t -o --only <number>\tNur einen Zustand anzeigen\n"
       << "\t -h --dont-hide\tLeere Auftr�e anzeigen\n"
       << "\t -L|--live\taktuelle Datenbank anzeigen (keine Dateisammlung)\n"
       << "\t -A|--art <article>\tNur einen Artikel anzeigen\n"
       << "\n";
}


int main(int argc, char *argv[])
{
  dot_out::e_colour colour=dot_out::Black;
  int opt;
  bool live=false;
  while ((opt=getopt_long(argc,argv,"s:bcrl:o:hLA:",options,NULL))!=EOF)
   { switch (opt) {
      case 'b' : colour=dot_out::Black; break;
      case 'c' : colour=dot_out::Colour; break;
      case 'r' : graph_data_node::show_referenz=true; break;
      case 'l': graph_data_node::limit=atol(optarg); break;
      case 's': graph_data_node::start=atol(optarg); break;
      case 'h': graph_data_node::dont_hide_empty=true; break;
      case 'o': graph_data_node::start=atol(optarg); 
                graph_data_node::limit=graph_data_node::start+1;
      		break;
      case 'L': live=true; break;
      case 'A': graph_data_node::article=atol(optarg); break;
      default: usage(argv[0]); return 1;
     }
   }  

  if ((!live && optind!=argc-1) || (live && optind!=argc))
  {usage(argv[0]); exit(1);}
  std::string mode;
  if (!live) mode=argv[optind];
  ManuProC::PrintUncaughtExceptions();
  try{
  if (!getenv("PGDATABASE"))
  {
#ifdef  MANU_PROC_TEST
   putenv("PGDATABASE=anleitungdb");
#elif defined  MABELLA_TEST
   putenv("PGDATABASE=mabella_testdb");
#else
   putenv("PGDATABASE=testdb");
#endif
  }
   ManuProC::dbconnect();
   // UTF-8?
   dot_out D(mode,colour);
   D.write();
  }
  catch(SQLerror &e){std::cout << e<<'\n';}
  catch(std::exception  &e){std::cout << e.what()<<'\n';}
 return 0;
}
