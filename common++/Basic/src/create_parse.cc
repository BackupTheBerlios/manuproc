// $Id: create_parse.cc,v 1.5 2004/06/24 16:03:23 christof Exp $
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

#include <Misc/create_parse.h>
#include <iostream>
#include <locale.h>

#define DE

template <>
int ManuProC::parse<int>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   // 2do: check
   return atoi(value.c_str());
}

template <>
bool ManuProC::parse<bool>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   if (!strcasecmp(value.c_str(),"true")) return true;
   if (!strcasecmp(value.c_str(),"false")) return false;
   if (!strcasecmp(value.c_str(),"yes")) return true;
   if (!strcasecmp(value.c_str(),"no")) return false;
#ifdef DE   
   if (!strcasecmp(value.c_str(),"ja")) return true;
   if (!strcasecmp(value.c_str(),"nein")) return false;
#endif   
   std::cerr << "strange bool value: \"" << value << "\"\n";
   return parse<int>(value);
}

template <>
long ManuProC::parse<long>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   return atol(value.c_str());
} 

template <>
double ManuProC::parse<double>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   /* Make sure we do NOT honor the locale for numeric input */
   /* since the database gives the standard decimal point */
   std::string oldlocale= setlocale(LC_NUMERIC, NULL);
   setlocale(LC_NUMERIC, "C");
   double f=strtod(value.c_str(),0);
   setlocale(LC_NUMERIC, oldlocale.c_str());
   return f;
}

template <>
float ManuProC::parse<float>(const std::string &value) throw(std::out_of_range)
{  return parse<double>(value);
}

template <>
std::string ManuProC::parse<std::string>(const std::string &value) throw(std::out_of_range)
{  return value;
}

#ifdef MPC_SIGC_VERSION
#  define MANUPROC_BASE
#endif

#ifndef MANUPROC_BASE // within glade--
#include <cstdio>
template<>
 std::string ManuProC::create<int>(const int &val)
{  char buf[30];
   snprintf(buf,sizeof buf,"%d",val);
   return buf; // itos?
}

template<>
 std::string ManuProC::create<double>(const double &val)
{  char buf[30];
   /* Make sure we do NOT honor the locale for numeric input */
   /* since the database gives the standard decimal point */
   std::string oldlocale= setlocale(LC_NUMERIC, NULL);
   setlocale(LC_NUMERIC, "C");
   snprintf(buf,sizeof buf,"%f",val);
   setlocale(LC_NUMERIC, oldlocale.c_str());
   return buf; // dtos
}

template<>
 std::string ManuProC::create<bool>(const bool &val)
{  return val?"true":"false"; 
}
#else
#include <Misc/itos.h>

template<>
 std::string ManuProC::create<int>(const int &val)
{  return itos(val);
}

template<>
 std::string ManuProC::create<double>(const double &val)
{  return dtos(val);
}

template<>
 std::string ManuProC::create<bool>(const bool &val)
{  return btos(val);
}
#endif
