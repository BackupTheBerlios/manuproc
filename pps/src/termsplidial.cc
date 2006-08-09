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

#include "termsplidial.hh"
#include "auftrag_bearbeiten.hh"
#include "Configuration.h"

extern auftrag_bearbeiten *auftragbearbeiten;

void termsplidial::on_okbutton_clicked()
{
 //if(auftragbearbeiten->splitEntry()) delete this;
 auftragbearbeiten->splitEntry();
}

void termsplidial::on_cancbutton_clicked()
{
// delete this;
}

termsplidial::termsplidial(auftrag_bearbeiten *ab, const ManuProC::Datum &val)
	: auftragbearbeiten(ab)
{  //termin_neu->select_month(val.Monat(),val.Jahr());
   //termin_neu->select_day(val.Tag());
#ifdef LIEFERDATUM_WOCHE_STANDARD
   termin_neu->preferWeek();
#endif
   termin_neu->set_value(val);
}

termsplidial::~termsplidial()
{
}
