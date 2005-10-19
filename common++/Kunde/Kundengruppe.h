
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
  
public:
 static const ID default_ID;

 friend class Handle<const Kundengruppe>;
 friend class cH_Kundengruppe;
 
 Kundengruppe() : ManuProcHandle<ID>(KundengruppeID::None) {}
  
 Kundengruppe(ID kgid, const std::string _obg,const std::string _grpnm,
 	const std::string komm="");
 Kundengruppe(ID kgid) throw(SQLerror);
 ID Id() const {return ID(entityid); }
 std::string GrpName() const { return grpname;}
 std::string Obergruppe() const { return obergruppe; }
 std::string Kommentar() const { return kommentar; } 
 
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
		const std::string grpname, const std::string komm);
        bool operator==(const cH_Kundengruppe& b) const
                {return (*this)->Id()==b->Id();} 
        bool operator<(const cH_Kundengruppe& b) const
                {return (*this)->Id()<b->Id();} 
        cH_Kundengruppe();
};




Query::Row &operator>>(Query::Row &is,cH_Kundengruppe &kg);
    
#endif


