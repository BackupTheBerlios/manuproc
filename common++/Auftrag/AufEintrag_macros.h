/* $Id: AufEintrag_macros.h,v 1.9 2003/03/25 17:07:40 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef CXX_AUFTRAG_AUFEINTRAGMACROS_H
#define CXX_AUFTRAG_AUFEINTRAGMACROS_H

#include <Auftrag/AufEintragZu.h>

// one might do this with inheritance instead of templates

/* callee needs:
 *   AuftragBase::mengen_t operator()(const ArtikelBase &,
 *		const AufEintragBase &,AuftragBase::mengen_t) const;
 * return the amount of the third argument you processed
 *
 *   void operator()(const ArtikelBase &,AuftragBase::mengen_t) const;
 *	for the remainder
 */

template <class T>
 bool distribute_children(const AufEintragBase &startAEB,
 		AuftragBase::mengen_t menge,
 		const ArtikelBase &article, const T &callee)
{  AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB));
   ArtikelBaum AE_artbaum(article);
   for(AufEintragZu::map_t::const_iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var)
   {  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first);
      AuftragBase::mengen_t AE_menge2=AE_faktor*menge;
      for(AufEintragZu::list_t::const_iterator zuloop_var=artloop_var->second.begin();
	   		zuloop_var!=artloop_var->second.end();++zuloop_var)
      {  AuftragBase::mengen_t mengen_var;
         if (menge>=0) mengen_var=AuftragBase::min(zuloop_var->Menge,AE_menge2);
         else mengen_var=-AuftragBase::min(-AE_menge2,AufEintrag(zuloop_var->AEB).getGeliefert());
         if (!mengen_var) continue;

         mengen_var=callee(artloop_var->first,zuloop_var->AEB,mengen_var);

         AE_menge2-=mengen_var;
         if(!AE_menge2) break;
      }
      // pass the remainder
      if (!!AE_menge2) callee(artloop_var->first,AE_menge2);
   }
   return !MapArt.empty();
}

/* callee needs:
 *   AuftragBase::mengen_t operator()(const AufEintragBase &,AuftragBase::mengen_t) const;
 * return the amount of the third argument you processed
 */

template <class T>
 AuftragBase::mengen_t distribute_parents(const AufEintragBase &startAEB, AuftragBase::mengen_t menge,
 			const T &callee)
{  assert(menge>0);
   AufEintragZu::list_t Eltern =
        AufEintragZu::get_Referenz_list(startAEB,AufEintragZu::list_eltern,
                                         AufEintragZu::list_ohneArtikel);
   for (AufEintragZu::list_t::iterator i=Eltern.begin();i!=Eltern.end();++i)
   {  AuftragBase::mengen_t m=AuftragBase::min(i->Menge,menge);
      if (!m) continue;

      m=callee(i->AEB,m);

      menge-=m;
      if (!menge) break;
   }
   return menge;
}

template <class T>
 AuftragBase::mengen_t auf_positionen_verteilen(const SQLFullAuftragSelector &selector,
 		AuftragBase::mengen_t menge, const T &callee)
{ SelectedFullAufList auftraglist=SelectedFullAufList(selector);

  AuftragBase::mengen_t m=menge;
  for (SelectedFullAufList::iterator i=auftraglist.begin();i!=auftraglist.end();++i)
   {
     AuftragBase::mengen_t M;
     if (menge>=0) M=AuftragBase::min(i->getRestStk(),m);
     else M=-AuftragBase::min(-m,i->getGeliefert());
     if (!M) continue;

     M=callee(*i,M);

     m-=M;
     if(!m) break;
   }
   return m;
}

#endif
