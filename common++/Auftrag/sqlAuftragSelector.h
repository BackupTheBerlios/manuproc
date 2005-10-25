/* $Id: sqlAuftragSelector.h,v 1.40 2005/10/25 12:13:14 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2005 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski
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

#include <string>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/AuftragBase.h>
#include <Auftrag/AufEintragBase.h>
#include <Instanzen/ppsInstanz.h>
#include <vector>
#include <Lager/FertigWarenLager.h>
#include <Misc/auto_init.h>

class SQLFullAuftragSelector // : public SQLAuftragSelector
{
public:
  enum prepareindex { idx_noPrepare, idx_AufidZnr, idx_Kunde_Art, idx_Art_Plan_Id, idx_anz };
private:
 std::string clausel;
 std::string order_clausel; // will be added to query int getClausel
 			   // can be manipulated after creation
 std::string pre_query; // do it as sql-query before run main clausel
 std::string post_query; // do it as sql-query after run main clausel 
 
 ArgumentList arguments;
 auto_init<bool> many; // many many lines are expected, use portal
 auto_init<prepareindex> prepnum; // which prepared statement is to use

 static std::string StatusQualifier(AufStatVal v);
 static std::string IDQualifier(AuftragBase::ID id);

public:
 SQLFullAuftragSelector() {}
 
 // id=AuftragBase::none_id => Alle Aufträge
 // id=AuftragBase::plan_auftrag_id => Alle außer 0er und 2er
 // id=ungeplante_id|dispo_auftrag_id => as is
 // wg=nur für Warengruppe wg selektieren; wg==noen_id alle Warengruppen
 struct sel_Status
  { sel_Status(ppsInstanz::ID in, AufStatVal st,AuftragBase::ID _id,
  		ArtikelTyp _wg=ArtikelTyp::none_id) 
    : instanz(in),status(st),id(_id),wg(_wg) {}
    ppsInstanz::ID instanz;
    AufStatVal status;
    AuftragBase::ID id; 
    ArtikelTyp::ID wg;
  };  

 SQLFullAuftragSelector(const sel_Status& selstr);

 struct sel_Aufid
  { sel_Aufid(const AuftragBase& a,const bool w=true) 
      : auftrag(a),with_storno(w) {}
    AuftragBase auftrag;
    bool with_storno;
  };
  
 SQLFullAuftragSelector(const sel_Aufid& selstr,const std::string tabelle);

 struct sel_AufidZnr
  { AufEintragBase auftrag_znr;
    sel_AufidZnr(const AufEintragBase& a) : auftrag_znr(a) {}
  };
 SQLFullAuftragSelector(const sel_AufidZnr& selstr);

 struct sel_Jahr_Artikel 
// wird von bestserv verwendet, sollte auch erfllte Eintr�e selektieren
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

 // sortiert nach Lieferdatum (asc) fr OPEN
 struct sel_Artikel_Planung_id
  { ppsInstanz::ID instanz;
    Kunde::ID kunde;
    ArtikelBase artikel;
    int auftragid;
    AufStatVal status;
    ManuProC::Datum lieferdatum;
    bool noprocess;
    
    sel_Artikel_Planung_id(ppsInstanz::ID i, Kunde::ID k,
                           ArtikelBase a, int _id,
                           AufStatVal s=OPEN,ManuProC::Datum d=ManuProC::Datum(),
                           bool nopr=false) 
    : instanz(i), kunde(k),artikel(a), auftragid(_id), 
      status(s),lieferdatum(d), noprocess(nopr)
    {}
  };
 SQLFullAuftragSelector(const sel_Artikel_Planung_id &selstr);

 struct sel_Artikel
  { ppsInstanz::ID instanz;
    ArtikelBase artikel;
    
    // this selector is capable of selecting all lines
    sel_Artikel(ppsInstanz::ID i=ppsInstanzID::None, ArtikelBase a=ArtikelBase()) 
    : instanz(i), artikel(a)
    {}
    // this used to be sel_InstanzAlle
    sel_Artikel(ppsInstanz::ID i,const ArtikelBase::ID aid) 
    : instanz(i), artikel(aid)
    {}
  };
 typedef sel_Artikel sel_InstanzAlle;
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
 
#ifdef MABELLA_EXTENSIONS
 struct sel_Kunde_Status_Lager
  { Kunde::ID kundennr;
    ppsInstanz::ID instanz;
    AufStatVal stat;
    FertigWarenLager lager;
    
    sel_Kunde_Status_Lager(ppsInstanz::ID i, Kunde::ID k, AufStatVal s, 
    			const FertigWarenLager &l) 
    : kundennr(k), instanz(i), stat(s), lager(l.Id())
    {}
  };
 SQLFullAuftragSelector(const sel_Kunde_Status_Lager &selstr); 
#endif

    sel_InstanzAlle(ppsInstanz::ID i,const ArtikelBase::ID aid=ArtikelBase::none_id) 
    : instanz(i), artikel(aid)
    {}

 void setClausel(const std::string &cl) { clausel = cl;}
 void setOrderClausel(const std::string &ocl) { order_clausel = ocl;} 
 const std::string getClausel() const { return clausel+order_clausel; }
 const std::string getPreQuery() const { return pre_query; }
 const std::string getPostQuery() const { return post_query; }
 bool many_lines() const { return many; }
 prepareindex prepare() const { return prepnum; }
 ArgumentList const& getArguments() const { return arguments; }
};

#endif
