// $Id: adjust_store.cc,v 1.38 2003/06/18 07:45:51 christof Exp $
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

enum action_bits { b_physical, b_exclude, b_tree, b_raise };

enum action_flags { f_none=0 };
bool operator&(action_flags a,action_bits b) { return int(a)&(1<<int(b)); }
void operator|=(action_flags &a,action_bits b) { ((int&)(a))|=1<<int(b); }
void operator-=(action_flags &a,action_bits b) { ((int&)(a))&=~(1<<int(b)); }
//action_flags operator|(action_flags a,action_bits b) { return action_flags(int(a)|(1<<int(b))); }
//action_flags operator|(action_bits a,action_bits b) { return action_flags((1<<int(a))|(1<<int(b))); }

static action_flags actions=f_none;

static void usage(const std::string &s)
{
 std::cerr <<"\nDieses Programm ermöglicht die folgenden Aktionen:\n"
           "\tA: Für eine Lagerinstanz wird aus dem physikalischen Lagertabellen\n"
           "\t   die Lagermenge bestimmt und anschließend die 2er(Dispo-)Aufträge\n"
           "\t   entsprechend angepaßt. Dabei werden die eventuelle Vormerkungen\n"
           "\t   (in form von 1er(plan-)Aufträgen berücksichtigt. Sollte mehr\n"
           "\t   vorgemerkt sein, als im Lager drin ist, so sird die Vormerkung\n"
           "\t   reduziert. (nicht mit -I!)\n"
           "\tC: Es wird sichergestellt, daß nur entweder 0er- oder 2er-Aufträge\n"
           "\t   (pro Instanz,Artikel,Lieferdatum) existieren.\n"
           "\tX: Reparaturen von Zuordnungen+lokalen Einschränkungen\n"
           "\tR: Erhöhen von Produziert-Selbst-Instanzen auf noch benötigte Menge\n"
           "\t*: Alle Analysen/Reparaturen auf einmal (meist mit -I)\n";
           
 std::cerr << "USAGE:  ";
 std::cerr << s <<" [-i<instanz>|-I]  -a<aktion> [-d<database>] [-h<dbhost>] [-l|-y] \n"
           "\twobei die aktion=[A|C|X|T|*] ist.\n"
           "\t-y Analysemodus (keine Reparaturen)\n"
           "\t-l Reparatur wiederholen bis keine Fehler mehr auftreten\n"
           "\t-I führt die Tests für alle Instanzen durch\n\n";
 exit(1);
}

static bool check_for(const std::string &pname,cH_ppsInstanz I,const bool analyse_only)
{   
   ppsInstanzReparatur RI(I->Id());
   bool alles_ok=true;
    if (actions&b_physical)
     {
      if(I->EigeneLagerKlasseImplementiert()) 
         alles_ok&=RI.ReparaturLager(getuid(),analyse_only);
      else if (!(actions&b_tree)) // bei * unterdrücken
         std::cout << "\t"<< I << " 'A' nicht sinnvoll\n";
     }
    if (actions&b_tree || actions&b_exclude)
    {  SQLFullAuftragSelector psel=SQLFullAuftragSelector::sel_InstanzAlle(I->Id());
       SelectedFullAufList K(psel);
      if (actions&b_exclude) alles_ok&=RI.Reparatur_0er_und_2er(K,analyse_only);
      if (actions&b_tree)
      {AuftragBase::tolerate_inconsistency=true;
       for(SelectedFullAufList::iterator i = K.begin();i!=K.end(); ++i)
       {  AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(*i,
       			AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
       	  AufEintragZu::map_t kinder=AufEintragZu::get_Kinder_nach_Artikel(*i);
       	  alles_ok&=RI.Eltern(*i,eltern,analyse_only,actions&b_raise);
       	  alles_ok&=RI.Lokal(*i,analyse_only);
       	  alles_ok&=RI.Kinder(*i,kinder,analyse_only);
       }
      }
    }
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
  bool analyse_only=false;
  bool all_instanz=false;
  bool loop=false;

  if(argc==1) usage(argv[0]);
  while ((opt=getopt_long(argc,argv,"h:d:i:a:yItl",options,NULL))!=EOF)
   {
    switch(opt)
     {
       case 'i' : instanz = ppsInstanz::ID(atoi(optarg));break;
       case 'I' : all_instanz=true;break;
       case 'a' : if (strchr(optarg,'A')||strchr(optarg,'*')) actions|=b_physical;
          if (strchr(optarg,'C')||strchr(optarg,'*')) actions|=b_exclude;
          if (strchr(optarg,'R')||strchr(optarg,'*')) actions|=b_raise;
          if (strchr(optarg,'X')||strchr(optarg,'*')||strchr(optarg,'R')) actions|=b_tree;
          break;
       case 'd' : database=optarg;break;
       case 'h' : dbhost=optarg;break;  
       case 'y' : analyse_only=true;break;  
       case 'l' : loop=true; break;
       case 't' : ManuProC::Tracer::Enable(AuftragBase::trace_channel); break;
       case '?' : usage(argv[0]);        
     }
   }

  if (((instanz==ppsInstanzID::None) ^ all_instanz) || actions==f_none
  	|| (!loop ^ analyse_only)) usage(argv[0]);

  ManuProC::PrintUncaughtExceptions();
  bool alles_ok=true;
  try{
    ManuProC::Connection conn;
    conn.setHost(dbhost);
    conn.setDbase(database);
    ManuProC::dbconnect(conn);

restart:
    alles_ok=true;
    if(instanz!=ppsInstanzID::None) 
      alles_ok&=check_for(argv[0],cH_ppsInstanz(instanz),analyse_only);
    else
     { std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
       for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
        {action_flags save=actions;
         if((*i)->KundenInstanz()) actions-=b_exclude;
         if((*i)->LagerInstanz() && !(*i)->EigeneLagerKlasseImplementiert())
            actions-=b_physical;
         alles_ok&=check_for(argv[0],*i,analyse_only);
         actions=save;
        }
     }    
    if (loop && !alles_ok) 
    {  std::cout << "adjust_store: repairing again\n";
       goto restart;
    }
    
    ManuProC::dbdisconnect();
  }catch(SQLerror &e){std::cout << e<<'\n'; return 1;}
  return !alles_ok;
}


