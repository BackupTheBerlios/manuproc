// $Id: createDynEnums.cc,v 1.16 2004/03/31 10:47:17 jacek Exp $
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

// $Id: createDynEnums.cc,v 1.16 2004/03/31 10:47:17 jacek Exp $

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
      
      std::cout << "//  DynamicEnums.h  created " << Zeitpunkt_new(time(0)) 
      		<< "\n"
      		"\n"
		"#ifndef MANUPROC_DYNAMICENUMS_H\n"
		"#define MANUPROC_DYNAMICENUMS_H\n"
		"\n"
		"#define MANUPROC_DYNAMICENUMS_CREATED\n"
		"\n"
		"namespace ManuProC {\n"
		" namespace DynamicEnums {\n";
      
      //--- warenart, artikeltyp
      std::cout << "  namespace ArtikelTyp {\n"
		"   enum enum_t {\n"
		"    None=-1, ";
      {  Query q("select text,id from artbez_warengruppe order by id");
         while ((q >> is).good())
         {  int id; std::string name;
            is >> name >> id;
            std::cout << toIdentifier(name) << '=' << id << ',';
         }
      }
      std::cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- artikeltyp attribute
      std::cout << "  namespace ArtikelTyp_Attributes {\n"
		"   enum enum_t {\n";
      {  
       int attr_column=0;

       std::cout << "LagerArtikel" << '=' << attr_column++ << ',';
       std::cout << "MitEAN" << '=' << attr_column++ << ',';
       std::cout << "NullPreisCheck" << '=' << attr_column++ << ',';

      }
      std::cout << "\n"
     		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();


      //--- einheiten
      std::cout << "  namespace Einheiten {\n"
		"   enum enum_t {\n"
		"    None=-1, ";
      {  Query q("select id from einheiten order by id");
         while ((q >> is).good())
         {  int id;
            is >> id;
            std::cout << toIdentifier(Einheit(Einheit::ID(id)).Bezeichnung()) << '=' << id << ',';
         }
      }
      std::cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- prozesse
      std::cout << "  namespace Prozesse {\n"
		"   enum enum_t {\n"
		"    None=0,";
      {  Query q("select prozessid,label,text from prozesse order by prozessid");
         while ((q >> is).good())
         {  int id; std::string label,text;
            is >> id >> label >> FetchIStream::MapNull(text,std::string());
            if (!text.empty()) label=label+'_'+text;
            std::cout << toIdentifier(label) << '=' << id << ',';
         }
      }
      std::cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- waehrung
      std::cout << "  namespace Waehrung {\n"
		"   enum enum_t {\n"
		"   None=-1,";
      {  Query q("select wid,kurz from waehrung order by wid");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            std::cout << toIdentifier(text) << '=' << id << ',';
         }
      }
      std::cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- prod_instanz
      std::cout << "  namespace Instanzen {\n"
		"   enum enum_t {\n"
		"    None=-1,";
      {  Query q("select insid,name from prod_instanz order by insid");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            std::cout << toIdentifier(text) << '=' << id << ',';
         }
      }
      
      std::cout << "\n"
      		"   };\n"
      		"  }\n\n";
      
      tr.close();
      tr.open();      
      
      //--- kunden
      std::cout << "  namespace Kundengruppen {\n"
		"   enum enum_t {\n"
		"    None=-1,";
      {  Query q("select grpnr,name from ku_gruppe order by grpnr");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            std::cout << toIdentifier(text) << '=' << id << ',';
         }
      }      
      
      std::cout << "\n"
      		"   };\n"
      		"  }\n\n";

      // ------------------
            
      std::cout << " }\n"
	      "}\n"
		"#endif // MANUPROC_DYNAMICENUMS_H\n";
      
      tr.close();
      ManuProC::dbdisconnect();
      return 0;
   }
   catch (...)
   {  return 1;
   }
}
