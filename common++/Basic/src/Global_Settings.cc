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
#include <iostream>

std::string Global_Settings::get_Wert0(const std::string &separator,int field) const
{ std::string gs = get_Wert();
  std::string::size_type start=0;
  while (field>0)
  {  start=gs.find(separator,start);
     if (start==std::string::npos) return "";
     start+=separator.size();
     --field;
  }
  std::string::size_type end=gs.find(separator,start);
  return std::string(gs,start,end==std::string::npos?end:(end-start));
}

std::string Global_Settings::get_Wert(const std::string &separator,int field) const
{ if (!field) return "";
  return get_Wert0(separator,field-1);
}

void Global_Settings::set_Wert(const std::string& _wert)
{  (*save_impl)(userid,program,name,_wert);
   wert=_wert;
}

Global_Settings::Global_Settings(int u,const std::string& p,const std::string& n)
: userid(u),program(p),name(n), wert((*load_impl)(u,p,n))
{
}

void Global_Settings::create(int userid,const std::string& program,
                const std::string& name,const std::string& wert)
{  (*save_impl)(userid,program,name,wert);
}

std::string Global_Settings::default_load(int userid,const std::string& program,const std::string& name)
{  std::cout << "Global_Settings: [" << userid << ',' << program
		<< ',' << name << "]\n";
   return std::string();
}


void Global_Settings::default_save(int userid,const std::string& program,
                             const std::string& name,const std::string& wert)
{  std::cout << "Global_Settings: [" << userid << ',' << program
		<< ',' << name << "]=" << wert << "\n";
}

Global_Settings::save_cb Global_Settings::save_impl=&Global_Settings::default_save;
Global_Settings::load_cb Global_Settings::load_impl=&Global_Settings::default_load;

void Global_Settings::set_impl(load_cb ld, save_cb sv)
{  if (ld) load_impl=ld;
   if (sv) save_impl=sv;
}

