/*  Midgard Character Generator
 *  Copyright (C) 2001-2002 Malte Thoma
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

#include "FlashingPixmap.hh"


FlashingPixmap::FlashingPixmap(gchar * const *d,
                     gchar * const *f,
                     unsigned int msec)
:Gtk::Pixmap(d), defaultPix(d),flashPix(f),stop(true)
{
  show();
  setTime(msec);
}

void FlashingPixmap::setTime(unsigned int msec)
{
  des.signal_di().connect();
  if(msec==0) 
   {
     stop=true;
     Gtk::Pixmap::set(defaultPix);
   }
  else 
   {
     stop=false;
     Gtk::Pixmap::set(flashPix);
     des = Gtk::Main::signal_timeout().connect(slot(this,&FlashingPixmap::timeout),msec);
   }
}



gint FlashingPixmap::timeout() 
{ 
  switchPix();
  return !stop;
}

void FlashingPixmap::switchPix()
{
  static bool def=true;
  if(def) {Gtk::Pixmap::set(defaultPix); def=false; }
  else    {Gtk::Pixmap::set(flashPix);   def=true;  }
}
