// $Id: PPixmap.hh,v 1.4 2002/07/05 12:36:56 christof Exp $
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

#ifndef PPIXMAP_HH
#define PPIXMAP_HH

#include <gtk--/pixmap.h>
#include <Picture2.h>
#include <vector>
#include <map>

class PPixmap 
{
   static std::vector<Picture2> vpix;
   static std::map<char,Picture2> mappix;

 public: 
   PPixmap();
   
   Gtk::Pixmap *getPixProz(double wieviel,double von); 
   Gtk::Pixmap *getPixProz(double prozent); 
   Gtk::Pixmap *getPixMap(char c);
};



#endif