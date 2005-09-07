// $Id: VerfuegbarPixmap.cc,v 1.2 2005/09/07 14:18:06 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#include "VerfuegbarPixmap.h"

Glib::RefPtr<Gdk::Pixbuf> ManuProC::VerfuegbarPixmap(const Verfuegbarkeit::Mengen &m)
{  char lines[10][8];
   const char *xpm_data[]= 
   {  "7 10 5 1",
//      " \tc None",
      "b\tc #000000",
      "g\tc #00FF00",
      "y\tc #FFFF00",
      "r\tc #FF0000",
      "e\tc #0000FF",
      lines[0],
      lines[1],
      lines[2],
      lines[3],
      lines[4],
      lines[5],
      lines[6],
      lines[7],
      lines[8],
      lines[9]
   };
   const unsigned cells=70;
   const float cellsf=cells;
   unsigned black=unsigned(cellsf*m.geliefert/m.summe().as_float()+.5),
   	green=unsigned(cellsf*m.vorraetig/m.summe().as_float()+.5),
   	yellow=unsigned(cellsf*m.geplant/m.summe().as_float()+.5),
   	red=unsigned(cellsf*m.ungeplant/m.summe().as_float()+.5);
   unsigned i=0;
   for (;i<black && i<cells;++i) lines[i/7][i%7]='b';
   for (;i<black+green && i<cells;++i) lines[i/7][i%7]='g';
   for (;i<black+green+yellow && i<cells;++i) lines[i/7][i%7]='y';
   for (;i<black+green+yellow+red && i<cells;++i) lines[i/7][i%7]='r';
   for (;i<cells;++i) lines[i/7][i%7]='e';
   for (i=0;i<10;++i) lines[i][7]=0;
   return Gdk::Pixbuf::create_from_xpm_data(xpm_data);
}
