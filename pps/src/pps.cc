/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// generated 2000/11/22 20:43:27 MET by jacek@mimi.
// using glademm V0.5.10
//
// newer (non customized) versions of this file go to auftrag.cc_glade

// This file is for your program, I won't touch it again!

#include <gnome--/main.h>

#include "auftrag_main.hh"
#include "auftrag_bearbeiten.hh"
#include"auftrag_lieferschein.hh"
#include"auftragbase.h"
#include<Aux/exception.h>

#include<Aux/dbconnect.h>


MyMessage *meldung;
auftrag_main *auftragmain;
auftrag_bearbeiten *auftragbearbeiten;

int main(int argc, char **argv)
{  
 ppsInstanz instanz;

 Petig::Connection conn;
 conn.setDbase("petigdb");
 int i;

 while ((i = getopt(argc, argv, "h:d:")) != EOF)
 switch (i)
   {  case 'h':
         conn.setHost(optarg);
         break;
      case 'd':
         conn.setDbase(optarg);
         break;
      default : break;
   }

#if defined(ENABLE_NLS)
   bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
   textdomain (PACKAGE);
#endif ///* ENABLE_NLS*/
   Gnome::Main m("auftrag", "0.0", 1, argv);
 Petig::PrintUncaughtExceptions();

 meldung = new MyMessage();
 try{ Petig::dbconnect(conn); }
  catch(SQLerror &e)
  { meldung->Show(e);
    return 1;
  }
  
 if (argc-optind>1) instanz=ppsInstanz::ppsInstId(atoi(argv[optind]));
 else instanz=ppsInstanz(ppsInstanz::INST_KNDAUF);

  auftragmain = new auftrag_main();
// auftrag_lieferschein *al = new auftrag_lieferschein();


weiter:

 try{ m.run();}
   catch(SQLerror &e) {meldung->Show(e); goto weiter;}

 delete auftragmain;
 Petig::dbdisconnect_nt(conn.Dbase());
 delete meldung;
 return 0;
}
