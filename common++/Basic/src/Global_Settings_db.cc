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

#include "Global_Settings.h"
#include <Misc/Query.h>

std::string Global_Settings::database_load(int userid,const std::string& program,const std::string& name)
{std::string wert;
 try
 {Query("select wert from global_settings "
 	"where (userid,program,name)=(?,?,?)")
   << userid << program << name
   >> wert;
 } catch (SQLerror &e)
 {  SQLerror::print(__FILELINE__,100);
 }
 return wert;
}


void Global_Settings::database_save(int userid,const std::string& program,
                             const std::string& name,const std::string& wert)
{  // saves one query in comparison to Global_Settings().set_Wert
   //  ... the select ...
   Query q("update global_settings set wert=? "
	"where (userid,program,name)=(?,?,?)");
  q << wert << userid << program << name;
  if (q.Result()==100)
  {  Query("insert into global_settings "
	"(userid,program,name,wert) values (?,?,?,?)")
	<< userid << program << name << wert;
  }
  SQLerror::test(__FILELINE__);
}

void Global_Settings::database_connected(bool on)
{  if (!on) set_impl(&default_load,&default_save);
   else set_impl(&database_load,&database_save);
}
