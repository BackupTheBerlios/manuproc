

#include "Kundengruppe.h"
#include <BaseObjects/ManuProcEntity_FetchIStream.h>


Kundengruppe::Kundengruppe(ID kgid, const std::string _grpnm,
	const std::string _obg, const std::string _komm) 
: ManuProcEntity<ID>(kgid), grpname(_grpnm), obergruppe(_obg),
	kommentar(_komm) 
{}


FetchIStream &operator>>(FetchIStream &is,Kundengruppe &kg)
 {  return is >> kg >> kg.obergruppe >> kg.grpname >> kg.kommentar;
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
   is >> _id >> _obg >> _bez >> _komm;
   cH_Kundengruppe ch_kg((Kundengruppe::ID)_id,_obg,_bez,_komm);
   kg=ch_kg;
   return is;
 } 
 
 
 
 
 