#ifndef _CLASS_INSTANZZ_HH
#  define _CLASS_INSTANZZ_HH

#include <Aux/EntryValueEmptyInt.h>
#include <Aux/EntryValueIntString.h>
#include <Aux/itos.h>
#include <Aux/Datum.h>

//////////////////////////////////////////////////////////////////////

class Data_neuer_auftrag : public RowDataBase
{
   const auftrag_main *AM;
   AufEintragBase AufEintrag;

public:
   Data_neuer_auftrag(auftrag_main* _AM,
      const AufEintragBase& selected_AufEintrag)
      : AM(_AM),AufEintrag(selected_AufEintrag) {};

   enum SPALTEN{KUNDE,ARTIKEL,MENGE,DATUM};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case KUNDE : {//return cH_EntryValueIntString(AufEintrag.get_Kunde()->firma());
         std::string k;
         list<cH_Kunde> LK=AufEintrag.get_Referenz_Kunden();
//cout << "Kundenliste " <<LK.size()<<'\n';
         for (list<cH_Kunde>::const_iterator i=LK.begin();i!=LK.end();)
           { k+=(*i)->firma();
             if (++i != LK.end()) k+=", ";
           }
//         return cH_EntryValueIntString(AB.get_Kunde()->firma());
         return cH_EntryValueIntString(k);
        }
      case ARTIKEL : {
         cH_ArtikelBezeichnung AZ(AufEintrag.ArtId());
         return cH_EntryValueIntString(AZ->Bezeichnung());
        }
      case MENGE   : return cH_EntryValueIntString(itos(AufEintrag.getStueck()));
      case DATUM   : {
         std::string lw;
         if (AM->Zeit_kw_bool())
           { int lieferwoche = AufEintrag.getLieferdatum().KW().Woche();
             int lieferjahr =  AufEintrag.getLieferdatum().KW().Jahr();
             string lj=itos (lieferjahr).substr(2,2);
             lw = itos(lieferwoche)+"/"+lj;
           }
         else lw =  AufEintrag.getLieferdatum().c_str();
         return cH_EntryValueIntString(lw); 
        }
     }
   return cH_EntryValueIntString("?");
 }
 const AufEintragBase& get_AufEintragBase() const {return AufEintrag;}
 AufEintragBase& get_AufEintragBase()  {return AufEintrag;}
};

class cH_Data_neuer_auftrag : public Handle<const Data_neuer_auftrag>
{
protected:
 cH_Data_neuer_auftrag() {}
public:
 cH_Data_neuer_auftrag(Data_neuer_auftrag *r) : Handle<const Data_neuer_auftrag>(r){}
};

#endif