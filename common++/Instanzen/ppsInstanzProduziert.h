// $Id: ppsInstanzProduziert.h,v 1.8 2002/12/13 09:27:21 thoma Exp $
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

#include <Artikel/ArtikelBase.h>
#include <Auftrag/AufEintrag.h>
#include <Lieferschein/LieferscheinBase.h>


namespace ManuProC{
     struct st_produziert{ 
                           enum Probleme {Mehr_ausgelagert_als_im_Lager_war,
                                          Mehr_produziert_als_moeglich};

                           ArtikelBase artikel;Kunde::ID kunde;
                           AufEintrag AE;AuftragBase::mengen_t menge;
                           int uid;LieferscheinBase::ID lfrsid;
                           // Die folgenden Argumente werden nur für die Planug benötigt
                           AuftragBase auftragbase; 
                           int ZNr;
                           Petig::Datum lieferdatum;
            st_produziert(ArtikelBase a,AuftragBase::mengen_t m,
              	           int _uid,
                          Kunde::ID _k=Kunde::eigene_id,
                          LieferscheinBase::ID _lfrsid=LieferscheinBase::none_id,
                          AuftragBase _auftragbase=AuftragBase(),
                          const Petig::Datum _datum=Petig::Datum())
                     : artikel(a),kunde(_k),menge(m),uid(_uid),
                        lfrsid(_lfrsid),auftragbase(_auftragbase),
                        ZNr(ManuProcEintrag::none_znr),
                        lieferdatum(_datum)
                         {}
// auftragbase ist nur für die ppsInstanz::Planung() interessant.
         private:
            friend class AufEintrag;
            st_produziert(const Kunde::ID kunde,AufEintrag ae,AuftragBase::mengen_t m,int _uid,
                          LieferscheinBase::ID _lfrsid=LieferscheinBase::none_id)
                   { *this=st_produziert(ae.Artikel(),m,_uid,kunde,_lfrsid);
                 AE=ae; }

      friend void ppsInstanz::Produziert(ManuProC::st_produziert &P,ManuProC::Auftrag::Action reason=ManuProC::Auftrag::r_None) const throw(SQLerror);
      friend void ppsInstanz::Planen(ManuProC::st_produziert &P) const throw(SQLerror);
      friend void ppsInstanz::Lager_abschreiben(ManuProC::st_produziert &P) const;
      AuftragBase::mengen_t abschreiben_oder_reduzieren(ppsInstanz::ID instanz,int id,AuftragBase::mengen_t abmenge,bool planen_und_abschreiben_von_ungeplaneten=true);
      void Reduce_Zuordnung_Add_Parent(const AufEintragBase &aeb,AuftragBase::mengen_t menge);
      void Reduce_Zuordnung_And_2er_Parent(const AufEintrag &ae,AuftragBase::mengen_t menge);
      void Reduce_DispoEltern(const AufEintragBase &aeb,AuftragBase::mengen_t menge);
      void check_dispo_auftraege(ppsInstanz::ID instanz);
      AuftragBase::mengen_t get_Menge_for(ppsInstanz::ID instanz,int id_);
      void fehler(ppsInstanz::ID instanz,Probleme typ,int id,AuftragBase::mengen_t m1,
                                      AuftragBase::mengen_t m2);
   };
}

