// $Id: trans.cc,v 1.4 2003/01/08 09:46:58 christof Exp $
/*  libcommonc++: ManuProC's main OO library
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

#include <Misc/Transaction.h>
#include <Misc/dbconnect.h>
#include <Misc/SQLerror.h>

int main()
{  ManuProC::dbconnect();
   { Transaction a;
   }
   { Transaction b;
   }
   
   ManuProC::dbconnect(ManuProC::Connection("","","","second"));
   
   {
   ManuProC::dbdefault();
   Transaction c;
   Transaction d("second");
   
   Transaction e("second");
   c.commit();
   Transaction f;
   }
   ManuProC::dbdisconnect("second");
   ManuProC::dbdisconnect();
}
