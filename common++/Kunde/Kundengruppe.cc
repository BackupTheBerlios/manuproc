// $Id: Kundengruppe.cc,v 1.15 2006/10/31 16:05:29 christof Exp $
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
	const std::string _grpnm, const std::string _komm, int _owner,
			bool _obgrp_uniq) 
: ManuProcHandle<ID>(kgid), grpname(_grpnm), obergruppe(_obg),
	kommentar(_komm),owner(_owner),obergruppe_uniq(_obgrp_uniq) 
{}

Kundengruppe::Kundengruppe(ID kgid) throw(SQLerror)
: ManuProcHandle<ID>(kgid) 
{
 if(kgid==Kundengruppe::none_id) return;

 Query("select coalesce(obergruppe,''),name,"
	"coalesce(kommentar,''),owner,obergruppe_uniq from"
	" ku_gruppe where grpnr=?") << kgid
	>> obergruppe
	>> grpname
	>> kommentar
	>> Query::Row::MapNull(owner,none_id)
	>> obergruppe_uniq;
}


Query::Row &operator>>(Query::Row &is,Kundengruppe &kg)
 {  return is >> (int&)kg.entityid 
	>> Query::Row::MapNull(kg.obergruppe,std::string()) 
 	>> kg.grpname 
 	>> Query::Row::MapNull(kg.kommentar,std::string())
	>> Query::Row::MapNull(kg.owner,Kundengruppe::none_id)
	>> kg.obergruppe_uniq;
 } 
 
Query::Row &operator>>(Query::Row &is,Kundengruppe::ID &kgid)
 {  return is >> (int&)kgid; 
 }  

cH_Kundengruppe::cH_Kundengruppe(Kundengruppe::ID _id,
	const std::string _obg,
	const std::string _grpnm, 
	const std::string _komm,
	const int _owner,
	const bool _obgrp_uniq)
{  
    *this=cH_Kundengruppe(new Kundengruppe(_id,_obg,_grpnm,_komm,
						_owner,_obgrp_uniq));
}

cH_Kundengruppe::cH_Kundengruppe()
{  
    *this=cH_Kundengruppe(new Kundengruppe());
}


Query::Row &operator>>(Query::Row &is,cH_Kundengruppe &kg)
 { int _id;
   std::string _obg;
   std::string _bez;
   std::string _komm;
   int _owner;
   bool _obgrp_uniq;

   is >> _id 
   >> Query::Row::MapNull(_obg,std::string()) 
   >> _bez 
   >> Query::Row::MapNull(_komm,std::string())
   >> Query::Row::MapNull(_owner,Kundengruppe::none_id)
   >> _obgrp_uniq;

   cH_Kundengruppe ch_kg((Kundengruppe::ID)_id,_obg,_bez,_komm,
					_owner,_obgrp_uniq);
   kg=ch_kg;
   return is;
 } 


cH_Kundengruppe::cH_Kundengruppe(int _owner, const std::string _obergrp) 
throw(SQLerror) 
{
 if(_owner==Kundengruppe::none_id) return;
 Kundengruppe::ID gid=Kundengruppe::none_id;

 Query q("select grpnr from"
	" ku_gruppe where obergruppe=? and owner=?");
 q << _obergrp << _owner;

 Query::Row fi=q.Fetch();
 if(fi.good())
   fi >> gid;

 *this=cH_Kundengruppe(new Kundengruppe(gid));
}


#if !defined(__GNUC__) || __GNUC__ > 2
template <>
 const Kundengruppe::ID ManuProcEntity<Kundengruppe::ID>::none_id;
#endif
