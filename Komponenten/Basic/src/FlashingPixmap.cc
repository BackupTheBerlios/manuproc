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
#include <glibmm/main.h> 

FlashingPixmap::FlashingPixmap(gchar * const *d,
                     gchar * const *f,
                     unsigned int msec)
:Gtk::Image(Gdk::Pixbuf::create_from_xpm_data(d)), defaultPix(d),flashPix(f),stop(true)
{
  show();
  setTime(msec);
}

void FlashingPixmap::setTime(unsigned int msec)
{
  des.disconnect();
  if(msec==0) 
   {
     stop=true;
     Gtk::Image::set(Gdk::Pixbuf::create_from_xpm_data(defaultPix));
   }
  else 
   {
     stop=false;
     Gtk::Image::set(Gdk::Pixbuf::create_from_xpm_data(flashPix));
     des = Glib::signal_timeout().connect(slot(*this,&FlashingPixmap::timeout),msec);
   }
}



bool FlashingPixmap::timeout() 
{ 
  switchPix();
  return !stop;
}

void FlashingPixmap::switchPix()
{
  static bool def=true;
  if(def) {Gtk::Image::set(Gdk::Pixbuf::create_from_xpm_data(defaultPix)); def=false; }
  else    {Gtk::Image::set(Gdk::Pixbuf::create_from_xpm_data(flashPix));   def=true;  }
}
