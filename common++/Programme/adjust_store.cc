// $Id: adjust_store.cc,v 1.10 2002/11/22 15:31:06 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2002 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
#include <Aux/exception.h>
#include <getopt.h>
#include <unistd.h>
#include <Instanzen/ppsInstanz.h>

void usage(const std::string &s)
{
 std::cerr <<"\nDieses Programm ermöglicht die folgenden Aktionen:\n"
           "\tA: Für eine Lagerinstanz wird aus dem physikalischen Lagertabellen\n"
           "\t   die Lagermenge bestimmt und anschließend die 2er(Dispo-)Aufträge\n"
           "\t   entsprechend angepaßt. Dabei werden die eventuelle Vormerkungen\n"
           "\t   (in form von 1er(plan-)Aufträgen berücksichtigt. Sollte mehr\n"
           "\t   vorgemerkt sein, als im Lager drin ist, so sird die Vormerkung\n"
           "\t   reduziert.\n"
           "\tB1:Es wird sichergestellt, daß ALLE Aufträge, die nicht zu den\n"
           "\t   Kundenaufträgen oder exterenen Bestellungen gehören die eigene\n"
           "\t   KundenID haben.\n"
           "\tB2:Es wird sichergestellt, daß 0er- und 2er-Aufträge den Status OPEN\n"
           "\t   erhalten\n"
           "\tB3:Es wird sichergestellt, daß für 0er- und 2er-Aufträge die gelieferte\n"
           "\t   Menge null ist.\n"
           "\tC: Es wird sichergestellt, daß nur entweder 0er- oder 2er-Aufträge\n"
           "\t   (pro Instanz,Artikel,Lieferdatum) existieren.\n";
 std::cerr << "USAGE:  ";
 std::cerr << s <<" -i<instanz> -a<aktion> [-d<database> -h<dbhost>]\n"
           "wobei die aktion=[A|B|C] ist.\n\n";

 exit(1);
}


const static struct option options[]=
{{ "instanz", required_argument, NULL, 'i' },
 { "aktion", required_argument, NULL, 'a' },
 { "database", required_argument,      NULL, 'd' },
 { "dbhost", required_argument,      NULL, 'h' },  
 { NULL,      0,       NULL, 0 }
};

int main(int argc,char *argv[])
{
  int opt;
  ppsInstanz::ID instanz= ppsInstanzID::None;
  std::string database="";
  std::string dbhost="";  
  std::string aktion;

  if(argc==1) usage(argv[0]);
  while ((opt=getopt_long(argc,argv,"h:d:i:a:",options,NULL))!=EOF)
   {
    switch(opt)
     {
       case 'i' : instanz = ppsInstanz::ID(atoi(optarg));break;
       case 'a' : aktion = optarg; break;
       case 'd' : database=optarg;break;
       case 'h' : dbhost=optarg;break;  
       case '?' : usage(argv[0]);        
     }
   }

  ManuProC::PrintUncaughtExceptions();
  try{
    ManuProC::Connection conn;
    conn.setHost(dbhost);
    conn.setDbase(database);
    ManuProC::dbconnect(conn);

    cH_ppsInstanz I(instanz);
    if     (aktion=="A" && I->LagerInstanz())  I->ReparaturLager(getuid());
    else if(aktion=="B" &&!I->KundenInstanz()) I->Reparatur_Konsistenz();
    else if(aktion=="C" &&!I->KundenInstanz()) I->Reparatur_0er_und_2er(getuid());
    else usage(argv[0]);

    ManuProC::dbdisconnect();
  }catch(SQLerror &e){std::cout << e<<'\n'; return 1;}
  return 0;
}


