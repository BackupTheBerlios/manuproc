
#ifndef COMMON_KUNDENGRUPPE
#define COMMON_KUNDENGRUPPE

#include <BaseObjects/ManuProcEntity.h>
#include <DynamicEnums/DynamicEnums.h>
#include <DynamicEnums/DefaultValues.h>
#include <Misc/FetchIStream.h>


namespace KundengruppeID=ManuProC::DynamicEnums::Kundengruppen;


class Kundengruppe : public ManuProcEntity<KundengruppeID::enum_t>
{

 std::string grpname;
 std::string obergruppe;
 std::string kommentar;
  
public:
 static const ID default_ID;

 friend class Handle<const Kundengruppe>;
 friend class cH_Kundengruppe;
 
 Kundengruppe() : ManuProcEntity<ID>(KundengruppeID::None) {}
  
 Kundengruppe(ID kgid, const std::string _obg,const std::string _grpnm,
 	const std::string komm="");
 ID Id() const {return ID(entityid); }
 std::string GrpName() const { return grpname;}
 std::string Obergruppe() const { return obergruppe; }
 std::string Kommentar() const { return kommentar; } 
 
 friend FetchIStream &operator>>(FetchIStream &is, Kundengruppe &kg);
 friend FetchIStream &operator>>(FetchIStream &is, Kundengruppe::ID &kgid); 

};

class cH_Kundengruppe : public Handle<const Kundengruppe>
{	
        cH_Kundengruppe(const Kundengruppe *p) : Handle<const Kundengruppe>(p) {}	

public:
	typedef Kundengruppe::ID ID;
	cH_Kundengruppe(ID nr, const std::string _obg,
		const std::string grpname, const std::string komm);
        bool operator==(const cH_Kundengruppe& b) const
                {return (*this)->Id()==b->Id();} 
        bool operator<(const cH_Kundengruppe& b) const
                {return (*this)->Id()<b->Id();} 
        cH_Kundengruppe();
};




FetchIStream &operator>>(FetchIStream &is,cH_Kundengruppe &kg);
    
#endif


