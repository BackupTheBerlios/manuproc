/* $Id: adjust_store.h,v 1.6 2002/10/24 14:06:50 thoma Exp $ */
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2002 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef _ADJUST_STORE_H_
#define _ADJUST_STORE_H_

#include <Auftrag/AuftragBase.h>
#include <Instanzen/ppsInstanz.h>
#include <Lager/Lager.h>


class adjust_store
{
//        static const int LagerVorratId=AuftragBase::dispo_auftrag_id;
//        static const  lagerdatum=ManuProC::Datum::lagerdatum();
        cH_ppsInstanz instanz;
        std::vector<LagerInhalt> LI;

        struct st_artmeng{ArtikelBase artikel; AuftragBase::mengen_t menge;
               st_artmeng(ArtikelBase a, AuftragBase::mengen_t m)
                  :artikel(a),menge(m){}};
        std::vector<st_artmeng> UnverplanteArtikel;

        void getLagerInhalt();
        void vormerkungen_subrahieren();  
        void FreieMengenAuftraege_anlegen();
 public:
        adjust_store(cH_ppsInstanz instanz);

};


#endif
