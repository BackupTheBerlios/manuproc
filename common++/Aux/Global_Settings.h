/* $Id: Global_Settings.h,v 1.4 2002/07/05 12:35:01 christof Exp $ */
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

class Global_Settings
{
    int userid;
    std::string program,name,wert;       
  public:
    Global_Settings(int userid,const std::string& program,
                const std::string& name);

    std::string get_Wert() const {return wert;}
    void set_Wert(const std::string& wert);
    void update_Date();

    //Neuen Eintrag anlegen :
      //Wenn 'wert' == "now()" dann wird ein Timestamp eingetragen
    static void create(int userid,const std::string& program,
                const std::string& name,const std::string& wert);
};
#endif
