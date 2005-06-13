// $Id: createDynConfig.cc,v 1.1 2005/06/13 09:57:50 christof Exp $
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

// $Id: createDynConfig.cc,v 1.1 2005/06/13 09:57:50 christof Exp $


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
      
      std::cout << "//  DynamicConfig.h  created " << Zeitpunkt_new(time(0)) 
      		<< "\n"
      		"\n"
		"#ifndef MANUPROC_DYNAMICCONFIG_H\n"
		"#define MANUPROC_DYNAMICCONFIG_H\n"
		"\n";
      
      //--- warenart, artikeltyp
      {  Query q("select text,id from artbez_warengruppe order by id");
         while ((q >> is).good())
         {  int id; std::string name;
            is >> name >> id;
            std::cout << "#define HAS_WARENGRUPPE_" << toIdentifier(name) << '\n';
         }
      }
      std::cout << "\n";

      {  
       int oid;
       Query("select oid from pg_class where relname='artbez_warengruppe'")
	  >> oid;

       const int ab_spalte=3; // ab der Spalte stehen Attribute (bools)

       Query q("select attname,attnum-? from pg_attribute where attrelid=?"
	 " and attnum>=? and attname not like "
	" '........pg.dropped.%........' order by attnum");
       q  << ab_spalte << oid << ab_spalte;

       while ((q >> is).good())
         {  std::string colname;
            int attnum;
            is  >> colname >> attnum;
            std::cout << "#define HAS_WG_ATTRIB_" << colname << '\n';
         }     
      }
      std::cout << "\n";
      tr.close();
      tr.open();


      //--- einheiten
      {  Query q("select id from einheiten order by id");
         while ((q >> is).good())
         {  int id;
            is >> id;
            std::cout << "#define HAS_UNIT_" << toIdentifier(Einheit(Einheit::ID(id)).Bezeichnung()) << '\n';
         }
      }
      std::cout << "\n";
      tr.close();
      tr.open();
      
      //--- prozesse
      {  Query q("select prozessid,label,text from prozesse order by prozessid");
         while ((q >> is).good())
         {  int id; std::string label,text;
            is >> id >> label >> FetchIStream::MapNull(text,std::string());
            if (!text.empty()) label=label+'_'+text;
            std::cout << "#define HAS_PROCESS_" << toIdentifier(label) << '\n';
         }
      }
      std::cout << "\n";
      tr.close();
      tr.open();
      
      //--- waehrung
      {  Query q("select wid,kurz from waehrung order by wid");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            std::cout << "#define HAS_CURRENCY_" << toIdentifier(text) << '\n';
         }
      }
      std::cout << "\n";
      tr.close();
      tr.open();
      
      //--- prod_instanz
      {  Query q("select insid,name from prod_instanz order by insid");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            std::cout << "#define HAS_STAGE_" << toIdentifier(text) << '\n';
         }
      }
      
      std::cout << "\n";
      
      tr.close();
      tr.open();      
      
      //--- kunden
      {  Query q("select grpnr,name from ku_gruppe order by grpnr");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            std::cout << "#define HAS_ADDR_GROUP_" + toIdentifier(text) << '\n';
         }
      }      
      
      std::cout << "\n";

      // ------------------
            
      tr.close();
      std::cout << "#endif\n";
      ManuProC::dbdisconnect();
      return 0;
   }
   catch (...)
   {  return 1;
   }
}
