// $Id: KontaktListe.h,v 1.1 2002/05/06 13:39:17 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef KUNDE_KONTAKTLISTE_HH
#define KUNDE_KONTAKTLISTE_HH

#include<vector>
#include <BaseObjects/ManuProcEntity.h>
#include <Kunde/TelefonArt.h>
#include <Kunde/Telefon.h>


class KontaktListe 
{
private: 

 std::vector<cH_Telefon> kontakt;
 bool val;

 void load(const TelArt &ta, ManuProcEntity::ID kundeid,
			ManuProcEntity::ID personid);
  
public:
 
 KontaktListe() : val(false){}
 KontaktListe(const TelArt &ta, ManuProcEntity::ID kundeid, ManuProcEntity::ID personid);
 void reload(const TelArt &ta, ManuProcEntity::ID kundeid, ManuProcEntity::ID personid);
 std::string get_first_kontakt(const TelArt &ta,ManuProcEntity::ID kundeid, 
 				ManuProcEntity::ID personid);
 bool valid() const { return val; } 
 
};


#endif

