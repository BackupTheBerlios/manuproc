// $Id: warp_order_align.cc,v 1.2 2004/11/04 17:25:43 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
#ifdef PETIG_EXTENSIONS
#include <Misc/dbconnect.h>
#include <Misc/Query.h>
#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintrag.h>
#include <Ketten/KettplanKette.h>
#include <set>

int main()
{ ManuProC::dbconnect();

  std::vector<Kette> warps;
  std::set<AufEintragBase> valid_orders;
  Query("select maschine,schaerdatum "
    "from ketten where abgewebt is null and geloescht is null")
      .FetchArray(warps);
  for (std::vector<Kette>::const_iterator i=warps.begin();i!=warps.end();++i)
  { AuftragBase aubase(ppsInstanzID::Weberei,
            (i->Schaerdatum().Jahr()%100)*10000+i->Schaerdatum().Monat()*100
              +i->Schaerdatum().Tag());
    Auftrag *a(0);
    try
    { a=new Auftrag(aubase);
    }
    catch (SQLerror &e)
    { if (e.Code()==100)
      { a=new Auftrag(Auftrag::Anlegen2(aubase),Kunde::default_id);
      }
      else throw;
    }
    assert(a);
    KettplanKette kpk(*i);
    std::vector<ArtikelGang> ag=kpk.get_artikel_sorted();
    unsigned idx=1;
    for (std::vector<ArtikelGang>::const_iterator j=ag.begin();j!=ag.end();
          ++j,++idx)
    { AufEintragBase aeb(*a,i->Maschine()*10+idx);
      valid_orders.insert(aeb);
      AufEintrag *orderentry(0);
      try
      { orderentry=new AufEintrag(aeb);
      }
      catch (AufEintrag::NoAEB_Error &e)
      { a->insert(aeb.ZNr(),j->Gaenge()*kpk.Kettlaenge(),
            // Datum???
            i->Schaerdatum()+30,j->Artikel(),
            OPEN,true);
        orderentry=new AufEintrag(aeb);
      }
      catch (std::exception &e)
      { std::cerr << e.what() << '\n';
      }
    }
  }
  ManuProC::dbdisconnect();
  return 0;
}

#else
int main() {}
#endif
