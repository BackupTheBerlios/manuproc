/* $Id: AufEintrag_macros.h,v 1.1 2003/03/07 08:27:50 christof Exp $ */
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

/* Yes I know that macros like these are regarded macro abuse,
	but they certainly reduce code duplication and I don't like
	creating function objects everywhere, CP */
/* hmmm, perhaps there's no alternative to function objects ... */

// distribute an amount to several children, scale the amount by the
// article history dependant multiplication factor

#define BEGIN_AE_DISTRIBUTE_CHILDREN(startAEB,menge,article,artloop_var,zuloop_var,mengen_var) \
	AufEintragZu::map_t MapArt(AufEintragZu::get_Kinder_nach_Artikel(startAEB)); \
	ArtikelBaum AE_artbaum(article); \
	for(AufEintragZu::map_t::const_iterator artloop_var=MapArt.begin();artloop_var!=MapArt.end();++artloop_var) \
	{  ArtikelBaum::faktor_t AE_faktor = AE_artbaum.Faktor(artloop_var->first); \
	   AuftragBase::mengen_t AE_menge2=AE_faktor*menge; \
	   for(AufEintragZu::list_t::const_iterator zuloop_var=artloop_var->second.begin(); \
	   		zuloop_var!=artloop_var->second.end();++j) \
	   {  mengen_t mengen_var=AuftragBase::min(zuloop_var->Menge,AE_menge2); \
	      if (!mengen_var) continue;
	      
#define END_AE_DISTRIBUTE_CHILDREN(mengen_var) \
	      AE_menge2-=mengen_var; \
	      if(!AE_menge2) break; \
	   } \
	}

#endif
