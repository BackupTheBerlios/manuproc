// $Id: AufEintrag.cc,v 1.10 2002/10/04 08:23:20 thoma Exp $
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
#include <Auftrag/Auftrag.h>

AufEintrag::AufEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	mengen_t _geliefert,
	int _dispoentrynr, int _disponr,
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const ManuProC::Datum& _prozdate,
	int _prozess,ppsInstanz::ID _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, rabatt_t _rabatt,
	AufStatVal _entrystatus, int _lasteditdate_uid,
	const ManuProC::Datum _lasteditdate,
	const ManuProC::Datum _letzte_lieferung,
   const cH_PreisListe &_preisliste) throw()
: AufEintragBase(_instanz,_auftragid,_zeilennr),

 bestellt(_bestellt),
 geliefert(_geliefert),
 artikel(_artikel),
 entrystatus(_entrystatus),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(_lasteditdate_uid),
 lasteditdate(_lasteditdate),
 letzte_lieferung(_letzte_lieferung),
 letztePlanInstanz(_letztePlanInstanz),
 maxPlanInstanz(_maxPlanInstanz),
 preis(_preis),
 rabatt(_rabatt),
 kdnr(_kdnr),
 youraufnr(_youraufnr),
 disponr(_disponr),
 auftragstatus(_aufstatus), 
 dispoentrynr(_dispoentrynr),
 prozess(Prozess::default_id),
 preisliste(_preisliste)
{
 prozess=cH_Prozess(Prozess::ID(_prozess ? _prozess : cH_Prozess::default_pid));
 if(! _prozess) prozdate=ManuProC::Datum();
 else prozdate.from_postgres(_prozdate.c_str());
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
{ return preis.Gesamtpreis(1,bestellt,rabatt);
}

const Preis AufEintrag::EPreis(bool brutto) const
{if(brutto) return preis;
 else
 return preis.Gesamtpreis(preis.PreisMenge(),0,rabatt);
}


void AufEintrag::setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror)
{
  AufEintragBase::setLetztePlanungFuer(planinstanz);
  letztePlanInstanz=planinstanz->Id();
}


#if 0
void AufEintrag::setVerarbeitung(const cH_Prozess p)
{
 AufEintragBase::setVerarbeitung(p);

 prozess=p;
 prozdate=ManuProC::Datum().today();
}
#endif


void AufEintrag::setLetzteLieferung(const ManuProC::Datum &datum) throw(SQLerror)
{
 AufEintragBase::setLetzteLieferung(datum);
 letzte_lieferung=datum;
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


void AufEintrag::move_to(int uid,AufEintragBase AEB,AuftragBase::mengen_t menge,bool reduce_old) throw(std::exception)
{
  Transaction tr;
  if(reduce_old)
   {
     mengen_t mt1=updateStkDiff__(uid,-menge,false,r_Produziert);
     assert(-menge==mt1);
   }
  mengen_t mt2=AufEintrag(AEB).updateStkDiff__(uid,+menge,false,r_Produziert);
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

/*
void AufEintrag::verplante_menge_freigeben_nach_abbestellung(const std::list<AufEintragZu::st_reflist> &K,mengen_t menge,int uid)
{
assert(!"XXXXX");
 int znr=-1,dummy;
 mengen_t mdummy;
 AuftragBase ab(Instanz(),dispo_auftrag_id);
 if (!ab.existEntry(Artikel(),getLieferdatum(),znr,dummy,mdummy,getEntryStatus()))  
   znr = ab.insertNewEntry(0,getLieferdatum(),Artikel(),getEntryStatus(),uid,false); 
 
 AufEintragBase newAEB(ab,znr);
 newAEB.updateStkDiffBase__(uid,menge);

 for (std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
  {
//cout << "\n\n\nLLL:\t"<<'\t'<<newAEB<<'\t'<<i->AEB<<'\t'<<i->Menge<<'\n';
    AufEintragZu(newAEB).Neu(i->AEB,i->Menge);

  }
}
*/

#include <Lager/Lager.h>
#include <Lager/Lager_Vormerkungen.h>

void AufEintrag::move_menge_to_dispo_zuordnung_or_lager(mengen_t menge,int uid,e_reduce_reason reason)
{
 std::list<AufEintragZu::st_reflist> K=AufEintragZu(*this).get_Referenz_list(*this,true);
 for (std::list<AufEintragZu::st_reflist>::const_iterator i=K.begin();i!=K.end();++i)
  {
    if(i->AEB.Id()==AuftragBase::ungeplante_id) continue;
    AufEintrag GeplanterAE(i->AEB);
    AuftragBase::mengen_t M;
    if(GeplanterAE.getRestStk()>=menge)  M=menge;
    else M=GeplanterAE.getRestStk();
    
    AufEintragZu(*this).setMengeDiff__(i->AEB,-M);

    if(Instanz()->LagerInstanz())
     {
      mengen_t mt=i->AEB.updateStkDiffBase__(uid,-M);

      H_Lager L(Instanz());
      L->dispo_auftrag_aendern(Artikel(),M);
      Lager_Vormerkungen::freigegeben_menge_neu_verplanen(Instanz(),Artikel(),M,uid,reason);

      assert(mt==mengen_t(-M));
     }
    else
     {
      std::list<AufEintragZu::st_reflist> E=AufEintragZu(i->AEB).get_Referenz_list_dispo(false);
      if(E.empty())
       {
         Auftrag ab((AuftragBase(Instanz(),dispo_auftrag_id)));
         AufEintragBase newaeb = ab.push_back(0,getLieferdatum(),Artikel(),getEntryStatus(),uid,false); 
         E.push_back(AufEintragZu::st_reflist(newaeb,Artikel(),0));
       }
      AufEintragBase aep_dispo;
      for (std::list<AufEintragZu::st_reflist>::const_iterator j=E.begin();j!=E.end();++j)
        {
         AufEintragZu(j->AEB).Neu(i->AEB,M); // versucht erst ein 'update'
         j->AEB.updateStkDiffBase__(uid,M);
         if(j->AEB.Id()==dispo_auftrag_id) aep_dispo=j->AEB;
        }
      std::list<AufEintragZu::st_reflist> R=AufEintragZu(*this).get_Referenz_list(*this,true);
      for(std::list<AufEintragZu::st_reflist>::const_iterator j=R.begin();j!=R.end();++j)
       {
//cout << aep_dispo<<'\t'<<j->AEB<<'\t'<<reason<<'\n';
         if(reason==r_Closed && j->AEB.Id()==ungeplante_id)
            AufEintragZu(aep_dispo).Neu(j->AEB,j->Menge);
       }
     }
    menge-=M;
    if(menge==AuftragBase::mengen_t(0)) return;
  }
}

