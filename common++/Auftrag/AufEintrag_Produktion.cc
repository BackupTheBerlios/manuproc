// $Id: AufEintrag_Produktion.cc,v 1.4 2003/07/25 12:53:17 jacek Exp $
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

AufEintragBase AufEintrag::unbestellteMengeProduzieren(cH_ppsInstanz instanz,
                const ArtikelBase &artikel,mengen_t menge,unsigned uid,bool rekursiv,
                const AufEintragBase &elter,const ProductionContext2 &ctx,
                ManuProC::Datum termin)
{  // Code wie in ProduziertNG
   ManuProC::Trace _t(trace_channel, __FUNCTION__,instanz,
			   NV("artikel",artikel),NV("menge",menge),NV("rekursiv",rekursiv));
   assert(instanz!=ppsInstanzID::Kundenauftraege && instanz!=ppsInstanzID::None);
   if (!termin.valid()) termin=ManuProC::Datum(1,1,1970);
   Transaction tr;
   AuftragBase zielauftrag(instanz,plan_auftrag_id);
   AufEintragBase neuerAEB(zielauftrag,
                       zielauftrag.PassendeZeile(termin,artikel,CLOSED,getuid()));
   AufEintrag ae(neuerAEB);
   // elter kann nicht übergeben werden, da sonst bereits mit Menge angelegt
   ae.MengeAendern(uid,menge,false,AufEintragBase(),ManuProC::Auftrag::r_Produziert);
   ae.abschreiben(menge);
   if (elter.valid()) AufEintragZu(elter).Neu(ae,0);
   if (rekursiv)
   {  // NaechsteInstanz geht nicht wegen static (nächste wovon)
      if (instanz->LagerInstanz())
      {  cH_ppsInstanz pi=ppsInstanz::getProduktionsInstanz(artikel);
         if (pi!=ppsInstanzID::None && !pi->ProduziertSelbst())
            unbestellteMengeProduzieren(pi,artikel,menge,uid,true,neuerAEB);
      }
      else
      {  ArtikelBaum AB(artikel);
         for(ArtikelBaum::const_iterator i=AB.begin();i!=AB.end();++i)
         {  cH_ppsInstanz bi=ppsInstanz::getBestellInstanz(i->rohartikel);
            if (bi!=ppsInstanzID::None && !bi->ProduziertSelbst())
               unbestellteMengeProduzieren(bi,i->rohartikel,i->menge*menge,
               		uid,true,neuerAEB);
         }
      }
   }
   tr.commit();
   return neuerAEB;
}

// etwas bestelltes wird produziert
namespace { class MichProduzieren : public distribute_parents_cb
{	AufEintrag &mythis;
	unsigned uid;
	ProductionContext2 ctx;
public:
	AuftragBase::mengen_t operator()(const AufEintragBase &elter,AuftragBase::mengen_t m) const
	{  if (elter.Id()==AuftragBase::dispo_auftrag_id) return 0;
	   mythis.ProduziertNG(uid,m,elter,elter,ctx);
	   return m;
	}
	MichProduzieren(AufEintrag &_mythis,const ProductionContext2 &_ctx) 
		: mythis(_mythis), uid(getuid()), ctx(_ctx) {}
};}

// Menge wurde geliefert.
// Um die Pfeile über uns muss sich jemand anderes kümmern

// menge kann negativ sein ...
// wo wird der 2er reduziert?
// wo werden die Instanzen darunter behandelt?
// ==> WurdeProduziert macht das ganze rekusiv
void AufEintrag::abschreiben(mengen_t menge) throw(SQLerror)
{int uid=getuid();
 ManuProC::Trace _t(trace_channel, __FUNCTION__,NV("this",*this),NV("Menge",menge));

  mengen_t GELIEFERT=getGeliefert()+menge;
  mengen_t BESTELLT=getStueck();
  AufStatVal STATUS=getEntryStatus();

 if(menge>=0 && getCombinedStatus()!=(AufStatVal)OPEN)
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
     setStatus(AufStatVal(STATUS),uid,true);

 // eigentlich könnte das if hier weg ... geht aber schneller so
 // dispomenge auf offene Menge beschränken
 if (Instanz() != ppsInstanzID::Kundenauftraege && menge>0)
    DispoBeschraenken(uid);

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
      L.raus_aus_lager(Artikel(),M,getuid(),true,ProductionContext(AufEintragBase(),ctx));
   }
   else if (Id()>=handplan_auftrag_id || Id()==plan_auftrag_id)
   {  ProduziertNG(getuid(),M,AufEintragBase(),AufEintragBase(),ctx);
   }
   else
   {  assert(M>=0);
      if (distribute_parents(*this,M,MichProduzieren(*this,ctx))!=0)
   	 assert(!"Rest geblieben");
   }

//#ifdef MABELLA_EXTENSIONS
// MABELLA FERTIGLAGER HACK

//if(Instanz() == ppsInstanzID::Kundenauftraege)
//  {  FertigWaren fw(artikel,(FertigWaren::enum_Aktion)'L',
//                      M.abs().as_int(),ctx.Id());
//     FertigWarenLager fwl(fw);
//     if(M < 0) fwl.Einlagern();
//     else if(M > 0) fwl.Auslagern();
//  }

// #endif
}

namespace {
class ProduziertNG_cb2 : public distribute_children_twice_cb
{  unsigned uid;
   AufEintragBase alterAEB,neuerAEB;
   ProductionContext2 ctx;
public:
	ProduziertNG_cb2(unsigned _uid, const AufEintragBase &aAEB, 
		const AufEintragBase &nAEB,const ProductionContext2 &_ctx)
		: uid(_uid), alterAEB(aAEB), neuerAEB(nAEB), ctx(_ctx) {}
	AuftragBase::mengen_t operator()(const ArtikelBase &art,
		const AufEintragBase &aeb,AuftragBase::mengen_t M,bool first) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("aeb",aeb),NV("M",M));
	   if (!aeb.Instanz()->ProduziertSelbst())
	   {  if (aeb.Instanz()->LagerInstanz())
	      {  AufEintragZu(alterAEB).setMengeDiff__(aeb,-M);
		 Lager L(aeb.Instanz());
		 // oder elter_neu?
		 L.raus_aus_lager(art,M,uid,true,ProductionContext(alterAEB,ctx,aeb));
	      }
	      else
	         AufEintrag(aeb).ProduziertNG(uid,M,alterAEB,neuerAEB,ctx);
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
	{  if (M<=0) return; // vielleicht ?
	   assert(M>0); // NaechsteInstanz?
	   cH_ppsInstanz wo=ppsInstanz::getBestellInstanz(art);
	   if (wo==neuerAEB.Instanz()) wo=ppsInstanz::getProduktionsInstanz(art);
	   assert(wo!=neuerAEB.Instanz());
	   AufEintrag::unbestellteMengeProduzieren(wo,art,M,uid,true,neuerAEB,ctx);
	}
};
}

namespace {
class ProduziertRueckgaengig2 : public distribute_children_cb
{  unsigned uid;
   AufEintrag alterAEB;
public:
	ProduziertRueckgaengig2(unsigned _uid, const AufEintrag &aAEB)
		: uid(_uid), alterAEB(aAEB) {}
	AuftragBase::mengen_t operator()(const ArtikelBase &art,
		const AufEintragBase &aeb,AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("aeb",aeb),NV("M",M));
	   if (aeb.Instanz()->ProduziertSelbst())
           {  AufEintragZu(alterAEB).setMengeDiff__(aeb,-M);
              AuftragBase::mengen_t m2=AufEintrag(aeb).AnElternMengeAnpassen();
              if (!!m2) // siehe ZI2 (1er im Lager)
              {  M+=m2;
                 assert(M<=0);
                 AufEintragZu(alterAEB).setMengeDiff__(aeb,-m2);
              }
           }
           return M;
	}

	// Überproduktion
	void operator()(const ArtikelBase &art,AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,NV("M",M));
	   //assert(!"needed");
	   ArtikelStamm as(art);
	   cH_ppsInstanz next=alterAEB.Instanz()->NaechsteInstanz(as);
	   if (next==ppsInstanzID::None)
	      next=ppsInstanz::getBestellInstanz(as);
	   if (/*alterAEB.Id()==AuftragBase::ungeplante_id &&*/ next->ProduziertSelbst())
	   {  AufEintrag::ArtikelInternNachbestellen(next,-M,
	   		alterAEB.getLieferdatum()
	   		  -alterAEB.Instanz()->ProduktionsDauer(),
	   		art,uid,alterAEB);
	   }
	   else std::cout << "ProduziertRueckgaengig2: Überproduktion " << M
	   	<< " von "<< art.Id() <<  '\n';
	}
};
}

// similar to move_to
// Produktion im Lager bedeutet:
// Einlagern==MengeVormerken   Auslagern==Produktion
void AufEintrag::ProduziertNG(unsigned uid, mengen_t M,
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
      MengeAendern(uid,-M.abs(),false,M>0 ? elter_alt : AufEintragBase(),
      		ManuProC::Auftrag::r_Produziert);
      if (M<0 && !getRestStk()) setStatus(AufStatVal(CLOSED),uid,true);
      AuftragBase zielauftrag(Instanz(),M>=0?plan_auftrag_id:ungeplante_id);
      AufStatVal st=(M<0) ? OPEN : CLOSED;
      neuerAEB=AufEintragBase(zielauftrag,
         	zielauftrag.PassendeZeile(getLieferdatum(),Artikel(),st,uid));
      AufEintrag ae(neuerAEB);
      ae.MengeAendern(uid,M.abs(),false,
         	M<0 ?elter_neu:AufEintragBase(),
         	ManuProC::Auftrag::r_Produziert);
      if (M>0)
      {  ae.abschreiben(M);
         AufEintragZu(elter_neu).Neu(neuerAEB,0);
      }
   }
   
   KinderProduzieren(M,neuerAEB,ctx);

   cH_ppsInstanz EI=Instanz()->EinlagernIn();
   if(EI->AutomatischEinlagern())
   {  assert(Instanz()->ProduziertSelbst()); // sonst Endlosrekursion
      ManuProC::Trace(trace_channel, "AutomatischEinlagern");
      Lager(EI).rein_ins_lager(Artikel(),M,uid,true);
   }
}

void AufEintrag::KinderProduzieren(mengen_t M, const AufEintragBase &neuerAEB,
			const ProductionContext2 &ctx)
{  ManuProC::Trace _t(trace_channel, __FUNCTION__,
			NV("this",*this),NV("M",M),
			NV("neu",neuerAEB), NV("ctx",ctx));
   unsigned uid=getuid();
   // Kinder bearbeiten
   ProduziertNG_cb2 callback(uid,*this,neuerAEB,ctx);
   if (M<0)
   {  distribute_children(*this,M,Artikel(),callback);
      // bei ProduziertSelbst hilft obiges nicht allein (keine Pfeile nach unten entstanden)
      AufEintrag neuerAE=neuerAEB;
      distribute_children(neuerAE,M,Artikel(),ProduziertRueckgaengig2(uid,neuerAE));
   }
   else
   {  // bei ProdSelbst 0er zuerst abbestellen ...
      distribute_children_twice_rev(*this,M,Artikel(),callback);
   }
}
