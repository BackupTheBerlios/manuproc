#include "windowTop.hh"
#include "kundendaten_aux.hh"

struct KndGrpIN {
	cH_Kundengruppe kg;
	Kundengruppe::ID gewaehlt;
	};
	
FetchIStream &operator>>(FetchIStream &is, struct KndGrpIN &kgi)
{
 int gew;
 is >> kgi.kg >> FetchIStream::MapNull(gew,-1);
 kgi.gewaehlt=(Kundengruppe::ID)gew;
 return is;
}

void windowTop::showGruppen()
{
 if(kundendaten->Id()==Kunde::none_id) return;
 
 std::string qu="select grpnr,obergruppe,name,kommentar,gewaehlt.grpnr"
 	 " from ku_gruppe g left join (select m.grpnr "
	 " from ku_gruppen_map m where kundennr=?) as gewaehlt "
	 " using (grpnr)";
 	 
 std::vector<struct KndGrpIN> kgr;
 
 (Query(qu) << kundendaten->Id()).FetchArray(kgr);

 std::vector<cH_RowDataBase> grp_in;
 std::vector<cH_RowDataBase> grp_notin; 
 
 for(std::vector<struct KndGrpIN>::const_iterator i=kgr.begin();
 	i!=kgr.end(); ++i)
   {
    if((*i).gewaehlt==Kundengruppe::none_id)
      grp_notin.push_back(new Data_Gruppe((*i).kg));
    else
      grp_in.push_back(new Data_Gruppe((*i).kg));    
   }
  alle_gruppen->setDataVec(grp_notin);
  gewaehlte_gruppen->setDataVec(grp_in);
}


