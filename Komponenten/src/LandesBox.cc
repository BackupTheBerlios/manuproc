// $Id: LandesBox.cc,v 1.2 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "LandesBox.hh"
#include <Kunde/LandesListe.h>

LandesBox::LandesBox()
{
  hide_int(true);
  string2_info_only(true);
  show_string2(true);
  _tabelle_="ku_land";
  _string1_="name";
  _string2_="lkz";
  _int_="id";
  _none_id_=LandesListe::none_id;
  setLabel("LKZ","Land","LKZ");
}

void LandesBox::set_value(int i)
{  cH_LandesListe LL(i);
   IntStringBox::set_value(LL->Id(),LL->Name(),LL->LKZ());
}
