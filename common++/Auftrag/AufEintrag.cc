// $Id: AufEintrag.cc,v 1.1 2002/05/06 13:39:17 christof Exp $
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

#include"AufEintrag.h"
#include <Aux/string0.h>
#include <Aux/Transaction.h>
#include <Auftrag/AufEintragZu.h>


AufEintrag::AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, mengen_t _bestellt,
	ArtikelBase _artikel, const Petig::Datum _lieferdatum,
	mengen_t _geliefert,
	int _dispoentrynr, int _disponr,
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,ppsInstanz::ID _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystatus, const Petig::Datum _lasteditdate) throw()
: AufEintragBase(_instanz,_auftragid,_zeilennr),

 bestellt(_bestellt),
 geliefert(_geliefert),
 artikel(_artikel),
 entrystatus(_entrystatus),
 lieferdatum(_lieferdatum),
 lasteditdate(_lasteditdate),
 letztePlanInstanz(_letztePlanInstanz),
 maxPlanInstanz(_maxPlanInstanz),
 preis(_preis),
 rabatt(_rabatt),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 disponr(_disponr),
 auftragstatus(_aufstatus), 
 dispoentrynr(_dispoentrynr),
 prozess(Prozess::default_id)

{
 prozess=cH_Prozess(Prozess::ID(_prozess ? _prozess : cH_Prozess::default_pid));
 if(! _prozess) prozdate=Petig::Datum();
 else prozdate.from_postgres(_prozdate.c_str());
}
	
AufEintrag::AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr,
   mengen_t _bestellt,
	ArtikelBase _artikel, const Petig::Datum _lieferdatum,
	AufStatVal _entrystatus) throw()
: AufEintragBase(_instanz,_auftragid,_zeilennr),
 bestellt(_bestellt),
 geliefert(0),
 artikel(_artikel),
 entrystatus(_entrystatus),
 lieferdatum(_lieferdatum),
 auftragstatus(_entrystatus),
 prozess(Prozess::default_id)
{}
	
/*
std::ostream &operator<<(std::ostream &o,const AufEintrag &aeb)
{  o << "{artikel="<< cH_ArtikelBezeichnung(aeb.artikel)->Bezeichnung() << "Instanz = "<<aeb.instanz->Id()
	<<" dispoentrynr="
	<<aeb.dispoentrynr
	<< " auftragid="<<aeb.auftragid
	<<" zeilennr="<<aeb.zeilennr << " bestellt="<<aeb.bestellt
	<<" geliefert="<<aeb.geliefert 
	<<" lieferdatum="<< aeb.lieferdatum.c_str() << " meterprostk="<<
	 "Auftrag-Prozess="<<aeb.prozess->Id()<<
	 "Prozess-Datum="<<aeb.prozdate<<
	 "Preis"<<aeb.preis<<
	 "Rabatt"<<aeb.rabatt<<
	 "EntryStat"<<aeb.entrystatus<<
	 "LasteEditDate"<<aeb.lasteditdate<<
	 "}";
   return o;
}
*/

const Preis AufEintrag::GPreis() const
{ return preis.Gesamtpreis(1,bestellt,rabatt/100.0);
}

const Preis AufEintrag::EPreis(bool brutto) const
{if(brutto) return preis;
 else
 return preis.Gesamtpreis(1,0,rabatt/100.0);
}

#if 0
void AufEintrag::setVerarbeitung(const cH_Prozess p)
{
 AufEintragBase::setVerarbeitung(p);

 prozess=p;
 prozdate=Petig::Datum().today();
}
#endif

void AufEintrag::abschreiben(mengen_t menge) throw(SQLerror)
{
 geliefert=AufEintragBase::abschreiben(menge);
 if(geliefert>=bestellt) entrystatus=(AufStatVal)CLOSED;
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

std::string AufEintrag::Planung() const
{
  int tiefe = ArtikelBaum(ArtId()).Tiefe();
  return itos(maxPlanInstanz)+"/"+itos(tiefe);  
}


void AufEintrag::move_to(AufEintragBase AEB,AuftragBase::mengen_t menge) throw(SQLerror)
{
//cout << "MOVE: "<<AufEintragBase(*this)<<'\t'<<AEB<<' '<<
//entrystatus<<' '<<auftragstatus<<'\t'<<menge<<'\n';
  Transaction tr;
  mengen_t mt1=updateStkDiff__(-menge,false);
  assert(-menge==mt1);
  mengen_t mt2=AufEintrag(AEB).updateStkDiff__(+menge,false);
  assert(menge==mt2);
  AufEintragZu(*this).Neu(AEB,menge); 
  tr.commit();
}      


std::vector<AufEintragBase> AufEintrag::getKundenAuftragV() const
{
 std::vector<AufEintragBase> V;
 std::list<AufEintragZu::st_reflist> L=AufEintragZu(*this).get_Referenz_listFull(false);
 for (std::list<AufEintragZu::st_reflist>::const_iterator i=L.begin();i!=L.end();++i)
  {
    V.push_back(i->AEB);
//cout << i->AEB2.Instanz()->Name()<<' '<<i->AEB2.Id()<<' '<<i->AEB2.ZNr()<<' '
//<<cH_ArtikelBezeichnung(AufEintrag(i->AEB2).ArtId())->Bezeichnung()<<'\n';
  }
 return V; 
}

AufEintragBase AufEintrag::getFirstKundenAuftrag() const
{
 std::vector<AufEintragBase> V=getKundenAuftragV();
 if(V.empty()) return *this;
 else return *(V.begin());
}
