/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Malte Thoma and Christof Petig
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

#include <ManuProCConfig.h>
#include "DataBase_init.hh"
#include "AuftragsVerwaltung.hh"
#include "Check.hh"
#include "steuerprogramm.hh"
#include <Misc/dbconnect.h>
#include <Misc/exception.h>
#include <Auftrag/Auftrag.h>
#include <Misc/Trace.h>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h> // für mkdir
#include <getopt.h>
//#include <Misc/inbetween.h>
#include "TestReihe.h"
#include <Lieferschein/RechnungBase.h>

static std::ostream *testlog;
// for more output ...
static bool verbose=false;
static bool do_not_stop=false;
static std::string header;

TestReihe *TestReihe::first;

UniqueValue::value_t log_trace = ManuProC::Tracer::channels.get();

static Check C;

void vergleichen(Check::was_checken w, const std::string &zusatz,
        const std::string &name, const std::string &graphname, bool vor_dem_test_reparieren)
{   (*testlog) << int(w) << ' ' << zusatz << ' ' << graphname << ' ' << name << '\n';
    testlog->flush(); 
    bool erfolgreich=C.teste(w,zusatz,vor_dem_test_reparieren);
    if(!erfolgreich)
    { std::cout << name << "("<<zusatz<<") fehlgeschlagen\n\n"; 
      if (!do_not_stop) exit(1);
    }
    else if (verbose)
    { std::cout << name << "("<<zusatz<<") ok\n";
    }
}

static void graphheader(const std::string &name)
{   (*testlog) << '#' << ' ' << name << '\n';
    header=name;
}

static void erfolgreich()
{   std::cout << header << " erfolgreich\n\n";
}

static int auftragstests(TestReihe *mode)
{  if (Check::analyse) std::cout << "Mit Analyse\n";
   else if (Check::reparieren) std::cout << "Mit Reparatur\n";

   graphheader(mode->bezeichnung);

   // ANLEGEN eines Auftrags mit Bandlager und Rohwarenlager
   if (mode->aufruf_mit_auftrag)
//   !in(mode,JumboLager,Zusatzinfo,Zusatzinfo2))
   {
      AuftragsVerwaltung  auftrag;
      AufEintragBase AEB=auftrag.anlegen();
      AufEintrag AE=AEB;
      std::cout << "Anlegen eines Auftrags ["<<AEB<<"] beendet\n\n";
      AE.setStatus(OPEN);
      vergleichen(Check::Menge,"mit_lager_open","Öffnen des Auftrags","");
      
      (*(mode->aufruf_mit_auftrag))(AE);
   }
   else (*(mode->aufruf))();

   if (!do_not_stop) erfolgreich();
   return 0;
}

static void usage(const std::string &argv0,const std::string &argv1)
{
  std::cerr << argv0 <<" [<option> ...] <test>\n";
  std::cerr << "options: --verbose|-v --analyse|-y --repair|-r\n"
  	"\t--continue|-c --trace|-t --reinit|-R --overwrite|-O\n"
  	"\t--tolerate|-T --resort|-S --delete|-D\n"
  	"\t\t--continue twice gives different results\n";
  std::cerr << "tests:\n";
  for (const TestReihe *i=TestReihe::first; i; i=i->next)
  {  std::cerr << '\t' << i->kuerzel << '\t' << i->bezeichnung << '\n';
  }
  exit(1);
}

int main(int argc,char *argv[])
{  putenv("LANG=C"); // omit any locale oddities/changes
   bool clean_only=true;

#if defined(PETIG_TEST) || defined(MANU_PROC_TEST)
   AuftragBase::setzeAktuellesJahr(2000);
#else
   AuftragBase::setzeAktuellesJahr(2003);
#endif
   static struct option long_options[] = {
     { "verbose", no_argument, 0, 'v' },
     { "repair", no_argument, 0, 'r' },
     { "continue", no_argument, 0, 'c' },
     { "trace", no_argument, 0, 't' },
     { "clean-only", no_argument, 0, 'C' },
     { "reinit", no_argument, 0, 'R' },
     { "analyse", no_argument, 0, 'y' },
     { "overwrite", no_argument, 0, 'O' },
     { "tolerate", no_argument, 0, 'T' },
     { "resort", no_argument, 0, 'S' },
     { "delete", no_argument, 0, 'D' },
     { 0,0,0,0 },
   };
   
   int opt;
   while ((opt=getopt_long(argc,argv,"vrctCRyOTSD",long_options,0))!=-1)
    switch(opt)
   {  case 'v': verbose=true; Check::verbose=true; break;
      case 'r': Check::reparieren=true; break;
      case 'c': if (!Check::continue_)
            Check::continue_=true; 
         else do_not_stop=true;
         break;
      case 'C': clean_only=true; break;
      case 'D': Check::delete_repair=true; break;
      case 'R': clean_only=false; break;
      case 'y': Check::analyse=true; break;
      case 'S': Check::resort=true; break;
      case 'T': AuftragBase::tolerate_inconsistency=true; break;
      case 't': ManuProC::Tracer::Enable(AuftragBase::trace_channel);
        ManuProC::Tracer::Enable(LieferscheinBase::trace_channel);
        ManuProC::Tracer::Enable(RechnungBase::trace_channel);
      	ManuProC::Tracer::Enable(log_trace);
      	break;
      case 'O': Check::overwrite=true; break;
      default: usage(argv[0],""); return 1;
   }

   if(argc-optind!=1) { usage(argv[0],""); return 1; }
   std::string mode_str=argv[optind];

#ifdef MANU_PROC_TEST
   putenv("PGDATABASE=anleitungdb");
#elif defined MABELLA_TEST
   putenv("PGDATABASE=mabella_testdb");
#elif defined PETIG_TEST
   putenv("PGDATABASE=testdb");
#endif

  TestReihe *mode=0;
  for (mode=TestReihe::first; mode; mode=mode->next)
  {  if (mode_str==mode->kuerzel) break; 
  }
   if(!mode) { usage(argv[0],mode_str); return 1; }

   std::cout << "Initalisierung der Datenbank ...";
   std::cout.flush();

   if (clean_only) // test for existance
   {  try
      {  ManuProC::dbconnect();
         Kunde k(1);
         ManuProC::dbdisconnect();
      }
      catch (SQLerror &e)
      {  clean_only=false;
      }
   }

   std::string kill_output=" 2>/dev/null >/dev/null";
   if (verbose) kill_output="";

   if (clean_only)
   {  system((std::string("./clean_db ")+getenv("PGDATABASE")+kill_output).c_str());
      if (!access((std::string(MANU_DATAPATH)+"/clean_db2").c_str(),X_OK))
         system((std::string("cd ")+MANU_DATAPATH+"; ./clean_db2 "+getenv("PGDATABASE")+kill_output).c_str());
   }
   else
      system((std::string(MANU_DATAPATH)+"/initdb.script "+MANU_DATAPATH+kill_output).c_str());
   std::cout << "...beendet\n";

   Petig::PrintUncaughtExceptions();
   try{
   mkdir("files.log",0777);
   mkdir("results",0777);
   std::ofstream logstream(("files.log/"+mode_str).c_str());
   testlog=&logstream;
   ManuProC::dbconnect();

   DataBase_init();
   return auftragstests(mode);

   }catch(SQLerror &e){std::cout << e<<'\n';}
  return 1;
}

