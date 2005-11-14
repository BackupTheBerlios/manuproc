// $Id: KundenBox2.cc,v 1.8 2005/11/14 07:40:23 christof Exp $
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
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

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
