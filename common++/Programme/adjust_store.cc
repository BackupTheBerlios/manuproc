// $Id: adjust_store.cc,v 1.22 2002/12/10 09:55:10 thoma Exp $
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
#include <Instanzen/ppsInstanzReperatur.h>

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
           "\t   (pro Instanz,Artikel,Lieferdatum) existieren.\n"
           "\tD: Summe der Zuordnungen VON 0er-Aufträgen AN 1|20000 == 1|20000.getStueck().\n"
           "\tE: Summe der Zuordnungen VON 2er-Aufträgen AN 1|20000 == 2.getStueck().\n"
           "\tF: Summe der Zuordnungen VON 2er-Aufträgen AN 1|20000 <= 1|20000.getRestStueck().\n"
           "\tK: Zuordnungen VON Kundenaufträgen: ZuordnungsMENGE,AuftragsINSTANZ,\n"
           "\t                                    AuftragsARTIKEL.\n";
 std::cerr << "USAGE:  ";
 std::cerr << s <<" [-i<instanz>|-I]  -a<aktion> [-d<database> -h<dbhost> -y] \n"
           "\twobei die aktion=[A|B|C|D|E|F|K] ist.\n"
           "\t-y Analysemodus (keine Reparaturen)\n"
           "\t-I führt die Tests für alle Instanzen durch\n\n";
 exit(1);
}

bool check_for(const std::string &pname,cH_ppsInstanz I,const std::string &aktion,const bool analyse_only)
{   
   ppsInstanzReparatur RI(I->Id());
   bool alles_ok=true;
    if     (aktion=="A")
     {
      if(I->EigeneLagerKlasseImplementiert()) RI.ReparaturLager(getuid(),analyse_only);
      else cout << "\t"<< I << "'A' nicht sinnvoll\n";
     }
    else if(aktion=="B" &&!I->KundenInstanz()) RI.Reparatur_Konsistenz(analyse_only);
    else if(aktion=="C" &&!I->KundenInstanz()) RI.Reparatur_0er_und_2er(getuid(),analyse_only);
    else if(aktion=="D" &&!I->KundenInstanz()) alles_ok=RI.ReparaturD_0_ZuSumme_1(getuid(),analyse_only);
    else if(aktion=="E" &&!I->KundenInstanz()) alles_ok=RI.ReparaturE_2_ZuSumme_1(getuid(),analyse_only);
    else if(aktion=="F" &&!I->KundenInstanz()) alles_ok=RI.ReparaturF_2_ZuSumme_1Rest(getuid(),analyse_only);
    else if(aktion=="K" && I->KundenInstanz()) alles_ok=RI.ReparaturK_Kundenzuordnung(getuid(),analyse_only);
    else usage(pname);
   return alles_ok;
}



const static struct option options[]=
{{ "instanz", required_argument, NULL, 'i' },
 { "aktion", required_argument, NULL, 'a' },
 { "database", required_argument,      NULL, 'd' },
 { "dbhost", required_argument,      NULL, 'h' },  
 { "analyse_only", no_argument,      NULL, 'y' },  
 { "alle_Instanzen", no_argument,      NULL, 'I' },  
 { NULL,      0,       NULL, 0 }
};

int ende(bool alles_ok)
{
  if(alles_ok) return 0;
  else         return 1;
}


int main(int argc,char *argv[])
{
  int opt;
  ppsInstanz::ID instanz= ppsInstanzID::None;
  std::string database="";
  std::string dbhost="";  
  std::string aktion;
  bool analyse_only=false;
  bool all_instanz=false;

  if(argc==1) usage(argv[0]);
  while ((opt=getopt_long(argc,argv,"h:d:i:a:yI",options,NULL))!=EOF)
   {
    switch(opt)
     {
       case 'i' : instanz = ppsInstanz::ID(atoi(optarg));break;
       case 'I' : all_instanz=true;break;
       case 'a' : aktion = optarg; break;
       case 'd' : database=optarg;break;
       case 'h' : dbhost=optarg;break;  
       case 'y' : analyse_only=true;break;  
       case '?' : usage(argv[0]);        
     }
   }

  if     (instanz==ppsInstanzID::None && !all_instanz) usage(argv[0]);
  else if(instanz!=ppsInstanzID::None &&  all_instanz) usage(argv[0]);


  ManuProC::PrintUncaughtExceptions();
  bool alles_ok=true;
  try{
    ManuProC::Connection conn;
    conn.setHost(dbhost);
    conn.setDbase(database);
    ManuProC::dbconnect(conn);

    if(instanz!=ppsInstanzID::None) 
      alles_ok=check_for(argv[0],cH_ppsInstanz(instanz),aktion,analyse_only);
    else
     { std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
       for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
        {
         if((*i)->KundenInstanz()) continue;
         bool x=check_for(argv[0],*i,aktion,analyse_only);
         if(!x) alles_ok=x;
        }
     }    

    ManuProC::dbdisconnect();
  }catch(SQLerror &e){std::cout << e<<'\n'; return ende(false);}
  return ende(alles_ok);
}


