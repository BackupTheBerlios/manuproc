// $Id: adjust_store.cc,v 1.33 2003/05/26 13:43:27 christof Exp $
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

#include <Misc/dbconnect.h>
#include <Misc/exception.h>
#include <getopt.h>
#include <unistd.h>
#include <Auftrag/ppsInstanzReparatur.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <Misc/Trace.h>

void usage(const std::string &s)
{
 std::cerr <<"\nDieses Programm ermöglicht die folgenden Aktionen:\n"
           "\tA: Für eine Lagerinstanz wird aus dem physikalischen Lagertabellen\n"
           "\t   die Lagermenge bestimmt und anschließend die 2er(Dispo-)Aufträge\n"
           "\t   entsprechend angepaßt. Dabei werden die eventuelle Vormerkungen\n"
           "\t   (in form von 1er(plan-)Aufträgen berücksichtigt. Sollte mehr\n"
           "\t   vorgemerkt sein, als im Lager drin ist, so sird die Vormerkung\n"
           "\t   reduziert.\n"
           "\tC: Es wird sichergestellt, daß nur entweder 0er- oder 2er-Aufträge\n"
           "\t   (pro Instanz,Artikel,Lieferdatum) existieren.\n"
           "\t*/X: Alle Analysen/Reparaturen auf einmal (meist mit -I) (außer A,C)\n";
           
 std::cerr << "USAGE:  ";
 std::cerr << s <<" [-i<instanz>|-I]  -a<aktion> [-d<database>] [-h<dbhost>] [-y] \n"
           "\twobei die aktion=[A|C|X|*] ist.\n"
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
      else std::cout << "\t"<< I << "'A' nicht sinnvoll\n";
     }
    else if (aktion=="*" || aktion=="X" || aktion=="C")
    {  SQLFullAuftragSelector psel=SQLFullAuftragSelector::sel_InstanzAlle(I->Id());
       SelectedFullAufList K(psel);
       bool alles_ok=true;
      if (aktion=="*" || aktion=="C")
       RI.Reparatur_0er_und_2er(K,analyse_only);
      if (aktion=="*" || aktion=="X")
      {AuftragBase::tolerate_inconsistency=true;
       for(SelectedFullAufList::iterator i = K.begin();i!=K.end(); ++i)
       {  AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(*i,
       			AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
       	  AufEintragZu::map_t kinder=AufEintragZu::get_Kinder_nach_Artikel(*i);
       	  alles_ok&=RI.Eltern(*i,eltern,analyse_only);
       	  alles_ok&=RI.Lokal(*i,analyse_only);
       	  alles_ok&=RI.Kinder(*i,kinder,analyse_only);
       }
      }
    }
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
  while ((opt=getopt_long(argc,argv,"h:d:i:a:yIt",options,NULL))!=EOF)
   {
    switch(opt)
     {
       case 'i' : instanz = ppsInstanz::ID(atoi(optarg));break;
       case 'I' : all_instanz=true;break;
       case 'a' : aktion = optarg; break;
       case 'd' : database=optarg;break;
       case 'h' : dbhost=optarg;break;  
       case 'y' : analyse_only=true;break;  
       case 't' : ManuProC::Tracer::Enable(AuftragBase::trace_channel); break;
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
      alles_ok&=check_for(argv[0],cH_ppsInstanz(instanz),aktion,analyse_only);
    else
     { std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
       for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
        {
         //if((*i)->KundenInstanz()) continue;
         alles_ok&=check_for(argv[0],*i,aktion,analyse_only);
        }
     }    

    ManuProC::dbdisconnect();
  }catch(SQLerror &e){std::cout << e<<'\n'; return 1;}
  return !alles_ok;
}


