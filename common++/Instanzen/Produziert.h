// $Id: Produziert.h,v 1.3 2002/09/27 06:43:29 thoma Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef _PRODUZIERT_H
#define _PRODUZIERT_H
#include "ppsInstanz.h"
#include <Auftrag/AuftragBase.h>
#include <Artikel/ArtikelBase.h>
#include <Auftrag/selFullAufEntry.h>


class Produziert 
{
  public:
   enum Probleme {Mehr_ausgelagert_als_im_Lager_war,
                  Mehr_produziert_als_moeglich};

  private: 
     cH_ppsInstanz instanz;
     ArtikelBase artikel;
     AuftragBase::mengen_t menge;
     int uid;

     void check_dispo_auftraege(); // reduziert die 2er wenn menge nicht komplett bei 1er abgeschrieben werden kann
     AuftragBase::mengen_t get_Menge_for(int id_);
     void rekursion();
     AuftragBase::mengen_t abschreiben_oder_reduzieren(int id,AuftragBase::mengen_t abmenge);
//     AuftragBase::mengen_t abschreiben_oder_reduzierenL(SelectedFullAufList L,AuftragBase::mengen_t abmenge);
     void fehler(enum Probleme,int id,AuftragBase::mengen_t m1,
                                      AuftragBase::mengen_t m2);
     void Reduce_Zuordnung_Add_Parent(const AufEintragBase &aeb,AuftragBase::mengen_t menge);


  public:
     Produziert(cH_ppsInstanz i,ArtikelBase a,AuftragBase::mengen_t m,int _uid)
      : instanz(i),artikel(a),menge(m),uid(_uid) 
      { assert(instanz!=ppsInstanzID::None) ; }

     void NichtSelbst();
     void Lager_abschreiben();
};

#endif
