// $Id: PreisBox.cc,v 1.6 2004/11/16 14:09:02 christof Exp $
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

#include "PreisBox.hh"
#include <Aux/itos.h>

PreisBox::PreisBox(ManuProcEntity<>::ID __none_id) 
	: IntStringBox(__none_id)
{
  _tabelle_="ku_preisliste";
  _string1_="notiz";
  _int_="prlsnr";
  setLabel("Nr.","Bezeichnung");

#ifdef MABELLA_EXTENSIONS
  sc_int->set_always_fill(true);
#endif
  
}



void PreisBox::set_value(int i)
{  cH_PreisListe PL(i);
   if(i!=PreisListe::none_id)
     IntStringBox::set_value(PL->Id(), PL->Name());
   else
     {sc_int->reset();
      sc1_string->reset();
     }
}

