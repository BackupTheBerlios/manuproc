// $Id: AufEintrag_Menge.cc,v 1.31 2004/09/02 07:45:54 christof Exp $
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
#include <Misc/FetchIStream.h>
#include <Misc/string0.h>
#include <Misc/Transaction.h>
#include <Auftrag/AufEintragZu.h>
#include <Auftrag/Auftrag.h>
#include <unistd.h>
#include <Misc/TraceNV.h>
#include <Lager/Lager.h>
#include <Auftrag/VerfuegbareMenge.h>
#include <Misc/Changejournal.h>
//#include <Lieferschein/Lieferschein.h>
#include <Auftrag/AufEintrag_loops.h>
#include <Misc/inbetween.h>
#include <Artikel/ArtikelStamm.h>
#include <Misc/relops.h>
#include <Auftrag/sqlAuftragSelector.h>
#include <Auftrag/selFullAufEntry.h>
#include <Auftrag/AufEintrag_delayedreclaim.h>

#ifdef MABELLA_EXTENSIONS
//#include <Lager/FertigWarenLager.h>
#include <Artikel/ArtikelBase.h>
#endif

// statische Variante (behandelt z.B. Vormerkungen, freie Menge etc.)
AufEintragBase AufEintrag::ArtikelInternNachbestellen(const cH_ppsInstanz &wo,
  mengen_t menge,const ManuProC::Datum &lieferdatum,const ArtikelBase& artikel,
  const AufEintragBase& ElternAEB)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
   	wo,menge,lieferdatum,artikel,NV("ElternAEB",ElternAEB));
   if (!menge) return AufEintragBase(); // hmmm
   if (wo==ppsInstanzID::None || wo==ppsInstanzID::Kundenauftraege)
   {  std::cerr << "ArtikelInternNachbestellen: ungültige Instanz für " << menge
   		<< " von " << cH_ArtikelBezeichnung(artikel)->Bezeichnung()
   		<< " Elter " << ElternAEB << '\n';
      return AufEintragBase();
   }
   assert(menge>0);
   assert(ElternAEB.valid());

   // 2er Menge (freie Menge im Lager, freie Produktionsplanung)
   // vormerken
   VerfuegbareMenge AIL(wo,artikel,lieferdatum);
   mengen_t M_dispo=min(AIL.getMengeDispo(),menge);
   if(M_dispo>0) menge-=AIL.reduce_in_dispo(M_dispo,ElternAEB);

   // Im Lager von späteren Aufträgen vorgemerkte Menge wegschnappen
   if (menge>0 && wo->LagerInstanz())
   {  M_dispo=min(AIL.getMengePlan(),menge);
      if (!!M_dispo) menge-=AIL.reduce_in_plan(M_dispo,ElternAEB);
   }
   // Rest nachbestellen
   AuftragBase ab(wo,ungeplante_id);
   int znr=none_znr;
   if (menge>0)
      znr=ab.BestellmengeAendern(menge,lieferdatum,artikel,OPEN,ElternAEB);
   return AufEintragBase(ab,znr);
}

// ehemals updateStkDiffInstanz__ mit menge>0
// + ehemals BaumAnlegen
void AufEintrag::ArtikelInternNachbestellen(mengen_t menge) const
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,NV("menge",menge));
  assert(menge>0);
   // Rekursion von 2ern verbieten
   assert(Id()!=dispo_id);

   // 1er im Lager wird erhöht ... nichts tun
   if (Instanz()->LagerInstanz() && Id()==plan_id)
      return;
  ppsInstanz::ID next=Instanz()->NaechsteInstanz(ArtikelStamm(Artikel()));
  if (next!=ppsInstanzID::None)
  {  AufEintrag::ArtikelInternNachbestellen(next,menge,getLieferdatum(),
  			Artikel(),*this);
  }
  else if (!Instanz()->ExterneBestellung())
  {  ManuProC::Datum newdate=getLieferdatum()-Instanz()->ProduktionsDauer();

     ArtikelBaum AB(Artikel());
     for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
     {  ArtikelInternNachbestellen(ppsInstanz::getBestellInstanz(i->rohartikel),
  		i->menge*menge,newdate,i->rohartikel,*this);
     }
  }
}

// user visible Variant
AuftragBase::mengen_t AufEintrag::MengeAendern(mengen_t mengendelta)
{  return MengeAendern(mengendelta,true,AufEintragBase(),true);
}

// bei 2ern geht der Pfeil in die andere Richtung
AuftragBase::mengen_t AufEintrag::MengeAendern(mengen_t menge,bool instanzen,
     const AufEintragBase &ElternAEB,bool planen) throw(SQLerror)
{
 ManuProC::Trace _t(trace_channel, __FUNCTION__,*this,
	   NV("menge",menge),NV("instanzen",instanzen),
	   NV("Eltern",ElternAEB),NV("planen",planen));
 assert(!planen || (!ElternAEB && instanzen));
 if (!menge)  return menge;

 if (instanzen && Instanz()->LagerInstanz() && Id()==plan_id && menge<0)
 {  // hierbei handelt es sich nicht um Instanzen im engeren Sinne, da 
    // 1er im Lager nie Kinder haben, sondern die Menge soll wiederverwendet
    // werden
    std::cerr << "MengeAendern: Deprecated, use Abbestellen() instead of instanzen=1\n";
    assert(!planen);
    return Abbestellen(-menge,ElternAEB);
 }

 Transaction tr; // Beschleunigung + Lock
 Query("lock auftragentry in exclusive mode");
 SQLerror::test("updateStkDiff: lock table auftragentry");

 mengen_t menge2=menge;
 if (menge2<0) // nicht mehr abbestellen als noch offen ist
 		// aber dennoch den Pfeil reduzieren (wegen ProdSelbst)
 {  menge2=-min(-menge2,getRestStk());
    if (menge2!=menge) 
       ManuProC::Trace(trace_channel,"(Menge reduziert)",-menge,getRestStk(),menge2);
 }
 if (!!menge2)
 { menge2=updateStkDiffBase__(menge2);
   bestellt+=menge2;
   if (menge2>0 && entrystatus==CLOSED)
      setStatus(OPEN,true,false,false);
   else if (Id()!=ungeplante_id && Id()!=dispo_id 
   		&& !getRestStk() && entrystatus==OPEN
   		&& (!Instanz()->LagerInstanz() || !!bestellt))
      setStatus(CLOSED,false,false,false); 
 }

 if (ElternAEB.valid())
 {  if (Id()!=dispo_id) AufEintragZu(ElternAEB).Neu(*this,menge);
    // bei dispoaufträgen geht der Pfeil in die andere Richtung
    else AufEintragZu(*this).Neu(ElternAEB,menge);
 }

 if(auftragstatus==OPEN && instanzen && !!menge2)
  {  
   // Rekursion von 0ern im Lager (es gibt keine 3er im Lager)
   // Rekursion bei 0er, 1er oder 3er in Produktion
        Verzeigern(menge2,planen);
  }
  tr.commit();
  // wir haben zwar weniger abbestellt, aber nur weil wir geliefert haben
  
  // Der Rückgabewert kann kleiner als die abbestellte Menge sein, wenn
  // eine Auslagerung erfolgt ist (die nicht rückgängig gemacht wird)
  // [prod selbst]
  // Dies ist in Abbestellen zwingend erforderlich!
  
  // sollte der Aufrufende die unmodifizierte Menge brauchen, dann bitte
  // das Argument behalten
 return menge2;
}

AuftragBase::mengen_t AufEintrag::ArtikelInternAbbestellen_cb::operator()
	(const ArtikelBase &i,const AufEintragBase &j,mengen_t M) const
{  return AufEintrag(j).Abbestellen(M,mythis);
}

class AufEintrag::ArtikelAbbestellen_bevorzugt : public distribute_children_twice_cb
{	ArtikelInternAbbestellen_cb backend;

public:
	ArtikelAbbestellen_bevorzugt(const AufEintrag &_mythis)
		: backend(_mythis)
	{}
	// falls LagerInstanz 1er zuerst nehmen
	mengen_t operator()(const ArtikelBase &art,
 		const AufEintragBase &aeb,AuftragBase::mengen_t m,bool first) const
 	{  if (aeb.Instanz()->LagerInstanz() && aeb.Id()==plan_id)
 	   {  if (first) 
 	      {  backend(art,aeb,m);
                 // Tatsache ignorieren, dass bei 1ern im Lager weniger frei wurde, wenn
                 // ein Teil bereits ausgelagert ist. [prod selbst]
 	         return m;
 	      }
 	   }
 	   else
 	   {  if (!first) 
 	         return backend(art,aeb,m);
 	   }
 	   return 0;
 	}
 	void operator()(const ArtikelBase &art,AuftragBase::mengen_t m) const 
 	{  backend(art,m); }
 	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b) const
 	{  return distribute_children_cb::operator()(b,a); }
};

struct AufEintrag::zweierKinderAbbestellen_cb : ArtikelInternAbbestellen_cb
{  mengen_t &rest;

   zweierKinderAbbestellen_cb(const AufEintragBase &z, mengen_t &m) 
     : ArtikelInternAbbestellen_cb(z), rest(m) {}
   void operator()(const ArtikelBase &,mengen_t m) const
   {  rest=m; }
};

// Menge abbestellen und abhängige Dinge tun 
// (z.B. freie Menge erhöhen, neu verplanen)
AuftragBase::mengen_t AufEintrag::Abbestellen(const mengen_t &menge,const AufEintragBase &parent)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
       NV("menge",menge),NV("parent",parent));
   assert(menge>=0);
   if (Id()==ungeplante_id)
      return -MengeAendern(-menge,true,parent);
   else if (Instanz()->LagerInstanz())
   {  // vorgemerkte Menge freigeben
      assert(Id()==plan_id);
      mengen_t M=menge;
      M=-MengeAendern(-M,false,parent);
      // Ich brauche hier genau die geänderte Menge, da sonst bereits 
      // ausgelagerte (aber nicht gelieferte Mengen) miteinberechnet werden
      
     // Menge im Lager freigeben == Einlagern ohne Produktion?
       // nach nachbestellten 2ern suchen ...
        AufEintragBase zweier(AufEintragBase::PassendeZeile
            (AuftragBase(Instanz(),dispo_id),LagerBase::Lagerdatum(),
            Artikel(),OPEN));
        AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel
            (zweier,AufEintragZu::list_kinder,AufEintragZu::list_unsorted));
        AufEintrag zweier_ae(zweier);
        ManuProC::Trace(trace_channel, __FILELINE__,M,MapArt.size(),zweier_ae.getRestStk());
        // Optimierung
        if (!MapArt.empty())
        {  mengen_t freieMenge=zweier_ae.getRestStk()-Summe(MapArt[Artikel()]);
           ManuProC::Trace(trace_channel, __FILELINE__,M,Summe(MapArt[Artikel()]),MapArt.size());
           assert(freieMenge>=0);
           // freie 2er Menge vom 2er abziehen
           zweier_ae.MengeAendern(-freieMenge,false,AufEintragBase());
           M+=freieMenge;
           // alles verteilen
           distribute_children(MapArt,M,Artikel(),zweierKinderAbbestellen_cb(zweier,M));
        }
        if (delayed_reclaim::Active())
        {  // in 2er rein
           zweier_ae.MengeAendern(M,false,AufEintragBase());
           delayed_reclaim::delay(Instanz(),Artikel());
        }
        else
           AufEintrag::MengeVormerken(Instanz(),Artikel(),M,true,ProductionContext());
             // ,*this);
     return M;
   }
   else
     { // [1er oder] 3er - dispo anlegen, Bestellpfeil erniedrigen
       assert(Id()>=handplan_id);
       AufEintragZu(parent).setMengeDiff__(*this,-menge);
       mengen_t noch_frei=min(menge,getRestStk());
       if (!!noch_frei)
       {  // wie oben (zusammenführen!!!)
#warning zusammenführen
          if (delayed_reclaim::Active())
          {  AuftragBase(make_value(Instanz()),dispo_id)
       	     		.BestellmengeAendern(noch_frei,getLieferdatum(),
	       	     		Artikel(),OPEN,*this);
	     delayed_reclaim::delay(Instanz(),Artikel());
             DispoBeschraenken(); // vielleicht in dr ???
          }
          else
          {  noch_frei=MengeNutzen(*this,noch_frei);
             if (!!noch_frei)
             {  AuftragBase(make_value(Instanz()),dispo_id)
       	     		.BestellmengeAendern(noch_frei,getLieferdatum(),
	       	     		Artikel(),OPEN,*this);
	        DispoBeschraenken();
	     }
          }
       }
     }
     return menge;
}

void AufEintrag::ArtikelInternAbbestellen(mengen_t menge) const
{ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
   NV("menge",menge));

 assert(menge>0);
 // Rekursion von 2ern verbieten
 assert(Id()!=dispo_id);
 assert(!(Instanz()->LagerInstanz() && Id()==plan_id));
 // dann Abbestellen nehmen !!!

 try{
    if (lager_bevorzugt_freigeben)
      distribute_children_twice(*this,menge,Artikel(),
      			ArtikelAbbestellen_bevorzugt(*this));
    else
      distribute_children(*this,menge,Artikel(),
   			ArtikelInternAbbestellen_cb(*this));
  }catch(NoAEB_Error &e)
  {std::cerr <<"Falsche Anzahl von AufEinträgen: "<<e.what()<<'\n';
   exit(1);}
}

// updateStkDiffInstanz__ --> Verzeigern

// angepasste Variante von ppsInstanzReparatur::Eltern
AuftragBase::mengen_t AufEintrag::AnElternMengeAnpassen()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this));
   AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(*this,
		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
   assert(Id()!=dispo_id);
   assert(Instanz()!=ppsInstanzID::Kundenauftraege);
   mengen_t menge,menge2;
   for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();++i)
   {  menge+=i->Menge;
      if (i->AEB.Id()==dispo_id) menge2+=i->Menge;
   }
   if (menge>getStueck())
   // mehr v.o. benötigt als jemals bestellt (z.B. durch ProdRückgängig)
   {  ManuProC::Trace(trace_channel,"(menge>getStueck())",menge,getStueck());
      if (Id()==plan_id) // && Instanz()->LagerInstanz())
      {  // sonst ist der Code ungetestet, könnte aber funktionieren
         // bitte Nachricht an mich (CP), sobald das stört
         assert(Instanz()->LagerInstanz());
         return menge-getStueck();
      }
      assert(Id()==ungeplante_id); // sonst Problem !
      MengeAendern(menge-getStueck(),true,AufEintragBase());
   }
   else if (menge<getRestStk())
   // mehr offen als nun v.o. benötigt
   {  ManuProC::Trace(trace_channel,"(menge<getRestStk())",menge,getRestStk());
      assert(in(Id(),ungeplante_id,plan_id));
      assert(Id()!=plan_id || Instanz()->LagerInstanz());
      // war mal MengeAendern(,true,)
      Abbestellen(getRestStk()-menge,AufEintragBase());
   }
   return 0;
}

// auf noch offene Menge beschränken
// auf f.o. umstellen geht nicht sinnvoll, da die Funktion ein Unikat ist
void AufEintrag::DispoBeschraenken()
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,*this);
  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(*this,AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);

  mengen_t S=AufEintragZu::Summe(L,dispo_id);
  if (getRestStk()<S)
  {   mengen_t noch_weg=S-getRestStk();
      ManuProC::Trace(trace_channel, __FILELINE__,
  		"Dispo muss um",noch_weg,"erniedrigt werden",S,getRestStk());
      for(AufEintragZu::list_t::const_iterator i=L.begin();i!=L.end();++i)
      {  if(i->AEB.Id()==dispo_id)
         { mengen_t M=min(i->Menge,noch_weg);
           M=AufEintrag(i->AEB).MengeAendern(-M,false,*this);
           ManuProC::Trace(trace_channel, __FILELINE__,NV("MengeAendern",M));
           noch_weg+=M;
         }
      }
  }
}

// ähnlich move_to_Move, allerdings wird 2er
class dr_Planen_dp_cb : public distribute_parents_cb
{       AufEintrag &quelle,&ziel,&A2er;
public:
        dr_Planen_dp_cb(AufEintrag &q,AufEintrag &z,AufEintrag &a2) 
		: quelle(q), ziel(z), A2er(a2) {}
        virtual AuftragBase::mengen_t operator()(const AufEintragBase &, 
                                AuftragBase::mengen_t) const;
};

AuftragBase::mengen_t dr_Planen_dp_cb::operator()(const AufEintragBase &ae,
                                AuftragBase::mengen_t M) const
{  quelle.MengeAendern(-M,true,ae);
   AufEintragZu(ae).Neu(ziel,M);
   if (A2er.valid()) A2er.MengeAendern(-M,false,ziel);
   return M;
}

struct dr_Planen_cb : distribute_children_cb
{  AufEintrag &A0er;
   AufEintrag &A2er;
   mutable AuftragBase::mengen_t rest;

   dr_Planen_cb(AufEintrag &a,AufEintrag &a2) : A0er(a),A2er(a2) {}
   AuftragBase::mengen_t getRest() const { return rest; }
   AuftragBase::mengen_t operator()(const ArtikelBase &,
                           const AufEintragBase &,AuftragBase::mengen_t) const;
   void operator()(const ArtikelBase &,AuftragBase::mengen_t m) const
   {  rest=m; }
};

// wir können move_to nicht verwenden, da dies delayed_reclaim nutzt
AuftragBase::mengen_t dr_Planen_cb::operator()(const ArtikelBase &art,
	const AufEintragBase &ziel,AuftragBase::mengen_t m) const
{  AufEintrag z(ziel);
   return distribute_parents(A0er,m,dr_Planen_dp_cb(A0er,z,A2er));
}

struct FreieMengeNutzen2_cb : auf_positionen_verteilen_cb
{	AufEintrag &A3er;

	FreieMengeNutzen2_cb(AufEintrag &a) : A3er(a) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t M) const
	{  if (ae.getLieferdatum()>=A3er.getLieferdatum())
	   {  AufEintrag A2er;
	      return M-distribute_parents(ae,M,dr_Planen_dp_cb(ae,A3er,A2er));
	   }
	   return 0;
	}
};

AufEintrag::mengen_t AufEintrag::MengeNutzen(AufEintrag &ae, mengen_t m)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,ae,m);
   assert(ae.Id()>=handplan_id);
   SQLFullAuftragSelector waiting_orders(make_value(SQLFullAuftragSelector::
      	 	sel_Artikel_Planung_id(ae.Instanz()->Id(),Kunde::eigene_id,
      	 		ae.Artikel(),AuftragBase::ungeplante_id)));
    return auf_positionen_verteilen(waiting_orders,m,FreieMengeNutzen2_cb(ae));
}

// ACHTUNG: MengeNutzen_2er und FreieMengeNutzen_cb werden von 
// 	delayed_reclaim::reclaim aufgerufen und dürfen delayed_reclaim 
//	daher nicht nutzen!
struct FreieMengeNutzen_cb : auf_positionen_verteilen_cb
{	AufEintrag &A2er;

	FreieMengeNutzen_cb(AufEintrag &a) : A2er(a) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, "",
			NV("2er",A2er),A2er.getLieferdatum());
	   if (ae.getLieferdatum()>=A2er.getLieferdatum())
	   {  dr_Planen_cb cb(ae,A2er); 
	      distribute_children(A2er,M,A2er.Artikel(),cb);
	      return cb.getRest();
	   }
	   return 0;
	}
};

// 2er versuchen für 0er zu nutzen (Produktion)
void AufEintrag::MengeNutzen_2er(AufEintrag &ae)
{  assert(ae.Id()==dispo_id);
   SQLFullAuftragSelector waiting_orders(make_value(SQLFullAuftragSelector::
      	 	sel_Artikel_Planung_id(ae.Instanz()->Id(),Kunde::eigene_id,
      	 			ae.Artikel(),AuftragBase::ungeplante_id)));
   auf_positionen_verteilen(waiting_orders,
            		ae.getRestStk(),FreieMengeNutzen_cb(ae));
}
