// $Id: ppsInstanz.h,v 1.40 2002/03/07 09:29:32 cvs_malte Exp $
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

#ifndef PPSINSTANZCLASS
#define PPSINSTANZCLASS
#include <ManuProCConfig.h>
#include <string>
#include <Aux/Handles.h>
#include <Aux/CacheStatic.h>
#include <Artikel/Prozess.h>
#include <vector>

class ppsInstanz : public HandleContent
{
public:
 enum ppsInstId {None=0,
 	Kundenauftraege=1,
#if defined(PETIG_EXTENSIONS) || defined(MABELLA_EXTENSIONS)
        Faerberei=2,
        Druckerei=3, 
        Weberei=4,
        Schaererei=5,
        Einkauf=6,
        Spritzgiesserei=7,
        Bandlager=8,
        Rohlager=9,
        Rollerei=10, 
#endif
//	MaxInstanz,
// veraltet
 	INST_KNDAUF=Kundenauftraege, INST_NONE=None, 
 	INST_FAERB=Faerberei,INST_DRUCK=Druckerei,INST_WEB=Weberei,
        INST_SCHAER=Schaererei,INST_SPRITZ=Spritzgiesserei,
        INST_GARNEINKAUF=Einkauf,INST_BANDLAGER=Bandlager,
        INST_ROHLAGER=Rohlager,
        };
 
private: 
 ppsInstId instid,lager_fuer,bestellung_fuer;
 std::string name;
 int sortierung;
 bool lieferschein;
 std::string lagername;
 
 void get_name();
 
public:
 typedef ppsInstId ID;
 static const ID default_id=Kundenauftraege;

 
 ppsInstanz(ppsInstId iid) : instid(iid),
                             lager_fuer(None),bestellung_fuer(None),
                             sortierung(0),
                             lieferschein(false)
                             {get_name(); }
 ppsInstanz() : instid(None),bestellung_fuer(None),
                             sortierung(0),lieferschein(false) {}

 ppsInstId Id() const { return instid; }
 int Sortierung() const {return sortierung; }
 bool Lieferschein() const {return lieferschein;}
 std::string get_Name() const {return name;}
 std::string Name() const {return name;}
 std::string shortName() const;
 ppsInstId LagerFuer() const { return lager_fuer; }
 bool LagerInstanz() const ;
 std::string LagerName() const {return lagername;}
 ppsInstId BestellungFuer() const { return bestellung_fuer; }
 cH_Prozess get_Prozess() const;
 bool Lieferant() const {return lieferschein;} // Gegenteil von 'Lieferant' ist 'Kunde'


 operator ppsInstId () const {return instid;}
 void set(ppsInstId i) {instid=i; get_name();}
 

 bool operator==(const ppsInstanz &b) const
 {  return instid==b.instid; }
 bool operator<(const ppsInstanz &b) const
 {  return Sortierung()<b.Sortierung(); }
 bool operator!=(const ppsInstanz &b) const
 {  return instid!=b.instid; }
 bool operator==(ppsInstId b) const
 {  return instid==b; }
 bool operator!=(ppsInstId b) const
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

     bool operator==(ppsInstanz::ppsInstId b) const
     {  return *(*this)==b; }
     bool operator!=(ppsInstanz::ppsInstId b) const
     {  return *(*this)!=b; }
     bool operator==(const cH_ppsInstanz &b) const
     {  return *(*this)==*b; }
};


#endif
