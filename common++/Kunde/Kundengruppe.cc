#include "Kundengruppe.h"
#include <BaseObjects/ManuProcEntity_FetchIStream.h>


cH_Kundengruppe::cache_t cH_Kundengruppe::cache;

cH_Kundengruppe::cH_Kundengruppe(ID id)
{  cH_Kundengruppe *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   { 
    *this=cH_Kundengruppe(new Kundengruppe(id));
      cache.Register(id,*this);
   }
}



const Kundengruppe::ID Kundengruppe::default_ID=ManuProC::DefaultValues::Kunden;

Kundengruppe::Kundengruppe(ID kgid, const std::string _obg, 
	const std::string _grpnm, const std::string _komm) 
: ManuProcHandle<ID>(kgid), grpname(_grpnm), obergruppe(_obg),
	kommentar(_komm) 
{}

Kundengruppe::Kundengruppe(ID kgid) throw(SQLerror)
: ManuProcHandle<ID>(kgid) 
{
 if(kgid==Kundengruppe::none_id) return;

 Query("select coalesce(obergruppe,''),name,"
	"coalesce(kommentar,'') from"
	" ku_gruppe where grpnr=?") << kgid
	>> obergruppe
	>> grpname
	>> kommentar;
}


FetchIStream &operator>>(FetchIStream &is,Kundengruppe &kg)
 {  return is >> kg.entityid 
 	>> FetchIStream::MapNull(kg.obergruppe,std::string()) 
 	>> kg.grpname 
 	>> FetchIStream::MapNull(kg.kommentar,std::string());
 } 
 
FetchIStream &operator>>(FetchIStream &is,Kundengruppe::ID &kgid)
 {  return is >> (int)kgid; 
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
 
 

 
 
 