

#include "Kundengruppe.h"
#include <BaseObjects/ManuProcEntity_FetchIStream.h>


Kundengruppe::Kundengruppe(ID kgid, const std::string _obg, 
	const std::string _grpnm, const std::string _komm) 
: ManuProcEntity<ID>(kgid), grpname(_grpnm), obergruppe(_obg),
	kommentar(_komm) 
{}


FetchIStream &operator>>(FetchIStream &is,Kundengruppe &kg)
 {  return is >> kg 
 	>> FetchIStream::MapNull(kg.obergruppe,std::string()) 
 	>> kg.grpname 
 	>> FetchIStream::MapNull(kg.kommentar,std::string());
 } 

cH_Kundengruppe::cH_Kundengruppe(Kundengruppe::ID _id,
	const std::string _obg,
	const std::string _grpnm, 
	const std::string _komm)
{  
    *this=cH_Kundengruppe(new Kundengruppe(_id,_obg,_grpnm,_komm));
}

cH_Kundengruppe::cH_Kundengruppe()
{  
    *this=cH_Kundengruppe(new Kundengruppe());
}


FetchIStream &operator>>(FetchIStream &is,cH_Kundengruppe &kg)
 { int _id;
   std::string _obg;
   std::string _bez;
   std::string _komm;
   is >> _id 
   >> FetchIStream::MapNull(_obg,std::string()) 
   >> _bez 
   >> FetchIStream::MapNull(_komm,std::string());  
   cH_Kundengruppe ch_kg((Kundengruppe::ID)_id,_obg,_bez,_komm);
   kg=ch_kg;
   return is;
 } 
 
 
 
 
 
 