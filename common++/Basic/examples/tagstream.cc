// $Id: tagstream.cc,v 1.6 2004/06/03 07:14:33 christof Exp $
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

#include <Misc/TagStream.h>
#include <iostream>

int main()
{  std::cout << "===== reading simple file =====\n";
   {  const char * const Testcase1="<?xml version=\"1.0\"?><test><test A=\"a\"/></test>";
      TagStream ts(Testcase1);
      ts.debug(10);
      ts.write(std::cout,true);
      std::cout << '\n';
   }
   std::cout << "===== iso 8859-1 =====\n";
   {  const char * const Testcase1="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
   		"<test>"
   		  "<süb ÄÖÜäöüß=\"ÄÖÜäöüß°ué\">èï<ä/>¥</süb>"
   		"</test>";
      TagStream ts(Testcase1);
      ts.debug(10);
      ts.write(std::cout,true);
      std::cout << '\n';
   }
   std::cout << "===== special chars =====\n";
   {  const char * const Testcase1="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
   		"<test>"
   		  "<sub>&#xfc;&auml;&amp;&lt;&gt;&quot;&#65;"
   		    "<subsub attr=\"&amp;\"/>&amp;"
   		  "</sub>"
   		"</test>";
      TagStream ts(Testcase1);
      ts.debug(10);
      ts.write(std::cout,true);
      std::cout << '\n';
   }
   std::cout << "===== reading BOM =====\n";
   {  const char * const Testcase1="ï»¿<?xml version=\"1.0\"?><test a=\"ï»¿\"/>";
      TagStream ts(Testcase1);
      ts.debug(10);
      ts.write(std::cout,true);
      std::cout << '\n';
   }
   return 0;
}
