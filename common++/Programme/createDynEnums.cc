// $Id: createDynEnums.cc,v 1.4 2002/06/20 13:27:55 christof Exp $

#include <Aux/dbconnect.h>
#include <Aux/FetchIStream.h>
#include <Aux/Transaction.h>
#include <ctime>
#include <iostream>
#include <Aux/Zeitpunkt_new.h>
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
      
      cout << "//  DynamicEnums.h  created " << Zeitpunkt_new(time(0)) 
      		<< "\n"
      		"\n"
		"#ifndef MANUPROC_DYNAMICENUMS_H\n"
		"#define MANUPROC_DYNAMICENUMS_H\n"
		"\n"
		"#define MANUPROC_DYNAMICENUMS_CREATED\n"
		"\n"
		"namespace ManuProC {\n"
		" namespace DynamicEnums {\n";
      
      //---
      cout << "  namespace ArtikelTyp {\n"
		"   enum enum_t {\n"
		"    ";
      {  Query q("select text,id from artbez_warengruppe order by id");
         while ((q >> is).good())
         {  int id; std::string name;
            is >> name >> id;
            cout << toIdentifier(name) << '=' << id << ',';
         }
      }
      cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- einheiten
      cout << "  namespace Einheiten {\n"
		"   enum enum_t {\n"
		"    ";
      {  Query q("select id from einheiten order by id");
         while ((q >> is).good())
         {  int id;
            is >> id;
            cout << toIdentifier(Einheit(Einheit::ID(id)).Bezeichnung()) << '=' << id << ',';
         }
      }
      cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- prozesse
      cout << "  namespace Prozesse {\n"
		"   enum enum_t {\n"
		"    None=0,";
      {  Query q("select prozessid,label,text from prozesse order by prozessid");
         while ((q >> is).good())
         {  int id; std::string label,text;
            is >> id >> label >> text;
            if (text.size()) label=label+'_'+text;
            cout << toIdentifier(label) << '=' << id << ',';
         }
      }
      cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- waehrung
      cout << "  namespace Waehrung {\n"
		"   enum enum_t {\n"
		"    ";
      {  Query q("select wid,kurz from waehrung order by wid");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            cout << toIdentifier(text) << '=' << id << ',';
         }
      }
      cout << "\n"
      		"   };\n"
      		"  }\n\n";
      tr.close();
      tr.open();
      
      //--- prod_instanz
      cout << "  namespace Instanzen {\n"
		"   enum enum_t {\n"
		"    None=0,";
      {  Query q("select insid,name from prod_instanz order by insid");
         while ((q >> is).good())
         {  int id; std::string text;
            is >> id >> text;
            cout << toIdentifier(text) << '=' << id << ',';
         }
      }
      cout << "\n"
      		"   };\n"
      		"  }\n\n";

      // ------------------
            
      cout << " }\n"
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
