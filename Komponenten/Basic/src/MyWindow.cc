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

#include "MyWindow.hh"
#include <Misc/Global_Settings.h>
#include <Misc/itos.h>
#include <unistd.h>

#ifdef __MINGW32__
#define getuid() 0
#endif

void MyWindow::saveWindowSize(Gtk::Window &window,const std::string &programm)
{
  gint width,height,x,y;
  Glib::RefPtr<Gdk::Window> fenster=window.get_window();
  fenster->get_size(width,height);
  fenster->get_position(x,y);
  Global_Settings::create(int(getuid()),programm,"Size",itos(width)+":"+itos(height));
  Global_Settings::create(int(getuid()),programm,"Position",itos(x)+":"+itos(y));
}

void MyWindow::setPositionSize(Gtk::Window &window,const std::string &programm)
{
  Global_Settings position=Global_Settings(int(getuid()),programm,"Position");
  int x=atoi(position.get_Wert(":",1).c_str());
  int y=atoi(position.get_Wert(":",2).c_str());
  Global_Settings size=Global_Settings(int(getuid()),programm,"Size");
  int width=atoi(size.get_Wert(":",1).c_str());
  int height=atoi(size.get_Wert(":",2).c_str());
  if(x==0) x+=5;
  if(y==0) y+=15;
  window.get_window()->move(x,y);
  window.set_default_size(width,height);
}

