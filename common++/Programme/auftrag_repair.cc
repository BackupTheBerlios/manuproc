// $Id: auftrag_repair.cc,v 1.17 2005/10/27 13:11:29 christof Exp $
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

enum action_bits { b_physical, b_exclude, b_tree, b_raise, b_links, b_minmenge };

// auf bitmask<action_bits> umstellen;
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
           "\tL: Löschen von ungültigen Zuordnungen (ohne Quelle oder Ziel)\n"
           "\tD: Löschen von ungültigen Einträgen (Vorsicht)\n"
           "\tN: Unschöne aber mögliche Zustände akzeptieren (für test)\n"
           "\tM: Mindestmenge auf Lager sicherstellen (Überbestellungen kappen)\n"
           "\t*: Alle Analysen/Reparaturen auf einmal (meist mit -I)\n";

 std::cerr << "USAGE:  ";
 std::cerr << s <<" [-i<instanz>|-I]  -a<aktion> [-d<database>] [-h<dbhost>] [-l|-y] \n"
           "\twobei die aktion=[A|C|X|T|M|*] [-A<Id>] ist.\n"
           "\t-y Analysemodus (keine Reparaturen)\n"
           "\t-s Warnungen unterdrücken\n"
           "\t-l Reparatur wiederholen bis keine Fehler mehr auftreten\n"
	   "\t-A nur für den Artikel mit der Id durchführen\n"
	   "\t-L Anzahl der Wiederholungen (99)\n"
           "\t-I führt die Tests für alle Instanzen durch\n\n"
           "most common use:\n"
           "\t./auftrag_repair -l -I -a\\*\trepair all (-aD to delete unneeded)\n";
 exit(1);
}

namespace { struct loopArgs
{ std::string const& pname;
  cH_ppsInstanz I;
  const bool analyse_only;
  const ArtikelBase::ID aid;
  bool alles_ok;
  bool reloop;
  ppsInstanzReparatur RI;

  loopArgs(std::string const&p,cH_ppsInstanz i, bool a,ArtikelBase::ID ai,
              ppsInstanzReparatur const& r)
    : pname(p), I(i), analyse_only(a), aid(ai), alles_ok(true), reloop(), 
      RI(r) {}
  void callback(AufEintrag &ae);
};}

void loopArgs::callback(AufEintrag &ae)
{ if (reloop) return;
  // eine erfolgreiche 0er/2er Reparatur wird im Lager auch 1er betreffen
  // d.h. danach sind alle alten Daten hinfällig
  if (actions&b_exclude)
  { reloop=!RI.Reparatur_0er_und_2er(ae,analyse_only);
    alles_ok&=!reloop;
  }
  if (!reloop && actions&b_tree)
  {
        try
         {AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(ae,
       			AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
       	  alles_ok&=RI.Eltern(ae,eltern,analyse_only,actions&b_raise);
       	  alles_ok&=RI.Lokal(ae,analyse_only);
       	  AufEintragZu::map_t kinder=AufEintragZu::get_Kinder_nach_Artikel(ae);
       	  alles_ok&=RI.Kinder(ae,kinder,analyse_only);
         } catch (SQLerror &e)
         {  std::cout << "SQL Fehler " << e << '\n';
            alles_ok=false;
         } catch (std::exception &e)
         {  std::cout << "Exception " << e.what() << '\n';
            alles_ok=false;
         }
  }
}

static bool check_for(const std::string &pname,cH_ppsInstanz I,
		const bool analyse_only,const ArtikelBase::ID aid)
{  AuftragBase::tolerate_inconsistency=true;
   ppsInstanzReparatur RI(I->Id());
   bool alles_ok=true;
    if (actions&b_minmenge && I->LagerInstanz())
    {  std::string qstr("select id from artikelstamm "
          // gar keine Lagermenge vorhanden
          "where (not exists "
          "(select true from auftragentry "
          "where (instanz,auftragid,artikelid)=(?,?,artikelstamm.id))"
          // oder zu wenige freie Menge
          " or exists "
          "(select true from auftragentry "
          "where (instanz,auftragid,artikelid)=(?,?,artikelstamm.id)"
          "and bestellt<mindbestand)"
          // oder genügend freie Menge bei gleichzeitigen Nachbestellungen
          " or (exists "
          "(select true from auftragentry "
          "where (instanz,auftragid,artikelid)=(?,?,artikelstamm.id)"
          "and bestellt>=mindbestand "
          "and exists "
          "(select true from auftragsentryzuordnung where "
          "(altinstanz,altauftragid,altzeilennr)=(instanz,auftragid,zeilennr)"
          ")))"
          ") "
          // und wird bei dieser Instanz bestellt (parallelInstanzen fehlen)
          // Einschränkung notwendig?
          "and (bestellen_bei=? or exists "
          "(select true from prod_instanz where lager_fuer=? and insid=bestellen_bei)"
          ") "
          // und ein Mindestbestand ist erforderlich
          "and mindbestand>0");
       if(aid!=ArtikelBase::none_id)
          qstr+=" and id="+itos(aid);
   
       Query q(qstr);
       q << I->Id() << AuftragBase::dispo_id
         << I->Id() << AuftragBase::dispo_id
         << I->Id() << AuftragBase::dispo_id
         << I->Id() << I->Id();
       std::vector<ArtikelBase> arr;
       q.FetchArray(arr);
       for (std::vector<ArtikelBase>::const_iterator i=arr.begin();i!=arr.end();++i)
       {  alles_ok&=RI.Reparatur_MindestMenge(analyse_only,*i);
       }
       qstr="select id from auftragentry join artikelstamm "
         "on (artikelstamm.id=auftragentry.artikelid ";
      if(aid!=ArtikelBase::none_id)
          qstr+=" and artikelstamm.id="+itos(aid)+") ";  
      else
          qstr+=" ) ";    
       qstr+="where (mindbestand is null or mindbestand=0) "
         "and (instanz,auftragid)=(?,?) "
         "and exists (select true from auftragsentryzuordnung "
         "where (altauftragid,altinstanz)"
         "=(auftragentry.instanz,auftragentry.auftragid))";

      Query q2(qstr);
       q2 << I->Id() << AuftragBase::dispo_id;
       q2.FetchArray(arr);
       for (std::vector<ArtikelBase>::const_iterator i=arr.begin();i!=arr.end();++i)
       {  alles_ok&=RI.Reparatur_MindestMenge(analyse_only,*i);
       }
    }
    if (actions&b_physical)
     {
      if(I->EigeneLagerKlasseImplementiert())
        try
        {alles_ok&=RI.ReparaturLager(analyse_only,aid);
        } catch (SQLerror &e)
        {  std::cout << "SQL Fehler " << e << '\n';
           alles_ok=false;
        }
      else if (!(actions&b_tree)) // Meldung bei * unterdrücken
         std::cout << "\t"<< I << " 'A' nicht sinnvoll\n";
     }
     int loops2=0,limit2=10;
   reload:
    if (actions&b_tree || actions&b_exclude)
    {  SQLFullAuftragSelector psel=
		SQLFullAuftragSelector::sel_InstanzAlle(I->Id(),aid);
#if 1 // less memory, slightly more database traffic		
       loopArgs la(pname,I,analyse_only,aid,RI);
       SelectedFullAufList::loop(psel,la,&loopArgs::callback);
       alles_ok &= la.alles_ok;
       if (la.reloop) { ++loops2; if (loops2<limit2) goto reload; }
#else       
       SelectedFullAufList K(psel);
      if (actions&b_exclude)
      {  alles_ok&=RI.Reparatur_0er_und_2er(K,analyse_only);
        // diese F. verändert zwar Teile von K (aber im Lager auch 1er an K vorbei)
         if (!alles_ok) { ++loops2; if (loops2<limit2) goto reload; }
      }
      if (actions&b_tree)
      {for(SelectedFullAufList::iterator i = K.begin();i!=K.end(); ++i)
        {try
         {AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(*i,
       			AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
       	  alles_ok&=RI.Eltern(*i,eltern,analyse_only,actions&b_raise);
       	  alles_ok&=RI.Lokal(*i,analyse_only);
       	  AufEintragZu::map_t kinder=AufEintragZu::get_Kinder_nach_Artikel(*i);
       	  alles_ok&=RI.Kinder(*i,kinder,analyse_only);
         } catch (SQLerror &e)
         {  std::cout << "SQL Fehler " << e << '\n';
            alles_ok=false;
         } catch (std::exception &e)
         {  std::cout << "Exception " << e.what() << '\n';
            alles_ok=false;
         }
        }
      }
#endif      
    }
   return alles_ok;
}



const static struct option options[]=
{{ "instanz", required_argument, NULL, 'i' },
 { "aktion", required_argument, NULL, 'a' },
 { "database", required_argument,      NULL, 'd' },
 { "dbhost", required_argument,      NULL, 'h' },
 { "analyse_only", no_argument,      NULL, 'y' },
 { "silence-warnings", no_argument,      NULL, 's' },
 { "alle_Instanzen", no_argument,      NULL, 'I' },
 { NULL,      0,       NULL, 0 }
};

int main(int argc,char *argv[])
{
  int opt;
  ppsInstanz::ID instanz= ppsInstanzID::None;
  ArtikelBase::ID artikelid=ArtikelBase::none_id;
  std::string database="";
  std::string dbhost="";
  bool analyse_only=false;
  bool all_instanz=false;
  bool loop=false;
  int loops=0,limit=99;

  if(argc==1) usage(argv[0]);
  while ((opt=getopt_long(argc,argv,"h:d:i:a:yItlsA:L:",options,NULL))!=EOF)
   {
    switch(opt)
     {
       case 'i' : instanz = ppsInstanz::ID(atoi(optarg));break;
       case 'I' : all_instanz=true;break;
       case 'a' : if (strchr(optarg,'A')||strchr(optarg,'*')) actions|=b_physical;
          if (strchr(optarg,'C')||strchr(optarg,'*')) actions|=b_exclude;
          if (strchr(optarg,'R')||strchr(optarg,'*')) actions|=b_raise;
          if (strchr(optarg,'X')||strchr(optarg,'*')||strchr(optarg,'R')) actions|=b_tree;
          if (strchr(optarg,'L')||strchr(optarg,'*')) actions|=b_links;
          if (strchr(optarg,'M')||strchr(optarg,'*')) actions|=b_minmenge;
          if (strchr(optarg,'D')) ppsInstanzReparatur::really_delete=true;
          if (strchr(optarg,'N')) ppsInstanzReparatur::not_strict=true;
          break;
       case 'd' : database=optarg;break;
       case 'h' : dbhost=optarg;break;
       case 'y' : analyse_only=true;break;
       case 'l' : loop=true; break;
       case 'L' : limit=atoi(optarg); break;
       case 's' : ppsInstanzReparatur::silence_warnings=true; break;
       case 't' : ManuProC::Tracer::Enable(AuftragBase::trace_channel); break;
       case 'A' : artikelid=atoi(optarg);break;
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
    ++loops;
    alles_ok=true;
    if (actions&b_links && !analyse_only)
    {  FetchIStream is;
       Query q("select instanz,auftragid,zeilennr from auftragentry "
       		"where not exists (select true from auftrag "
       		"where (auftragentry.instanz,auftragentry.auftragid)="
       		"(auftrag.instanz,auftrag.auftragid))");
       while ((q>>is).good())
       {  AufEintragBase aeb;
          is >> aeb >> Query::check_eol();
          std::cout << "Verwaister AufEintrag (ohne Auftrag) " << aeb << "\n";
          if (!analyse_only && 
          	(ppsInstanzReparatur::really_delete || aeb.Id()<AufEintrag::handplan_id))
             Query("delete from auftragentry where (instanz,auftragid,zeilennr)="
             	"(?,?,?)") << aeb;
       }
       if (!analyse_only)
       {Query("delete from auftragsentryzuordnung where not exists "
    	"(select true from auftragentry where (instanz,auftragid,zeilennr)="
    		"(altinstanz,altauftragid,altzeilennr)) or not exists "
    	"(select true from auftragentry where (instanz,auftragid,zeilennr)="
    		"(neuinstanz,neuauftragid,neuzeilennr))");
        if (Query::Lines())
        {  std::cout << Query::Lines() << " ungültige Zuordnungen gelöscht\n";
        }
       }
    }
    if(instanz!=ppsInstanzID::None)
	alles_ok&=check_for(argv[0],cH_ppsInstanz(instanz),analyse_only,artikelid);
    else
     { std::vector<cH_ppsInstanz> VI=cH_ppsInstanz::get_all_instanz();
       for(std::vector<cH_ppsInstanz>::const_iterator i=VI.begin();i!=VI.end();++i)
        {action_flags save=actions;
         if((*i)->KundenInstanz()) actions-=b_exclude;
         if((*i)->LagerInstanz() && !(*i)->EigeneLagerKlasseImplementiert())
            actions-=b_physical;
         alles_ok&=check_for(argv[0],*i,analyse_only,artikelid);
         actions=save;
        }
     }
    if (loop && !alles_ok && loops < limit)
    {  std::cout << "adjust_store: repairing again\n";
       goto restart;
    }

    if (loop && loops>1)
       std::cout << "adjust_store: repairing took " << loops << " loops\n";
    ManuProC::dbdisconnect();
  }catch(SQLerror &e){std::cout << e<<'\n'; return 1;}
  return !alles_ok;
}
