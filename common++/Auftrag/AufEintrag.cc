// $Id: AufEintrag.cc,v 1.32 2003/03/08 08:51:54 christof Exp $
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
#include <Misc/string0.h>
#include <Misc/Transaction.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/Auftrag.h>
#include <unistd.h>
#include <Auftrag/ppsInstanzProduziert.h>
#include <Misc/Trace.h>
#include <Lager/Lager.h>
#include <Auftrag/AufEintragZuMengenAenderung.h>

AufEintrag::AufEintrag(const AufEintragBase &aeb, mengen_t _bestellt,
	ArtikelBase _artikel, const ManuProC::Datum _lieferdatum,
	AufStatVal _aufstatus, int _kdnr, const std::string _youraufnr,
	const Preis &_preis, rabatt_t _rabatt, AufStatVal _entrystat, 
	int uid, const cH_PreisListe &_preisliste) throw()
: AufEintragBase(aeb),
 bestellt(_bestellt),
 geliefert(0),
 artikel(_artikel),
 entrystatus(_entrystat),
 lieferdatum(_lieferdatum),
 lasteditdate_uid(uid),
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
 preisliste(_preisliste)
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
 prozess(Prozess::default_id)
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


void AufEintrag::setLetztePlanungFuer(cH_ppsInstanz planinstanz) throw(SQLerror)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
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
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
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
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
  int tiefe = ArtikelBaum(ArtId()).Tiefe();
  return itos(maxPlanInstanz)+"/"+itos(tiefe);  
}

// reason ist wichtig, da r_Produziert einen bereits geschlossenen Auftrag erzeugt
void AufEintrag::move_to(int uid,AufEintrag ziel,AuftragBase::mengen_t menge,ManuProC::Auftrag::Action reason) throw(std::exception)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,*this,"To=",ziel,"Menge=",menge,"Reason=",reason);
  Transaction tr;

  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,
		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
  for(AufEintragZu::list_t::reverse_iterator i=L.rbegin();i!=L.rend();++i)
  { AuftragBase::mengen_t M=AuftragBase::min(i->Menge,menge);
    if (!M) continue;
    MengeAendern(uid,-M,true,i->AEB,reason);
    if (reason!=ManuProC::Auftrag::r_Produziert)
    {  ziel.MengeAendern(uid,M,true,i->AEB,reason);
    }
    else
    {  ziel.MengeAendern(uid,M,true,i->AEB,reason);
       ziel.WurdeProduziert(M,i->AEB);
    }
    menge-=M;
    if(!menge) break;
  }
  tr.commit();
}      

AufEintragBase AufEintrag::getFirstKundenAuftrag() const
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__);
 std::vector<AufEintragBase> V=AufEintragZu(*this).getKundenAuftragV();
 if(V.empty()) return *this;
 else return *(V.begin());
}

void AufEintrag::Produziert(mengen_t menge,
   ManuProcEntity<>::ID lfrsid) throw(SQLerror)
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Menge=",menge);
  Kunde::ID kunde=Auftrag(*this).getKundennr();
  ManuProC::st_produziert sp(kunde,*this,menge,getuid(),lfrsid);
  Instanz()->Produziert(sp,ManuProC::Auftrag::r_Produziert);
}


#warning überdenken!!!
// warum nicht AufEintragBase zurückgeben?
// zielauftrag sollte Auftrag sein und nicht AuftragBase!

// reason: r_Produziert bewirkt ein sofortiges Schließen des neuen Auftrages
// (und kein internes Nachbestellen mehr)
int AufEintrag::Planen(int uid,mengen_t menge,const AuftragBase &zielauftrag,
   const Petig::Datum &datum,ManuProC::Auftrag::Action reason,
   AufEintragBase *verplanter_aeb,bool rekursiv) throw(std::exception)
{
   ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,"Menge=",menge,"Reason=",reason,"Zielauftrag=",zielauftrag,"rekursiv=",rekursiv);
   assert(Id()==AuftragBase::ungeplante_id);
   assert(entrystatus==OPEN);
   assert(auftragstatus==OPEN);
   assert(menge>0);
   assert(reason==ManuProC::Auftrag::r_Planen);
//   	 || reason==ManuProC::Auftrag::r_Produziert);
   assert(!Instanz()->LagerInstanz());
   assert(!rekursiv);

// ist das überhaupt noch erforderlich?
   if(verplanter_aeb) *verplanter_aeb=*this;

   Transaction tr;
   Auftrag ZA(zielauftrag);
//   ZA.setStatusAuftrag_(OPEN);

// wenn schon eine Zeile mit diesem Datum/Artikel/Status in diesem Auftrag
// existiert, Menge erhöhen. So kann man von Aufträge aus verschiedenen 
// Quellen zusammen fertigen

   // Zeile erzeugen/finden
   AufEintragBase neueZeile=AufEintragBase(zielauftrag,
   	zielauftrag.PassendeZeile(datum,Artikel(),entrystatus,uid));
// nix zu planen -> erledigt   
   if(menge==0) {tr.commit();   return neueZeile.ZNr();}
   
  AufEintrag AE1er(neueZeile);
   // dispo (2er) Auftrag anlegen bei Überplanung
   if (reason==ManuProC::Auftrag::r_Planen && menge-getRestStk() > 0)
    { assert(!Instanz()->LagerInstanz()); // CP
      mengen_t dispomenge = menge-getRestStk();
      // nur soviel unten verwenden (tatsächlich zu uns ziehen, 
      // 	Rest wird von uns in Ueberplanen bestellt)
      menge-=dispomenge;
      
      AE1er.Ueberplanen(uid,Artikel(),dispomenge,datum);

      // Produktionsplaner (ungetestet)
      if(zielauftrag.Instanz()->GeplantVon()!=ppsInstanzID::None)
       {
	    assert(!"yet implemented");
	    // hier wurde ehemals ein 2er im Planer angelegt
	    // Code siehe CVS
       }
    }
   
    move_to(uid,AE1er,menge,reason);

//---- ProzessInstanz setzen ------    
  // nur wenn aktiv durch Benutzer geplant 
  if (ManuProC::Auftrag::r_Planen 
  	&& zielauftrag.Id() != AuftragBase::plan_auftrag_id)
  { assert(zielauftrag.Id()>=handplan_auftrag_id);
    // Kundenauftrag suchen
    AufEintragZu::list_t ReferenzAufEintrag =
			         AufEintragZu(*this).get_Referenz_listFull(false);
    for (AufEintragZu::list_t::iterator i=ReferenzAufEintrag.begin();i!=ReferenzAufEintrag.end();++i)
    {
     if(i->AEB.Instanz()!=ppsInstanzID::Kundenauftraege) continue;
     i->AEB.setLetztePlanungFuer(instanz);
     i->AEB.calculateProzessInstanz();
    }
  }

 tr.commit();
 return neueZeile.ZNr();
}

// was soll diese Funktion? sieht mir fehlerhaft aus CP
void AufEintrag::ProduktionsPlanung(int uid,mengen_t menge,const AuftragBase &zielauftrag, 
      const ManuProC::Datum &datum,cH_ppsInstanz instanz) throw(std::exception)
{
//   Transaction tr;
   assert(!"AufEintrag::ProduktionsPlanung called");
   assert(Id()==AuftragBase::ungeplante_id);
   AufEintragBase newAEB;
   Planen(uid,menge,zielauftrag,datum,ManuProC::Auftrag::r_Planen,&newAEB);
   AuftragBase zielauftrag_instanz(instanz,AuftragBase::ungeplante_id);
   int znr=zielauftrag_instanz.BestellmengeAendern(menge,datum,Artikel(),OPEN,uid,newAEB);
//   tr.commit();
}


// einen Dispo Auftrag für einen Auftrag anlegen (wegen freier Menge)
void AufEintrag::Ueberplanen(int uid,const ArtikelBase& artikel,mengen_t menge,const ManuProC::Datum &datum)
{
 ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,str(),
   "Artikel=",artikel,"Menge=",menge,"Datum=",datum);
   // automatisch geplant ??
   assert(Id()!=plan_auftrag_id);

   AuftragBase dispoAB(Instanz(),dispo_auftrag_id);
   // warum dispoplanung? was tut das letzte true?
   st_BestellmengeAendern st(false,false,true);
   //int znr=
   dispoAB.BestellmengeAendern(menge,datum,artikel,OPEN,uid,*this,st);

   // zusätzliche Menge vermerken und Material bestellen
   MengeAendern(uid,menge,true,AufEintragBase(),ManuProC::Auftrag::r_Anlegen); // oder Planen?
}
