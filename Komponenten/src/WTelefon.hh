// $Id: WTelefon.hh,v 1.6 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef _WID_TELEFON_HH
#  define _WID_TELEFON_HH

#include "WTelefon_glade.hh"
#include <Kunde/Telefon.h>
#include <Kunde/Kunde.h>
#include <list> 

class WTelefon : public WTelefon_glade
{	friend class WTelefon_glade;
 private:
  ManuProcEntity::ID kundennr;
  ManuProcEntity::ID persnr;

//   std::list<cH_Telefon> TelList;

   void setTitels();
   void showTel();

   void anderer_typ();
   void land_activate();
   void vorwahl_activate();
   void nummer_activate();
   void durchwahl_activate();
   void text_activate();

   bool getSelectedTel(cH_Telefon &T) const;
   void on_buttonNumLoeschen_clicked();
   void felder_anpassen(TelArt art);

   static gint try_grab_focus(GtkWidget *w,gpointer gp);
   
   std::list<cH_Telefon> TelList;

 public:
   WTelefon();
   
   void set_value(const cH_Telefon &v);
   cH_Telefon get_value() const;
   
   void showTel(std::list<cH_Telefon> VT);
   void setKdPer(const ManuProcEntity::ID kid, const ManuProcEntity::ID pid)
	{kundennr=kid, persnr=pid;}

   void clear(bool withtree=true) const;

   SigC::Signal1<void,cH_Telefon> add;
   SigC::Signal1<void,cH_Telefon> remove;
};


#endif
