// $Id: AufEintrag_ro.cc,v 1.4 2003/09/15 11:58:40 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

#include <Auftrag/AufEintrag.h>
#include <Auftrag/selFullAufEntry.h>
#include <Misc/TraceNV.h>
#include <unistd.h>
#include <sys/types.h>


AufEintrag::AufEintrag(const AufEintragBase &aeb, mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _aufstatus, int _kdnr, const std::string _youraufnr,
	const Preis &_preis, rabatt_t _rabatt, AufStatVal _entrystat, 
	const cH_PreisListe &_preisliste) throw()
: AufEintragBase(aeb),
 bestellt(_bestellt),
 geliefert(0),
 artikel(_artikel),
 entrystatus(_entrystat),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(getuid()),
 lasteditdate(ManuProC::Datum::today()),
 letztePlanInstanz(ppsInstanzID::None),
 maxPlanInstanz(0),
 preis(_preis),
 rabatt(_rabatt),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 disponr(0),
 auftragstatus(_aufstatus), 
 dispoentrynr(0),
 prozess(Prozess::default_id),
 preisliste(_preisliste),provsatz(-1)
{
}
	
AufEintrag::AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr,
   mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _entrystatus) throw()
: AufEintragBase(_instanz,_auftragid,_zeilennr),
 bestellt(_bestellt),
 geliefert(0),
 artikel(_artikel),
 entrystatus(_entrystatus),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(0),
 auftragstatus(_entrystatus),
 prozess(Prozess::default_id),provsatz(-1)
{}
	

const Preis AufEintrag::GPreis() const
{ return preis.Gesamtpreis(1,bestellt.as_float(),rabatt);
}

const Preis AufEintrag::EPreis(bool brutto) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 if(brutto) return preis;
 else
 return preis.Gesamtpreis(preis.PreisMenge().as_int(),0,rabatt);
}

const std::string AufEintrag::getEntryStatusStr() const
{  return AuftragBase::getStatusStr(entrystatus);
}

bool AufEintrag::allesOK() const
{
 if (!getStueck()) return false;
 if (!getLieferdatum().valid()) return false;
 return true;
}

AufEintrag::AufEintrag(const AufEintragBase &aebb) throw (SQLerror,NoAEB_Error)
  : AufEintragBase(aebb), artikel(0),
    prozess(Prozess::default_id),provsatz(-1)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,aebb);
 try{
  SelectedFullAufList aufids((SQLFullAuftragSelector)
    (SQLFullAuftragSelector::sel_AufidZnr(aebb)));

  if ( aufids.aufidliste.size()!=1 ) throw NoAEB_Error(aufids.aufidliste.size());

  *this=*(aufids.aufidliste.begin());
 } catch (SQLerror &e) {std::cerr << e <<'\n';}
}


