// $Id: create_parse.h,v 1.2 2004/02/06 11:31:49 christof Exp $
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
    	 static T parse_value(const std::string &val) throw(std::out_of_range);
    	template <class T>
    	 static T parse_value_def(const std::string &val, const T &def);
    	template <class T>
    	 static std::string create_value(const T &val);
}

// you can skip these declarations while reading this file
template <> std::string ManuProC::create_value<int>(const int &val);
template <> std::string ManuProC::create_value<double>(const double &val);
template <> std::string ManuProC::create_value<bool>(const bool &val);
template <> int ManuProC::parse_value<int>(const std::string &value) throw(std::out_of_range);
template <> bool ManuProC::parse_value<bool>(const std::string &value) throw(std::out_of_range);
template <> long ManuProC::parse_value<long>(const std::string &value) throw(std::out_of_range);
template <> double ManuProC::parse_value<double>(const std::string &value) throw(std::out_of_range);
template <> float ManuProC::parse_value<float>(const std::string &value) throw(std::out_of_range);
template <> std::string ManuProC::parse_value<std::string>(const std::string &value) throw(std::out_of_range);

// g++ 2.95 does not like these inlined
template <class T>
 T ManuProC::parse_value_def(const std::string &val, const T &def)
{  try { return parse_value<T>(val); } 
   catch (std::out_of_range &e) { return def; }
}

#endif
