
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


