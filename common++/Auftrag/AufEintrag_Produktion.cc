// $Id: AufEintrag_Produktion.cc,v 1.37 2004/02/19 15:17:04 jacek Exp $
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

#include <Artikel/ArtikelBase.h>
#endif

AufEintragBase AufEintrag::unbestellteMengeProduzieren2(cH_ppsInstanz instanz,
                const ArtikelBase &artikel,mengen_t menge,bool rekursiv,
                const AufEintragBase &elter,const ProductionContext2 &ctx,
                ManuProC::Datum termin)
{  if (!termin.valid()) termin=ManuProC::Datum(1,1,1970);
   Transaction tr;
   AuftragBase zielauftrag(instanz,plan_auftrag_id);
   AufEintragBase neuerAEB(zielauftrag,
                       zielauftrag.PassendeZeile(termin,artikel,CLOSED));
   AufEintrag ae(neuerAEB);
   // elter kann nicht übergeben werden, da sonst bereits mit Menge angelegt
   ae.MengeAendern(menge,false,AufEintragBase());
   ae.abschreiben(menge);
   if (elter.valid()) AufEintragZu(elter).Neu(ae,0);
   // weiter unten als produziert markieren
   if (rekursiv)
   {  if (instanz->LagerInstanz())
      {  cH_ppsInstanz pi=ppsInstanz::getProduktionsInstanz(artikel);
         if (pi!=ppsInstanzID::None && !pi->ProduziertSelbst())
            unbestellteMengeProduzieren(pi,artikel,menge,true,neuerAEB);
      }
      else
      {  ArtikelBaum AB(artikel);
         for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
         {  cH_ppsInstanz bi=ppsInstanz::getBestellInstanz(i->rohartikel);
            if (bi!=ppsInstanzID::None && !bi->ProduziertSelbst())
               unbestellteMengeProduzieren(bi,i->rohartikel,i->menge*menge,
               		true,neuerAEB);
         }
      }
   }
   cH_ppsInstanz EI=instanz->EinlagernIn();
   if(EI->AutomatischEinlagern())
   {  assert(instanz->ProduziertSelbst()); // sonst Endlosrekursion
      ManuProC::Trace(trace_channel, "AutomatischEinlagern");
      Lager(EI).rein_ins_lager(artikel,menge,true,ProductionContext(neuerAEB,ctx));
   }
   tr.commit();
   return neuerAEB;
}

AufEintragBase AufEintrag::unbestellteMengeProduzieren(cH_ppsInstanz instanz,
                const ArtikelBase &artikel,mengen_t menge,bool rekursiv,
                const AufEintragBase &elter,const ProductionContext2 &ctx,
                ManuProC::Datum termin)
{  // Code wie in ProduziertNG
   // vielleicht zusammenführen?
   ManuProC::Trace _t(trace_channel, __FUNCTION__,instanz,
			   NV("artikel",artikel),NV("menge",menge),NV("rekursiv",rekursiv),
			   NV("elter",elter),NV("ctx",ctx),NV("termin",termin));
   assert(instanz!=ppsInstanzID::Kundenauftraege && instanz!=ppsInstanzID::None);

   if (instanz->LagerInstanz() && rekursiv)
   // petig:S: Auslagern ruft uMP ohne rekursion auf
   {  if (instanz->ProduziertSelbst()) return AufEintragBase();
      Lager L(instanz);
      L.raus_aus_lager(artikel,menge,false,ProductionContext(elter,ctx));
      // bräuchten wir den AEB?
      return AufEintragBase();
   }
   else return unbestellteMengeProduzieren2(instanz,artikel,menge,rekursiv,
   							elter,ctx,termin);
}

// etwas bestelltes wird produziert
class AufEintrag::MichProduzieren : public distribute_parents_cb
{	AufEintrag &mythis;
	ProductionContext2 ctx;
public:
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  if (elter.Id()==AuftragBase::dispo_auftrag_id) return 0;
	   mythis.ProduziertNG(m,elter,elter,ctx);
	   return m;
	}
	MichProduzieren(AufEintrag &_mythis,const ProductionContext2 &_ctx) 
		: mythis(_mythis), ctx(_ctx) {}
};

// Menge wurde geliefert.
// Um die Pfeile über uns muss sich jemand anderes kümmern

// menge kann negativ sein ...
// wo wird der 2er reduziert?
// wo werden die Instanzen darunter behandelt?
// ==> WurdeProduziert macht das ganze rekusiv
void AufEintrag::abschreiben(mengen_t menge) throw(SQLerror)
{ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),NV("Menge",menge));

  mengen_t GELIEFERT=getGeliefert()+menge;
  mengen_t BESTELLT=getStueck();
  AufStatVal STATUS=getEntryStatus();

 if(menge>=0 && !in(getCombinedStatus(),(AufStatVal)OPEN,(AufStatVal)CLOSED))
    throw(SQLerror(__FILELINE__,-1,"Auftragszeile ist nicht offen sondern "+itos(STATUS)));
 if(menge<0 && getCombinedStatus()==(AufStatVal)UNCOMMITED)
    throw(SQLerror(__FILELINE__,-1,"Auftragszeile ist nicht bestätigt"));
 if (menge<0 && -menge>getGeliefert())
 {  menge=-getGeliefert(); GELIEFERT=0; }

 if (!menge) return;

 AufStatVal oldstatus=getCombinedStatus();

 if(mengen_t(GELIEFERT)>=getStueck()) STATUS=(AufStatVal)CLOSED;
 else if(menge<0 && mengen_t(GELIEFERT)<getStueck()) STATUS=(AufStatVal)OPEN;

 Transaction tr;
 Query("lock table auftragentry in exclusive mode");

   Query("update auftragentry set geliefert=?, status=?, "
     "bestellt=?, letzte_lieferung=now() "
     "where (instanz,auftragid,zeilennr) = (?,?,?)").lvalue()
     << GELIEFERT << STATUS << BESTELLT
     << static_cast<const AufEintragBase&>(*this);
 SQLerror::test(__FILELINE__);

 geliefert=GELIEFERT;
 if(geliefert>=bestellt) entrystatus=(AufStatVal)CLOSED;

 // zumindest 2er werden nie geschlossen, nur auf 0 gesetzt
 if(STATUS!=oldstatus && Id()!=dispo_auftrag_id)
     setStatus(AufStatVal(STATUS),true,false,false);

 // eigentlich könnte das if hier weg ... geht aber schneller so
 // dispomenge auf offene Menge beschränken
 if (Instanz() != ppsInstanzID::Kundenauftraege && menge>0)
    DispoBeschraenken();

 tr.commit();
}

void AufEintrag::ProduziertNG(mengen_t M,const ProductionContext2 &ctx)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__, NV("this",*this), 
		NV("M",M), NV("ctx",ctx));
   // immer mal vorausgesetzt, das wird hier nicht intern verwendet
   // (sonst könnte es eine Endlosschleife geben)
   // dies ist hier um Lieferscheine im Lager (=auslagern) zu handeln
   if (Instanz()->LagerInstanz())
   {  Lager L(Instanz());
      L.raus_aus_lager(Artikel(),M,true,ProductionContext(AufEintragBase(),ctx));
   }
   else if (Id()>=handplan_auftrag_id || Id()==plan_auftrag_id)
   {  ProduziertNG(M,AufEintragBase(),AufEintragBase(),ctx);
   }
   else
   {  assert(M>=0);
      if (distribute_parents(*this,M,MichProduzieren(*this,ctx))!=0)
   	 assert(!"Rest geblieben");
   }


#ifdef MABELLA_EXTENSIONS

 if(Instanz()->getBestellInstanz(Artikel())==ppsInstanzID::None)
     {
      if(Instanz() == ppsInstanzID::Kundenauftraege)
        {  FertigWaren fw(artikel,(FertigWaren::enum_Aktion)'L',
                            M.abs().as_int(),ctx.Id());

           FertigWarenLager fwl(fw,ctx.lagerid);

           if(M < 0) fwl.Einlagern(ctx);
           else if(M > 0) fwl.Auslagern(ctx);
        }
     }
#endif

}

class AufEintrag::ProduziertNG_cb2 : public distribute_children_twice_cb
{  AufEintragBase alterAEB,neuerAEB;
   ProductionContext2 ctx;
   bool reverse;
public:
	ProduziertNG_cb2(const AufEintragBase &aAEB, 
		const AufEintragBase &nAEB,const ProductionContext2 &_ctx,
		bool rev)
		: alterAEB(aAEB), neuerAEB(nAEB), ctx(_ctx), reverse(rev) {}
	AuftragBase::mengen_t operator()(const ArtikelBase &art,
		const AufEintragBase &aeb,AuftragBase::mengen_t M,bool first) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
			NV("aeb",aeb),NV("M",M),NV("first",first));
	   if (!aeb.Instanz()->ProduziertSelbst())
	   {  if (aeb.Instanz()->LagerInstanz())
	      {  // Zuordnung anpassen
	         if (M>0) AufEintragZu(alterAEB).setMengeDiff__(aeb,-M);
		 Lager L(aeb.Instanz());
		 L.raus_aus_lager(art,M,true,ProductionContext(neuerAEB,ctx,aeb));
		 // der Verweis muss unten angelegt werden (aeb kann falsch sein)
		 // muss noch nachbestellt werden (vielleicht ist das wie NeuBestellen)
		 if (M<0) 
		    AufEintrag(neuerAEB).ArtikelInternNachbestellen(-M);
	      }
	      else
	         AufEintrag(aeb).ProduziertNG(M,alterAEB,neuerAEB,ctx);
           }
           else if (M<0) return 0; // soll ProdRueckgaengig2 machen ???
           else
           {  AufEintrag ae(aeb);
              if (first && aeb.Id()==AuftragBase::plan_auftrag_id)
              {  // wieviel geht noch ohne abzubestellen?
                 AufEintragZu::list_t eltern=AufEintragZu::get_Referenz_list(ae,
              		AufEintragZu::list_eltern,AufEintragZu::list_ohneArtikel);
                 AuftragBase::mengen_t menge;
                 for (AufEintragZu::list_t::iterator i=eltern.begin();i!=eltern.end();++i)
                    menge+=i->Menge;
		 ManuProC::Trace(AuftragBase::trace_channel,__FILELINE__,
			   NV("M",M),NV("menge",menge),NV("Rest",ae.getRestStk()));
                 M=AuftragBase::min(M,menge-ae.getRestStk());
                 if (!!M) AufEintragZu(alterAEB).setMengeDiff__(ae,-M);
   	      }
              else
              {  AufEintragZu(alterAEB).setMengeDiff__(ae,-M);
                 AuftragBase::mengen_t m2=ae.AnElternMengeAnpassen();
                 assert(!m2);
              }
           }
           return M;
	}

	// Überproduktion
	void operator()(const ArtikelBase &art,AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("art",art),NV("M",M));
	   if (M<0) // Ein Teil der Lieferung konnte nicht rückgängig gemacht werden
	   {  // similar code in AufEintrag::ArtikelInternNachbestellen
	      ArtikelStamm as(art);
	      ppsInstanz::ID inst= neuerAEB.Instanz()->NaechsteInstanz(as);
	      ManuProC::Datum termin=AufEintrag(neuerAEB).getLieferdatum();
	      if (inst==ppsInstanzID::None)
	      {  inst=ppsInstanz::getBestellInstanz(as)->Id();
	         termin=termin-neuerAEB.Instanz()->ProduktionsDauer();
	      }
	      AufEintrag::ArtikelInternNachbestellen(inst,-M,termin,art,neuerAEB);
	      return;
	   }
	   assert(M>0); // NaechsteInstanz?
	   cH_ppsInstanz wo=ppsInstanz::getBestellInstanz(art);
	   if (wo==neuerAEB.Instanz()) wo=ppsInstanz::getProduktionsInstanz(art);
	   assert(wo!=neuerAEB.Instanz());
	   AufEintrag::unbestellteMengeProduzieren(wo,art,M,true,neuerAEB,ctx);
	}
	// sortierung
	bool operator()(const AufEintragZu::st_reflist &a,const AufEintragZu::st_reflist &b)const
	{  if (reverse) return distribute_children_cb::operator()(b,a);
	   else return distribute_children_cb::operator()(a,b);
	}
};

// similar to move_to
// Produktion im Lager bedeutet:
// Einlagern==MengeVormerken   Auslagern==Produktion
void AufEintrag::ProduziertNG(mengen_t M,
		const AufEintragBase &elter_alt,
		const AufEintragBase &elter_neu,
		const ProductionContext2 &ctx)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
			NV("this",*this),M,NV("alt",elter_alt),
			NV("neu",elter_neu), NV("ctx",ctx));
   assert(Id()!=dispo_auftrag_id);
   assert(!Instanz()->LagerInstanz());

   AufEintragBase neuerAEB=*this;
   if ((Id()==plan_auftrag_id && M>0)
	|| Id()>=handplan_auftrag_id)
   {  // Überproduktion wird einfach vermerkt (geht nur bei 3ern)
      if (M<0) assert(-M<=getGeliefert());
     try
     {abschreiben(M); // M<0 ? -M : M);
      if (elter_alt.valid())
      {  mengen_t zmenge=AufEintragZu(elter_alt).getMenge(*this);
         if (M<0)
         {  assert(elter_neu.valid());
            AufEintragZu(elter_neu).setMengeDiff__(*this,M);
         }
         else if (!!zmenge)
            AufEintragZu(elter_alt).setMengeDiff__(*this,-min(zmenge,M));
         else
            std::cout << "ProduziertNG: kann von " << elter_alt << " nach " << *this
            	<< " keine " << M << " abziehen\n";
      }
      if (M>0 && elter_neu!=elter_alt) AufEintragZu(elter_neu).Neu(*this,0);
     } catch (SQLerror &e)
     {  if (tolerate_inconsistency) return;
        throw;
     }
   }
   else
   {  if (M<0) 
      {  assert(Id()==plan_auftrag_id);
         abschreiben(M);
      }
      MengeAendern(-M.abs(),false,M>0 ? elter_alt : AufEintragBase());
      if (M<0 && !getRestStk()) setStatus(AufStatVal(CLOSED),true,false,false);
      // mit M>0 bin ich mir nicht sicher ... CP
     if (M>0 || !cH_ppsInstanz(Instanz()->EinlagernIn())->AutomatischEinlagern())
     {AuftragBase zielauftrag(Instanz(),M>=0?plan_auftrag_id:ungeplante_id);
      AufStatVal st=(M<0) ? OPEN : CLOSED;
      neuerAEB=AufEintragBase(zielauftrag,
         	zielauftrag.PassendeZeile(getLieferdatum(),Artikel(),st));
      AufEintrag ae(neuerAEB);
      ae.MengeAendern(M.abs(),false,
         	M<0 ?elter_neu:AufEintragBase());
      if (M>0)
      {  ae.abschreiben(M);
         AufEintragZu(elter_neu).Neu(neuerAEB,0);
      }
     }
     // nur um sicher zu gehen, dass ich an den richtigen Fall gedacht habe (Überproduktion) ...
     else assert(getLieferdatum()==Lager::Lagerdatum());
   }
   
   KinderProduzieren(M,neuerAEB,ctx);

   cH_ppsInstanz EI=Instanz()->EinlagernIn();
   if(EI->AutomatischEinlagern())
   {  assert(Instanz()->ProduziertSelbst()); // sonst Endlosrekursion
      ManuProC::Trace(trace_channel, "AutomatischEinlagern");
      Lager(EI).rein_ins_lager(Artikel(),M,true,ProductionContext(neuerAEB,ctx));
   }
}

void AufEintrag::KinderProduzieren(mengen_t M, const AufEintragBase &neuerAEB,
			const ProductionContext2 &ctx)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
			NV("this",*this),NV("M",M),
			NV("neu",neuerAEB), NV("ctx",ctx));
   // Kinder bearbeiten
   ProduziertNG_cb2 callback(*this,neuerAEB,ctx,M>0);
   if (M<0) // normal direction
   {  distribute_children_artbaum(*this,M,Artikel(),callback);
   }
   else // reverse direction
   {  // bei ProdSelbst:
      // gelieferte 1er nutzen, 0er abbestellen dann erst 1er abbestellen
      distribute_children_twice(*this,M,Artikel(),callback);
   }
}
