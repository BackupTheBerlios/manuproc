// $Id: with_class.hh,v 1.4 2002/07/05 12:36:56 christof Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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
// $Id: with_class.hh,v 1.4 2002/07/05 12:36:56 christof Exp $

#ifndef _WITH_CLASS_HH
#  include "with_class_glade.hh"
#  define _WITH_CLASS_HH
class with_class : public with_class_glade
{	friend class with_class_glade;
        void on_Beenden_activate();
        void on_button_data_clicked();
        void on_leaf_selected(cH_RowDataBase d);
   public:
      with_class();
};
#endif
