// $Id: create_parse.h,v 1.5 2004/11/12 07:41:44 christof Exp $
/*  ManuProC_Base: Main ManuProC Library
 *  Copyright (C) 2004  Christof Petig
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

#ifndef D086626E_F86A_453E_8C15_0B36F04A5CAD
#define D086626E_F86A_453E_8C15_0B36F04A5CAD

#include <string>
#include <stdexcept>

namespace ManuProC
{       template <class T>
    	 static T parse(const std::string &val) throw(std::out_of_range);
    	template <class T>
    	 static T parse_def(const std::string &val, const T &def=T());
    	template <class T>
    	 static std::string create(const T &val);
}

// you can skip these declarations while reading this file
namespace ManuProC {
template <> std::string create<int>(const int &val);
template <> std::string create<double>(const double &val);
template <> std::string create<bool>(const bool &val);
template <> int parse<int>(const std::string &value) throw(std::out_of_range);
template <> bool parse<bool>(const std::string &value) throw(std::out_of_range);
template <> long parse<long>(const std::string &value) throw(std::out_of_range);
template <> double parse<double>(const std::string &value) throw(std::out_of_range);
template <> float parse<float>(const std::string &value) throw(std::out_of_range);
template <> std::string parse<std::string>(const std::string &value) throw(std::out_of_range);
}

// g++ 2.95 does not like these inlined
namespace ManuProC {
template <class T>
 T parse_def(const std::string &val, const T &def)
{  try { return parse<T>(val); } 
   catch (std::out_of_range &e) { return def; }
}
}

#endif
