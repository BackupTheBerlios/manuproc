/* $Id: sqlAuftragSelector.h,v 1.22 2003/04/28 09:29:46 christof Exp $ */
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
#include <Auftrag/AufEintragBase.h>
#include<Instanzen/ppsInstanz.h>
#include <vector>

class SQLFullAuftragSelector // : public SQLAuftragSelector
{
 std::string clausel;

 static std::string StatusQualifier(AufStatVal v);
 static std::string IDQualifier(AuftragBase::ID id);

public:
 SQLFullAuftragSelector() {}

 // id=AuftragBase::none_id => Alle Aufträge
 // id=AuftragBase::plan_auftrag_id => Alle außer 0er und 2er
 // id=ungeplante_id|dispo_auftrag_id => as is
 struct sel_Status
  { sel_Status(ppsInstanz::ID in, AufStatVal st,AuftragBase::ID _id) 
    : instanz(in),status(st),id(_id) {}
    ppsInstanz::ID instanz;
    AufStatVal status;
    AuftragBase::ID id; 
  };  

 SQLFullAuftragSelector(const sel_Status& selstr);

 struct sel_Aufid
  { sel_Aufid(const AuftragBase& a,const bool w=true) 
      : auftrag(a),with_storno(w) {}
    AuftragBase auftrag;
    bool with_storno;
  };
 SQLFullAuftragSelector(const sel_Aufid& selstr,
 		const std::string artbez_table="artbez_3_1");

 struct sel_AufidZnr
  { AufEintragBase auftrag_znr;
    sel_AufidZnr(const AufEintragBase& a) : auftrag_znr(a) {}
  };
 SQLFullAuftragSelector(const sel_AufidZnr& selstr);

 struct sel_Jahr_Artikel 
// wird von bestserv verwendet, sollte auch erfüllte Einträge selektieren
  { unsigned int jahr; // jahrgang _oder_ Lieferdatum in diesem Jahr
    std::vector <ArtikelBase::ID> artikelid;
    ppsInstanz::ID instanz;

    sel_Jahr_Artikel(ppsInstanz::ID i, unsigned int j, ArtikelBase::ID a) 
    	: jahr(j), instanz(i) { artikelid.push_back(a); }
    sel_Jahr_Artikel(ppsInstanz::ID i, unsigned int j,const std::vector<ArtikelBase::ID> &a) 
    	: jahr(j), artikelid(a), instanz(i) {}
  };
 SQLFullAuftragSelector(const sel_Jahr_Artikel &selstr);

 struct sel_Kunde_Artikel
// wird zum Abschreiben verwendet
// d.h. sortiert nach Lieferdatum (asc)
  { Kunde::ID kundennr;
    ArtikelBase artikel;
    ppsInstanz::ID instanz;
    sel_Kunde_Artikel(ppsInstanz::ID i, Kunde::ID k,ArtikelBase a) 
    : kundennr(k), artikel(a), instanz(i)
    {}
  };
 SQLFullAuftragSelector(const sel_Kunde_Artikel &selstr);

 // sortiert nach Lieferdatum (asc) für OPEN
 struct sel_Artikel_Planung_id
  { ppsInstanz::ID instanz;
    Kunde::ID kunde;
    ArtikelBase artikel;
    int auftragid;
    AufStatVal status;
    Petig::Datum lieferdatum;
    
    sel_Artikel_Planung_id(ppsInstanz::ID i, Kunde::ID k,
                           ArtikelBase a, int _id,
                           AufStatVal s=OPEN,Petig::Datum d=Petig::Datum()) 
    : instanz(i), kunde(k),artikel(a), auftragid(_id), status(s),lieferdatum(d)
    {}
  };
 SQLFullAuftragSelector(const sel_Artikel_Planung_id &selstr);

 struct sel_Artikel
  { ppsInstanz::ID instanz;
    ArtikelBase artikel;
    
    sel_Artikel(ppsInstanz::ID i, ArtikelBase a) 
    : instanz(i), artikel(a)
    {}
  };
 SQLFullAuftragSelector(const sel_Artikel &selstr);

 struct sel_Kunde_Status
  { Kunde::ID kundennr;
    ppsInstanz::ID instanz;
    AufStatVal stat;
    
    sel_Kunde_Status(ppsInstanz::ID i, Kunde::ID k, AufStatVal s) 
    : kundennr(k), instanz(i), stat(s)
    {}
  };
 SQLFullAuftragSelector(const sel_Kunde_Status &selstr);

 struct sel_InstanzAlle
  { ppsInstanz::ID instanz;
    
    sel_InstanzAlle(ppsInstanz::ID i) 
    : instanz(i)
    {}
  };
 SQLFullAuftragSelector(const sel_InstanzAlle &selstr);

 void setClausel(const std::string &cl) { clausel = cl;}
 const std::string getClausel() const { return clausel; }
};

#endif
