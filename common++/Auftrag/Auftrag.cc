// $Id: Auftrag.cc,v 1.9 2003/07/03 09:15:16 christof Exp $
/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include"AuftragFull.h"
#include<Misc/Ausgabe_neu.h>


std::string Auftrag::getAuftragidToStr() const
{ return Formatiere(auftragid,0,6,"","",'0'); }



void Auftrag::setStatusAuftrag_(AufStatVal st) throw(SQLerror)
{
 setStatusAuftragBase(st);
 status=st;
}

void Auftrag::setRabatt(const rabatt_t _auftragsrabatt) throw(SQLerror)
{
  AuftragBase::setRabatt(_auftragsrabatt);
  auftragsrabatt=_auftragsrabatt;
}

Auftrag::ID Auftrag::Copy(AuftragCopy &ac) throw(SQLerror)
{
// Auftrag a(Anlegen(Instanz()), ac.kundennr, ac.yanr);

}

Auftrag::youraufnr_cache_t Auftrag::youraufnr_cache;

std::string Auftrag::getYourAufNr(const AuftragBase &ab)
{  youraufnr_cache_t::const_iterator i=youraufnr_cache.find(ab);
   if (i!=youraufnr_cache.end()) return i->second;
   try
   {  std::string yan;
      Query("select youraufnr from auftrag where (instanz,auftragid)=(?,?)")
   	<< ab
   	>> FetchIStream::MapNull(yan);
      youraufnr_cache[ab]=yan;
      return yan;
   }
   catch (SQLerror &e)
   {  // erzeugt implizit einen Leerstring
      return youraufnr_cache[ab];
   }
}
