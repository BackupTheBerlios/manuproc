/* $Id: LieferscheinEntry.cc,v 1.24 2003/07/03 10:06:39 christof Exp $ */
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

#include "LieferscheinEntry.h"
#include <Misc/FetchIStream.h>
#include <Misc/Transaction.h>
#include <Auftrag/AufEintrag.h>
#include <unistd.h>
#include <Lieferschein/Lieferschein.h>
#include <Misc/FetchIStream.h>

bool LieferscheinEntry::Valid() const
{
  if(Zeile()!=0) return true;
  return false;
}

void LieferscheinEntry::setPalette(int p) throw(SQLerror)
{
  Query("update lieferscheinentry set palette=? "
	"where (instanz,lfrsid,zeile)=(?,?,?)")
	<< p << *this;
  SQLerror::test(__FILELINE__);
}

// It seems that this routine works <50% of the time - needs a rewrite
void LieferscheinEntry::changeMenge(int stueck,mengen_t menge) throw(SQLerror)
{
  if(stueck==Stueck() && menge==Menge()) return ; //nichts geändert

  Transaction tr;
  Query::Execute("lock table lieferscheinentry in exclusive mode");

  AuftragBase::mengen_t abmenge=Abschreibmenge(stueck,menge);

  if(RefAuftrag().valid()) // => Keine Zusatzinfos
   {
     AufEintragBase AEB(RefAuftrag(),AufZeile());
     try{
       AufEintrag AE(AEB);
          // (intern Überlieferungen verbieten?)
//       if (abmenge>0 && abmenge>AE.getRestStk() 
//       		&& Instanz()!=ppsInstanzID::Kundenauftraege) 
//       	  return false;
       assert (!(abmenge<0 && -abmenge>AE.getGeliefert()));
       AE.Produziert(abmenge,Id());
     }catch(AufEintrag::NoAEB_Error &e){std::cerr << AEB<<" existiert nicht\n"; return;}
   }
  else if(!ZusatzInfo()) ;// s.u. updateLieferscheinMenge(stueck,menge) ;
  else // Zusatzinfos existieren
   {
     std::vector<st_AufEintragMenge> VZ=getZusatzInfos();
     if(abmenge>0) 
     for(std::vector<LieferscheinEntry::st_AufEintragMenge>::iterator i=VZ.begin();i!=VZ.end();++i)
      {
        AuftragBase::mengen_t actualmenge=abmenge;
        if(i->aeb.valid())
         {
           AufEintrag AE(i->aeb);
           if(AE.getEntryStatus()!=OPEN) continue;
           if(AE.getRestStk()<abmenge) actualmenge=AE.getRestStk();
           AE.Produziert(actualmenge,Id());
           updateZusatzEntry(*i,i->menge + actualmenge);
         }
        else 
           updateZusatzEntry(*i,i->menge + actualmenge);
        abmenge-=actualmenge;
        if(!abmenge) break;
      }
     else
     for(std::vector<LieferscheinEntry::st_AufEintragMenge>::reverse_iterator i=VZ.rbegin();i!=VZ.rend();++i)
      {
        AuftragBase::mengen_t actualmenge=abmenge;
        if(i->menge.abs() < actualmenge.abs()) actualmenge = -i->menge;
        if(i->menge + actualmenge==AuftragBase::mengen_t(0)) 
             deleteZusatzEntry(*i);
        else updateZusatzEntry(*i,i->menge + actualmenge);
        if(i->aeb.valid()) 
           AufEintrag(i->aeb).Produziert(actualmenge,Id());
        abmenge-=actualmenge;
        if(!abmenge) break;
      }
   }
  if(stueck==0) deleteMe(*this);
  else updateLieferscheinMenge(stueck,menge);

  tr.commit();
}

void LieferscheinEntry::updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror)
{
   std::string Q1="update lieferscheinentry set stueck="+itos(stueck)
         +", menge=nullif("+menge.String()+",0) where (instanz,lfrsid,zeile)=("
         +itos(Instanz()->Id())+","+itos(Id())+","+itos(Zeile())+")";
   Query::Execute(Q1);
   SQLerror::test(__FILELINE__);
}

void LieferscheinEntry::deleteMe(const LieferscheinEntry &LE)  throw(SQLerror)
{
   std::string Q1="delete from lieferscheinentry "
         " where (instanz,lfrsid,zeile)=("
         +itos(LE.Instanz()->Id())+","+itos(LE.Id())+","+itos(LE.Zeile())+")";
   Query::Execute(Q1);
   SQLerror::test(__FILELINE__);
}

LieferscheinBase::mengen_t LieferscheinEntry::Abschreibmenge(int stueck,mengen_t menge) const
{
   mengen_t alte_menge=Stueck();
   if(Menge()!=0) alte_menge=alte_menge*Menge();
   
   mengen_t neue_menge=stueck;
   if(menge!=0 || Menge()!=0) neue_menge=neue_menge*menge;
   
   return neue_menge-alte_menge;
}

void LieferscheinEntry::updateZusatzEntry(const st_AufEintragMenge &Z,const AuftragBase::mengen_t &menge) throw(SQLerror)
{
  std::string Q="update lieferscheinentryzusatz set menge="
      +menge.String()+" where (instanz,lfrsid,lfsznr) = ("
            +itos(Instanz()->Id())+","
            +itos(Id())+","+itos(Zeile())+") and ";

  if(Z.aeb.Id()==ManuProcEntity<>::none_id)
    Q += "auftragid is null and auftragznr is null"; 
  else 
    Q += "(auftragid,auftragznr)=("
         +itos(Z.aeb.Id())+","+itos(Z.aeb.ZNr())+")";

  Query::Execute(Q);
  SQLerror::test(__FILELINE__);
#warning Zusätze neu laden!
}

void LieferscheinEntry::deleteZusatzEntry(const st_AufEintragMenge &Z) throw(SQLerror)
{
  std::string Q="delete from lieferscheinentryzusatz "
      " where (instanz,lfrsid,lfsznr) = ("
            +itos(Instanz()->Id())+","
            +itos(Id())+","+itos(Zeile())+") and ";

  if(Z.aeb.Id()==ManuProcEntity<>::none_id)
    Q += "auftragid is null and auftragznr is null"; 
  else 
    Q += "(auftragid,auftragznr)=("
         +itos(Z.aeb.Id())+","+itos(Z.aeb.ZNr())+")";

  Query::Execute(Q);
  SQLerror::test(__FILELINE__);
#warning Zusätze neu laden!
}

FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry::st_AufEintragMenge& z)
{
  return is >> z.aeb >> z.menge;
}

ArgumentList &operator<<(ArgumentList &q, const LieferscheinEntryBase &leb)
{  return q << leb.Instanz()->Id() << leb.Id() << leb.Zeile();
}

FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry& z)
{bool zusatzinfo;
 AufEintragBase refauftrag;
 is >> z.lieferid >> z.zeilennr >> z.artikel 
 	>> FetchIStream::MapNull(z.stueck)
 	>> FetchIStream::MapNull(z.menge) 
 	>> FetchIStream::MapNull(z.palette)
     >> FetchIStream::MapNull(zusatzinfo) >> refauftrag ;
 z.instanz=refauftrag.Instanz();
 
 if(!zusatzinfo) 
 { z.VZusatz.push_back(LieferscheinEntry::st_AufEintragMenge(refauftrag,z.menge)); }
 else
 {(Query("select instanz,auftragid,auftragznr,menge "
   "from lieferscheinentryzusatz "
   "where (instanz,lfrsid,lfsznr) = (?,?,?) "
   "order by auftragid,auftragznr")
 	<< z)
 	.FetchArray(z.VZusatz);
 }
 return is;  
}

LieferscheinEntry::LieferscheinEntry(const LieferscheinEntryBase &lsbase)
 throw(SQLerror) : LieferscheinEntryBase(lsbase) 
{
  (Query("select lfrsid, zeile,artikelid, stueck, "
	  " menge, palette, zusatzinfo, instanz, refauftragid, refzeilennr"
	  " from lieferscheinentry ly "
	  " where (instanz,lfrsid,zeile) = (?,?,?)")
 	<< *this
 	).FetchOne(*this);
}

LieferscheinEntry LieferscheinEntry::create(const LieferscheinBase &lsb,
	const AufEintragBase &auf, const ArtikelBase &art, int anzahl,
	mengen_t _menge,int _palette, bool _zusatzinfo) throw(SQLerror) 
{
 LieferscheinEntry LE;
 LE.instanz=lsb.Instanz();
 LE.lieferid=lsb.Id();
 LE.artikel=art;
 LE.stueck=anzahl;
 LE.menge=_menge;
 LE.palette=_palette;
 if (!_zusatzinfo) LE.VZusatz.push_back(st_AufEintragMenge(auf,LE.menge));

 Transaction tr;
 Query("lock table lieferscheinentry in exclusive mode");
 SQLerror::test(__FILELINE__":LieferscheinEntry: lock table");
 
 Query("select coalesce(max(zeile)+1,1) from lieferscheinentry "
	"where (instanz,lfrsid)=(?,?)")
	<< lsb.Instanz()->Id() << lsb.Id()
	>> LE.zeilennr;
 SQLerror::test(__FILELINE__":LieferscheinEntry: select max(zeilennr)");

 Query("insert into lieferscheinentry"
 		"(instanz,lfrsid,zeile, artikelid, refauftragid,refzeilennr, stueck,"
		"menge,palette,zusatzinfo)"
 	"values (?,?,?, ?, ?,?, ?,?,?,?)").lvalue()
 	<< LE << art.Id() 
 	<< Query::NullIf(auf.Id(),AufEintragBase::none_id)
 	<< Query::NullIf(auf.ZNr(),AufEintragBase::none_znr)
 	<< LE.Stueck() 
 	<< Query::NullIf(LE.Menge(),0)
 	<< LE.Palette() << _zusatzinfo;
 SQLerror::test(__FILELINE__":LieferscheinEntry: insert into lieferscheinentry");
 
 tr.commit();
 return LE;
}

LieferscheinEntry LieferscheinEntry::create(const LieferscheinBase &lsb,
			const ArtikelBase &art, int anzahl,
			mengen_t _menge,int _palette, bool zusatz) throw(SQLerror) 
{  
 return create(lsb,AufEintragBase(lsb.Instanz(),0,0),art,anzahl,_menge,_palette,zusatz);
}

void LieferscheinEntry::deleteEntry(LieferscheinEntry &lse) throw(SQLerror)
{
 lse.changeMenge(0,0);
}


void LieferscheinEntry::setZusatzInfo(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror)
{
  Query("insert into lieferscheinentryzusatz (instanz,lfrsid,lfsznr,"
      "auftragid,auftragznr,menge) values (?,?,?, ?,?, ?)").lvalue() 
	<< *this 
  	<< Query::NullIf(AEB.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(AEB.ZNr(),AufEintragBase::none_znr)
  	<< menge;
  SQLerror::test(__FILELINE__);
}

