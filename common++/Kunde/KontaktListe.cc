// $Id: KontaktListe.cc,v 1.3 2002/07/05 12:35:01 christof Exp $
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


#include<KontaktListe.h>

void KontaktListe::load(ManuProcEntity::ID kundeid,
			ManuProcEntity::ID personid)
{
 std::vector<ManuProcEntity::ID> kontaktids;

 Telefon::getTelIDs(kontaktids,TEL_NONE,kundeid,personid);

 for(std::vector<ManuProcEntity::ID>::iterator i=kontaktids.begin();
 		i!=kontaktids.end(); ++i)
   {
    kontakt.push_back(cH_Telefon(*i));
   }

}



void KontaktListe::reload(ManuProcEntity::ID kundeid,
			ManuProcEntity::ID personid)
{
 kontakt.erase(kontakt.begin(),kontakt.end());
 load(kundeid,personid);
 val=true;
}

KontaktListe::KontaktListe
(ManuProcEntity::ID kundeid, ManuProcEntity::ID personid) 
: val(true)
{
 load(kundeid,personid);
}


std::string KontaktListe::get_first_kontakt(const TelArt &ta,
	ManuProcEntity::ID kundeid, ManuProcEntity::ID personid)
{
 for(std::vector<cH_Telefon>::iterator i=kontakt.begin();i!=kontakt.end(); ++i)
   {
    if(ta==(*i)->TelefonArt())
      if(kundeid==(*i)->getKunde())
        if(personid==(*i)->getPerson())
	  return (*i)->Text();
   }

 return "";
}


