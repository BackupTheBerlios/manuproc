// $Id: ppsInstanzReparatur.h,v 1.2 2003/02/10 14:33:59 christof Exp $
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

#ifndef AUX_PPSINSTANZ_REP_H
#define AUX_PPSINSTANZ_REP_H

#include <Instanzen/ppsInstanz.h>
#include <Auftrag/AufEintrag.h>
//class AufEintragBase;
class LagerInhalt;
//class AuftragBase;

class ppsInstanzReparatur : public ppsInstanz
{
 public:
   ppsInstanzReparatur(ID iid=ppsInstanzID::None)
      : ppsInstanz(iid) {}

 /////////////////////////////////////////////////////////////////////////
 // Reperatur
 private:
      struct st_table{std::string table; std::string column;
              st_table(const std::string &t,const std::string &c) 
               : table(t),column(c) {}};


      std::vector<LagerInhalt> getLagerInhalt() const;
      void vormerkungen_subrahieren(int uid,const  std::vector<LagerInhalt> &LI,const bool analyse_only) const;
      void DispoAuftraege_anlegen(const int uid,const ArtikelBase &artikel,const fixedpoint<ManuProC::Precision::AuftragsMenge> &menge) const;
      // 0er und 2er müssen immer offen sein
      void force_open_0er_und_2er(const bool analyse_only) const throw(SQLerror);
      // Alle Aufträge außer Kundenaufträgen und externen Bestellungen 
      // müssen die eigene KundenID haben
      void force_eigene_KundenId(const bool analyse_only) const throw(SQLerror);
      void force_2er_0er_geliefert_ist_null(const bool analyse_only) const throw(SQLerror);
      void force_execute(const std::vector<st_table> &Vtable,
          const std::vector<ManuProcEntity<>::ID> &Vauftragid,
          const int Wert,const std::string &was,const bool analyse_only) const throw(SQLerror);

      enum e_zumode{Dungeplant,Egeplant,Fdispo};
      bool Reparatur_Zuordnungen(const int uid,const bool analyse_only,
         const  ManuProcEntity<>::ID auftragid,const bool kinder,const e_zumode zumode) const throw(SQLerror);

      typedef fixedpoint<ManuProC::Precision::AuftragsMenge> ABmt;
//      bool check_D_ungeplant(const int uid,const bool analyse_only,const AufEintrag &AE,const ABmt &M0sum,const ABmt &Msum) const;
      bool check_D_ungeplant(const int uid,const bool analyse_only,const AufEintrag &AE,const AufEintragZu::list_t &L) const;
      bool check_E_geplant(const int uid,const bool analyse_only,const AufEintrag &AE,const ABmt &Msum) const;
      bool check_F_dispo(const int uid,const bool analyse_only,const AufEintrag &AE,const ABmt &Msum) const;

//      void check_D_ungeplantReparatur(const int uid,const AufEintrag &AE,const AuftragBase::mengen_t &menge) const; 
      void check_F_dispoReparatur(const int uid,const AufEintrag &AE,const AuftragBase::mengen_t &menge) const; 

      void Reparatur_Kundenauftrag_AE(const int uid,const AufEintrag &KundeAE,AufEintrag &KindAE,const ABmt &menge) const;
      void Reparatur_Kundenauftrag_AEB(const int uid,const AufEintrag &KundeAE,const AufEintragBase &KindAE,const ABmt &menge) const;
      void MengenReparatur(const int uid,const AufEintrag &AE, AufEintrag &AEB,const ABmt& zumenge) const;

      bool KK_teste_summen_fuer(const AufEintragBase aeb,const ArtikelBase KundenArtikel,const int uid,const bool analyse_only) const;


      void analyse(const std::string &s,const AufEintrag &AE,const std::string &x,const std::string &y) const;
      // Wrapper:
      void analyse(const std::string &s,const AufEintrag &AE) const{analyse(s,AE,"","");}
      void analyse(const std::string &s,const AufEintrag &AE,const AufEintragBase &x,const ABmt &y) const;
      void analyse(const std::string &s,const AufEintrag &AE,const ABmt &x,const ABmt &y) const;
      void analyse(const std::string &s,const AufEintrag &AE,const ArtikelBase &x,const ArtikelBase &y) const;
      void analyse(const std::string &s,const AufEintrag &AE,const cH_ppsInstanz &x,const cH_ppsInstanz &y) const;
 public:
      // Einlesen des Lagerinhalts und Anpassen der 2er unter Berücksichtigung der 1er
      void ReparaturLager(const int uid,const bool analyse_only) const throw(SQLerror);
      // Entweder existieren 0er oder es existieren 2er
      void Reparatur_0er_und_2er(const int uid,const bool analyse_only) const throw(SQLerror);
      void Reparatur_Konsistenz(const bool analyse_only) const throw(SQLerror);
      // Summe aller 0er-Zuordnungen zu einem 1|20000 = Menge des 1|20000
      bool ReparaturD_0_ZuSumme_1(const int uid,const bool analyse_only) const throw(SQLerror);
      // Summe aller 2er-Zuordnungen zu einem 1|20000 = Menge des 1|20000
      bool ReparaturE_2_ZuSumme_1(const int uid,const bool analyse_only) const throw(SQLerror);
      // Summe aller 2er-Zuordnungen zu einem 1|20000 <= REST-Menge des 1|20000
      bool ReparaturF_2_ZuSumme_1Rest(const int uid,const bool analyse_only) const throw(SQLerror);
      // Zuordnung von Kunden an Bestell-Instanz' entspricht Kundenbestellmenge
      bool ReparaturK_Kundenzuordnung(const int uid,const bool analyse_only) const;
      bool ReparaturKK_KundenKinder(const int uid,const bool analyse_only) const;
      // keine Eltern für Kunden und 2er
      bool ReparaturG_keine_Eltern(const int uid,const bool analyse_only) const;
      // ZuordnungsReparatur
      bool ReparaturST_AuftragsZuordnung(const int uid,const bool analyse_only,const bool kinder) const;
      // Lager-Zuordnungen
      bool ReparaturH_LagerZuordnungen(const int uid,const bool analyse_only) const;
};

#endif
