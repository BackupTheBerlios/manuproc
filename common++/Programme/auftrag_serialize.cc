// $Id: auftrag_serialize.cc,v 1.1 2006/08/03 11:17:31 christof Exp $
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
#include <Misc/Query.h>

static void show(const AufEintrag &ae)
{  std::cout << ae << ": " << ae.getStueck();
   if (!!ae.getGeliefert()) std::cout << '(' << ae.getGeliefert() << ')';
   std::cout << ' ' << cH_ArtikelBezeichnung(ae.Artikel())->Bezeichnung()
      << ' ' << ae.getLieferdatum();
   if (ae.getCombinedStatus()==UNCOMMITED) std::cout << " UNCOMMITTED";
   else if (ae.getCombinedStatus()==CLOSED) std::cout << " CLOSED";
   else if (ae.getCombinedStatus()==STORNO) std::cout << " STORNO";
   std::cout << '\n';
}

static void show(const AufEintragBase &ae, bool kinder, int indent=0)
{  AufEintragZu::list_t l=AufEintragZu::get_Referenz_list(ae,kinder,AufEintragZu::list_ohneArtikel);
   ++indent;
   for (AufEintragZu::list_t::const_iterator i=l.begin();i!=l.end();++i)
   {  std::cout << std::string(indent,' ') << (kinder?"=":"<=") 
   	<< i->Menge << (kinder?"=>":"=");
      try 
      {  show(i->AEB);
         show(i->AEB,kinder,indent);
      } catch (...) // SQLerror &e)
      {  std::cout << i->AEB << " fehlt !!!\n";
      }
   }
}

static void show_all(const AufEintragBase &ae)
{  show(ae);
   show(ae,AufEintragZu::list_kinder);
   show(ae,AufEintragZu::list_eltern);

}

int main(int argc,char *argv[])
{  if (argc<4) 
   {  std::cerr << "Usage: " << argv[0] << " <instanz> <id> <zeile>\n";
      return 1;
   }
   ppsInstanz::ID instanz=ppsInstanz::ID(atoi(argv[1]));
   int id=atoi(argv[2]),zeile=atoi(argv[3]);
   if (int(instanz)<1 || id<0) return 2;

   ManuProC::PrintUncaughtExceptions();
   ManuProC::dbconnect();

   if (zeile>0)
   { show_all(AufEintragBase(instanz,id,zeile));
   }
   else
   {  Query q("select zeilennr from auftragentry "
   	"where (instanz,auftragid)=(?,?) order by zeilennr");
      q << AuftragBase(instanz,id);
      Query::Row is;
      while ((q>>is).good())
      {  show_all(AufEintragBase(instanz,id,is.Fetch<int>()));
      }
   }
   ManuProC::dbdisconnect();
}
