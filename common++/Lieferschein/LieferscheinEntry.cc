/* $Id: LieferscheinEntry.cc,v 1.23 2003/07/03 09:15:16 christof Exp $ */
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
bool LieferscheinEntry::changeMenge(int stueck,mengen_t menge) throw(SQLerror)
{
  if(stueck==Stueck() && menge==Menge()) return true ; //nichts geändert

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
       if(abmenge<0 && -abmenge>AE.getGeliefert())  return false;
       AE.Produziert(abmenge,Id());
     }catch(AufEintrag::NoAEB_Error &e){std::cerr << AEB<<" existiert nicht\n"; return false;}
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
  return true;
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
}

FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry::st_AufEintragMenge& z)
{
  return is >> z.aeb /*>> z.stueck*/ >> z.menge 
  	>> FetchIStream::MapNull(z.yourauftrag,std::string());
}

ArgumentList &operator<<(ArgumentList &q, const LieferscheinEntryBase &leb)
{  return q << leb.Instanz()->Id() << leb.Id() << leb.Zeile();
}

FetchIStream& operator>>(FetchIStream& is,LieferscheinEntry& z)
{
 is >> z.lieferid >> z.zeilennr >> z.artikel >>  z.stueck>> z.menge >> z.palette >> z.yourauftrag 
     >> z.zusatzinfo >> z.refauftrag ;
 z.instanz=z.refauftrag.Instanz();
 
 if(!z.zusatzinfo) return is;
 // ohne youraufnr könnte ich auf den join verzichten ...
 (Query("select lyz.instanz,coalesce(lyz.auftragid,?),coalesce(lyz.auftragznr,?),"
   "lyz.menge,youraufnr "
   "from lieferscheinentryzusatz lyz "
   "left join auftrag a on (lyz.instanz,lyz.auftragid) =(a.instanz,a.auftragid) " 
   "where (lyz.instanz,lyz.lfrsid,lyz.lfsznr) = (?,?,?) "
   "order by lyz.auftragid,lyz.auftragznr").lvalue()
	<< AuftragBase::none_id << AufEintragBase::none_znr
 	<< z)
 	.FetchArray(z.VZusatz);
 return is;  
}

LieferscheinEntry::LieferscheinEntry(const LieferscheinEntryBase &lsbase)
 throw(SQLerror) : LieferscheinEntryBase(lsbase) 
{
 std::string qstr =
  "select ly.lfrsid, ly.zeile,ly.artikelid,  coalesce(ly.stueck,0), "
  " coalesce(ly.menge,0), coalesce(ly.palette,0), coalesce(youraufnr,''),"
  " coalesce(ly.zusatzinfo,'f'), ly.instanz, "
  " coalesce(ly.refauftragid,"+itos(ManuProcEntity<>::none_id)+"),"
  " coalesce(ly.refzeilennr,"+itos(ManuProcEintrag::none_znr)+")"
  " from lieferscheinentry ly "
  " left join auftrag a on (ly.refauftragid,ly.instanz) = (a.auftragid,a.instanz)"
  " where (ly.instanz,ly.lfrsid,ly.zeile) = (?,?,?)";
 (Query(qstr).lvalue() << *this).FetchOne(*this);
}

LieferscheinEntry LieferscheinEntry::create(const LieferscheinBase &lsb,
	const AufEintragBase &auf, const ArtikelBase &art, int anzahl,
	mengen_t _menge,int _palette, bool _zusatzinfo) throw(SQLerror) 
{
 LieferscheinEntry LE;
 LE.instanz=lsb.Instanz();
 LE.lieferid=lsb.Id();
 LE.artikel=art;
 LE.refauftrag=auf;
 LE.stueck=anzahl;
 LE.menge=_menge;
 LE.palette=_palette;
 LE.zusatzinfo=_zusatzinfo;

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
 	<< LE << art.Id() << Query::NullIf(LE.RefAuftrag().Id(),0)
 	<< Query::NullIf(LE.AufZeile(),0)
 	<< LE.Stueck() 
 	<< Query::NullIf(LE.Menge(),0)
 	<< LE.Palette() << LE.ZusatzInfo();
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
  	<< Query::NullIf(AEB.Id(),ManuProcEntity<>::none_id)
  	<< Query::NullIf(AEB.ZNr(),ManuProcEintrag::none_znr)
  	<< menge;
  SQLerror::test(__FILELINE__);
}

