/* $Id: AuftragMitEintrag.h,v 1.4 2002/06/20 06:29:53 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#ifndef CXX_AUFTRAG_AUFMITEINTRAG_H
#define CXX_AUFTRAG_AUFMITEINTRAG_H

#include <Auftrag/Auftrag.h>
#include <Auftrag/AufEintrag.h>

class AuftragMitEintrag : public AufEintragBase
{  Auftrag auftrag;
   AufEintrag eintrag;

public:
  AuftragMitEintrag(ppsInstanz::ID _instanz,int _auftragid, int _zeilennr, 
        mengen_t _bestellt,
	int _artikel, const Petig::Datum _lieferdatum,
	mengen_t _geliefert,
	int _dispoentrynr, int _disponr, 
	AufStatVal _aufstatus,
	int _kdnr, const std::string _youraufnr,
	const Petig::Datum& _prozdate,
	int _prozess,int _letztePlanInstanz, int _maxPlanInstanz,
	const Preis &_preis, rabatt_t _rabatt,
	AufStatVal _entrystat, const Petig::Datum _lasteditdate) throw();

   const Auftrag &getAuftrag() const { return auftrag; }
   const AufEintrag &getAufEintrag() const { return eintrag; }
}

std::ostream &operator<<(std::ostream &o,const AufEintrag &aeb);

#endif
