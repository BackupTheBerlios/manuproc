// $Id: AufEintragBase.cc,v 1.14 2002/01/22 09:15:55 christof Exp $
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
//#include <Aux/ppsInstanz.h>
//#include <Auftrag/AuftragsBaum.h> 
#include <Artikel/ArtikelBaumFull.h>
#include <Auftrag/AuftragsEntryZuordnung.h>

AufEintragBase::AufEintragBase(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, mengen_t _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	mengen_t _geliefert,
	int _dispoentrynr, int _disponr,
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,int _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystatus, const Petig::Datum _lasteditdate) throw()
: AufEintragBase2(_instanz,_auftragid,_zeilennr),
 artikel(_artikel),
 dispoentrynr(_dispoentrynr),
 disponr(_disponr),
 auftragstatus(_aufstatus),
 entrystatus(_entrystatus),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 geliefert(_geliefert),
 lieferdatum(_lieferdatum),
 lasteditdate(_lasteditdate),
 prozess(Prozess::default_id),
 letztePlanInstanz(_letztePlanInstanz),
 maxPlanInstanz(_maxPlanInstanz),
 bestellt(_bestellt),
 preis(_preis),
 rabatt(_rabatt)
{
 prozess=cH_Prozess(Prozess::ID(_prozess ? _prozess : cH_Prozess::default_pid));
 if(! _prozess) prozdate=Petig::Datum();
 else prozdate.from_postgres(_prozdate.c_str());
}
	



std::ostream &operator<<(std::ostream &o,const AufEintragBase &aeb)
{  o << "{artikel="<< cH_ArtikelBezeichnung(aeb.artikel)->Bezeichnung() << "Instanz = "<<aeb.instanz->Id()<< " menge="
	<</*aeb.menge <<*/ " dispoentrynr="
	<<aeb.dispoentrynr
	<< " auftragid="<<aeb.auftragid
	<<" zeilennr="<<aeb.zeilennr << " bestellt="<<aeb.bestellt
	<<" geliefert="<<aeb.geliefert << " rest=" /*<< aeb.rest*/
	<<" lieferdatum="<< aeb.lieferdatum.c_str() << " meterprostk="<<
	 /*aeb.artikel->Stueckgroesse()<<*/
	 "Auftrag-Prozess="<<aeb.prozess->Id()<<
	 "Prozess-Datum="<<aeb.prozdate<<
	 "Preis"<<aeb.preis<<
	 "Rabatt"<<aeb.rabatt<<
	 "EntryStat"<<aeb.entrystatus<<
	 "LasteEditDate"<<aeb.lasteditdate<<
	 "}";
   return o;
}


const Preis AufEintragBase::GPreis() const
{ return preis.In(preis.getWaehrung(),bestellt)*(1-rabatt/10000.0);
// Braucht man die warnings noch? 14.11.01 MAT
#warning rabatt auf fixedpoint umstellen
#warning ob rabatt auf den Einzelpreis oder den Gesamtpreis (wie hier) gerechnet werden soll?
}


void AufEintragBase::setVerarbeitung(const cH_Prozess p)
{
 AufEintragBase2::setVerarbeitung(p);

 prozess=p;
 prozdate=Petig::Datum().today();
}

void AufEintragBase::abschreiben(mengen_t menge) throw(SQLerror)
{
 geliefert=AufEintragBase2::abschreiben(menge);
 if(geliefert>=bestellt) entrystatus=(AufStatVal)CLOSED;
}



const std::string AufEintragBase::getEntryStatusStr() const
{  return AuftragBase::getStatusStr(entrystatus);
}


bool AufEintragBase::allesOK() const
{
 if (!getStueck()) return false;
 if (!getLieferdatum().valid()) return false;
 return true;
}


/*
std::vector<pair<cH_Prozess,long> > AufEintragBase::getProzess2() const
{
  AuftragsBaum AB(*this,true);
  std::vector<pair<cH_Prozess,long> > L;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     if(i->AEB2.Id()==0) continue; // 0 = ungeplante Aufträge       
     L.push_back(pair<cH_Prozess,long>(
        cH_ppsInstanz(AufEintragBase(i->AEB2).getAuftragInstanz())->get_Prozess(),
        i->menge));

//std::cout << "Auftrag: "<<Id()<<' '<<Instanz()
//     <<"\tKinder : " <<i->AEB2.Id()<<' ' <<i->AEB2.Instanz()<<'\n';
   }
 return L;
}
*/
/*
std::string AufEintragBase::getProzess2_c_str() const
{
 std::vector<pair<cH_Prozess,long> > L=getProzess2();
 std::string s;
 for(std::vector<pair<cH_Prozess,long> >::const_iterator i=L.begin();i!=L.end();++i)
  {
   mengen_t menge = i->second;
   s+=i->first->getTyp()+" "+i->first->getText()+"("+itos(menge)+")";
   if(i+1!=L.end()) s+= ", ";
  }
 return s;
}
*/
std::string AufEintragBase::Planung() const
{
//  int tiefe = ArtikelBaumFull(ArtId()).Tiefe();
  AufEintragZu AEZ(*this);
  AEZ.get_Referenz_listFull(true,false); // nur um die Tiefe zu berechnen
  int tiefe = AEZ.Tiefe();
  return itos(maxPlanInstanz)+"/"+itos(tiefe);  
//  return cH_ppsInstanz((ppsInstanz::ppsInstId)letztePlanInstanz)->Name();  
}


/*
long AufEintragBase::get_Referenz_AufEintragBase2_Summe(int instanz,bool ursprung,bool kinder) const throw(SQLerror)
{
  std::list<pair<AufEintragBase2,long> > L=get_Referenz_AufEintragBase2(ursprung,kinder);
  long int summe=0;
  for(std::list<pair<AufEintragBase2,long> >::const_iterator i=L.begin();i!=L.end();++i)
   {
     //Aufträge mit Id=0 sind ungeplant!
     if(i->first.Instanz()==instanz && i->first.Id()!=0)
        summe+=i->second;
   }
  return summe;
}

*/
