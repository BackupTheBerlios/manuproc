// $Id: AufEintrag_Lager.cc,v 1.10 2003/07/31 14:48:53 christof Exp $
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

#ifdef MABELLA_EXTENSIONS
//#include <Lager/FertigWarenLager.h>
#include <Artikel/ArtikelBase.h>
#endif

// etwas bestelltes wird eingelagert -> produziert markieren & vormerken (?)
namespace { class MichEinlagern : public distribute_parents_cb
{	AufEintrag &mythis;
	unsigned uid;
	ProductionContext ctx;
public:
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  if (elter.Id()==AuftragBase::dispo_auftrag_id) return 0;
	   if (ctx.aeb.valid() && ctx.aeb!=elter) return 0;
	   mythis.Einlagern2(uid,m,elter,elter,ctx.leb);
	   return m;
	}
	MichEinlagern(AufEintrag &_mythis,const ProductionContext &_ctx) 
		: mythis(_mythis), uid(getuid()), ctx(_ctx) {}
};}

// etwas bestelltes wird abbestellt -> intern abbestellen & vormerken
namespace { class AbbestellenUndVormerken : public distribute_parents_cb
{	AufEintrag &mythis;
	unsigned uid;
	
public:
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  mythis.MengeAendern(uid,-m,true,elter,ManuProC::Auftrag::r_Closed);
	   AuftragBase zielauftrag(mythis.Instanz(),AuftragBase::plan_auftrag_id);
	   AufEintragBase neuerAEB(zielauftrag,
                       zielauftrag.PassendeZeile(mythis.getLieferdatum(),
                       			mythis.Artikel(),OPEN,uid));
	   AufEintrag ae(neuerAEB);
	   ae.MengeAendern(uid,m,false,elter,ManuProC::Auftrag::r_Closed);
	   return m;
	}
	AbbestellenUndVormerken(AufEintrag &_mythis) 
		: mythis(_mythis), uid(getuid()) {}
};}

// Lagerinhalt ist verschwunden -> Vormerkung löschen & neu bestellen
namespace { class NeuBestellen : public distribute_parents_cb
{	AufEintrag &mythis;
	unsigned uid;
public:
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  mythis.MengeAendern(uid,-m,false,elter,ManuProC::Auftrag::r_Reparatur);
	   AufEintrag::ArtikelInternNachbestellen(mythis.Instanz(),m,
	   	mythis.getLieferdatum(),mythis.Artikel(),uid,elter);
	   return m;
	}
	NeuBestellen(AufEintrag &_mythis,unsigned u) : mythis(_mythis), uid(u) {}
};}

namespace { struct Auslagern_cb : public auf_positionen_verteilen_cb
{	unsigned uid;
	bool fuer_auftraege;
	ProductionContext2 ctx;
	
	Auslagern_cb(unsigned u, bool fa, const ProductionContext2 &_ctx) 
		: uid(u), fuer_auftraege(fa), ctx(_ctx) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t abschreibmenge) const
	{  if (ae.Id()==AuftragBase::plan_auftrag_id)
	   {  if (!fuer_auftraege)
	         distribute_parents(ae,abschreibmenge,NeuBestellen(ae,uid));
	      else ae.abschreiben(abschreibmenge);
	   }
	   else
	   {  assert(ae.Id()==AuftragBase::dispo_auftrag_id);
	      ae.MengeAendern(uid,-abschreibmenge,false,AufEintragBase(),ManuProC::Auftrag::r_Reparatur);
	      // als produziert markieren!
	      if (fuer_auftraege)
	         AufEintrag::unbestellteMengeProduzieren(ae.Instanz(),
	         		ae.Artikel(),abschreibmenge,uid);
	   }
	   return abschreibmenge;
	}
};}

AuftragBase::mengen_t AufEintrag::Auslagern
	(const AuftragBase &ab,const ArtikelBase &artikel,mengen_t menge, 
	 unsigned uid, bool fuer_auftraege,
	 const ProductionContext2 &ctx)
{  assert(ab.Instanz()->LagerInstanz());
   ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("ab",ab),
		NV("artikel",artikel),NV("menge",menge),
		NV("fuer_auftraege",fuer_auftraege),NV("ctx",ctx));
   if (menge<0)
   { WiederEinlagern(uid,ab.Instanz(),artikel,-menge);
     return 0;
   }
   SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::sel_Artikel_Planung_id
  			(ab.Instanz()->Id(),Kunde::eigene_id,artikel,ab.Id(),
  			 menge<0?CLOSED:OPEN)));
   Auslagern_cb callback(uid,fuer_auftraege,ctx);
   if (fuer_auftraege)
      return auf_positionen_verteilen(sel,menge,callback);
   else 
      return auf_positionen_verteilen_rev(sel,menge,callback);
     //if(abmenge<0)
     //   abschreibmenge=-min(-abmenge,i->getGeliefert());
}

AufEintragBase AufEintrag::default_opfer(cH_ppsInstanz i,mengen_t menge,const ArtikelBase &artikel)
{  SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::sel_Artikel_Planung_id
  			(i->Id(),Kunde::eigene_id,artikel,plan_auftrag_id,
  			 OPEN)));
   SelectedFullAufList L(sel);
   if (L.empty()) return AufEintragBase();
   return L.back();
}

AufEintragBase (*AufEintrag::opfer_auswaehlen)(cH_ppsInstanz,mengen_t,const ArtikelBase &)
	= &AufEintrag::default_opfer;

AuftragBase::mengen_t AufEintrag::Auslagern
	(mengen_t menge, unsigned uid, const ProductionContext &ctx,
		bool fuer_auftraege)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
		NV("ctx",ctx),NV("menge",menge));
   assert(ctx.aeb.valid());
   assert(Id()==plan_auftrag_id || Id()==ungeplante_id);

   if (Id()==plan_auftrag_id)
   {  // Zuordnung?
      abschreiben(menge);
   }
   else if (Id()==ungeplante_id)
   {  // abbestellen
      MengeAendern(uid,-menge,true,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
      // LagerMenge reduzieren (wir sollen ja das Lager führen)
      mengen_t brauch_noch=menge;
// müsste Produzierte Menge unten löschen wenn nicht prod_selbst!
     while (!!brauch_noch)
     {AufEintragBase opfer=(*opfer_auswaehlen)(Instanz(),brauch_noch,Artikel());
      if (!opfer)
      {  std::cerr << "LOG: ausgeliefert ohne Lagerinhalt\n";
         brauch_noch=0;
      }
      else
         brauch_noch=AufEintrag(opfer).Auslagern(brauch_noch,uid,ctx,false);
     }
      // produzieren
      unbestellteMengeProduzieren(Instanz(),Artikel(),menge,uid,true,
      		ctx.aeb/*oder *this?*/,ctx.leb);
   }
   return 0;
}

namespace { class Einlagern_cb : public auf_positionen_verteilen_cb
{	bool abbestellen;
        ProductionContext ctx;
        
public:
	Einlagern_cb(const ProductionContext &_ctx) 
		: abbestellen(false), ctx(_ctx) {}
	Einlagern_cb(bool abbest,const ProductionContext &_ctx) 
		: abbestellen(abbest), ctx(_ctx)
	{  assert(!abbest || !ctx.aeb.valid()); }
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t m) const
	{  AuftragBase::mengen_t rest;
	   if (!abbestellen)
	      rest=distribute_parents(ae,m,MichEinlagern(ae,ctx));
	   else
	      rest=distribute_parents(ae,m,AbbestellenUndVormerken(ae));
           return m-rest;
	}
};}

// abbestellen bedeutet: Menge wurde abbestellt und kann nun anderweitig vor-
// 	gemerkt werden, es werden keine geschlossenen 1er erzeugt 
//	sondern die Menge auf unteren Instanzen abgezogen
void AufEintrag::MengeVormerken(cH_ppsInstanz instanz,const ArtikelBase &artikel,
		mengen_t menge, bool abbestellen, const ProductionContext &ctx)
{
   ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("instanz",instanz),
      NV("artikel",artikel),NV("menge",menge),(abbestellen?"abbestellen":"produzieren"),
      NV("ctx",ctx));
  assert(instanz->LagerInstanz());
  assert(menge>=0);
  if(menge==0) return;
   Transaction tr;
   mengen_t m=
    auf_positionen_verteilen(SQLFullAuftragSelector(SQLFullAuftragSelector::
      			sel_Artikel_Planung_id(instanz->Id(),
      				Kunde::eigene_id,artikel,
      				ungeplante_id)),
      			menge,Einlagern_cb(abbestellen,ctx));
   if (m!=0)
      AuftragBase(instanz,dispo_auftrag_id).
   		BestellmengeAendern(m,LagerBase::Lagerdatum(),artikel,
	   		OPEN,getuid(),AufEintragBase());
   tr.commit();
}

// Ins Lager gekommene Menge neu vormerken
   // wird aufgerufen wenn Menge ins Lager kommt (LagerBase::rein_ins_lager)
   // und gibt reservierte Menge zurück
   // sollte Aufträge als produziert markieren
// ehemals AuftragBase::menge_neu_verplanen
void AufEintrag::Einlagern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
         const mengen_t &menge,bool produziert,
         const ProductionContext &ctx,
         const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge),NV("Reason",reason));

  assert(reason==ManuProC::Auftrag::r_Produziert);
  MengeVormerken(instanz,artikel,menge,!produziert,ctx);
}

namespace { class WiederEinlagern_cb : public auf_positionen_verteilen_cb
{
public:
	AuftragBase::mengen_t operator()(AufEintrag &i, AuftragBase::mengen_t M) const
	{  unsigned uid=getuid();
	   assert(i.Id()==AuftragBase::plan_auftrag_id);
           i.abschreiben(M);
           // Eltern des 0ers sind Eltern des 1ers, allerdings waren dessen 
           // Zuordnungen 0
           // Menge nachbestellen
           AuftragBase zielauftrag(i.Instanz(),AuftragBase::ungeplante_id);
           AufEintragBase neuerAEB(zielauftrag,
               	zielauftrag.PassendeZeile(i.getLieferdatum(),i.Artikel(),OPEN,uid));
           AufEintrag ae(neuerAEB);
           ae.MengeAendern(uid,-M,true,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
           // Menge wird direkt neu verplant (evtl. wieder abbestellt)
           i.MengeAendern(uid,M,true,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
           // assert(i.getCombinedStatus()==CLOSED);
           if (i.getCombinedStatus()!=CLOSED)
           {  std::cerr << "merkwürdig ... " << i << ':' << i.getCombinedStatus() << ','
           		<< i.getRestStk() << '\n';
              i.setStatus(CLOSED,uid);
           }
           return M;
	}
};}

// eigentlich auslagern mit negativer Menge ???
// allerdings: Reihenfolge ist umgekehrt
void AufEintrag::WiederEinlagern(const int uid,cH_ppsInstanz instanz,const ArtikelBase artikel,
         mengen_t menge,const ManuProC::Auftrag::Action reason) throw(SQLerror)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge),NV("Reason",reason));

  assert(reason==ManuProC::Auftrag::r_Produziert);
  // eigentlich sind nur die 1er interessant, dann aber OPEN wie CLOSED ...
  SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::
      			sel_Artikel(instanz->Id(),artikel)));
  // Vorzeichen ändern
  menge=-menge;
  menge=auf_positionen_verteilen_rev(sel,menge,WiederEinlagern_cb());
  if (!!menge)
  {  // eigentlich soll das nicht passieren, da es ja vorher produziert wurde ...
     ManuProC::Trace(trace_channel, __FUNCTION__,NV("es ist ein Rest geblieben",menge));
     Einlagern(uid,instanz,artikel,-menge,false,ProductionContext());
  }
}

// ehemals von ProduziertNG kopiert ... aber anders?
void AufEintrag::Einlagern2(unsigned uid, mengen_t M,
		const AufEintragBase &elter_alt,
		const AufEintragBase &elter_neu,
		const ProductionContext2 &ctx)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
			NV("this",*this),M,NV("alt",elter_alt),NV("neu",elter_neu));
   if (!M) return;		
   assert(Instanz()->LagerInstanz());
   assert(Id()!=dispo_auftrag_id && Id()<handplan_auftrag_id);
   AufEintragBase neuerAEB=*this;
   
   if (M<0) 
   {  assert(Id()==plan_auftrag_id);
//      abschreiben(M); // dat kann nicht richtig sein ...
      // Rekursion bedeutet hier: freigewordene Menge neu verplanen
      MengeAendern(uid,M,true,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
      if (!getRestStk()) setStatus(AufStatVal(CLOSED),uid,true);
   }
   else // M>0
   {  MengeAendern(uid,-M,false,elter_alt,ManuProC::Auftrag::r_Produziert);
      AuftragBase zielauftrag(Instanz(),plan_auftrag_id);
      neuerAEB=AufEintragBase(zielauftrag,
         		zielauftrag.PassendeZeile(getLieferdatum(),Artikel(),OPEN,uid));
      AufEintrag ae(neuerAEB);
      ae.MengeAendern(uid,M,false,elter_neu,ManuProC::Auftrag::r_Produziert);
   }

   KinderProduzieren(M,neuerAEB,ctx);
}

