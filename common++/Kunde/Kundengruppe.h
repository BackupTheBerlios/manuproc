
#ifndef COMMON_KUNDENGRUPPE
#define COMMON_KUNDENGRUPPE

#include <BaseObjects/ManuProcEntity.h>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>

#include <Aux/FetchIStream.h>


namespace KundengruppeID=ManuProC::DynamicEnums::Kundengruppen;


class Kundengruppe : public ManuProcEntity<KundengruppeID::enum_t>
{
 static const ID default_ID=ManuProC::DefaultValues::Kunden;
 std::string grpname;
  
public:

 friend class Handle<const Kundengruppe>;
 friend class cH_Kundengruppe;
 
 Kundengruppe() : ManuProcEntity<ID>(KundengruppeID::None) {}
  
 Kundengruppe(ID kgid, std::string _grpnm);
 ID Id() const {return ID(entityid); }
 std::string GrpName() const { return grpname;}
 
 friend FetchIStream &operator>>(FetchIStream &is, Kundengruppe &kg);

};

class cH_Kundengruppe : public Handle<const Kundengruppe>
{	
        cH_Kundengruppe(const Kundengruppe *p) : Handle<const Kundengruppe>(p) {}	

public:
	typedef Kundengruppe::ID ID;
	cH_Kundengruppe(ID nr, std::string grpname);
        bool operator==(const cH_Kundengruppe& b) const
                {return (*this)->Id()==b->Id();} 
        bool operator<(const cH_Kundengruppe& b) const
                {return (*this)->Id()<b->Id();} 
        cH_Kundengruppe();
};




FetchIStream &operator>>(FetchIStream &is,cH_Kundengruppe &kg);
    
#endif


