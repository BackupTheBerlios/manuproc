// $Id: ppsInstanz.h,v 1.32 2005/10/11 10:19:30 christof Exp $
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
#include <Misc/Handles.h>
#include <Misc/CacheStatic.h>
#include <Misc/SQLerror.h>
#include <Artikel/Prozess.h>
#include <Auftrag/auftrag_enums.h>
#include <vector>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>
#include <Misc/fixedpoint.h>
#include <BaseObjects/ManuProcEntity.h>

namespace ppsInstanzID=ManuProC::DynamicEnums::Instanzen;

class cH_ppsInstanz;
class ArtikelBase;
class ArtikelStamm;
class Query_Row;

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
 bool externe_bestellung;
public: // see Stroustrup about OO abuse
 std::string overview; // short hint on which intance is used in pps
 ID alt_group_nr; // group of equivalent instances

private:
 friend FetchIStream &operator>>(FetchIStream &,ppsInstanz&);
 void get_name() throw(SQLerror);
 void check() const; 
public:
 static const ID default_id=ManuProC::DefaultValues::Instanz;
 
 ppsInstanz(ID iid=ppsInstanzID::None);

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
 ID EinlagernIn() const { return einlagern_in;}
 bool AutomatischEinlagern() const {return automatisch_einlagern;}
 bool ExterneBestellung() const {return externe_bestellung;}
 // nächste Instanz für einen Artikel (dort wird von hier bestellt)
 ID NaechsteInstanz(const ArtikelStamm &art) const;

 cH_Prozess get_Prozess() const;
//private:
// Gegenteil von 'Lieferant' ist 'Kunde'
// bool Lieferant() const { return instid!=ppsInstanzID::Kundenauftraege; }
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
 static cH_ppsInstanz getBestellInstanz(const ArtikelStamm &artikel);
 static cH_ppsInstanz getProduktionsInstanz(const ArtikelStamm &artikel);
};


class cH_ppsInstanz : public Handle<const ppsInstanz>
{
     typedef CacheStatic<ppsInstanz::ID,cH_ppsInstanz> cache_t;
     static cache_t cache;
  public:
     cH_ppsInstanz(){};
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
Query_Row &operator>>(Query_Row &is, ppsInstanz::ID &v);
Query_Row &operator>>(Query_Row &is, cH_ppsInstanz &i);

#endif
