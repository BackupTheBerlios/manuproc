/*  Midgard Character Generator
 *  Copyright (C) 2001-2003 Malte Thoma, Christof Petig
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

#ifndef FLASHINGPIXMAP
#define FLASHINGPIXMAP

#include <gtkmm/image.h>
#include <gtkmm/main.h> 
#include <Misc/compiler_ports.h>

class FlashingPixmap : public Gtk::Image
{
      Glib::RefPtr<Gdk::Pixbuf> defaultPix;
      Glib::RefPtr<Gdk::Pixbuf> flashPix;
      SigC::Connection des;
      bool stop,def;

      void switchPix();
      bool timeout();

      void setDefault(Glib::RefPtr<Gdk::Pixbuf> p) {defaultPix=p;}
      void setFlash(Glib::RefPtr<Gdk::Pixbuf> p) {flashPix=p;}
   public:
      FlashingPixmap() : stop(true),def(true) {}
      /*__deprecated*/ FlashingPixmap(const gchar * const *d,const gchar * const *f,unsigned int msec);
      FlashingPixmap(Glib::RefPtr<Gdk::Pixbuf> d,Glib::RefPtr<Gdk::Pixbuf> f,unsigned int msec);

      __deprecated void set(const gchar * const *d,const gchar * const *f,unsigned int msec);
      void set(Glib::RefPtr<Gdk::Pixbuf> d,Glib::RefPtr<Gdk::Pixbuf> f,unsigned int msec);
      void setTime(unsigned int sec) ;
};

#endif
