// $Id: KundenBox2.cc,v 1.3 2003/09/30 15:59:19 jacek Exp $
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

#include "KundenBox2.hh"

KundenBox2::KundenBox2()
{
  _tabelle_="kunden";
  _string1_="coalesce(sortname,firma)";
  _string2_="coalesce(ort,'')";
  _int_="kundennr";
  _none_id_=Kunde::none_id;  
  setLabel("Id","Firma","Ort");
  show_string2(true);
}

void KundenBox2::set_value(int i)
{  cH_Kunde PL(i);
   IntStringBox::set_value(PL->Id(),PL->firma(),PL->ort());
}
/*
cH_Kunde KundenBox2::get_value() const 
{
  return cH_Kunde(IntStringBox::get_value());
}
*/
Kunde::ID KundenBox2::get_value() const
{
  return IntStringBox::get_value();
}

void KundenBox2::EinschraenkenKdGr(Kundengruppe::ID kgrid)
{
 if(kgrid!=KundengruppeID::None)
   {Join(" join ku_gruppen_map kg on (kg.kundennr="+_tabelle_+
        ".kundennr and kg.grpnr="+itos(kgrid)+") ");
   }
 else
   {Join("");
   }
}