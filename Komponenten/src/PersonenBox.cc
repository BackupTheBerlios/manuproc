// $Id: PersonenBox.cc,v 1.4 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
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

#include "PersonenBox.hh"

PersonenBox::PersonenBox()
{
  _tabelle_="ku_person";
  _string1_="name";
  _string2_="vname";
  _int_="persnr";
  _none_id_=Person::none_id;
  setLabel("Id","Nachname","Vorname");
  show_string2(true);
}

void PersonenBox::set_value(int i)
{  cH_Person PL(i);
   IntStringBox::set_value(PL->Id(),PL->Name(),PL->Vorname());
}

cH_Person PersonenBox::get_value()
{
  return cH_Person(IntStringBox::get_value());
}

