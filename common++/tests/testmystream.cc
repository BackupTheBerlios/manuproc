// $Id: testmystream.cc,v 1.2 2004/11/26 15:08:17 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/mystream.h>
#include <iostream>
#include <Misc/TagStream.h>

int main()
{ mystream m;
    m << 2.3 << ':' << "test";
    std::cout << m.str() << '\n';
    m << ":test2";
    std::cout << m.str(true) << '\n';
    m << ":test3";
    std::cout << m.str(true) << '\n';
    
    TagStream ts;
    ts.push_back(Tag("content"));
    ts.write(m,true);
    std::cout << m.str(true) << '\n';
   return 0;
}
