// $Id: ppsInstanzReparatur.h,v 1.20 2004/05/04 10:44:20 jacek Exp $
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
#include <stdexcept>
class LagerInhalt;
class SelectedFullAufList;

class ppsInstanzReparatur : public cH_ppsInstanz
{public:
   ppsInstanzReparatur(ppsInstanz::ID iid=ppsInstanzID::None)
      : cH_ppsInstanz(iid) {}
   const cH_ppsInstanz &Instanz() const { return *this; }
   
   // indicates that already fetched information changed
   struct reload : public std::exception
   {	virtual const char* what() const throw() 
        { return "ppsInstanzReparatur::reload"; }
        reload() {}
   };
                

 /////////////////////////////////////////////////////////////////////////
 // Reparatur
 private:
      typedef fixedpoint<ManuProC::Precision::AuftragsMenge> ABmt;

      std::vector<LagerInhalt> getLagerInhalt() const;
      bool vormerkungen_subtrahieren(const  std::vector<LagerInhalt> &LI,const bool analyse_only) const;
      void DispoAuftraege_anlegen(const ArtikelBase &artikel,const fixedpoint<ManuProC::Precision::AuftragsMenge> &menge) const;

      void analyse(const std::string &s,const AufEintragBase &AE,const std::string &x=std::string(),const std::string &y=std::string()) const;
      // Wrapper:
      void analyse(const std::string &s,const AufEintragBase &AE,const AufEintragBase &x,const ABmt &y=0) const;
      void analyse(const std::string &s,const AufEintragBase &AE,const ABmt &x,const ABmt &y=0) const;
      void analyse(const std::string &s,const AufEintragBase &AE,const ArtikelBase &x,const ArtikelBase &y=ArtikelBase()) const;
      void analyse(const std::string &s,const AufEintragBase &AE,const cH_ppsInstanz &x,const cH_ppsInstanz &y) const;
      
      static void Zuordnung_erniedrigen(AufEintrag &ae,
	AufEintragZu::list_t &eltern,AuftragBase::mengen_t &m,
	AuftragBase::ID typ);
      static void KinderErniedrigen(AufEintrag &ae,
	AufEintragZu::list_t &kinder,AuftragBase::mengen_t m);

 public:
      // Einlesen des Lagerinhalts und Anpassen der 2er unter Berücksichtigung der 1er
      bool ReparaturLager(const bool analyse_only,
      		const ArtikelBase::ID aid=ArtikelBase::none_id) const throw(SQLerror);
      // Entweder existieren 0er oder es existieren 2er
      // void Reparatur_0er_und_2er(const bool analyse_only) const throw(SQLerror);
      bool Reparatur_0er_und_2er(SelectedFullAufList &L,const bool analyse_only) const throw(SQLerror);

      // neues Interface!
      bool Eltern(AufEintrag &ae, AufEintragZu::list_t &eltern, bool analyse_only, bool raise_prodselbst) const;
      bool Lokal(AufEintrag &ae, bool analyse_only) const;
      bool Kinder(AufEintrag &ae, AufEintragZu::map_t &kinder, bool analyse_only) const;

      bool Lagermenge_setzen(bool analyse_only, const ArtikelBase &art,const AuftragBase::mengen_t &gesmenge,bool retry=false) const;
      
      static bool really_delete,silence_warnings;
};

#endif
