/* $Id: sqlAuftragSelector.h,v 1.11 2001/12/05 07:55:59 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef SQLSELECTORH
#define SQLSELECTORH

#include<string>
#include<Artikel/ArtikelBase.h>
#include<Auftrag/AuftragBase.h>
#include <Auftrag/AufEintragBase2.h>
#include<Aux/ppsInstanz.h>
#include <vector>

class SQLFullAuftragSelector // : public SQLAuftragSelector
{
 std::string clausel;

public:
 struct sel_Status
  { sel_Status(ppsInstanz::ID in, int st) : instanz(in),status(st) {}
    ppsInstanz::ID instanz;
    int status;
  };  
 SQLFullAuftragSelector(const sel_Status& selstr);

 struct sel_Aufid
  { sel_Aufid(const AuftragBase& a) : auftrag(a) {}
    AuftragBase auftrag;
  };
 SQLFullAuftragSelector(const sel_Aufid& selstr);

 struct sel_AufidZnr
  { AufEintragBase2 auftrag_znr;
    sel_AufidZnr(const AufEintragBase2& a) : auftrag_znr(a) {}
  };
 SQLFullAuftragSelector(const sel_AufidZnr& selstr);

 struct sel_Jahr_Artikel 
// wird von bestserv verwendet, sollte auch erfüllte Einträge selektieren
  { unsigned int jahr; // jahrgang _oder_ Lieferdatum in diesem Jahr
    std::vector <ArtikelBase::ID> artikelid;
    ppsInstanz::ID instanz;

    sel_Jahr_Artikel(ppsInstanz::ID i, unsigned int j, ArtikelBase::ID a) 
    	: jahr(j), instanz(i) { artikelid.push_back(a); }
    sel_Jahr_Artikel(ppsInstanz::ID i, unsigned int j,const vector<ArtikelBase::ID> &a) 
    	: jahr(j), artikelid(a), instanz(i) {}
  };
 SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr);

 struct sel_Kunde_Artikel
// wird zum Abschreiben verwendet
// d.h. sortiert nach Lieferdatum (asc)
  { unsigned int kundennr;
    unsigned int artikelid;
    ppsInstanz::ID instanz;
    sel_Kunde_Artikel(ppsInstanz::ID i, unsigned int k,unsigned int a) 
    : kundennr(k), artikelid(a), instanz(i)
    {}
  };
 SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr);

 void setClausel(const std::string &cl) { clausel = cl;}
 const std::string getClausel() const { return clausel; }
};

#endif
