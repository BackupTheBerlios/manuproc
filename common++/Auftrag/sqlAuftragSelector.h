/* $Id: sqlAuftragSelector.h,v 1.1 2001/04/23 08:11:58 christof Exp $ */
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

#include<string>
#include<Artikel/ArtikelBase.h>

#ifndef SQLSELECTORH
#define SQLSELECTORH

class SQLAuftragSelector
{
 string clausel;
 
public:
 struct sel_Aufid
  { sel_Aufid(int id) : aufid(id) {}
    int aufid;
  };
  
 struct sel_AufidZnr
  { sel_AufidZnr(int id, int zeile) : aufid(id), znr(zeile) {}
    int aufid;
    int znr;
  };
  
 struct sel_Jahr
  { sel_Jahr(int j) : jahrgang(j) {}
    int jahrgang;
  };  
  
 struct sel_Jahr_Artikel 
// wird von bestserv verwendet, sollte auch erf�llte Eintr�ge selektieren
  { unsigned int jahr; // jahrgang _oder_ Lieferdatum in diesem Jahr
    unsigned int artikelid;
    sel_Jahr_Artikel(unsigned int j,unsigned int a) : jahr(j), artikelid(a) {}
  };
  
 struct sel_Kunde_Artikel
// wird zum Abschreiben verwendet
// d.h. sortiert nach Lieferdatum (asc)
  { unsigned int kundennr;
    unsigned int artikelid;
    sel_Kunde_Artikel(unsigned int k,unsigned int a) : kundennr(k), artikelid(a)
    {}
  };
  
 struct sel_Status
  { sel_Status(int st) : status(st) {}
    int status;
  };  

 struct sel_Status_Mab
  { sel_Status_Mab(const string st) : status(st) {}
    string status;
  };  

#if 0  // deprecated
 struct sel_KdArtBrFbAuf
  { sel_KdArtBrFbAuf(int kd,int artnr,int artbr, int artfb, int aufm ) :
  	kdnr(kd),anr(artnr),abr(artbr),afb(artfb),aauf(aufm) {}
    int kdnr;
    int anr;
    int abr;
    int afb;
    int aauf;
  };  
#endif

 struct sel_KdArtikel
  { sel_KdArtikel(int kd, const ArtikelBase &a) :
  	kdnr(kd),artikel(a) {}
    int kdnr;
    ArtikelBase artikel;
  };  



 SQLAuftragSelector(const sel_Aufid& selstr);
 SQLAuftragSelector(const sel_Status& selstr, int aid=0);
 SQLAuftragSelector(const sel_Status_Mab& selstr);
 SQLAuftragSelector(const sel_Jahr& selstr, char *order=0);
 SQLAuftragSelector(const sel_Status& selstr, char *order);
// SQLAuftragSelector(const sel_KdArtBrFbAuf& selstr);
 SQLAuftragSelector(const sel_KdArtikel& selstr);
 SQLAuftragSelector();
 	
 void setClausel(const string &cl) { clausel = cl;}
 const char *getClausel() const { return clausel.c_str(); }
 

};


class SQLFullAuftragSelector : public SQLAuftragSelector
{
public:
 SQLFullAuftragSelector(const sel_Status& selstr);
 SQLFullAuftragSelector(const sel_Aufid& selstr);
 SQLFullAuftragSelector(const sel_AufidZnr& selstr);
 SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr);
 SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr);
};

#endif

