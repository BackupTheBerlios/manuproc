// $Id: ppsInstanz.h,v 1.7 2002/10/24 14:06:28 thoma Exp $
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
#include <Artikel/Prozess.h>
#include <vector>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>

class AufEintrag;
class ArtikelBase;
class cH_ppsInstanz;

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

 void get_name();
 void check() const; 
public:
 static const ID default_id=ManuProC::DefaultValues::Instanz;
 
 ppsInstanz(ID iid) : instid(iid),
                             lager_fuer(ppsInstanzID::None),
                             sortierung(0),
                             lieferschein(false),produziert_selbst(false),
                             typ('0')
                             {get_name(); check(); }
 ppsInstanz() : instid(ppsInstanzID::None),
                             sortierung(0),lieferschein(false),produziert_selbst(false) {}

 ID Id() const { return instid; }
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
 bool ProduktionsInstanz() const ;
 bool PlanungsInstanz() const ;
public:
// ID BestellungFuer() const { return bestellung_fuer; }
 ID EinlagernIn() const { return einlagern_in;}
 bool AutomatischEinlagern() const {return automatisch_einlagern;}
 

 cH_Prozess get_Prozess() const;
  // Gegenteil von 'Lieferant' ist 'Kunde'
 bool Lieferant() const { return instid!=ppsInstanzID::Kundenauftraege; }
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
 // Für die Produktion
 public:
      void Produziert(ManuProC::st_produziert &P) const ;
      void Lager_abschreiben(ManuProC::st_produziert &P) const ;
 private:
      void rekursion(ManuProC::st_produziert &P) const ;

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

     static std::vector<cH_ppsInstanz> get_all_instanz();

     bool operator==(ppsInstanz::ID b) const
     {  return *(*this)==b; }
     bool operator!=(ppsInstanz::ID b) const
     {  return *(*this)!=b; }
     bool operator==(const cH_ppsInstanz &b) const
     {  return *(*this)==*b; }
};

#endif
