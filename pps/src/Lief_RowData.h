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


#ifndef LIEFERSCHEINROWDATA
#define LIEFERSCHEINROWDATA

#include<Aux/EntryValueIntString.h>
#include<Aux/Datum.h>
#include<Lieferschein/LieferscheinEntry.h>
#include<rowdata.h>
#include<Artikel/ArtikelBezeichnung.h>

class Lief_RowData : public RowDataBase
{
 friend class cH_RowData;
 friend class const_Handle<Lief_RowData>;

 LieferscheinEntry liefentry;
 cH_ArtikelBezeichnung artikelbez;
  
public:
 Lief_RowData(const LieferscheinEntry &le, ExtBezSchema::ID ebzid) : 
 	liefentry(le), artikelbez(le.ArtikelID(),ebzid) {}

 const cH_EntryValue Value(guint _seqnr,gpointer _gp) const;
 int GeliefertS() const { return liefentry.Anzahl(); }	
 fixedpoint<3> GeliefertM() const { return liefentry.Menge(); }
 const ArtikelBase::ID ArtikelID() const { return artikelbez->Id(); }
 int AufId() const { return liefentry.AufId(); }
 int Zeile() const { return liefentry.Zeile(); }
 const LieferscheinEntry &getLiefEntry() const { return liefentry; }
};


class cH_Lief_RowData : public cH_RowDataBase
{
private:
 
 cH_Lief_RowData(const Lief_RowData *r) : cH_RowDataBase(r) {}
 cH_Lief_RowData() : cH_RowDataBase() {}
 
public:
 cH_Lief_RowData(const LieferscheinEntry &le, ExtBezSchema::ID ebzid)
   : cH_RowDataBase(new Lief_RowData(le,ebzid))
 {}
 cH_Lief_RowData(const cH_RowDataBase &b):
 	cH_RowDataBase(dynamic_cast<const Lief_RowData*>(&*b))
 {}
 
 const Lief_RowData &operator*() const
        {  return (Lief_RowData&)(this->cH_RowDataBase::operator*());
        }         
        
 const Lief_RowData *operator->() const
        {  return (Lief_RowData*)(this->cH_RowDataBase::operator->());
        }                 
        
};


#endif
