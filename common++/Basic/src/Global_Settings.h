/* $Id: Global_Settings.h,v 1.2 2003/11/04 09:07:13 christof Exp $ */
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

#ifndef GLOBALSETTINGSCLASS
#define GLOBALSETTINGSCLASS
#include <string>

struct Global_Settings
{   typedef void (*save_cb)(int userid,const std::string& program,
		const std::string& name, const std::string& value);
    typedef std::string (*load_cb)(int userid,const std::string& program,
		const std::string& name);

    int userid;
    std::string program,name,wert;       

private:    
    static save_cb save_impl;
    static load_cb load_impl;
    static void default_save(int userid,const std::string& program,
                    const std::string& name, const std::string& value);
    static std::string default_load(int userid,const std::string& program,
                    const std::string& name);
    static void database_save(int userid,const std::string& program,
                    const std::string& name, const std::string& value);
    static std::string database_load(int userid,const std::string& program,
                    const std::string& name);
public:
    Global_Settings(int userid,const std::string& program,
                const std::string& name);

    std::string get_Wert() const {return wert;}
    // VORSICHT: Diese Funktion beginnt mit Index 1
    std::string get_Wert(const std::string &seperator,int field) const;
    // sinnvolle Variante mit 1.Element=[0]
    std::string get_Wert0(const std::string &seperator,int field) const;

    void set_Wert(const std::string& wert);
    // saves one query in comparison to Global_Settings().set_Wert
    static void create(int userid,const std::string& program,
                const std::string& name,const std::string& wert);

    static void database_connected(bool on=true);
    static void set_impl(load_cb ld, save_cb sv);
};
#endif
