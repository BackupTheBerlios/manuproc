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


#ifndef OFFAUFROWDATA
#define OFFAUFROWDATA

#include<Aux/EntryValueIntString.h>
#include<Aux/Datum.h>
#include<Auftrag/AufEintragBase2.h>
#include<Artikel/ArtikelBezeichnung.h>
#include<rowdata.h>
#include<OffAuf_Value.h>

class OffAuf_RowData : public RowDataBase
{
 friend class cH_RowData;
 friend class const_Handle<OffAuf_RowData>;

 AufEintragBase2 auftrag;
 cH_ArtikelBezeichnung artikel;
 int offen;
 int geliefert;
 Petig::Datum liefdatum; 

public:
 OffAuf_RowData() : artikel(cH_ArtikelBezeichnung::Default()),
 		offen(0),geliefert(0) {}
 OffAuf_RowData(const AufEintragBase2 &auf,const cH_ArtikelBezeichnung art,
 			int off, int gel, const Petig::Datum liefd)
  : auftrag(auf),artikel(art),offen(off),geliefert(gel),liefdatum(liefd) {}

 const cH_EntryValue Value(int _seqnr) const;
 int Offen() const { return offen; }
 int Geliefert() const { return geliefert; }   	
 const ArtikelBase::ID ArtikelID() const { return artikel->Id(); }
 int AufId() const { return auftrag.AufId(); }
 const AufEintragBase2 &AuftragEntry() const { return auftrag; } 
 const ArtikelBase &Artikel() const { return *artikel; }
 void abschreiben(int menge);
 static void abschreiben(int aufid,int aufznr,int menge) throw(SQLerror);
};


class cH_OffAuf_RowData : public cH_RowDataBase
{
private:
 
 cH_OffAuf_RowData(const OffAuf_RowData *r) : cH_RowDataBase(r) {}
 cH_OffAuf_RowData() : cH_RowDataBase() {}
 
public:
 cH_OffAuf_RowData(const AufEintragBase2 &auf,const cH_ArtikelBezeichnung art,
 			int off, int gel, const Petig::Datum liefd)
   : cH_RowDataBase(new OffAuf_RowData(auf,art,off, gel,liefd))
 {}
 cH_OffAuf_RowData(const cH_RowDataBase &b):
 	cH_RowDataBase(dynamic_cast<const OffAuf_RowData*>(&*b))
 {}
 
 const OffAuf_RowData &operator*() const
        {  return (OffAuf_RowData&)(this->cH_RowDataBase::operator*());
        }         
        
 const OffAuf_RowData *operator->() const
        {  return (OffAuf_RowData*)(this->cH_RowDataBase::operator->());
        }                 
        
};


#endif
