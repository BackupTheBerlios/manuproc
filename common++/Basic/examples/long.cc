// $Id: long.cc,v 1.2 2001/12/05 07:55:59 christof Exp $
/*  libcommonc++: ManuProC's main OO library
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


#include <Aux/Long.h>
#include <iostream>

int main()
{  Long l;
   cout << l << '\n';
   l+=2;
   cout << l << '\n';
   l+=Long(3);
   cout << l << '\n';
   cout << ++l << '\n';
   cout << l++ << '\n';
   cout << l << '\n';
   return 0;
}
// 0 2 5 6 6 7
