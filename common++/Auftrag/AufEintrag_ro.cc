// $Id: AufEintrag_ro.cc,v 1.13 2005/10/11 15:16:14 christof Exp $
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
 geliefert(),
 artikel(_artikel),
 entrystatus(_entrystat),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(getuid()),
 lasteditdate(ManuProC::Datum::today()),
 letztePlanInstanz(ppsInstanzID::None),
 maxPlanInstanz(),
 preis(_preis),
 rabatt(_rabatt),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 disponr(),
 auftragstatus(_aufstatus), 
 dispoentrynr(),
 prozess(Prozess::default_id),
 preisliste(_preisliste),provsatz(-1),
 tmp_geliefert()
{
}
	
AufEintrag::AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr,
   mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _entrystatus) throw()
: AufEintragBase(_instanz,_auftragid,_zeilennr),
 bestellt(_bestellt),
 geliefert(),
 artikel(_artikel),
 entrystatus(_entrystatus),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(),
 auftragstatus(_entrystatus),
 prozess(Prozess::default_id),provsatz(-1),
 tmp_geliefert()
{}
	

const Preis AufEintrag::GPreis() const
#warning newly changed; not tested // 18.11.2003
{ 
 Preis ret(preis.Gesamtpreis(preis.getWaehrung(),bestellt.as_int(),0.0,rabatt),
		preis.getWaehrung(),preis.PreisMenge());
 return ret;
}

const Preis AufEintrag::EPreis(bool brutto) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);

 if(brutto) return preis;
 else
#warning newly changed; not tested // 18.11.2003,
#warning 23.01.2004 0->1 in Gesamtpreis
 return Preis(preis.Gesamtpreis(preis.getWaehrung(),
		preis.PreisMenge().as_int(),0.0,rabatt),
		preis.getWaehrung(),preis.PreisMenge());
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
  : AufEintragBase(aebb), artikel(),
    prozess(Prozess::default_id),provsatz(-1),
	tmp_geliefert()
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,aebb);
 try{
  SelectedFullAufList aufids((SQLFullAuftragSelector)
    (SQLFullAuftragSelector::sel_AufidZnr(aebb)));

  if ( aufids.size()!=1 ) throw NoAEB_Error(aufids.size());

  *this=*(aufids.begin());
 } catch (SQLerror &e) {std::cerr << e <<'\n';}
}
