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

#ifndef FLASHINGPIXMAP
#define FLASHINGPIXMAP

#include <gtkmm/image.h>
#include <gtkmm/main.h> 


class FlashingPixmap : public Gtk::Image
{
      gchar * const *defaultPix;
      gchar * const *flashPix;
      SigC::Connection des;
      bool stop;

      void switchPix();
      bool timeout();

      void setDefault(gchar * const *p) {defaultPix=p;}
      void setFlash(gchar * const *p) {flashPix=p;}
   public:
      FlashingPixmap():stop(true){}
      FlashingPixmap(gchar * const *d,
                     gchar * const *f,
                     unsigned int msec);
      void set(gchar * const *d,gchar * const *f,unsigned int msec)
         {setDefault(d);setFlash(f);setTime(msec);}
      void setTime(unsigned int sec) ;
};

#endif


