// $Id: createDynDescr.cc,v 1.1 2004/03/26 13:20:37 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig
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

// $Id: createDynDescr.cc,v 1.1 2004/03/26 13:20:37 jacek Exp $

#include <Misc/dbconnect.h>
#include <Misc/FetchIStream.h>
#include <Misc/Transaction.h>
#include <ctime>
#include <iostream>
#include <Misc/Zeitpunkt_new.h>
#include <Artikel/Einheiten.h>

std::string toIdentifier(const std::string &s)
{  std::string res;
   for (std::string::const_iterator i=s.begin();i!=s.end();++i)
   {  if (*i=='ä') res+="ae";
      else if (*i=='ö') res+="oe";
      else if (*i=='ü') res+="ue";
      else if (*i=='ß') res+="ss";
      else if (*i=='Ä') res+="Ae";
      else if (*i=='Ö') res+="Oe";
      else if (*i=='Ü') res+="Ue";
      else if (('A'<=*i && *i<='Z') || ('a'<=*i && *i<='z')
   	|| (i!=s.begin() && '0'<=*i && *i<='9'))
         res+=*i;
      else res+='_';
   }
   return res;
}

int main()
{  try
   {  ManuProC::dbconnect();
      Transaction tr;
      FetchIStream is;
      
      std::cout << "//  DynamicDescriptions.h  created " << Zeitpunkt_new(time(0)) 
      		<< "\n"
      		"\n"
		"#ifndef MANUPROC_DYNAMICENUMS_H\n"
		"#define MANUPROC_DYNAMICENUMS_H\n"
		"\n"
		"#define MANUPROC_DYNAMICENUMS_CREATED\n"
		"\n"
		"namespace ManuProC {\n"
		" namespace DynamicDescriptions {\n";
      



		"#endif // MANUPROC_DYNAMICENUMS_H\n";
      
      tr.close();
      ManuProC::dbdisconnect();
      return 0;
   }
   catch (...)
   {  return 1;
   }
}

