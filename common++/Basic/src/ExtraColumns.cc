// $Id: ExtraColumns.cc,v 1.10 2006/08/03 11:18:28 christof Exp $
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
#include <Misc/TraceNV.h>

static const UniqueValue::value_t trace_channel=ManuProC::Tracer::channels.get();
static ManuProC::Tracer::Environment trace_channel_e("EC_DEBUG",trace_channel);

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
    q << table;
    Query::Row is;
    while ((q>>is).good()) 
    { info->second.available_columns.insert(is.Fetch<std::string>());
    }        
#else
    Query q("pragma table_info(?)");
    q << table;
    Query::Row is;
    while ((q>>is).good()) 
    {std::string s;
     is >> s;
     is >> s;
     info->second.available_columns.insert(s);
    }        
#endif

  }
  else assert(info->second.table_name==table && info->second.key_columns==keycols);
  which=&info->second;
  key_values.setNeededParams(info->second.key_columns.size());
}

ExtraColumns::ExtraColumns(const std::string &table, const std::vector<std::string> &keycols)
  : which(), is_good()
{ register_table(table,keycols);
}

ExtraColumns::ExtraColumns(const std::string &table, const std::string &keycol1)
  : which(), is_good()
{ std::vector<std::string> cols(1);
  cols[0]=keycol1;
  register_table(table,cols);
}

ExtraColumns::ExtraColumns(const std::string &table, const std::string &keycol1,
              const std::string &keycol2)
  : which(), is_good()
{ std::vector<std::string> cols(2);
  cols[0]=keycol1;
  cols[1]=keycol2;
  register_table(table,cols);
}

ExtraColumns::ExtraColumns(const std::string &table, const std::string &keycol1,
              const std::string &keycol2, const std::string &keycol3)
  : which(), is_good()
{ std::vector<std::string> cols(3);
  cols[0]=keycol1;
  cols[1]=keycol2;
  cols[2]=keycol3;
  register_table(table,cols);
}

std::string ExtraColumns::from_where() const
{ std::string res="from "+which->table_name;
  std::string add=" where ";
  for (std::vector<std::string>::const_iterator i=which->key_columns.begin();
        i!=which->key_columns.end();++i)
  { res+=add+*i+"=?";
    add=" and ";
  }
  return res;
}

void ExtraColumns::Execute_if_complete()
{ if (key_values.complete())
  { if (which->requested_columns.empty())
    { 
#ifdef MPC_SQLITE  
      return;
#else
      Query q("select * "+from_where());
      q << key_values;
      Query::Row is;
      if ((q>>is).good())
      { while (is.good())
        { std::string field=is.getFieldName();
          value_t &v=column_values[field];
          int ind=0;
          is >> Query::Row::WithIndicator(v.value,ind);
          if (!ind) v.null=false;
          ManuProC::Trace _t(trace_channel, __FUNCTION__,field,ind,v.value);
        }
        is_good=true;
      }
      return;
#endif
    }
    std::string cols;
    for (std::set<std::string>::const_iterator i=which->requested_columns.begin();
        i!=which->requested_columns.end();++i)
    { if (!cols.empty()) cols+=',';
      cols+=*i;
    }
    Query q("select "+cols+" "+from_where());
    q << key_values;
    Query::Row is;
    if ((q>>is).good())
    { for (std::set<std::string>::const_iterator i=which->requested_columns.begin();
        i!=which->requested_columns.end();++i)
      { value_t &v=column_values[*i];
        int ind=0;
        is >> Query::Row::WithIndicator(v.value,ind);
        if (!ind) v.null=false;
      }
      is_good=true;
    }
  }
}

Query::Row::Fake ExtraColumns::fake_contents(const std::string &column)
{ if (!good()) throw Query::Error("fake_istream",100,"no line available");
  std::map<std::string,value_t>::const_iterator v=column_values.find(column);
  if (v!=column_values.end()) 
  { if (which->requested_columns.find(column)==which->requested_columns.end())
      which->requested_columns.insert(column);
    if (v->second.null) return Query::Row::Fake();
    else return Query::Row::Fake(v->second.value);
  }
  ManuProC::Trace _t(trace_channel, __FUNCTION__,column);
  if (which->available_columns.find(column)==which->available_columns.end())
    return Query::Row::Fake();
  ManuProC::Trace(trace_channel, "","insert");
  which->requested_columns.insert(column);
  int ind=0;
  value_t &v2=column_values[column];
  Query q("select "+column+" "+from_where());
  q << key_values >> Query::Row::WithIndicator(v2.value,ind);
  if (!ind) 
  { v2.null=false; return Query::Row::Fake(v2.value); }
  return Query::Row::Fake();
}

bool ExtraColumns::hasColumn(const std::string &column) const
{ return (which->available_columns.find(column)!=which->available_columns.end());
}
