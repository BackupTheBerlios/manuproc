// $Id: ppsInstanz.h,v 1.1 2002/09/26 14:54:03 thoma Exp $
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

namespace ppsInstanzID=ManuProC::DynamicEnums::Instanzen;

class ppsInstanz : public HandleContent
{
public:
	typedef ppsInstanzID::enum_t ID;
 
private: 
 ID instid,lager_fuer,bestellung_fuer;
 std::string name;
 int sortierung;
 bool lieferschein;
 std::string lagername;
 bool produziert_selbst; 
 void get_name();
 int produktionsdauer_tage;
 
public:
 static const ID default_id=ppsInstanzID::Kundenauftraege;
// static const ID default_id=DEFAULT_INSTANZ;
 
 ppsInstanz(ID iid) : instid(iid),
                             lager_fuer(ppsInstanzID::None),bestellung_fuer(ppsInstanzID::None),
                             sortierung(0),
                             lieferschein(false),produziert_selbst(false)
                             {get_name(); }
 ppsInstanz() : instid(ppsInstanzID::None),bestellung_fuer(ppsInstanzID::None),
                             sortierung(0),lieferschein(false),produziert_selbst(false) {}

 ID Id() const { return instid; }
 int Sortierung() const {return sortierung; }
 bool Lieferschein() const {return lieferschein;}
 bool ProduziertSelbst() const {return produziert_selbst;}
 std::string get_Name() const {return name;}
 std::string Name() const {return name;}
 std::string shortName() const;
 ID LagerFuer() const { return lager_fuer; }
 bool LagerInstanz() const ;
private: // wenn das keiner mehr braucht, dann auch aus der Datenbank löschen MAT
// std::string LagerName() const {return lagername;}
public:
 ID BestellungFuer() const { return bestellung_fuer; }
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
