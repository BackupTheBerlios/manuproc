// $Id: fixedpoint.cc,v 1.2 2001/03/02 11:42:07 cvs_christof Exp $
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

#include "fixedpoint.h"
#include <cstdlib>
#include <cstdio>

#ifdef TEST_MAIN
#include <iostream>

int main()
{  cout << zehnhochminus<3>() << '\n';
   fixedpoint<> a(4.60),b(3.2),c(4.599),d("4.599");
   cout << "a="<<a << " b="<<b<< " c="<<c<< " d="<<d<< '\n';
   cout << "a/7=" << a/7 << " b=a/7=" << (b=a/7) << '\n';
   cout << "float(b)="<< float(b) << '\n';
   cout << "a+b=" << a+b << '\n';
   return 0;
}
#endif
