// $Id: Aufmachung.cc,v 1.2 2002/05/06 13:41:22 christof Exp $
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

#include<Aufmachung.h>

Aufmachung &Aufmachung::operator=(const Aufmachung &a)
{
 aufmachid=a.aufmachid;
 bezeichnung=a.bezeichnung;
 abkuerzung=a.abkuerzung;
 meter_pro_stk=a.meter_pro_stk;
 yard=a.yard;
 
 return *this;
}

Aufmachung::Aufmachung(const Aufmachung &a)
: aufmachid(a.aufmachid), bezeichnung(a.bezeichnung),
abkuerzung(a.abkuerzung), meter_pro_stk(a.meter_pro_stk),
yard(a.yard)
{}


