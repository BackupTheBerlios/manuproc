#ifndef _CLASS_PREISL_HH
#  define _CLASS_PREISL_HH

#include <Aux/EntryValueIntString.h>
#include <Kunde/PreisListe.h>
#include <Kunde/Kundengruppe.h>

class Data_SPreis : public RowDataBase
{
   int sortierung;
   cH_PreisListe PL;
 public:
   Data_SPreis(int s,cH_PreisListe pl) : sortierung(s),PL(pl) {}

   enum Spalten  {SUCH,NOTIZ,ID};
   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
     {
       switch((Spalten)seqnr) 
        {
          case ID    : return cH_EntryValueIntString(PL->Id());
          case NOTIZ : return cH_EntryValueIntString(PL->Name());
          case SUCH  : return cH_EntryValueIntString(sortierung);
        }
      return cH_EntryValueIntString();
     }
   cH_PreisListe getPreisListe() const {return PL;}
};


class cH_Data_SPreis : public Handle<Data_SPreis>
{
public:
 cH_Data_SPreis(Data_SPreis *r) : Handle<Data_SPreis>(r) {}
};


class Data_Gruppe : public RowDataBase
{
 cH_Kundengruppe kg;

 public:
   Data_Gruppe(int gid,const std::string obg, const std::string bez, 
		const std::string komm) 
		: kg((Kundengruppe::ID)gid,obg,bez,komm) {}
   Kundengruppe::ID GrpId() const { return kg->Id(); }
   Data_Gruppe(cH_Kundengruppe _kg) : kg(_kg) {}

   enum Spalten  {GRP_ID=0,GRP_OBID,GRP_BEZ,GRP_KOMM};
   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
     {
       switch((Spalten)seqnr) 
        {
          case GRP_ID	: return cH_EntryValueIntString(kg->Id());
          case GRP_OBID	: return cH_EntryValueIntString(kg->Obergruppe());
          case GRP_BEZ	: return cH_EntryValueIntString(kg->GrpName());
          case GRP_KOMM	: return cH_EntryValueIntString(kg->Kommentar());
	  default : return cH_EntryValueIntString();
        }
      return cH_EntryValueIntString();
     }
};


class cH_Data_Gruppe : public Handle<Data_Gruppe>
{
public:
 cH_Data_Gruppe(Data_Gruppe *r) : Handle<Data_Gruppe>(r) {}
};



#endif
