// $Id: ExtraColumns.h,v 1.1 2004/09/24 15:20:11 christof Exp $
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

class ExtraColumns {
    class TableInformation
    { std::string table_name;
      std::set<std::string> key_columns;
      std::set<std::string> requested_columns;
      std::set<std::string> available_columns;
    };
    static map<std::string,TableInformation> table_infos;
    
    struct value_t
    { bool null;
      std::string value;
    };
    std::map<std::string,value_t> column_values;
    ArgumentList key_values;
    void Execute_if_complete();

    FetchIStream::Fake &fake_istream(const std::string &column);
public:
    ExtraColumns(const std::string &table, const std::string &keycol1);
    ExtraColumns(const std::string &table, const std::string &keycol1,
          const std::string &keycol2);
    // 3 ...

    template <class T>
     ExtraColumns &operator <<(const T &val)
    {  key_values << val;
       Execute_if_complete();
       return *this;
    }
    template <class T>
     void get(const std::string &column, const T &def_value=T())
    { fake_istream( >>
    }
};


#endif
