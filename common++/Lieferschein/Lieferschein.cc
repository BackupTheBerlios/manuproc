/* $Id: Lieferschein.cc,v 1.42 2004/02/09 15:14:29 jacek Exp $ */
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

#include"Lieferschein.h"
#include <Misc/Transaction.h>
#include <Misc/FetchIStream.h>
//#include <Instanzen/Produziert.h>
#include <unistd.h> 
#include <Misc/TraceNV.h>


#if __GNUC__ > 2 && defined (DPD_LIEFERSCHEINE)
	 const int Lieferschein::Fertig;
	 const int Lieferschein::Offen;
#endif


Lieferschein::Lieferschein(const LieferscheinBase &lsbase, const ManuProC::Datum &_lsdatum,
int _kdnr,int _rngid, int _paeckchen, int _pakete, const ManuProC::Datum &_geliefertam,
int _dpdlnr)
: LieferscheinBase(lsbase), lsdatum(_lsdatum), kunde(_kdnr), rngid(_rngid)
, geliefertam(_geliefertam)
#ifdef DPD_LIEFERSCHEINE
,dpdliefnr(_dpdlnr),paeckchen(_paeckchen),pakete(_pakete)
#endif
,notiz_valid(false)
,verknr(Kunde::none_id)
{}

int Lieferschein::push_back(const ArtikelBase &artikel, int anzahl, 
		mengen_t mengeneinheit, int palette)
{  
   // Eine Instanz, die Lieferscheine schreibt, produziert auch selber (assert in ppsInstanz).
   // Wenn man einn Lieferschein schriebe, ohne daß diese Instanz 
   // selber produzierte, würde doppelt produziert werden.
   assert(Instanz()->Lieferschein());
   ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("artikel",artikel),
           NV("anzahl",anzahl),NV("mengeneinheit",mengeneinheit),NV("palette",palette));

   Transaction tr;
   LieferscheinEntry LE=LieferscheinEntry::create(*this, artikel,0,0,palette);
   // damit der Code nicht 2x erscheint
   LE.lagerid=lagerid;
   LE.changeMenge(anzahl,mengeneinheit);
   tr.commit();
   return LE.ZNr();
}

// fast das gleiche ...
int Lieferschein::push_back(AufEintrag &aufeintrag,
		const ArtikelBase &artikel, int anzahl, 
		mengen_t menge, int palette)
{
   assert(Instanz()->Lieferschein());
 ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("AufEintrag",aufeintrag),
           NV("artikel",artikel),
           NV("Anzahl",anzahl),NV("Menge",menge),NV("Palette",palette));                     
 Transaction tr;          
 LieferscheinEntry LE=LieferscheinEntry::create(*this, aufeintrag ,artikel, 0,0,palette);
 LE.lagerid=lagerid;
 LE.changeMenge(anzahl,menge);
 tr.commit();
 return LE.ZNr();
}

void Lieferschein::aufraumen() throw(SQLerror)
{
 Query("update lieferschein set rngid=0 where rngid is null "
// 		"and instanz=? "
 		"and not exists(select true from lieferscheinentry "
 		"where lieferscheinentry.lfrsid=lieferschein.lfrsid)");
//   	<< Instanz()->Id()
	;
 SQLerror::test(__FILELINE__,100);
}


void Lieferschein::closeLfrs()
{
 Query("update lieferschein set rngid=0 where rngid is null "
 		"and (instanz,lfrsid) = (?,?)") 
   	<< Instanz()->Id() << Id();
 Query("update lieferscheinentry set status=? where (instanz,lfrsid)=(?,?)"
	" and status!=? ")
 	<< (AufStatVal)CLOSED << Instanz()->Id() << Id() <<
	   (AufStatVal)STORNO;
 SQLerror::test(__FILELINE__);
}


#ifdef DPD_LIEFERSCHEINE

void Lieferschein::setDPDlnr(int d) const throw(SQLerror)
{
 Query("update lieferschein set dpdliefnr=? where (instanz,lfrsid) = (?,?)") 
   	<< Query::NullIf(d,Offen) << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setPakete(const int p) throw(SQLerror)
{  
 Query("update lieferschein set pakete=? where (instanz,lfrsid) = (?,?)") 
   	<< p << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setPaeckchen(const int p) throw(SQLerror)
{  
 Query("update lieferschein set paeckchen=? where (instanz,lfrsid) = (?,?)") 
   	<< p << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setGewichtNetto(const fixedpoint<1> i) throw(SQLerror)
{  
 Query("update lieferschein set netto_kg=? where (instanz,lfrsid) = (?,?)") 
   	<< i << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}

void Lieferschein::setGewichtBrutto(const fixedpoint<1> i) throw(SQLerror)
{  
 Query("update lieferschein set brutto_kg=? where (instanz,lfrsid) = (?,?)")
   << i << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}


void Lieferschein::setDPDDatum() const throw(SQLerror)
{
 Query("update lieferschein set geliefertam=now() "
 	"where (instanz,lfrsid)=(?,?)") << Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);
}
#endif

int Lieferschein::maxZnr() throw(SQLerror)
{
 return (Query("select max(zeile) from lieferscheinentry where "
 	"(instanz,lfrsid) = (?,?)") 
 	<< Instanz()->Id() << Id()
 	).FetchOneMap<int>();
}

const Preis::rabatt_t Lieferschein::AufRabatt() const throw(SQLerror)
{
 return (Query("select max(rabatt) from "
 	"auftrag a join lieferscheinentry e "
   	"on (refauftragid=auftragid and a.instanz=e.instanz) where "
   	"(e.instanz,e.lfrsid)=(?,?)") << Instanz()->Id() << Id()
   	).FetchOneMap<Preis::rabatt_t>();;
}

LieferscheinBase::mengen_t Lieferschein::StandardLaenge(const ArtikelBase artikel) throw (SQLerror)
{
 return (Query("select menge from lieferscheinentry e join lieferschein l "
   "using (lfrsid,instanz) where artikelid=? " // "and l.instanz=? "
   "order by datum desc limit 1") << artikel.Id() /* << Instanz()->Id() */
   ).FetchOneMap<LieferscheinBase::mengen_t>();
}

#ifdef DPD_LIEFERSCHEINE
const ManuProC::Datum Lieferschein::getMaxZahlziel() const throw(SQLerror)
{
 return (Query("SELECT max(a.zahlungsziel) from "
 	"auftrag a join lieferscheinentry e "
   	"on (refauftragid=auftragid and a.instanz=e.instanz) "
   	"where (e.instanz,e.lfrsid)=(?,?)")
   	<< Instanz()->Id() << Id()
   	).FetchOneMap<ManuProC::Datum>();
}
#endif

const std::string Lieferschein::Notiz() const throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,Instanz(),Id());
 if(notiz_valid) return notiz;

 Query("select notiz from lieferschein where (instanz,lfrsid)=(?,?)")
 	<< Instanz()->Id() << Id()
 	>> FetchIStream::MapNull(notiz);
 notiz_valid=true;
 return notiz;
}

void Lieferschein::Notiz(const std::string n) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,Instanz(),Id(),n);

 Query("update lieferschein set notiz=? where (instanz,lfrsid)=(?,?)")
 	<< n 
 	<< Instanz()->Id() << Id();
 SQLerror::test(__FILELINE__);

 notiz=n;
}

#ifdef MABELLA_EXTENSIONS
void Lieferschein::setVerknr(const Kunde::ID vknr) throw(SQLerror)
{
 Query("update lieferschein set verknr=? where"
	" (instanz,lfrsid)=(?,?)")
	<< Query::NullIf(vknr,Kunde::none_id)
	<< Instanz()->Id()
	<< Id();
 SQLerror::test(__FILELINE__);

 verknr=vknr;
}
#endif

void Lieferschein::setDatum(const ManuProC::Datum &d) throw(SQLerror)
{  
 Query("update lieferschein set geliefertam=? where "
 	"(instanz,lfrsid) = (?,?)") 
 	<< d << Instanz()->Id() << Id() ;
 SQLerror::test(__FILELINE__);
}

const UniqueValue::value_t LieferscheinBase::trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("DEBUG_LIEFERSCHEIN",LieferscheinBase::trace_channel);
