// $Id: simplest.hh,v 1.3 2002/12/03 08:44:30 christof Exp $
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

// generated 2001/6/20 15:16:37 CEST by christof@puck.(none)
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to simplest.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _SIMPLEST_HH
#  include "simplest_glade.hh"
#  define _SIMPLEST_HH
class simplest : public simplest_glade
{   
        void on_Beenden_activate();
        void on_leaf_selected(cH_RowDataBase d);
   public:
	simplest();
};
#endif
