
#ifndef COMMON_KUNDENGRUPPE
#define COMMON_KUNDENGRUPPE

#include <BaseObjects/ManuProcHandle.h>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>
#include <Misc/Query.h>
#include <Misc/CacheStatic.h>

namespace KundengruppeID=ManuProC::DynamicEnums::Kundengruppen;




class Kundengruppe : public ManuProcHandle<KundengruppeID::enum_t>
{

 std::string grpname;
 std::string obergruppe;
 std::string kommentar;
 int owner; // referenz auf kunden
  
public:
 static const ID default_ID;

 friend class Handle<const Kundengruppe>;
 friend class cH_Kundengruppe;
 
 Kundengruppe() : ManuProcHandle<ID>(KundengruppeID::None),
                  owner(none_id) {}
  
 Kundengruppe(ID kgid, const std::string _obg,const std::string _grpnm,
 	const std::string komm="", int _owner=none_id);
 Kundengruppe(ID kgid) throw(SQLerror);
 Kundengruppe(int _owner, const std::string _obergrp) throw(SQLerror);
 ID Id() const {return ID(entityid); }
 std::string GrpName() const { return grpname;}
 std::string Obergruppe() const { return obergruppe; }
 std::string Kommentar() const { return kommentar; } 
 int Owner() const { return owner; }
 
 friend Query::Row &operator>>(Query::Row &is, Kundengruppe &kg);
 friend Query::Row &operator>>(Query::Row &is, Kundengruppe::ID &kgid); 

};

class cH_Kundengruppe : public Handle<const Kundengruppe>
{	
        typedef CacheStatic<Kundengruppe::ID,cH_Kundengruppe> cache_t;
        static cache_t cache;
        cH_Kundengruppe(const Kundengruppe *p) : Handle<const Kundengruppe>(p) {}	

public:
	typedef Kundengruppe::ID ID;
	cH_Kundengruppe(ID id);
	cH_Kundengruppe(ID nr, const std::string _obg,
		const std::string grpname, const std::string komm,
                int _owner=Kundengruppe::none_id);
        cH_Kundengruppe(int kdnr, const std::string _obergrp) throw(SQLerror);
        bool operator==(const cH_Kundengruppe& b) const
                {return (*this)->Id()==b->Id();} 
        bool operator<(const cH_Kundengruppe& b) const
                {return (*this)->Id()<b->Id();} 
        cH_Kundengruppe();
};




Query::Row &operator>>(Query::Row &is,cH_Kundengruppe &kg);
    
#endif


