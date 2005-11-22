// $Id: ArtikelBezeichnung_anlegen.cc,v 1.1 2005/11/22 13:04:29 christof Exp $
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
#include <Misc/string0.h>
#include <Misc/Transaction.h>
#include <Misc/Query.h>
//#include <Misc/dbconnect.h>
#include <unistd.h>
#include <sys/types.h>


#ifdef MABELLA_EXTENSIONS
#include <Misc/EAN.h>
#endif

void ArtikelBezeichnung::Anlegen(const cH_ExtBezSchema &schema,const ArtikelBase &art,
  	const std::string &columns, const std::string &values)
{  std::string col="(id,"+columns+",";
   std::string val="("+itos(art.Id())+","+values+",";
#if defined(MABELLA_EXTENSIONS)  && defined(MANUPROC_DYNAMICENUMS_CREATED)
  if(schema->Id()==ExtBezSchema::default_id && 
	ArtikelTyp::hasAttribute(schema->Typ(),
	  ManuProC::DynamicEnums::ArtikelTyp_Attributes::mit_ean))
     {
     col +="ean,";
     val += EAN::next_value(schema->Typ())+",";
     }  
#endif

#if defined MABELLA_EXTENSIONS && defined(MANUPROC_DYNAMICENUMS_CREATED)
   int prozess=40; // Rollen allgemein   
   if(ArtikelTyp::hasAttribute(schema->Typ(),
	  ManuProC::DynamicEnums::ArtikelTyp_Attributes::bez_as_view))
     {col+="prozess,uid,";
      val+=itos(prozess)+","+itos(getuid())+","; 
     }
#endif

   assert(col[col.size()-1]==',' && val[val.size()-1]==',');
   col[col.size()-1]=')';
   val[val.size()-1]=')';

   Query("insert into "+Tabellenname(schema) +" "+ col+" values "+val);
   SQLerror::test(__FILELINE__,100);
}

void ArtikelBezeichnung::Anlegen(const cH_ExtBezSchema &schema,const ArtikelBase &art,
  	std::vector<std::string> const& columns,
  	std::vector<std::string> const& values)
{  ArgumentList args;
   std::string cols="id",vals="?";
   args << art;
   assert(columns.size()==values.size());
   for (unsigned i=0;i<columns.size();++i)
   { cols+=","+columns[i];
     vals+=",?";
     args << values[i];
   }
#if defined(MABELLA_EXTENSIONS)  && defined(MANUPROC_DYNAMICENUMS_CREATED)
  if(schema->Id()==ExtBezSchema::default_id && 
	ArtikelTyp::hasAttribute(schema->Typ(),
	  ManuProC::DynamicEnums::ArtikelTyp_Attributes::mit_ean))
     {
     cols +=",ean";
     vals +=",?";
     args << EAN::next_value(schema->Typ());
     }  
#endif

#if defined MABELLA_EXTENSIONS && defined(MANUPROC_DYNAMICENUMS_CREATED)
   int prozess=40; // Rollen allgemein   
   if(ArtikelTyp::hasAttribute(schema->Typ(),
	  ManuProC::DynamicEnums::ArtikelTyp_Attributes::bez_as_view))
     {cols+=",prozess,uid";
      vals+=",?,?";
      args << prozess << getuid(); 
     }
#endif

   (Query("insert into "+Tabellenname(schema) +" ("+ cols+") values ("+vals+")")
     << args).Check100();
}

ArtikelBase ArtikelBezeichnung::Vorhanden(const cH_ExtBezSchema &schema, 
      int signifikanz, std::vector<cH_EntryValue> const& values)
{ try
  { return ArtikelBase(ArtikelBezeichnung(signifikanz,values,schema).Id());
  }
  catch (...) 
  { return ArtikelBase(); }
}

// alle Schemata durchsuchen
ArtikelBase ArtikelBezeichnung::Vorhanden(ArtikelTyp::ID atid, int signifikanz,
  	std::vector<cH_EntryValue> const& values)
{ std::vector<ExtBezSchema::ID> ids=ExtBezSchema::getSchemaIDs(atid);
  ArtikelBase result;
  for (std::vector<ExtBezSchema::ID>::const_iterator i=ids.begin();i!=ids.end();++i)
  { result=Vorhanden(cH_ExtBezSchema(*i,ArtikelTyp(atid)),signifikanz,values);
    if (!!result) return result;
  }
  return ArtikelBase();
}
