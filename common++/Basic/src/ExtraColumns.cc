// $Id: ExtraColumns.cc,v 1.3 2004/09/24 16:00:15 christof Exp $
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

#include <Misc/ExtraColumns.h>
#include <cassert>

typedef std::map<std::string,ExtraColumns::TableInformation> map_t;

map_t ExtraColumns::table_infos;

void ExtraColumns::register_table(const std::string &table, const std::vector<std::string> &keycols)
{ map_t::iterator info=table_infos.find(table);
  if (info==table_infos.end())
  { info=table_infos.insert(std::make_pair(table,TableInformation(table))).first;
    info->second.key_columns=keycols;
    // look for available_columns
#ifndef MPC_SQLITE
    Query q("select attname from pg_attribute where attrelid"
        "=(select oid from pg_class where relname=?)");
#else
#  error SQLite not supported, yet        
#endif
    q << table;
    Query::Row is;
    while ((q>>is).good()) 
    { info->second.available_columns.insert(is.Fetch<std::string>());
    }
  }
  else assert(info->second.table_name==table && info->second.key_columns==keycols);
  key_values.setNeededParams(info->second.key_columns.size());
}

ExtraColumns::ExtraColumns(const std::string &table, const std::vector<std::string> &keycols)
{ register_table(table,keycols);
}

ExtraColumns::ExtraColumns(const std::string &table, const std::string &keycol1)
{ std::vector<std::string> cols(1);
  cols[0]=keycol1;
  register_table(table,cols);
}

ExtraColumns::ExtraColumns(const std::string &table, const std::string &keycol1,
              const std::string &keycol2)
{ std::vector<std::string> cols(2);
  cols[0]=keycol1;
  cols[1]=keycol2;
  register_table(table,cols);
}
             