// $Id: Zuordnung_zeigen.cc,v 1.2 2003/05/23 13:53:27 christof Exp $
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

#include <iostream>
#include <Misc/dbconnect.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/AufEintrag.h>
#include <Misc/exception.h>

static void show(const AufEintrag &ae)
{  std::cout << ae << ": " << ae.getStueck();
   if (!!ae.getGeliefert()) std::cout << '(' << ae.getGeliefert() << ')';
   std::cout << ' ' << cH_ArtikelBezeichnung(ae.Artikel())->Bezeichnung()
      << ' ' << ae.getLieferdatum() << '\n';
}

static void show(const AufEintragBase &ae, bool kinder, int indent=0)
{  AufEintragZu::list_t l=AufEintragZu::get_Referenz_list(ae,kinder,AufEintragZu::list_ohneArtikel);
   ++indent;
   for (AufEintragZu::list_t::const_iterator i=l.begin();i!=l.end();++i)
   {  std::cout << std::string(indent,' ') << (kinder?"=":"<=") 
   	<< i->Menge << (kinder?"=>":"=");
      show(i->AEB);
      show(i->AEB,kinder,indent);
   }
}

int main(int argc,char *argv[])
{  if (argc<4) 
   {  std::cerr << "Usage: " << argv[0] << " <instanz> <id> <zeile>\n";
      return 1;
   }
   ppsInstanz::ID instanz=ppsInstanz::ID(atoi(argv[1]));
   int id=atoi(argv[2]),zeile=atoi(argv[3]);
   if (int(instanz)<1 || id<0 || zeile<1) return 2;

   ManuProC::PrintUncaughtExceptions();
   ManuProC::dbconnect();
   
   show(AufEintragBase(instanz,id,zeile));
   show(AufEintragBase(instanz,id,zeile),AufEintragZu::list_kinder);
   std::cout << "\nEltern\n";
   show(AufEintragBase(instanz,id,zeile),AufEintragZu::list_eltern);
   ManuProC::dbdisconnect();
}
