/* $Id: JumboLager.h,v 1.8 2002/11/27 13:26:51 christof Exp $ */
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef _JUMBOLAGER_HH_
#define _JUMBOLAGER_HH_

#include "Lager.h"
#include "JumboRolle.h"


class JumboLager : public LagerBase
{
  public:
      enum Jumbo_LogTyp {None, Umraeumen='1',SchonDraussen='2',
      	    KarteNichtGescannt='3',
      	    Inventur='i', Inventur_Ohne_Bestand='j',
            Einlagern='A', Auslagern='B', Zurueck='C', InventurEnde='E',
            InventurPlus='I', InventurMinus='J',
            Einlagern_L='L', Rest_L='Q', Rest='R'  };

  private:
      static const float reste_faktor = 0.3;

      void Jumbo_Log(const JumboRolle& jumbo,Jumbo_LogTyp typ,const int uid,const std::string& user,const Zeitpunkt_new *zeit);
      virtual std::vector<class LagerInhalt> LagerInhalt_(const ArtikelBase& artikel=ArtikelBase()) const;

  public:
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
      JumboLager() : LagerBase(ppsInstanzID::Bandlager) {}
#else
	JumboLager() : LagerBase(ppsInstanzID::None) {}      
#endif

      void Jumbo_Einlagern(const LagerPlatz position,JumboRolle& jumbo,Jumbo_LogTyp typ,const int uid,const std::string& user,const Zeitpunkt_new *zeit=0);
      void Jumbo_Entnahme(JumboRolle& jumbo,Jumbo_LogTyp typ,const int uid,const std::string& user,const Zeitpunkt_new *zeit=0);
};

#endif
