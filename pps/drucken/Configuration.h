/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

// für globale Einstellungen, vielleicht später als Argument mitgeben
// sollte irgendwann alle Parameter enthalten (ean_code ...)
struct Configuration
{	bool combine;
	bool preview_only;
	std::string copies;
	bool toTeX;
	bool batch;
	std::string printer;	
	std::string texfile;
	std::string order_clausel;
	bool zaehle_spalten;
	
	Configuration() : combine(), preview_only(), copies("1,1,1"),
			printer("ps"), toTeX(false),
			zaehle_spalten() {}
};
extern struct Configuration Configuration;
