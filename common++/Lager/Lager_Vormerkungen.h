/* $Id: Lager_Vormerkungen.h,v 1.25 2002/11/06 15:06:59 malte Exp $ */
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

#ifndef _PRODLAGER_HH_
#define _PRODLAGER_HH_
#include <Aux/Datum.h>
#include <Auftrag/AufEintrag.h>
#include <Instanzen/ppsInstanz.h>
#include <Lager/Lager.h>
#include <utility>
#include <vector>

class Lager_Vormerkungen : AufEintrag 
{
friend void AufEintragBase::vormerken_oder_bestellen(int uid,const AuftragBase::mengen_t &vormerkmenge,
            AuftragBase::mengen_t bestellmenge,
                        const ArtikelBase &artikel,const Petig::Datum &lieferdatum,
                                    AuftragBase::st_tryUpdateEntry st_bool=st_tryUpdateEntry()) throw(SQLerror);

friend void Lager::menge_neu_verplanen(int uid,
              const ArtikelBase& artikel,AuftragBase::mengen_t menge,
                            const AufEintragBase::e_reduce_reason reason) throw(SQLerror);

      Lager_Vormerkungen(const AufEintrag&);

      // vorgemerkt wird ein 2er Artikel
      // geschnappt wird ein 1er Artikel
      void artikel_vormerken_oder_schnappen(bool schnappen,AuftragBase::mengen_t menge,const ArtikelBase &artikel,int uid,bool reduce_old,
         std::vector<AufEintrag> dispo_auftrag=std::vector<AufEintrag>() );
};

#endif
