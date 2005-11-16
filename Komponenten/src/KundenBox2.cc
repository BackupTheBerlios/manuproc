// $Id: KundenBox2.cc,v 1.11 2005/11/16 09:12:29 christof Exp $
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

#include "config.h"
#include <Misc/i18n.h>
#include "KundenBox2.hh"

KundenBox2::KundenBox2()
{
  _tabelle_="kunden";
  _string1_="coalesce(sortname,firma||coalesce(' '||name2,''))";
  _string2_="coalesce(ort,'')";
  _int_="kundennr";
  _none_id_=Kunde::none_id;  
  setLabel(_("Id"),_("Name"),_("Ort"));
  show_string2(true);
}

void KundenBox2::set_value(int i)
{  cH_Kunde PL(i);
   IntStringBox::set_value(PL->Id(),PL->getFullName(),PL->ort());
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

#if defined(ENABLE_NLS)
namespace { struct gettext_init { gettext_init(); }; }
static gettext_init init;
gettext_init::gettext_init()
{
   bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
   bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
}
#endif
