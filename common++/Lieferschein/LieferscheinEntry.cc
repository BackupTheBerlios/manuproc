/* $Id: LieferscheinEntry.cc,v 1.73 2004/09/15 10:17:27 jacek Exp $ */
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
#include <Lager/FertigWarenLager.h>

#ifdef MABELLA_EXTENSIONS
#include <Artikel/ArtikelBase.h>
#endif

#if !defined(__GNUC__) || __GNUC__>3 || (__GNUC__==3 && __GNUC_MINOR__ >0)
#define NurEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ++((x).begin()) != (x).end())
#elif __GNUC__==3 && __GNUC_MINOR__==0
#define NurEinKind(x) (std::operator!=((x).begin(),(x).end()) && ++((x).begin()) == (x).end())
#define MehrAlsEinKind(x) (std::operator!=((x).begin(),(x).end()) && std::operator!=(++((x).begin()),(x).end()))
#else
#define NurEinKind(x) ((x).begin()!=(x).end() && ((x).begin()+1) == (x).end())
#define MehrAlsEinKind(x) ((x).begin()!=(x).end() && ((x).begin()+1) != (x).end())
#endif

std::ostream &operator<<(std::ostream &o, const LieferscheinEntryBase &b)
{  if (!b.valid()) return o << "n.a.";
   return o << b.Instanz()->Id() << '.' << b.Id() << '.' << b.ZNr();
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
   for (zusaetze_t::const_iterator i=VZusatz.begin();std_neq(i,VZusatz.end());++i)
      std::cerr << i->aeb << '=' << i->menge << ' ';
   std::cerr << '\n';
}

#warning Diese Funktion ist IMHO unsinnig
void LieferscheinEntry::changeStatus(AufStatVal new_status, 
				bool ein_auftrag) 
{ 
 if((status==OPEN || status==CLOSED) && new_status==STORNO)
   changeStatus(new_status,ein_auftrag,0,0);
 else
   changeStatus(new_status,ein_auftrag,stueck,menge);
}

namespace { struct stornoliste
{	AufEintragBase aeb;
	AuftragBase::mengen_t menge,abmenge;
	
	stornoliste(const AufEintragBase &a,AuftragBase::mengen_t m,AuftragBase::mengen_t am)
	: aeb(a), menge(m), abmenge(am) {}
}; }

void LieferscheinEntry::changeStatus(AufStatVal new_status, 
		bool ein_auftrag,
		int _stueck, mengen_t _menge) throw(SQLerror,LagerError)
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
	NV("new_status",new_status),
	NV("status",status),NV("ein_auftrag",ein_auftrag),
	NV("stueck",_stueck),NV("menge",_menge));


  if(status==CLOSED || status==STORNO) return; // not changable any more

  if(status>new_status) return;
				// down changed


  if(status!=OPEN && status==new_status) return; //nothing changed 
						// if OPEN == OPEN
					// means really changed amount
					// so go forward
  Transaction tr;

  if(new_status==OPEN || ((status==OPEN || status==CLOSED) && new_status==STORNO))
   {
    AuftragBase::mengen_t abmenge=Abschreibmenge(_stueck,_menge);
    ManuProC::Trace(trace_channel,"",NV("abmenge",abmenge));

    if (ein_auftrag && VZusatz[0].aeb.valid())
    {  assert(NurEinKind(VZusatz));
       AufEintrag AE(VZusatz[0].aeb);
       // verhindern dass negative Menge auftreten
       assert (!(abmenge<0 && -abmenge>AE.getGeliefert()));
       AE.ProduziertNG(abmenge,*this);
       change_status(new_status);
    }
    else 
     {
       AuftragBase::mengen_t abmenge2=abmenge;
       if(abmenge>0) 
       {if (status==UNCOMMITED) // erstmaliges Anpassen an Aufträge
        {  assert(abmenge==AuftragBase::Gesamtmenge(_stueck,_menge));
           VZusatz.clear();
        }
        // was ist mit 0ern im Einkauf, diese werden auch bei Überproduktion nicht
        // direkt erledigt - allerdings beim Einlagern ??? CP
        SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Artikel_Planung_id
        			(instanz->Id(),KdID(),artikel,AuftragBase::handplan_auftrag_id));
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
              neuerAEB=AufEintrag::unbestellteMengeProduzieren(Instanz(),artikel,
              		abmenge2,true,AufEintragBase(),*this);
           addZusatzEntry(neuerAEB,abmenge2);
        }
       }
       else
       {//showZusatzInfos();
        zusaetze_t VZ=getZusatzInfos();
  #warning in dieser Reihenfolge Menge ermitteln aber umgekehrt abschreiben,
  //	damit der Erste eventuelle Lagerbestände reservieren kann!
  	typedef std::vector<stornoliste> list_t;
  	list_t liste;
        for(LieferscheinEntry::zusaetze_t::reverse_iterator i=VZ.rbegin();i!=VZ.rend();++i)
        {
          AuftragBase::mengen_t actualmenge=abmenge2;
          if(i->menge < actualmenge.abs()) actualmenge = -i->menge;
          ManuProC::Trace(trace_channel,__FILELINE__,
          	NV("abmenge2",abmenge2),NV("actualmenge",actualmenge),
          	NV("i->menge",i->menge));
          if (!actualmenge) continue;
          
          liste.push_back(stornoliste(i->aeb,i->menge,actualmenge));
          abmenge2-=actualmenge;
          if(!abmenge2) break;
        }
        for (list_t::reverse_iterator i=liste.rbegin();i!=liste.rend();++i)
        { if(i->aeb.valid()) 
             AufEintrag(i->aeb).ProduziertNG(i->abmenge,*this);
          if(!(i->menge + i->abmenge)) deleteZusatzEntry(i->aeb);
          else updateZusatzEntry(i->aeb,i->menge + i->abmenge);
        }
       }
     // change status realy now
     change_status(new_status);
     }
   } 
  else
    change_status(new_status);
  // END OF   
  // if(new_status==OPEN || ((status==OPEN || status==CLOSED) && new_status==STORNO))
  
 tr.commit();  
}


void LieferscheinEntry::change_status(AufStatVal new_status)
{
 if(new_status==STORNO)
   {Query("delete from lieferscheinentry  where "
	"(lfrsid,instanz,zeile)=(?,?,?)")
	<< Id() << Instanz()->Id() << Zeile();
   }
 else
  {
     Query("update lieferscheinentry set status=? where "
	"(lfrsid,instanz,zeile)=(?,?,?)")
	<< Query::NullIf(new_status,(AufStatVal)NOSTAT)
	<< Id() << Instanz()->Id() << Zeile();
  }
}



void LieferscheinEntry::changeMenge(int _stueck, mengen_t _menge,
				bool ein_auftrag) throw(SQLerror)
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
	NV("stueck",_stueck),NV("menge",_menge),NV("ein_auftrag",ein_auftrag));
  if(_stueck==Stueck() && _menge==Menge()) return ; //nichts geändert

  if(status>OPEN) return;

  Transaction tr;
  Query::Execute("lock table lieferscheinentry in exclusive mode");

  if(status==OPEN) changeStatus((AufStatVal)OPEN,ein_auftrag,_stueck,_menge);
  updateLieferscheinMenge(_stueck,_menge);

  tr.commit();
}

void LieferscheinEntry::updateLieferscheinMenge(int _stueck,mengen_t _menge)  throw(SQLerror)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
	NV("stueck",_stueck),NV("menge",_menge));
   this->stueck=_stueck;
   this->menge=_menge;
   Query("update lieferscheinentry set stueck=?,menge=? "
   	"where (instanz,lfrsid,zeile)=(?,?,?)")
   	<< _stueck << Query::NullIf(_menge)
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

LieferscheinBase::mengen_t LieferscheinEntry::Abschreibmenge(int _stueck,
							mengen_t _menge) const
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
		NV("status",status), NV("stueck",_stueck), NV("menge",_menge));
   if(status==(AufStatVal)UNCOMMITED)
     return AuftragBase::Gesamtmenge(_stueck,_menge);

   mengen_t alte_menge=Stueck();
   if(Menge()!=0) alte_menge=alte_menge*Menge();
   
   mengen_t neue_menge=_stueck;
   if(_menge!=0 || Menge()!=0) neue_menge=neue_menge*_menge;
   ManuProC::Trace (trace_channel, "",NV("alte_menge",alte_menge),
		   	NV("neue_menge",neue_menge));
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
 int status;
 is >> z.lieferid >> z.zeilennr >> z.artikel 
 	>> FetchIStream::MapNull(z.stueck)
 	>> FetchIStream::MapNull(z.menge) 
 	>> FetchIStream::MapNull(z.palette)
     >> FetchIStream::MapNull(zusatzinfo) >> refauftrag 
     >> FetchIStream::MapNull(z.lagerid,FertigWarenLager::none_lagerid)
     >> FetchIStream::MapNull(status,(AufStatVal)NOSTAT);
 z.status=(AufStatVal)status;
#ifdef PETIG_EXTENSIONS 
  // gibt es einen Grund, das nicht anzunehmen, immerhin war es früher so
 if (status==NOSTAT) z.status=OPEN;
#endif
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
 throw(SQLerror) : LieferscheinEntryBase(lsbase), kid(Kunde::none_id) 
{
  (Query("select lfrsid, zeile,artikelid, stueck, "
	  " menge, palette, zusatzinfo, instanz, refauftragid, refzeilennr,"
	  " lagerid, status "
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
 LE.lagerid=-1; //lsb.lagerid;
 LE.artikel=art;
 LE.stueck=anzahl;
 LE.menge=_menge;
 LE.palette=_palette;
 LE.status=(AufStatVal)UNCOMMITED;
#ifdef LIEFERSCHEINE_IMMER_BESTAETIGT
 LE.status=(AufStatVal)OPEN;
#endif
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
		"menge,palette,zusatzinfo,status)"
 	"values (?,?,?, ?, ?,?, ?,?,?,?,?)").lvalue()
 	<< LE << art.Id() 
 	<< Query::NullIf(auf.Id(),AufEintragBase::none_id)
 	<< Query::NullIf(auf.ZNr(),AufEintragBase::none_znr)
 	<< LE.Stueck() 
 	<< Query::NullIf(LE.Menge(),0)
 	<< LE.Palette() << _zusatzinfo
	<< Query::NullIf(LE.status,(AufStatVal)NOSTAT);
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
 lse.changeMenge(0,0,false);
 deleteMe(lse);
}


Kunde::ID LieferscheinEntry::KdID() const throw(SQLerror)
{
 if(kid!=Kunde::none_id) return kid;

 Query("select kundennr from lieferschein where (lfrsid,instanz)=(?,?)")
	<< Id() 
	<< Instanz()->Id()
	>> kid;

 return kid;
}



//============================ Zusatzinfos =================================

void LieferscheinEntry::setZusatzInfos(const zusaetze_t &zis)
{  VZusatz=zis;
   setZusatzInfos();
}

void LieferscheinEntry::setZusatzInfos()
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this));
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
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
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
  for (zusaetze_t::iterator i=VZusatz.begin();std_neq(i,VZusatz.end());++i)
     if (i->aeb==Z)
     {  i->menge=menge;
        break;
     }
}

void LieferscheinEntry::deleteZusatzEntry(const AufEintragBase &Z) throw(SQLerror)
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
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
  for (zusaetze_t::iterator i=VZusatz.begin();std_neq(i,VZusatz.end());++i)
     if (i->aeb==Z)
     {  i=VZusatz.erase(i);
        break;
     }
  if (NurEinKind(VZusatz)^nek0) setZusatzInfos();
}

void LieferscheinEntry::addZusatzEntry_db(const AufEintragBase &AEB,const mengen_t &menge) throw(SQLerror)
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
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
{ ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
	NV("AEB",AEB),NV("menge",menge));
  for (zusaetze_t::const_iterator i=VZusatz.begin();std_neq(i,VZusatz.end());++i)
  {  ManuProC::Trace(trace_channel, __FILELINE__, NV("i->aeb",i->aeb),
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
   for (zusaetze_t::const_iterator i=VZusatz.begin();std_neq(i,VZusatz.end());++i)
      map[i->aeb]+=i->menge;
   std::vector<LieferscheinEntry::st_AuftragMenge> result;
   for (map_t::const_iterator i=map.begin();std_neq(i,map.end());++i)
   {  if (i->first!=AuftragBase())
         result.push_back(st_AuftragMenge(i->first,i->second));
   }
   map_t::const_iterator i=map.find(AuftragBase());
   if (std_neq(i,map.end())) 
      result.push_back(st_AuftragMenge(i->first,i->second));
   return result;
}

void LieferscheinEntry::setLagerid(int _lagid) throw(SQLerror)
{
 Query("update lieferscheinentry set lagerid=? where "
	"(instanz,lfrsid,zeile)=(?,?,?) ")
	<< Query::NullIf(_lagid,FertigWarenLager::none_lagerid) << *this;
 lagerid=_lagid;
}


fixedpoint<1> LieferscheinEntry::DurchAuftraegeAbgedeckt() const
{  mengen_t m=0;

   if (Status()!=(AufStatVal)UNCOMMITED)
   {  for (zusaetze_t::const_iterator i=VZusatz.begin();std_neq(i,VZusatz.end());++i)
         if (i->aeb!=AufEintragBase())
            m+=i->menge;
   }
   else
   {  SQLFullAuftragSelector psel(SQLFullAuftragSelector::sel_Artikel_Planung_id
           	(instanz->Id(),KdID(),artikel,AuftragBase::handplan_auftrag_id));
      SelectedFullAufList auftraglist(psel);

      for (SelectedFullAufList::iterator i=auftraglist.aufidliste.begin();
          	        i!=auftraglist.aufidliste.end(); ++i)
         m+=i->getRestStk();
      m=AuftragBase::min(m,AuftragBase::Gesamtmenge(Stueck(),Menge()));
   }
   mengen_t m2=AuftragBase::Gesamtmenge(Stueck(),Menge());
   if (!m) return 0;
   if (m==m2) return 1;
   return 100.0*m/m2.as_float();
}
