// $Id: ExtraColumns.h,v 1.7 2005/11/08 14:36:31 christof Exp $
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

#ifndef E70FA3BE_6B3E_4FF7_AB14_6B173AE5BAF1
#define E70FA3BE_6B3E_4FF7_AB14_6B173AE5BAF1

#include <Misc/Query.h>
#include <set>
#include <map>
#include <vector>

class ExtraColumns {
public:
    struct TableInformation
    { std::string table_name;
      std::vector<std::string> key_columns;
      std::set<std::string> requested_columns;
      std::set<std::string> available_columns;
      TableInformation(const std::string &tab) : table_name(tab) {}
    };
private:
    static std::map<std::string,TableInformation> table_infos;
    
    struct value_t
    { bool null;
      std::string value;
      value_t() : null(true) {}
    };
    std::map<std::string,value_t> column_values;
    ArgumentList key_values;
    TableInformation *which;
    bool is_good;
    
    void Execute_if_complete();

#ifdef HAVE_LIBPQ_INT_H
    Query::Row::Fake fake_istream(const std::string &column);
#endif
    void register_table(const std::string &table, const std::vector<std::string> &keycols);
    std::string from_where() const;
public:
    ExtraColumns(const std::string &table, const std::string &keycol1);
    ExtraColumns(const std::string &table, const std::string &keycol1,
          const std::string &keycol2);
    ExtraColumns(const std::string &table, const std::string &keycol1,
          const std::string &keycol2,const std::string &keycol3);
    // 3 ... insert as needed or use the following
    ExtraColumns(const std::string &table, const std::vector<std::string> &keycols);

    bool good() const { return is_good; }
    bool hasColumn(const std::string &column) const;
    template <class T>
     ExtraColumns &operator <<(const T &val)
    {  key_values << val;
       Execute_if_complete();
       return *this;
    }
#ifdef HAVE_LIBPQ_INT_H    
    template <class T>
     T get(const std::string &column, const T &def_value=T())
    { return fake_istream(column).FetchMap(def_value);
    }
#endif
};

#endif
