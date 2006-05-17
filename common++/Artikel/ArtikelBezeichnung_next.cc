// $Id: ArtikelBezeichnung_next.cc,v 1.5 2006/05/17 07:35:01 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Artikel/ArtikelBezeichnung.h>
#include <Artikel/ArtikelStamm.h>
#include <Misc/Query.h>

ArtikelBase ArtikelBezeichnung::Next(
	const std::vector<ExtBezSchema::BezKomp> &bez_seq) const
 // look for the lexically next article in this schema
{ ArtikelBase result;
  const int signifikanz=1; // arbitrary, should get a parameter
  std::vector<ExtBezSchema::const_iterator> entries;
  std::string order;
  if(bez_seq.empty())
    {for (ExtBezSchema::const_sigiterator i=schema->sigbegin(signifikanz);
     				      i!=schema->sigend(signifikanz);++i)
       { entries.push_back(i);
         if (!order.empty()) order+=',';
         order+=" atoi("+i->spaltenname+"),"+i->spaltenname;
       }
     }
   else
    {for (ExtBezSchema::const_iterator i=bez_seq.begin();
     				      i!=bez_seq.end();++i)
       { entries.push_back(i);
         if (!order.empty()) order+=',';
         order+=" atoi("+i->spaltenname+"),"+i->spaltenname;;
       }
     }


    
  for (std::vector<ExtBezSchema::const_iterator>::reverse_iterator r=entries.rbegin();
          r!=entries.rend();++r)
  {ArgumentList args;
   std::string query;
   query = "SELECT id FROM "+schema->TableName()+ " WHERE ";
   int f=0;

   if(bez_seq.empty())
   for (ExtBezSchema::const_sigiterator i=schema->sigbegin(signifikanz);
           i!=schema->sigend(signifikanz);)
     {
       if (static_cast<ExtBezSchema::const_iterator>(i)==*r)
	 {query+="( (atoi("+i->spaltenname+")>atoi(?)) or"+
		 "  ("+i->spaltenname+">?) )";
	 } 
       else {query+=i->spaltenname; query+="=?";}
       args << (*this)[i->bezkomptype]->getStrVal();
       if (static_cast<ExtBezSchema::const_iterator>(i)==*r) break;
       query+=" AND ";
       ++i;
     }
   else
   for (ExtBezSchema::const_iterator i=bez_seq.begin();i!=bez_seq.end();)
     {
       if (static_cast<ExtBezSchema::const_iterator>(i)==*r)
	 {query+="( (atoi("+i->spaltenname+")>atoi(?)) or"+
		 "  ("+i->spaltenname+">?) )";
	 }  
       else {query+=i->spaltenname; query+="=?";}
       args << (*this)[i->bezkomptype]->getStrVal();
       if (static_cast<ExtBezSchema::const_iterator>(i)==*r) break;
       query+=" AND ";
       ++i;
     }

   query+=" ORDER BY "+order+" LIMIT 1";
   try
   { Query(query) << args >> result;
     return result;
   } catch (SQLerror &e) {}
  }
  return ArtikelBase();
}
