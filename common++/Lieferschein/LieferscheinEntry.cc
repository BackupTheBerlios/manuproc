/* $Id: LieferscheinEntry.cc,v 1.37 2003/07/25 12:53:17 jacek Exp $ */
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
#include <Auftrag/selFullAufEntry.h>
#include <Artikel/Einheiten.h>
#include <Misc/TraceNV.h>
#include <iostream>
#include <Misc/relops.h>

#ifdef MABELLA_EXTENSIONS
//#include <Lager/FertigWaren.h>
#include <Artikel/ArtikelBase.h>
#endif

#if !defined(__GNUC__) || __GNUC__>=3
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())
#else
#define NurEinKind(x) ((x).begin()!=(x).end() && ((x).begin()+1) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ((x).begin()+1) != (x).end())
#endif

std::ostream &operator<<(std::ostream &o, const LieferscheinEntryBase &b)
{  return o << b.Instanz()->Id() << '.' << b.Id() << '.' << b.ZNr();
}

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

void LieferscheinEntry::showZusatzInfos() const
{  std::cerr << "Zusatzinfos " << *this << ": ";
   for (zusaetze_t::const_iterator i=VZusatz.begin();i!=VZusatz.end();++i)
      std::cerr << i->aeb << '=' << i->menge << ' ';
   std::cerr << '\n';
}

void LieferscheinEntry::changeMenge(int stueck, mengen_t menge, const Lieferschein &ls, bool ein_auftrag) throw(SQLerror)
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this),
	NV("stueck",stueck),NV("menge",menge),NV("ein_auftrag",ein_auftrag));
  if(stueck==Stueck() && menge==Menge()) return ; //nichts geändert

  Transaction tr;
  Query::Execute("lock table lieferscheinentry in exclusive mode");

  AuftragBase::mengen_t abmenge=Abschreibmenge(stueck,menge);

  if (ein_auftrag)
  {  assert(NurEinKind(VZusatz));
     if(VZusatz[0].aeb.valid()) // => Keine Zusatzinfos
     {  AufEintrag AE(VZusatz[0].aeb);
        // verhindern dass negative Menge auftritt
        assert (!(abmenge<0 && -abmenge>AE.getGeliefert()));
        AE.ProduziertNG(abmenge,*this);
     }
  }
  else 
   {
     AuftragBase::mengen_t abmenge2=abmenge;
     if(abmenge>0) 
     {
      // was ist mit 0ern im Einkauf, diese werden auch bei Überproduktion nicht
      // direkt erledigt - allerdings beim Einlagern ??? CP
      SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Artikel_Planung_id
      			(instanz->Id(),ls.getKunde()->Id(),artikel,AuftragBase::handplan_auftrag_id));
      SelectedFullAufList auftraglist(psel);


      for (SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
     	        !!abmenge2 && i!=auftraglist.aufidliste.end(); ++i)
      {  AuftragBase::mengen_t abmenge3=AuftragBase::min(abmenge2,i->getRestStk());
         if (!abmenge3) continue;
           
         i->ProduziertNG(abmenge3,*this);
         addZusatzEntry(*i,abmenge3);

         abmenge2-=abmenge3;
         if (!abmenge2) break;
      }
      if(abmenge2>0)
         // da ist noch ein Rest geblieben, setzt ZusatzInfo
      {  AufEintragBase neuerAEB;
         if (instanz!=ppsInstanzID::Kundenauftraege) 
            neuerAEB=AufEintrag::unbestellteMengeProduzieren(Instanz(),artikel,abmenge2,getuid());
         addZusatzEntry(neuerAEB,abmenge2);
      }
//      showZusatzInfos();
     }
     else
     {//showZusatzInfos();
      zusaetze_t VZ=getZusatzInfos();
      for(LieferscheinEntry::zusaetze_t::reverse_iterator i=VZ.rbegin();i!=VZ.rend();++i)
      {
        AuftragBase::mengen_t actualmenge=abmenge2;
        if(i->menge < actualmenge.abs()) actualmenge = -i->menge;
        ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
        	NV("abmenge2",abmenge2),NV("actualmenge",actualmenge),
        	NV("i->menge",i->menge));
        if (!actualmenge) continue;
        
        if(i->aeb.valid()) 
           AufEintrag(i->aeb).ProduziertNG(actualmenge,*this);
        if(!(i->menge + actualmenge)) deleteZusatzEntry(i->aeb);
        else updateZusatzEntry(i->aeb,i->menge + actualmenge);
        abmenge2-=actualmenge;
        if(!abmenge2) break;
      }
     }
   }

  updateLieferscheinMenge(stueck,menge);

//#ifdef MABELLA_EXTENSIONS // auf keinen Fall nach offenen Aufträgen suchen
//#warning Uebler Hack, muss weg wenn Mabella Instanzen hat!
//  if(Instanz() == ppsInstanzID::Kundenauftraege)
//  {  FertigWaren fw(artikel,(FertigWaren::enum_Aktion)'L',
//  			abmenge.abs().as_int(),Id());
//     if(abmenge < 0) fw.Einlagern(1);
//     else if(abmenge > 0) fw.Auslagern(1);
//   }
//#endif

  tr.commit();
}

void LieferscheinEntry::updateLieferscheinMenge(int stueck,mengen_t menge)  throw(SQLerror)
{  this->stueck=stueck;
   this->menge=menge;
   Query("update lieferscheinentry set stueck=?,menge=? "
   	"where (instanz,lfrsid,zeile)=(?,?,?)")
   	<< stueck << Query::NullIf(menge)
   	<< *this;
   SQLerror::test(__FILELINE__);
}

void LieferscheinEntry::deleteMe(const LieferscheinEntry &LE)  throw(SQLerror)
{
   Query("delete from lieferscheinentry "
         "where (instanz,lfrsid,zeile)=(?,?,?)")
         << LE;
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
{ is >> z.aeb >> z.menge;
  // eliminate the valid Instanz
  if (!z.aeb) z.aeb=AufEintragBase();
  return is;
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
 // eliminate the valid Instanz
 if (!refauftrag) refauftrag=AufEintragBase();
 
 if(!zusatzinfo) 
 { LieferscheinEntry::mengen_t m=z.menge;
   if (!m) m=1;
   z.VZusatz.push_back(LieferscheinEntry::st_AufEintragMenge(refauftrag,z.stueck*m)); 
 }  
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
 return create(lsb,AufEintragBase(lsb.Instanz(),AufEintragBase::none_id,
 		AufEintragBase::none_znr),art,anzahl,_menge,_palette,zusatz);
}

void LieferscheinEntry::deleteEntry(LieferscheinEntry &lse) throw(SQLerror)
{
 lse.changeMenge(0,0,Lieferschein(),false);
 deleteMe(lse);
}


//============================ Zusatzinfos =================================

void LieferscheinEntry::setZusatzInfos(const zusaetze_t &zis)
{  VZusatz=zis;
   setZusatzInfos();
}

void LieferscheinEntry::setZusatzInfos()
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this));
  Query("delete from lieferscheinentryzusatz "
      "where (instanz,lfrsid,lfsznr) = (?,?,?)") << *this;
  SQLerror::test(__FILELINE__,100);
  if (NurEinKind(VZusatz))
  {  Query("update lieferscheinentry "
	"set zusatzinfo=false,refauftragid=?,refzeilennr=? "
	"where (instanz,lfrsid,zeile) = (?,?,?)")
	<< Query::NullIf(VZusatz[0].aeb.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(VZusatz[0].aeb.ZNr(),AufEintragBase::none_znr)
	<< *this;
  }
  else
  {  Query("update lieferscheinentry "
	"set zusatzinfo=?,refauftragid=null,refzeilennr=null "
	"where (instanz,lfrsid,zeile) = (?,?,?)")
	<< (!VZusatz.empty())
	<< *this;
     for (zusaetze_t::const_iterator i=VZusatz.begin();i!=VZusatz.end();++i)
        addZusatzEntry_db(i->aeb,i->menge);
  }
}

void LieferscheinEntry::updateZusatzEntry(const AufEintragBase &Z,const AuftragBase::mengen_t &menge) throw(SQLerror)
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this),
	NV("AEB",Z),NV("menge",menge));
  if (NurEinKind(VZusatz))
  {  assert (VZusatz[0].aeb==Z);
     VZusatz[0].menge=menge;
     setZusatzInfos();
     return;
  }
  std::string Q="update lieferscheinentryzusatz set menge=?"
      " where (instanz,lfrsid,lfsznr) = (?,?,?) and ";
  if(Z.Id()!=AufEintragBase::none_id)
    Q += "(auftragid,auftragznr)=(?,?)";
  else
  { Q += "auftragid is ? and auftragznr is ?";
    assert(Z.ZNr()==AufEintragBase::none_znr);
  }

  Query(Q) << menge
  	<< *this 
  	<< Query::NullIf(Z.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(Z.ZNr(),AufEintragBase::none_znr);
  SQLerror::test(__FILELINE__);
  for (zusaetze_t::iterator i=VZusatz.begin();i!=VZusatz.end();++i)
     if (i->aeb==Z)
     {  i->menge=menge;
        break;
     }
}

void LieferscheinEntry::deleteZusatzEntry(const AufEintragBase &Z) throw(SQLerror)
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this),
	NV("AEB",Z));
  bool nek0=NurEinKind(VZusatz);
  std::string Q="delete from lieferscheinentryzusatz "
      "where (instanz,lfrsid,lfsznr) = (?,?,?) and ";
  if(Z.Id()!=AufEintragBase::none_id)
    Q += "(auftragid,auftragznr)=(?,?)";
  else
  { Q += "auftragid is ? and auftragznr is ?";
    assert(Z.ZNr()==AufEintragBase::none_znr);
  }

  Query(Q) << *this 
  	<< Query::NullIf(Z.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(Z.ZNr(),AufEintragBase::none_znr);
  SQLerror::test(__FILELINE__,100);
  for (zusaetze_t::iterator i=VZusatz.begin();i!=VZusatz.end();++i)
     if (i->aeb==Z)
     {  i=VZusatz.erase(i);
        break;
     }
  if (NurEinKind(VZusatz)^nek0) setZusatzInfos();
}

void LieferscheinEntry::addZusatzEntry_db(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror)
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this),
	NV("AEB",AEB),NV("menge",menge));
  Query("insert into lieferscheinentryzusatz (instanz,lfrsid,lfsznr,"
      "auftragid,auftragznr,menge) values (?,?,?, ?,?, ?)").lvalue() 
	<< *this 
  	<< Query::NullIf(AEB.Id(),AufEintragBase::none_id)
  	<< Query::NullIf(AEB.ZNr(),AufEintragBase::none_znr)
  	<< menge;
  SQLerror::test(__FILELINE__);
}

void LieferscheinEntry::addZusatzEntry(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror)
{ ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("this",*this),
	NV("AEB",AEB),NV("menge",menge));
  for (zusaetze_t::const_iterator i=VZusatz.begin();i!=VZusatz.end();++i)
  {  ManuProC::Trace(AuftragBase::trace_channel, __FILELINE__, NV("i->aeb",i->aeb),
  		NV("AEB",AEB));
     if (i->aeb==AEB)
     {  updateZusatzEntry(AEB,i->menge+menge);
        return;
     }
  }
  bool nek0=NurEinKind(VZusatz);
  // dummy Eintrag wegnehmen
  if (nek0 && !VZusatz[0].aeb && !VZusatz[0].menge)
  {  VZusatz.erase(VZusatz.begin()); 
     nek0=false; 
  }
  VZusatz.push_back(st_AufEintragMenge(AEB,menge));

  if (NurEinKind(VZusatz)^nek0) setZusatzInfos();
  else addZusatzEntry_db(AEB,menge);
}

std::vector<LieferscheinEntry::st_AuftragMenge> LieferscheinEntry::getAuftragsMenge() const
{  typedef std::map<AuftragBase,AuftragBase::mengen_t> map_t;
   map_t map;
   for (zusaetze_t::const_iterator i=VZusatz.begin();i!=VZusatz.end();++i)
      map[i->aeb]+=i->menge;
   std::vector<LieferscheinEntry::st_AuftragMenge> result;
   for (map_t::const_iterator i=map.begin();i!=map.end();++i)
   {  if (i->first!=AuftragBase())
         result.push_back(st_AuftragMenge(i->first,i->second));
   }
   map_t::const_iterator i=map.find(AuftragBase());
   if (i!=map.end()) 
      result.push_back(st_AuftragMenge(i->first,i->second));
   return result;
}

