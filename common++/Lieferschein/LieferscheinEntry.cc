/* $Id: LieferscheinEntry.cc,v 1.25 2003/07/03 13:32:29 christof Exp $ */
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
void LieferscheinEntry::changeMenge(int stueck, mengen_t menge, bool ein_auftrag) throw(SQLerror)
{
  if(stueck==Stueck() && menge==Menge()) return ; //nichts geändert

  Transaction tr;
  Query::Execute("lock table lieferscheinentry in exclusive mode");

  AuftragBase::mengen_t abmenge=Abschreibmenge(stueck,menge);

  if (ein_auftrag)
  {  if(RefAuftrag().valid()) // => Keine Zusatzinfos
     {  AufEintrag AE(RefAuftrag());
        // verhindern dass negative Menge auftritt
        assert (!(abmenge<0 && -abmenge>AE.getGeliefert()));
        AE.Produziert(abmenge,Id());
     }
  }
#endif
  
  else if(!ZusatzInfo()) ;// s.u. updateLieferscheinMenge(stueck,menge) ;
  else // Zusatzinfos existieren
   {
     zusaetze_t VZ=getZusatzInfos();
     if(abmenge>0) 
     for(LieferscheinEntry::zusaetze_t::iterator i=VZ.begin();i!=VZ.end();++i)
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
     for(LieferscheinEntry::zusaetze_t::reverse_iterator i=VZ.rbegin();i!=VZ.rend();++i)
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
  updateLieferscheinMenge(stueck,menge);

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

FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry::st_AufEintragMenge& z)
{
  return is >> z.aeb >> z.menge;
}

ArgumentList &operator<<(ArgumentList &q, const LieferscheinEntryBase &leb)
{  return q << leb.Instanz()->Id() << leb.Id() << leb.Zeile();
}

void LieferscheinEntry::ZusaetzeLaden()
{  VZusatz.clear();
   (Query("select instanz,auftragid,auftragznr,menge "
   "from lieferscheinentryzusatz "
   "where (instanz,lfrsid,lfsznr) = (?,?,?) "
   "order by auftragid,auftragznr")
 	<< *this)
 	.FetchArray(VZusatz);
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
   z.VZusatz.push_back(LieferscheinEntry::st_AufEintragMenge(refauftrag,z.menge)); 
 else
   z.ZusaetzeLaden();
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
 deleteMe(*this);
}


//============================ Zusatzinfos =================================

void LieferscheinEntry::setZusatzInfos(const zusaetze_t &zis)
{  VZusatz=zis;

  Query("delete from lieferscheinentryzusatz "
      "where (instanz,lfrsid,lfsznr) = (?,?,?)") << *this;
  SQLerror::test(__FILELINE__);
  if (zis.size()==1)
  {  Query("update from lieferscheinentry "
	"set zusatzinfo=false,refauftragid=?,refzeilennr=? "
	"where (instanz,lfrsid,lfsznr) = (?,?,?)")
	<< Query::NullIf(zis[0].aeb.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(zis[0].aeb.ZNr(),AufEintragBase::none_znr)
	<< *this;
  }
  else
  {  Query("update from lieferscheinentry "
	"set zusatzinfo=?,refauftragid=null,refzeilennr=null "
	"where (instanz,lfrsid,lfsznr) = (?,?,?)")
	<< (!zis.empty())
	<< *this;
     for (zusaetze_t::const_iterator i=zis.begin();i!=zis.end();++i)
        addZusatzEntry(i->aeb,i->menge);
  }
}

void LieferscheinEntry::updateZusatzEntry(const st_AufEintragMenge &Z,const AuftragBase::mengen_t &menge) throw(SQLerror)
{
  std::string Q="update lieferscheinentryzusatz set menge=?"
      " where (instanz,lfrsid,lfsznr) = (?,?,?) and ";
  if(Z.aeb.Id()!=AufEintragBase::none_id)
    Q += "(auftragid,auftragznr)=(?,?)";
  else
  { Q += "auftragid is ? and auftragznr is ?";
    assert(Z.aeb.ZNr()==AufEintragBase::none_znr);
  }

  Query(Q) << menge
  	<< *this 
  	<< Query::NullIf(Z.aeb.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(Z.aeb.ZNr(),AufEintragBase::none_znr);
  SQLerror::test(__FILELINE__);
  ZusaetzeLaden();
}

void LieferscheinEntry::deleteZusatzEntry(const st_AufEintragMenge &Z) throw(SQLerror)
{
  std::string Q="delete from lieferscheinentryzusatz "
      "where (instanz,lfrsid,lfsznr) = (?,?,?) and ";
  if(Z.aeb.Id()!=AufEintragBase::none_id)
    Q += "(auftragid,auftragznr)=(?,?)";
  else
  { Q += "auftragid is ? and auftragznr is ?";
    assert(Z.aeb.ZNr()==AufEintragBase::none_znr);
  }

  Query(Q) << *this 
  	<< Query::NullIf(Z.aeb.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(Z.aeb.ZNr(),AufEintragBase::none_znr);
  SQLerror::test(__FILELINE__);
  ZusaetzeLaden();
}

void LieferscheinEntry::addZusatzEntry(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror)
{
  Query("insert into lieferscheinentryzusatz (instanz,lfrsid,lfsznr,"
      "auftragid,auftragznr,menge) values (?,?,?, ?,?, ?)").lvalue() 
	<< *this 
  	<< Query::NullIf(AEB.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(AEB.ZNr(),AufEintragBase::none_znr)
  	<< menge;
  SQLerror::test(__FILELINE__);
}

std::vector<LieferscheinEntry::st_AuftragMenge> LieferscheinEntry::getAuftragsMenge() const
{  typedef std::map<AuftragBase,AuftragBase::mengen_t> map_t;
   map_t map;
   for (zusaetze_t::const_iterator i=VZusatz.begin();i!=VZusatz.end();++i)
      map[i->aeb]+=i->menge;
   std::vector<LieferscheinEntry::st_AuftragMenge> result;
   for (map_t::const_iterator i=map.begin();i!=map.end();++i)
      result.push_back(st_AuftragMenge(i->first,i->second));
   return result;
}

