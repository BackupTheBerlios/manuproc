// $Id: AufEintragBase.cc,v 1.5 2001/07/16 09:54:26 christof Exp $
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

#include"AufEintragBase.h"
#include <Aux/string0.h>
 

AufEintragBase::AufEintragBase(ppsInstanz::ppsInstId _instanz,int _auftragid, int _zeilennr, int _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	int _geliefert,
	int _dispoentrynr, int _disponr, int _jahrgang,
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const std::string _prozdate,
	int _prozess,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystatus, const Petig::Datum _lasteditdate,
	const cH_ExtBezSchema schema) throw()
: AufEintragBase2(_instanz,_auftragid,_zeilennr),
  artikel(0,cH_ExtBezSchema(ExtBezSchema::default_ID)),
  prozess(Prozess::default_id)
{
 artikel=cH_AufArtikel((AufArtikel::ID)_artikel,schema);
 kdnr = _kdnr;
 bestellt = _bestellt;
 lieferdatum = _lieferdatum;
 dispoentrynr = _dispoentrynr;
 kdnr = _kdnr;
 jahrgang = _jahrgang;

 float tmpstkgr = artikel->Stueckgroesse();
 tmpstkgr = tmpstkgr ? tmpstkgr : 1;
 menge = _bestellt*tmpstkgr;
 rest = (_bestellt-_geliefert)*tmpstkgr;
 geliefert = _geliefert;
 status=_aufstatus;
 entrystatus=_entrystatus;
 lasteditdate=_lasteditdate;
 dispoentrynr = _dispoentrynr;
 disponr = _disponr;
 youraufnr = _youraufnr;

 prozess=cH_Prozess(_prozess ? _prozess : cH_Prozess::default_pid);
 if(! _prozess) prozdate=Petig::Datum();
 else prozdate.from_postgres(_prozdate.c_str());

 preis=_preis;
 rabatt=_rabatt;
}
	



std::ostream &operator<<(std::ostream &o,const AufEintragBase &aeb)
{  o << "{artikel="<< aeb.artikel->Bezeichnung() << "Instanz = "<<aeb.instanz<< " menge="
	<<aeb.menge << " dispoentrynr="
	<<aeb.dispoentrynr
	<< " auftragid="<<aeb.auftragid
	<<" zeilennr="<<aeb.zeilennr << " bestellt="<<aeb.bestellt
	<<" geliefert="<<aeb.geliefert << " rest=" << aeb.rest
	<<" lieferdatum="<< aeb.lieferdatum.c_str() << " meterprostk="<<
	 aeb.artikel->Stueckgroesse()<<
	 "Auftrag-Prozess="<<aeb.prozess->Id()<<
	 "Prozess-Datum="<<aeb.prozdate<<
	 "Preis"<<aeb.preis<<
	 "Rabatt"<<aeb.rabatt<<
	 "EntryStat"<<aeb.entrystatus<<
	 "LasteEditDate"<<aeb.lasteditdate<<
	 "}";
   return o;
}


#warning geht so nicht mehr (Umstellung von ArtikelZusammensetzung)
double AufArtikel::Stueckgroesse() const
{ return ArtikelBaum::Stueckgroesse();
}

cH_AufArtikel::cache_t cH_AufArtikel::cache;

cH_AufArtikel::cH_AufArtikel(AufArtikel::ID pid, const cH_ExtBezSchema &schema)
{  cH_AufArtikel *cached(cache.lookup(pid));
   if (cached) *this=*cached;
   else
   {  if (pid==default_pid) *this=cH_AufArtikel(new AufArtikel());
      else *this=cH_AufArtikel(new AufArtikel(pid,schema));
      cache.Register(pid,*this);
   }
}


const Preis AufEintragBase::GPreis() const
{ return preis.In(preis.getWaehrung(),bestellt)*(1-rabatt/10000.0);
#warning rabatt auf fixedpoint umstellen
#warning ob rabatt auf den Einzelpreis oder den Gesamtpreis (wie hier) gerechnet werden soll?
}


void AufEintragBase::setVerarbeitung(const cH_Prozess p)
{
 AufEintragBase2::setVerarbeitung(p);

 prozess=p;
 prozdate=Petig::Datum().today();
}


const std::string AufEintragBase::getEntryStatusStr() const
{
 switch(entrystatus)
   {case (AufStatVal)UNCOMMITED : return "ubestätigt"; 
    case (AufStatVal)OPEN : return "offen"; 
    case (AufStatVal)CLOSED : return "fertig";
    case (AufStatVal)STORNO : return "storno";
   }
 return "-";
}


