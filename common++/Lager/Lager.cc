// $Id: Lager.cc,v 1.45 2003/12/02 10:33:47 jacek Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "Lager.h"
#include <Misc/Transaction.h>
#include <algorithm>
#include <Misc/relops.h>
#include <Misc/TraceNV.h>
#include <Misc/FetchIStream.h>
#include <Auftrag/AufEintrag.h>

#ifndef MABELLA_LAGERHACK
#include "FertigWarenLager.h"
#endif

std::ostream &operator<<(std::ostream &o,const ProductionContext &pc)
{  o << pc.aeb << '|' << pc.leb;
   if (pc.lager_aeb.valid()) o << '|' << pc.lager_aeb;
   return o;
}

Lager::Lager(cH_ppsInstanz instanz)
: LagerBase(instanz)
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
   if(instanz==ppsInstanzID::Rohwarenlager) 
     assert(!"Use class 'RohwarenLager()' instead of 'Lager(ppsInstanzID::Rohwarenlager)'\n");
   else if(instanz==ppsInstanzID::Bandlager)
     assert(!"Use class 'JumboLager()' instead of 'Lager(ppsInstanzID::Bandlager)'\n");
#endif        
}



void Lager::rein_ins_lager(const ArtikelBase &artikel,
	const AuftragBase::mengen_t &menge,bool produziert,
	        const ProductionContext &ctx) const
{  
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
	NV("artikel",artikel),NV("menge",menge),
     	NV("produziert",produziert),NV("ctx",ctx));
#ifndef MABELLA_LAGERHACK

#ifdef MABELLA_EXTENSIONS
 FertigWaren::enum_Aktion a;
 if(ctx.leb.valid()) a=(FertigWaren::enum_Aktion)'L';
 else a=(FertigWaren::enum_Aktion)'M';

 FertigWaren fw(artikel,a,menge.as_int(),ctx.leb.Id());
 FertigWarenLager fwl(fw);
 fwl.Einlagern(ctx);
#else
 LagerBase::rein_ins_lager(artikel,menge,produziert,ctx); 
#endif

#else
 LagerBase::rein_ins_lager(artikel,menge,produziert,ctx); 
#endif



}

void Lager::wiedereinlagern(const ArtikelBase &artikel,
	const AuftragBase::mengen_t &menge) const
{  
 LagerBase::wiedereinlagern(artikel,menge); 
}

void Lager::raus_aus_lager(const ArtikelBase &artikel,
	AuftragBase::mengen_t menge,bool fuer_auftrag,
	        const ProductionContext &ctx) const
{  
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
	NV("artikel",artikel),NV("menge",menge),
     	NV("fuer_auftrag",fuer_auftrag),NV("ctx",ctx));
#ifndef MABELLA_LAGERHACK

#ifdef MABELLA_EXTENSIONS
 FertigWaren::enum_Aktion a;
 if(ctx.leb.valid()) a=(FertigWaren::enum_Aktion)'L';
 else a=(FertigWaren::enum_Aktion)'M';

 FertigWaren fw(artikel,a,menge.as_int(),ctx.leb.Id());
 FertigWarenLager fwl(fw);
 fwl.Auslagern(ctx);
#else
 LagerBase::raus_aus_lager(artikel,menge,fuer_auftrag,ctx); 
#endif

#else

 LagerBase::raus_aus_lager(artikel,menge,fuer_auftrag,ctx); 

#endif
}





void LagerBase::rein_ins_lager(const ArtikelBase &artikel,
	const AuftragBase::mengen_t &menge,bool produziert,
	        const ProductionContext &ctx) const
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
	NV("this",*this),NV("Artikel",artikel),NV("Menge",menge),
     	NV("produziert",produziert),NV("ctx",ctx));
  // vielleicht doch besser nach AufEintrag?
  AufEintrag::Einlagern(*this,artikel,menge,produziert,ctx);
}

void LagerBase::raus_aus_lager(const ArtikelBase &artikel,
	AuftragBase::mengen_t menge,bool fuer_auftrag,
	        const ProductionContext &ctx) const 
{
  ManuProC::Trace _t(AuftragBase::trace_channel, __FUNCTION__,
     		NV("artikel",artikel),NV("menge",menge),
     		NV("fuer_auftrag",fuer_auftrag),NV("ctx",ctx));
  Transaction tr;

  if (ctx.lager_aeb.valid())
  {  assert(fuer_auftrag);
     AufEintrag(ctx.lager_aeb).Auslagern(menge,ctx);
#if 0     
     if (menge>0)
     {  AufEintrag::unbestellteMengeProduzieren(*this,artikel,menge,false,ctx.aeb);
        menge=0;
     }
     assert(!menge);
#endif     
  }
  else
  {
   // alle Aufträge zu einem Artikel suchen, 1er vor 2er, 0er nicht
   // geringstes Datum zuerst, abschreiben
   // bei einem 2er 1er erzeugen
   if (menge!=0 && fuer_auftrag)
      menge=AufEintrag::Auslagern(
 	  	AuftragBase(*this,AuftragBase::plan_auftrag_id),artikel,menge,fuer_auftrag,ctx.leb);
   if (menge!=0)
      menge=AufEintrag::Auslagern(
      		AuftragBase(*this,AuftragBase::dispo_auftrag_id),artikel,menge,fuer_auftrag,ctx.leb);
   if (menge!=0 && !fuer_auftrag)
      menge=AufEintrag::Auslagern(
 	  	AuftragBase(*this,AuftragBase::plan_auftrag_id),artikel,menge,fuer_auftrag,ctx.leb);
   
   if (menge!=0 && fuer_auftrag)
      // einfach als produziert vermerken
   {  if (menge>0)
         AufEintrag::unbestellteMengeProduzieren(*this,artikel,menge);
      else
         assert(menge>0);
   }
  }
  
  tr.commit();
}


std::vector<class LagerInhalt> LagerBase::LagerInhalt()  const
{
  return LagerInhalt_(ArtikelBase());
} 
  
class LagerInhalt LagerBase::LagerInhalt(const ArtikelBase& artikel) const
{
  std::vector<class LagerInhalt> L=LagerInhalt_(artikel);
  if(L.empty()) return class LagerInhalt(artikel,0,0,0,0);
  return *(L.begin());
}
        
void LagerBase::LagerInhaltSum(std::vector<class LagerInhalt>& LI) 
{
  std::sort(LI.begin(),LI.end());  
  std::vector<class LagerInhalt>::iterator i,j,k;
  for(i=LI.begin();std_neq(i,LI.end());)
   {
     j=i; ++j;
     if(*i!=*j) {++i; continue;}
     k=j;
     for(;std_neq(k,LI.end()) && *i==*k ;++k)  *i+=*k ;
     i=LI.erase(j,k);     
   }
}

FetchIStream &operator>>(FetchIStream &is, LagerInhalt &li)
{  is >> FetchIStream::MapNull(li.artikel) 
	>> FetchIStream::MapNull(li.stueck) 
	>> FetchIStream::MapNull(li.reststueck) 
	>> FetchIStream::MapNull(li.menge)
	>> FetchIStream::MapNull(li.restmenge);
   return is;
}

