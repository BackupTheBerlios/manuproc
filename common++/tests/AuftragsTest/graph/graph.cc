// $Id: graph.cc,v 1.6 2002/10/24 14:06:51 thoma Exp $
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
#include <Aux/dbconnect.h>
#include <Aux/exception.h>
#include "getopt.h"
#include "../steuerprogramm.hh"

const static struct option options[]=
{{ "black",  no_argument, NULL, 'b' },
 { "colour",  no_argument, NULL, 'c' },
 { NULL,      0,       NULL, 0 }
};       
        

void usage(std::string s)
{
  cout << "USAGE: " << s <<"\t[M]engen [-bg] \n"
       << "\t\t[P]lanung \n"
       << "\t\t[S]plit \n"
       << "\t\t[L]ager \n"
       << "\t\t[Z]wei Aufträge \n"
       << "\t\t[D]atum des zweiten Auftrags früher \n"
       << "\t\t[Ls] Lieferschein \n"
       << "\t\t[Lm] Lieferschein Mengenänderung\n"
       << "\t\t[LZ] Lieferschein mit Zustazeinträgen\n"
       << "\t\t[LA] Lieferschein mit Zwei Aufträgen\n"
       << "\t\t[ZK] Zwei Kunden Zwei Aufträgen\n"
       << "\t\t[MP] ManuProC\n"
       << "\t\t[X]  LEGENDE \n";
  cout << "\t b => black [default] "
       << "\t c => colour \n\n";
}


int main(int argc, char *argv[])
{
  if(argc!=2 && argc!=3) {usage(argv[0]); exit(1);}

  dot_out::e_colour colour=dot_out::Black;
  int opt;
  while ((opt=getopt_long(argc,argv,"bc",options,NULL))!=EOF)
   { switch (opt) {
      case 'b' : colour=dot_out::Black; break;
      case 'c' : colour=dot_out::Colour; break;
     }
   }  

  emode mode=None;
  if     (string(argv[optind])=="M") mode=Menge;
  else if(string(argv[optind])=="P") mode=Planung;
  else if(string(argv[optind])=="S") mode=Split;
  else if(string(argv[optind])=="L") mode=Lager;
  else if(string(argv[optind])=="Z") mode=ZweiAuftraege;
  else if(string(argv[optind])=="D") mode=ZweiterAuftrag_frueheresDatum;
  else if(string(argv[optind])=="Ls")mode=Lieferschein;
  else if(string(argv[optind])=="Lm")mode=LieferscheinMenge;
  else if(string(argv[optind])=="LZ")mode=LieferscheinZusatz;
  else if(string(argv[optind])=="LA")mode=LieferscheinZweiAuftraege;
  else if(string(argv[optind])=="ZK")mode=ZweiKunden;
  else if(string(argv[optind])=="MP")mode=ManuProCTest;
  else if(string(argv[optind])=="X") mode=Legende;
  Petig::PrintUncaughtExceptions();
  try{try{
#ifdef  MANU_PROC_TEST
   putenv("PGDATABASE=anleitungdb");
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
