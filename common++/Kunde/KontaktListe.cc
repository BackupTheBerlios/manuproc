
#include<KontaktListe.h>

void KontaktListe::load(const TelArt &ta, ManuProcEntity::ID kundeid,
			ManuProcEntity::ID personid)
{
 std::vector<ManuProcEntity::ID> kontaktids;

 Telefon::getTelIDs(kontaktids,ta,kundeid,personid);

 for(std::vector<ManuProcEntity::ID>::iterator i=kontaktids.begin();
 		i!=kontaktids.end(); ++i)
   {
    kontakt.push_back(cH_Telefon(*i));
   }

}



void KontaktListe::reload(const TelArt &ta, ManuProcEntity::ID kundeid,
			ManuProcEntity::ID personid)
{
 kontakt.erase(kontakt.begin(),kontakt.end());
 load(ta,kundeid,personid);
 val=true;
}

KontaktListe::KontaktListe
(const TelArt &ta, ManuProcEntity::ID kundeid, ManuProcEntity::ID personid) 
: val(true)
{
 load(ta,kundeid,personid);
}


std::string KontaktListe::get_first_kontakt(const TelArt &ta,
	ManuProcEntity::ID personid, ManuProcEntity::ID kundeid)
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


