// $Id: ppsInstanz.h,v 1.21 2002/12/09 13:13:23 thoma Exp $
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

#ifndef AUX_PPSINSTANZ_H
#define AUX_PPSINSTANZ_H
#include <ManuProCConfig.h>
#include <string>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>
#include <Aux/SQLerror.h>
#include <Artikel/Prozess.h>
#include <Auftrag/auftrag_enums.h>
#include <vector>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>
#include <Aux/fixedpoint.h>
#include <BaseObjects/ManuProcEntity.h>

class AufEintrag;
class AufEintragBase;
class ArtikelBase;
class cH_ppsInstanz;
class LagerInhalt;
class AuftragBase;

namespace ppsInstanzID=ManuProC::DynamicEnums::Instanzen;
namespace ManuProC{ struct st_produziert; }


class ppsInstanz : public HandleContent
{
public:
	typedef ppsInstanzID::enum_t ID;
 
private: 
 ID instid,lager_fuer,einlagern_in;
 std::string name;
 int sortierung;
 bool lieferschein;
 std::string lagername;
 bool produziert_selbst; 
 bool automatisch_einlagern;
 int produktionsdauer_tage;
 char typ;
 ID geplant_von;
 bool externe_bestellung;

 void get_name() throw(SQLerror);
 void check() const; 
public:
 static const ID default_id=ManuProC::DefaultValues::Instanz;
 
 ppsInstanz(ID iid=ppsInstanzID::None) : instid(iid),
                      lager_fuer(ppsInstanzID::None),
                      einlagern_in(ppsInstanzID::None),
                      sortierung(0),
                      lieferschein(false),produziert_selbst(false),
                      automatisch_einlagern(false),
                      produktionsdauer_tage(0),      
                      typ('0'),geplant_von(ppsInstanzID::None),
                      externe_bestellung(false)
                      {get_name(); check(); }

 ID Id() const { return instid; }
 bool valid() const {return Id()!=ppsInstanzID::None;}
 int Sortierung() const {return sortierung; }
 bool Lieferschein() const {return lieferschein;}
 bool ProduziertSelbst() const {return produziert_selbst;}
 std::string get_Name() const {return name;}
 std::string Name() const {return name;}
 std::string shortName() const;
private:
 ID LagerFuer() const { return lager_fuer; }
public:
 bool LagerInstanz() const ;
 bool EigeneLagerKlasseImplementiert() const;
 bool ProduktionsInstanz() const ;
 bool PlanungsInstanz() const ;
 ID EinlagernIn() const { return einlagern_in;}
 bool AutomatischEinlagern() const {return automatisch_einlagern;}
 bool ExterneBestellung() const {return externe_bestellung;}
 ID GeplantVon() const {return geplant_von;} 
 

 cH_Prozess get_Prozess() const;
private:
  // Gegenteil von 'Lieferant' ist 'Kunde'
 bool Lieferant() const { return instid!=ppsInstanzID::Kundenauftraege; }
public:
 bool KundenInstanz() const {return instid==ppsInstanzID::Kundenauftraege; }
 int ProduktionsDauer() const {return produktionsdauer_tage;}


 operator ID () const {return instid;}
 void set(ID i) {instid=i; get_name();}
 

 bool operator==(const ppsInstanz &b) const
 {  return instid==b.instid; }
 bool operator<(const ppsInstanz &b) const
 {  return Sortierung()<b.Sortierung(); }
 bool operator!=(const ppsInstanz &b) const
 {  return instid!=b.instid; }
 bool operator==(ID b) const
 {  return instid==b; }
 bool operator!=(ID b) const
 {  return instid!=b; }


 static cH_ppsInstanz getBestellInstanz(const ArtikelBase &artikel);
 static cH_ppsInstanz getProduktionsInstanz(const ArtikelBase &artikel);

 //////////////////////////////////////////////////////////////////////////
 // F�r die Produktion
 public:
      void Produziert(ManuProC::st_produziert &P,ManuProC::Auftrag::Action reason=ManuProC::Auftrag::r_None) const throw(SQLerror);
      void Lager_abschreiben(ManuProC::st_produziert &P) const ;
      // Geplant wird von pps wenn im Einkauf ware bestellt wird ohne
      // einen spezielen '0er' auszuw�hlen.
      void Planen(ManuProC::st_produziert &P) const throw(SQLerror); //NICHT rekursiv
 private:
      void rekursion(ManuProC::st_produziert &P) const ;

 /////////////////////////////////////////////////////////////////////////
 // Reperatur
 private:
      struct st_table{std::string table; std::string column;
              st_table(const std::string &t,const std::string &c) 
               : table(t),column(c) {}};

      std::vector<LagerInhalt> getLagerInhalt() const;
      void vormerkungen_subrahieren(int uid,const  std::vector<LagerInhalt> &LI,const bool analyse_only) const;
      void DispoAuftraege_anlegen(const int uid,const ArtikelBase &artikel,const fixedpoint<ManuProC::Precision::AuftragsMenge> &menge) const;
      // 0er und 2er m�ssen immer offen sein
      void force_open_0er_und_2er(const bool analyse_only) const throw(SQLerror);
      // Alle Auftr�ge au�er Kundenauftr�gen und externen Bestellungen 
      // m�ssen die eigene KundenID haben
      void force_eigene_KundenId(const bool analyse_only) const throw(SQLerror);
      void force_2er_0er_geliefert_ist_null(const bool analyse_only) const throw(SQLerror);
      void force_execute(const std::vector<st_table> &Vtable,
          const std::vector<ManuProcEntity<>::ID> &Vauftragid,
          const int Wert,const std::string &was,const bool analyse_only) const throw(SQLerror);

      enum e_zumode{Dungeplant,Egeplant,Fdispo};
      bool Reparatur_Zuordnungen(const int uid,const bool analyse_only,
         const  ManuProcEntity<>::ID auftragid,const bool kinder,const e_zumode zumode) const throw(SQLerror);

      typedef fixedpoint<ManuProC::Precision::AuftragsMenge> ABmt;
      bool check_D_ungeplant(const bool analyse_only,const AufEintrag &AE,const ABmt &M0sum,const ABmt &Msum) const;
      bool check_E_geplant(const bool analyse_only,const AufEintrag &AE,const ABmt &Msum) const;
      bool check_F_dispo(const bool analyse_only,const AufEintrag &AE,const ABmt &Msum) const;

      void Reparatur_Kundenauftrag_AE(const int uid,const AufEintrag &KundeAE,AufEintrag &KindAE,const ABmt &menge) const;
      void Reparatur_Kundenauftrag_AEB(const int uid,const AufEintrag &KundeAE,const AufEintragBase &KindAE,const ABmt &menge) const;
      void MengenReparatur(const int uid,const AufEintrag &AE, AufEintrag &AEB,const ABmt& zumenge) const;
//      void ReparaturDatum(const int uid,const AufEintrag &AE,const AufEintrag &AEK,const ABmt &menge) const ;


      void analyse(const std::string &s,const AufEintrag &AE,const std::string &x,const std::string &y) const;
      // Wrapper:
      void analyse(const std::string &s,const AufEintrag &AE) const{analyse(s,AE,"","");}
      void analyse(const std::string &s,const AufEintrag &AE,const AufEintragBase &x,const ABmt &y) const;
      void analyse(const std::string &s,const AufEintrag &AE,const ABmt &x,const ABmt &y) const;
      void analyse(const std::string &s,const AufEintrag &AE,const ArtikelBase &x,const ArtikelBase &y) const;
      void analyse(const std::string &s,const AufEintrag &AE,const cH_ppsInstanz &x,const cH_ppsInstanz &y) const;
 public:
      // Einlesen des Lagerinhalts und Anpassen der 2er unter Ber�cksichtigung der 1er
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
};


class cH_ppsInstanz : public Handle<const ppsInstanz>
{
     typedef CacheStatic<ppsInstanz::ID,cH_ppsInstanz> cache_t;
     static cache_t cache;
     friend class std::map<ppsInstanz::ID,cH_ppsInstanz>;
     cH_ppsInstanz(){};
  public:
     cH_ppsInstanz(ppsInstanz::ID iid);
     cH_ppsInstanz(const ppsInstanz *s) : Handle<const ppsInstanz>(s) {};

     static std::vector<cH_ppsInstanz> get_all_instanz() throw(SQLerror);

     bool operator==(ppsInstanz::ID b) const
     {  return *(*this)==b; }
     bool operator!=(ppsInstanz::ID b) const
     {  return *(*this)!=b; }
     bool operator==(const cH_ppsInstanz &b) const
     {  return *(*this)==*b; }
};

std::ostream &operator<<(std::ostream &o,const cH_ppsInstanz &i);

#endif
