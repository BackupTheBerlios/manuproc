#ifndef _CLASS_INSTANZZ_HH
#  define _CLASS_INSTANZZ_HH

#include <Aux/EntryValueEmptyInt.h>
#include <Aux/EntryValueIntString.h>
#include <Aux/itos.h>
#include <Aux/Datum.h>
#include <Auftrag/AufEintragZu.h>
//#include <Auftrag/AufEintrag.h>

//////////////////////////////////////////////////////////////////////

class Data_neuer_auftrag : public RowDataBase
{
   const auftrag_main *AM;
   AufEintrag aufeintrag;

public:
   Data_neuer_auftrag(auftrag_main* _AM,
      const AufEintrag& selected_AufEintrag)
      : AM(_AM),aufeintrag(selected_AufEintrag) {};

   enum SPALTEN{KUNDE,ARTIKEL,MENGE,DATUM};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case KUNDE : {//return cH_EntryValueIntString(aufeintrag.get_Kunde()->firma());
         std::string k;
         std::list<cH_Kunde> LK=AufEintragZu(aufeintrag).get_Referenz_Kunden();
         if(LK.size()==1)
          {
            if (AM->Kunden_nr_bool()) return cH_EntryValueIntString((*LK.begin())->Id());
            else return cH_EntryValueIntString((*LK.begin())->firma());
          }
         for (std::list<cH_Kunde>::const_iterator i=LK.begin();i!=LK.end();)
           { k+=(*i)->firma();
             if (++i != LK.end()) k+=", ";
           }
         return cH_EntryValueIntString(k);
        }
      case ARTIKEL : {
         cH_ArtikelBezeichnung AZ(aufeintrag.ArtId());
         return cH_EntryValueIntString(AZ->Bezeichnung());
        }
      case MENGE   : return cH_EntryValueIntString(aufeintrag.getStueck().as_int());
      case DATUM   : {
         std::string lw;
         if (AM->Zeit_kw_bool())
           { int lieferwoche = aufeintrag.getLieferdatum().KW().Woche();
             int lieferjahr =  aufeintrag.getLieferdatum().KW().Jahr();
             std::string lj=itos (lieferjahr).substr(2,2);
             lw = itos(lieferwoche)+"/"+lj;
           }
         else lw =  aufeintrag.getLieferdatum().c_str();
         return cH_EntryValueIntString(lw); 
        }
     }
   return cH_EntryValueIntString("?");
 }
 const AufEintrag& get_AufEintrag() const {return aufeintrag;}
 AufEintrag& get_AufEintrag()  {return aufeintrag;}
};

class cH_Data_neuer_auftrag : public Handle<const Data_neuer_auftrag>
{
protected:
 cH_Data_neuer_auftrag() {}
public:
 cH_Data_neuer_auftrag(Data_neuer_auftrag *r) : Handle<const Data_neuer_auftrag>(r){}
};

#endif