

#include "Kundengruppe.h"
#include <BaseObjects/ManuProcEntity_FetchIStream.h>


Kundengruppe::Kundengruppe(ID kgid, std::string _grpnm) 
: ManuProcEntity<ID>(kgid), grpname(_grpnm) 
{}


FetchIStream &operator>>(FetchIStream &is,Kundengruppe &kg)
 {  return is >> kg >> kg.grpname;
 } 

cH_Kundengruppe::cH_Kundengruppe(Kundengruppe::ID _id,std::string _grpnm)
{  
    *this=cH_Kundengruppe(new Kundengruppe(_id,_grpnm));
}

cH_Kundengruppe::cH_Kundengruppe()
{  
    *this=cH_Kundengruppe(new Kundengruppe());
}


FetchIStream &operator>>(FetchIStream &is,cH_Kundengruppe &kg)
 { int _id;
   std::string _grpname;
   is >> _id >> _grpname;
   cH_Kundengruppe _kg((Kundengruppe::ID)_id,_grpname);
   kg=_kg;
   return is;
 } 
 
 
 
 
 