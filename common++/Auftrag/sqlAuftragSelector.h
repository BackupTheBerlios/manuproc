/* $Id: sqlAuftragSelector.h,v 1.5 2001/06/25 09:40:42 christof Exp $ */
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
#include<Aux/ppsInstanz.h>

#if 0
class SQLAuftragSelector
{
 string clausel;
 
public:
/*
 struct sel_Aufid
  { sel_Aufid(ppsInstanz::ppsInstId _i, int id) : instanz(_i), aufid(id) {}
    ppsInstanz::ppsInstId instanz;
    int aufid;
  };
 struct sel_AufidZnr
  { sel_AufidZnr(ppsInstanz::ppsInstId _i, int id, int zeile) : instanz(_i), aufid(id), znr(zeile) {}
    ppsInstanz::ppsInstId instanz;
    int aufid;
    int znr;
  };
*/  
  
 struct sel_Jahr
  { sel_Jahr(int j) : jahrgang(j) {}
    int jahrgang;
  };  
  
  
 struct sel_Status_Mab
  { sel_Status_Mab(const string st) : status(st) {}
    string status;
  };  

 struct sel_KdArtikel
  { sel_KdArtikel(int kd, const ArtikelBase &a) :
  	kdnr(kd),artikel(a) {}
    int kdnr;
    ArtikelBase artikel;
  };  

 SQLAuftragSelector(const sel_Status& selstr, int aid=0);
 SQLAuftragSelector(const sel_Status_Mab& selstr);
 SQLAuftragSelector(const sel_Jahr& selstr, char *order=0);
 SQLAuftragSelector(const sel_Status& selstr, char *order);
// SQLAuftragSelector(const sel_KdArtBrFbAuf& selstr);
 SQLAuftragSelector(const sel_KdArtikel& selstr);
 SQLAuftragSelector();
 	
};
#endif


class SQLFullAuftragSelector // : public SQLAuftragSelector
{
 string clausel;
public:
 struct sel_Status
  { sel_Status(int st) : status(st) {}
    int status;
  };  

 SQLFullAuftragSelector(const sel_Status& selstr);
 struct sel_Aufid
  { sel_Aufid(const AuftragBase& a) : auftrag(a) {}
    AuftragBase auftrag;
  };
 SQLFullAuftragSelector(const sel_Aufid& selstr);
 struct sel_AufidZnr
  { sel_AufidZnr(const AuftragBase& a, int zeile) : auftrag(a), znr(zeile) {}
    AuftragBase auftrag;
    int znr;
  };
 SQLFullAuftragSelector(const sel_AufidZnr& selstr);
 struct sel_Jahr_Artikel 
// wird von bestserv verwendet, sollte auch erfüllte Einträge selektieren
  { unsigned int jahr; // jahrgang _oder_ Lieferdatum in diesem Jahr
    unsigned int artikelid;
    sel_Jahr_Artikel(unsigned int j,unsigned int a) : jahr(j), artikelid(a) {}
  };
  
 SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr);
 struct sel_Kunde_Artikel
// wird zum Abschreiben verwendet
// d.h. sortiert nach Lieferdatum (asc)
  { unsigned int kundennr;
    unsigned int artikelid;
    sel_Kunde_Artikel(unsigned int k,unsigned int a) : kundennr(k), artikelid(a)
    {}
  };
 SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr);

 void setClausel(const string &cl) { clausel = cl;}
 const string getClausel() const { return clausel; }
};

#endif
