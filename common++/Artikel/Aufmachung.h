/* $Id: Aufmachung.h,v 1.3 2002/05/06 13:41:22 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include"Aux/SQLerror.h"
#include<string>
#include<vector>
//#include<gtk--/list.h>
//#include<gtk--/combo.h>

#ifndef AUFMACHUNGH
#define AUFMACHUNGH

/// Eine Kapselung der Aufmachung
class Aufmachung
{
 int aufmachid;
 std::string bezeichnung;
 std::string abkuerzung;
 int meter_pro_stk;
 bool yard;

public:

 Aufmachung &operator=(const Aufmachung &a);
 Aufmachung(const Aufmachung &a);
 Aufmachung(int aufid) throw(SQLerror);
 Aufmachung(char *abkurz) throw(SQLerror);
 Aufmachung(){aufmachid=0;meter_pro_stk=0;yard=false;}
 // können wir nicht die gtk abhängigkeit aus dieser Library nehmen???
 // ist _nur_ wegen dieser Funktion da
// static void fillListAndVector(Gtk_Combo &entry, std::vector<Aufmachung> *aufvec=0) throw(SQLerror); 

 const std::string getBezeichnung() const { return bezeichnung;}
 int getAufmId() const { return aufmachid;} 
 int getMtrProStk() const { return meter_pro_stk;}
 const std::string getAbkurz() const { return abkuerzung;}
};


#endif

