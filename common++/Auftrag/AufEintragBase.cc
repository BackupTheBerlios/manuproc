// $Id: AufEintragBase.cc,v 1.10 2001/11/19 12:49:24 christof Exp $
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
#include <Auftrag/AuftragsBaum.h> 

AufEintragBase::AufEintragBase(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, int _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	int _geliefert,
	int _dispoentrynr, int _disponr, int _jahrgang,
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,
	const Preis &_preis, int _rabatt,
	AufStatVal _entrystatus, const Petig::Datum _lasteditdate) throw()
: AufEintragBase2(_instanz,_auftragid,_zeilennr),
 artikel(_artikel),
 dispoentrynr(_dispoentrynr),
 disponr(_disponr),
 status(_aufstatus),
 entrystatus(_entrystatus),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 geliefert(_geliefert),
 lieferdatum(_lieferdatum),
 lasteditdate(_lasteditdate),
 jahrgang(_jahrgang),
 prozess(Prozess::default_id),
 bestellt(_bestellt),
 preis(_preis),
 rabatt(_rabatt)
{
 prozess=cH_Prozess(_prozess ? _prozess : cH_Prozess::default_pid);
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


const std::string AufEintragBase::getEntryStatusStr() const
{
 switch(entrystatus)
   {case (AufStatVal)UNCOMMITED : return "ubest�tigt"; 
    case (AufStatVal)OPEN : return "offen"; 
    case (AufStatVal)CLOSED : return "fertig";
    case (AufStatVal)STORNO : return "storno";
   }
 return "-";
}


bool AufEintragBase::allesOK() const
{
 if (!getStueck()) return false;
 if (!getLieferdatum().valid()) return false;
 return true;
}


std::vector<pair<cH_Prozess,long> > AufEintragBase::getProzess2() const
{
  AuftragsBaum AB(*this,true);
  std::vector<pair<cH_Prozess,long> > L;
  for(AuftragsBaum::const_iterator i=AB.begin();i!=AB.end();++i)
   {
     if(i->AEB2.Id()!=0) // 0 = ungeplante Auftr�ge       
       L.push_back(pair<cH_Prozess,long>(
         cH_ppsInstanz(AufEintragBase(i->AEB2).getAuftragInstanz())->get_Prozess(),
         i->menge));

//cout << "Auftrag: "<<Id()<<' '<<Instanz()
//     <<"\tKinder : " <<i->AEB2.Id()<<' ' <<i->AEB2.Instanz()<<'\n';
   }
 return L;
}

std::string AufEintragBase::getProzess2_c_str() const
{
 std::vector<pair<cH_Prozess,long> > L=getProzess2();
 std::string s;
 for(std::vector<pair<cH_Prozess,long> >::const_iterator i=L.begin();i!=L.end();++i)
  {
   int menge = i->second;
   s+=i->first->getTyp()+" "+i->first->getText()+"("+itos(menge)+")";
   if(i+1!=L.end()) s+= ", ";
  }
 return s;
}


AufEintragBase2 AufEintragBase::get_AufEintrag_from_Artikel_by_Lfdate   
               (const ArtikelBase& artikel,const cH_ppsInstanz& instanz)
{
  std::list<AufEintragBase2> LI=get_AufEintragList_from_Artikel(artikel,instanz);
  std::list<pair<AufEintragBase2,long> > LR;
  for(std::list<AufEintragBase2>::const_iterator i=LI.begin();i!=LI.end();++i)
   {
     std::list<pair<AufEintragBase2,long> > LRtmp =
               AufEintragBase(*i).get_Referenz_AufEintragBase2();
     LR.merge(LRtmp); 
   }
  Petig::Datum lieferdatum;
  AufEintragBase2 AEB;
  // lieferdatum initialisieren
  if(LR.begin()!=LR.end()) 
  { lieferdatum=AufEintragBase(LR.begin()->first).getLieferdatum();
    AEB=LR.begin()->first;
   for(std::list<pair<AufEintragBase2,long> >::const_iterator i=LR.begin();i!=LR.end();++i)
   {
    Petig::Datum ld=AufEintragBase(i->first).getLieferdatum();
    if(ld<lieferdatum) 
      {
        AEB=i->first;
        lieferdatum=ld;
      }
   }
  }
}
