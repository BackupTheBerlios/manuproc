// $Id: AufEintrag_Lager.cc,v 1.38 2004/11/03 09:07:16 christof Exp $
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

#include <unistd.h>
#include <sys/types.h>

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

bool AufEintrag::lager_bevorzugt_freigeben=false;

// etwas bestelltes wird eingelagert -> produziert markieren & vormerken (?)
class AufEintrag::MichEinlagern : public distribute_parents_cb
{	AufEintrag &mythis;
	ProductionContext ctx;
public:
	mengen_t operator()(const AufEintragBase &elter,mengen_t m) const
	{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("elter",elter),
		NV("m",m));
	   if (elter.Id()==dispo_auftrag_id) return 0;
	   // if (ctx.aeb.valid() && ctx.aeb!=elter) return 0;
	   assert(ctx.aeb!=elter); // wurde das verwendet? CP
	   mythis.Einlagern2(m,elter,elter,ctx);
	   return m;
	}
	MichEinlagern(AufEintrag &_mythis,const ProductionContext &_ctx) 
		: mythis(_mythis), ctx(_ctx) {}
};

// etwas bestelltes wird abbestellt -> intern abbestellen & vormerken
class AufEintrag::AbbestellenUndVormerken : public distribute_parents_cb
{	AufEintrag &mythis;
	
public:
	mengen_t operator()(const AufEintragBase &elter,mengen_t m) const
	{  mythis.MengeAendern(-m,true,elter);
	   AuftragBase zielauftrag(mythis.Instanz(),plan_auftrag_id);
	   AufEintragBase neuerAEB(zielauftrag,
                       zielauftrag.PassendeZeile(mythis.getLieferdatum(),
                       			mythis.Artikel(),OPEN));
	   AufEintrag ae(neuerAEB);
	   ae.MengeAendern(m,false,elter);
	   return m;
	}
	AbbestellenUndVormerken(AufEintrag &_mythis) 
		: mythis(_mythis) {}
};

// Lagerinhalt ist verschwunden -> Vormerkung löschen & neu bestellen
class AufEintrag::NeuBestellen : public distribute_parents_cb
{	AufEintrag &mythis;
public:
	mengen_t operator()(const AufEintragBase &elter,mengen_t m) const
	{  mythis.MengeAendern(-m,false,elter);
	   // Optimierung: direkt 0er nehmen?
#if 0	   
           // passenden 0er erhöhen
            AuftragBase(child_ae.Instanz(),ungeplante_id)
              .BestellmengeAendern(m,child_ae.getLieferdatum(),child_ae.Artikel(),OPEN,j->AEB);
#endif
	   AufEintrag::ArtikelInternNachbestellen(mythis.Instanz(),m,
	   	mythis.getLieferdatum(),mythis.Artikel(),elter);
	   return m;
	}
	NeuBestellen(AufEintrag &_mythis) : mythis(_mythis) {}
};

struct AufEintrag::Auslagern_cb : public auf_positionen_verteilen_cb
{	bool fuer_auftraege;
	ProductionContext ctx;
	
	Auslagern_cb(bool fa, const ProductionContext &_ctx) 
		: fuer_auftraege(fa), ctx(_ctx) {}
	mengen_t operator()(AufEintrag &ae, mengen_t abschreibmenge) const
	{  if (ae.Id()==plan_auftrag_id)
	   {  if (!fuer_auftraege)
	      {  distribute_parents(ae,abschreibmenge,NeuBestellen(ae));
	         // Spezialfall durch unbestellteMengeProduzieren
	         if (!!ctx.aeb && abschreibmenge>0) 
	         {  AufEintragZu(ctx.aeb).Neu(ae,0);
	            unbestellteMengeProduzieren2(ae.Instanz(),
	                     ae.Artikel(),abschreibmenge,true,ctx.aeb,ctx.leb,
	                     ManuProC::Datum());
	         }
	      }
	      else 
	         ae.abschreiben(abschreibmenge);
	   }
	   else
	   {  assert(ae.Id()==dispo_auftrag_id);
	      ae.MengeAendern(-abschreibmenge,false,AufEintragBase());
	      // als produziert markieren!
	      if (fuer_auftraege)
	         AufEintrag::unbestellteMengeProduzieren(ae.Instanz(),
	         		ae.Artikel(),abschreibmenge,false,ctx.aeb);
#warning warum false?	         		
	   }
	   return abschreibmenge;
	}
};

// static
AuftragBase::mengen_t AufEintrag::Auslagern
	(const AuftragBase &ab,const ArtikelBase &artikel,mengen_t menge, 
	 bool fuer_auftraege,
	 const ProductionContext &ctx)
{  assert(ab.Instanz()->LagerInstanz());
   ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("ab",ab),
		NV("artikel",artikel),NV("menge",menge),
		NV("fuer_auftraege",fuer_auftraege),NV("ctx",ctx));
   if (menge<0)
   { WiederEinlagern(ab.Instanz(),artikel,-menge);
     return 0;
   }
   SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::sel_Artikel_Planung_id
  			(ab.Instanz()->Id(),Kunde::eigene_id,artikel,ab.Id(),
  			 menge<0?CLOSED:OPEN)));
   Auslagern_cb callback(fuer_auftraege,ctx);
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
   for (SelectedFullAufList::const_reverse_iterator i=L.rbegin();
   		i!=SelectedFullAufList::const_reverse_iterator(L.rend());++i)
   {  if (!!i->getRestStk()) return *i;
   }
   return AufEintragBase();
}

AufEintragBase (*AufEintrag::opfer_auswaehlen)(cH_ppsInstanz,mengen_t,const ArtikelBase &)
	= &AufEintrag::default_opfer;

// Zuordnung muss Instanz darüber machen (Ausnahme: 0er Pfeil anlegen)
		// warum? - ändern?
// member
AuftragBase::mengen_t AufEintrag::Auslagern
	(mengen_t menge, const ProductionContext &ctx,bool fuer_auftraege)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),
		NV("ctx",ctx),NV("menge",menge),NV("fuer_auftraege",fuer_auftraege));
   assert(ctx.aeb.valid());
   assert(Id()==plan_auftrag_id || Id()==ungeplante_id);

   if (Id()==plan_auftrag_id)
   {  if (fuer_auftraege) 
      {  abschreiben(menge);
         // wird für AP gebraucht
         if (menge>0) AufEintragZu(ctx.aeb).Neu(*this,0);
         
#if 0	// sonst geht Storno_LS nicht         
        // die Frage ist eher ob die untere Instanz automatisch produziert
         if (!Instanz()->ProduziertSelbst() && Instanz()->AutomatischEinlagern()
         	&& Instanz()->LagerInstanz() && menge<0)
         {  Abbestellen() ?
         
            // war das nur ein unbestellteMengeProduzieren?
            // dann ganz wegnehmen
            AufEintragZu::list_t kinder=AufEintragZu::get_Referenz_list
            		(*this,AufEintragZu::list_kinder,AufEintragZu::list_ohneArtikel);
            const bool egal=false;
            if (kinder.empty())
               MengeAendern(menge,egal,AufEintragBase());
            else // Menge neu verplanen
            {  // für AP:-2 notwendig
               // the concept breaks here: 2er do not have a connection
               // to the producing entry !!!
               // with luck everything looks well though
               MengeAendern(menge,true,AufEintragBase());
            }
         }
#endif
      }
      else
      {  MengeNeubestellen(menge);
      }
   }
   else if (Id()==ungeplante_id)
   {  // abbestellen
      assert(fuer_auftraege);
      MengeAendern(-menge,true,AufEintragBase());
      // LagerMenge reduzieren (wir sollen ja das Lager führen)
      mengen_t brauch_noch=menge;
// müsste Produzierte Menge unten löschen wenn nicht prod_selbst!
     while (!!brauch_noch)
     {AufEintragBase opfer=(*opfer_auswaehlen)(Instanz(),brauch_noch,Artikel());
      ManuProC::Trace(trace_channel,"",NV("opfer",opfer));
      if (!opfer)
      {  std::cerr << "LOG: ausgeliefert ohne Lagermenge von "<< Artikel()<<"\n";
         unbestellteMengeProduzieren2(Instanz(),Artikel(),menge,true,
                         ctx.aeb,ctx.leb,getLieferdatum());
         brauch_noch=0;
      }
      else 
      {  AufEintrag opfer2(opfer);
         mengen_t M=AuftragBase::min(brauch_noch,opfer2.getRestStk());
         if (!!M)
         {  // Menge verschwindet einfach aus dem Lager
            opfer2.MengeNeubestellen(M);
            // taucht einfach so wieder auf
            opfer2.MengeAendern(M,false,AufEintragBase());
            // produzieren
            opfer2.Auslagern(M,ctx,true);
            brauch_noch-=M;
         }
      }
     }
   }
   return 0;
}

class AufEintrag::Einlagern_cb : public auf_positionen_verteilen_cb
{	ProductionContext ctx;
        
public:
	Einlagern_cb(const ProductionContext &_ctx) : ctx(_ctx) 
	{  //assert(ctx.aeb.valid());
	}
	mengen_t operator()(AufEintrag &ae, mengen_t m) const
	{  return m-distribute_parents(ae,m,MichEinlagern(ae,ctx));
	}
};

struct AufEintrag::Abbestellen_cb : public auf_positionen_verteilen_cb
{	Abbestellen_cb() {}
	mengen_t operator()(AufEintrag &ae, mengen_t m) const
	{  return m-distribute_parents(ae,m,AbbestellenUndVormerken(ae));
	}
};

// abbestellen bedeutet: Menge wurde abbestellt und kann nun anderweitig vor-
// 	gemerkt werden, es werden keine geschlossenen 1er erzeugt 
//	sondern die Menge auf unteren Instanzen abgezogen

// vielleicht nicht mehr voll genutzt? [wegen Mindestmenge]
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
   mengen_t m;
   SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::
      		sel_Artikel_Planung_id(instanz->Id(),Kunde::eigene_id,artikel,
      				ungeplante_id)));
   if (abbestellen)
      m=auf_positionen_verteilen(sel,menge,Abbestellen_cb());
   else
      m=auf_positionen_verteilen(sel,menge,Einlagern_cb(ctx));
   if (m!=0)
      AuftragBase(instanz,dispo_auftrag_id).
   		BestellmengeAendern(m,LagerBase::Lagerdatum(),artikel,
	   		OPEN,AufEintragBase());
   tr.commit();
}

// EinlagernIn rückgängig
class AufEintrag::EinlagernRueckgaengig : public distribute_parents_cb
{	cH_ppsInstanz instanz;
	ArtikelBase artikel;
	ProductionContext ctx;
public:
	mengen_t operator()(const AufEintragBase &elter,mengen_t m) const
	{  assert(elter.Instanz()==instanz);
	   assert(instanz->LagerInstanz());
	   AufEintrag e(elter);
	   assert(e.Artikel()==artikel);
	   assert(elter.Id()==plan_auftrag_id);
	   // nutzt uns ctx.aeb noch etwas? (das Kind)
	   return Auslagern_cb(false,ctx.leb)(e,-m);
	}
	EinlagernRueckgaengig(cH_ppsInstanz i,ArtikelBase a,
		const ProductionContext &c) 
		: instanz(i),artikel(a),ctx(c) {}
};

// Ins Lager gekommene Menge neu vormerken
   // wird aufgerufen wenn Menge ins Lager kommt (LagerBase::rein_ins_lager)
   // und gibt reservierte Menge zurück
   // sollte Aufträge als produziert markieren
// ehemals AuftragBase::menge_neu_verplanen
void AufEintrag::Einlagern(cH_ppsInstanz instanz,const ArtikelBase artikel,
         const mengen_t &menge,bool produziert,
         const ProductionContext &ctx) throw(SQLerror)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge));

  if (menge<0)
  {  // Rückgängig von EinlagernIn
     assert(ctx.aeb.valid());
     assert(produziert);
     distribute_parents(ctx.aeb,menge,EinlagernRueckgaengig(instanz,artikel,ctx));
  }
  else 
  {  if (produziert)
     {  // sonst werden eventuell Mengen reserviert, die später abbestellt werden
        // vielleicht maskiert dies aber auch nur einen bug beim Abbestellen
        // (dass 1er im Lager nicht gelöscht werden, wenn die Menge zu groß wird)
        // siehe test L
        delayed_reclaim dlr;
        MengeVormerken(instanz,artikel,menge,!produziert,ctx);
     }
     else MengeVormerken(instanz,artikel,menge,!produziert,ctx);
  }
}

class AufEintrag::WiederEinlagern_cb : public auf_positionen_verteilen_cb
{
public:
	mengen_t operator()(AufEintrag &i, mengen_t M) const
	{  assert(i.Id()==plan_auftrag_id);
	   assert(M<0);
           i.abschreiben(M);
           // eventuell muss das in den 2er, wenn hier zu wenig offene Aufträge existieren
           {  AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(i,AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
              mengen_t sum=AufEintragZu::Summe(eltern);
              if (sum<i.getRestStk())
              {  i.Abbestellen(i.getRestStk()-sum,AufEintragBase());
              }
           }

           if (i.Instanz()->ProduziertSelbst()) return M;
           
           // was ist mit EinlagernIn? dann darf das hier auch nicht getan werden

           // Eltern des 0ers sind Eltern des 1ers, allerdings waren dessen 
           // Zuordnungen 0
           // Menge nachbestellen
           AuftragBase zielauftrag(i.Instanz(),ungeplante_id);
           AufEintragBase neuerAEB(zielauftrag,
               	zielauftrag.PassendeZeile(i.getLieferdatum(),i.Artikel(),OPEN));
           AufEintrag ae(neuerAEB);
           ae.MengeAendern(-M,true,AufEintragBase());
           // Menge wird direkt neu verplant (evtl. wieder abbestellt)
           i.MengeAendern(M,true,AufEintragBase());
           // assert(i.getCombinedStatus()==CLOSED);
           if (i.getCombinedStatus()!=CLOSED)
           {  std::cerr << "merkwürdig ... " << i << ':' << i.getCombinedStatus() << ','
           		<< i.getRestStk() << '\n';
              i.setStatus(CLOSED,false,true,false);
           }
           return M;
	}
};

// eigentlich auslagern mit negativer Menge ???
// allerdings: Reihenfolge ist umgekehrt
void AufEintrag::WiederEinlagern(cH_ppsInstanz instanz,const ArtikelBase artikel,
         mengen_t menge) throw(SQLerror)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("Instanz",instanz),
   NV("Artikel",artikel),NV("Menge",menge));

  // eigentlich sind nur die 1er interessant, dann aber OPEN wie CLOSED ...
  SQLFullAuftragSelector sel(make_value(SQLFullAuftragSelector::
      			sel_Artikel(instanz->Id(),artikel)));
  // Vorzeichen ändern
  menge=-menge;
  menge=auf_positionen_verteilen_rev(sel,menge,WiederEinlagern_cb());
  if (!!menge)
  {  // eigentlich soll das nicht passieren, da es ja vorher produziert wurde ...
     ManuProC::Trace(trace_channel, __FUNCTION__,NV("es ist ein Rest geblieben",menge));
     Einlagern(instanz,artikel,-menge,false,ProductionContext());
  }
}

// ehemals von ProduziertNG kopiert ... aber anders?
// etwas bestelltes wird eingelagert -> produziert markieren & vormerken (?)
void AufEintrag::Einlagern2(mengen_t M,
		const AufEintragBase &elter_alt,
		const AufEintragBase &elter_neu,
		const ProductionContext &ctx)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
			NV("this",*this),M,NV("alt",elter_alt),NV("neu",elter_neu),
			NV("ctx",ctx));
   if (!M) return;		
   assert(Instanz()->LagerInstanz());
   assert(Id()!=dispo_auftrag_id && Id()<handplan_auftrag_id);
   AufEintragBase neuerAEB=*this;
   
   if (M<0) 
   {  assert(Id()==plan_auftrag_id);
      // Rekursion bedeutet hier: freigewordene Menge neu verplanen
      MengeAendern(M,true,AufEintragBase());
      if (!getRestStk()) setStatus(AufStatVal(CLOSED),true,false,false);
   }
   else // M>0
   {  // true bewirkt in AP dass gelieferte Menge im Einkauf abbestellt wird
      // false bewirkt in L dass geliefert und nicht abbestellt wird
      bool instanzen=(Id()==ungeplante_id && Instanz()->AutomatischEinlagern());
      MengeAendern(-M,instanzen,elter_alt);
      AuftragBase zielauftrag(Instanz(),plan_auftrag_id);
      neuerAEB=AufEintragBase(zielauftrag,
         	zielauftrag.PassendeZeile(getLieferdatum(),Artikel(),OPEN));
      AufEintrag ae(neuerAEB);
      ae.MengeAendern(M,false,elter_neu);
      if (ctx.aeb.valid()) // nach unten verzeigern
         AufEintragZu(neuerAEB).Neu(ctx.aeb,0);
   }

   if (!ctx.aeb.valid())
      KinderProduzieren(M,neuerAEB,ctx.leb);
}

// ehemals increase_parents__reduce_assingments
void AufEintrag::MengeNeubestellen(mengen_t menge) throw(SQLerror)
{
  ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),NV("Menge",menge));
  assert(Id()==plan_auftrag_id);  
//  AufEintragZu::list_t L=AufEintragZu::get_Referenz_list(child_aeb,AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
  distribute_parents(*this,menge,NeuBestellen(*this));
}

