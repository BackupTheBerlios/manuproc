#ifndef _CLASS_DATA_LAGER_HH
#  define _CLASS_DATA_LAGER_HH

#include <Misc/EntryValueEmptyInt.h>
#include <Misc/EntryValueIntString.h>
#include <Misc/EntryValueDatum.h>
#include <Misc/EntryValueFixedEinheit.h>
#include <Misc/itos.h>
#include <Misc/Datum.h>
#include <Misc/Ausgabe_neu.h>
#include <Auftrag/AufEintragZu.h>
#include <tclistnode.h>
//#include <Auftrag/AufEintrag.h>
#include <Artikel/Einheiten.h>

//////////////////////////////////////////////////////////////////////
class Data_Lager : public RowDataBase
{
   const AufEintrag AE ;
   const auftrag_main *AM ;
   bool subleaf;

public:
   Data_Lager(const AufEintrag &ae,const auftrag_main* am,bool sl=false)
       :AE(ae),AM(am),subleaf(sl) {}
   enum SPALTEN{ARTIKEL,MENGE,LIEFERDATUM};

 virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case ARTIKEL : {
         cH_ArtikelBezeichnung AZ(AE.Artikel());
         return cH_EntryValueIntString(AZ->Bezeichnung());
        }
      case MENGE  : {
          if(subleaf) return cH_EntryValueIntString("");
          return cH_EntryValueFixedEinheit<>(AE.getStueck(),Einheit(AE.Artikel()).Bezeichnung());
        }
      case LIEFERDATUM : {
         if(!AE.valid()) return cH_EntryValue();
         std::string lw;
         try{
         if (AM->Zeit_kw_bool())
            return cH_EntryValueKalenderwoche(AE.getLieferdatum().KW());
         else   
            return cH_EntryValueDatum(AE.getLieferdatum());
         }catch(ManuProC::Datumsfehler &e) {return cH_EntryValueIntString("Falsches Lieferdatum in Auftrag:"+itos(AE.Id())+" ZNr."+itos(AE.ZNr()));}
       }
     }
   return cH_EntryValueIntString("?x?");
 }

  AufEintrag &getAufEintrag() const {return const_cast<AufEintrag&>(AE);}
};

class cH_Data_Lager : public Handle<const Data_Lager>
{
protected:
 cH_Data_Lager() {}
public:
 cH_Data_Lager(const cH_RowDataBase &rd)
     : Handle<const Data_Lager>(dynamic_cast<const Data_Lager *>(&*rd)) {}
 cH_Data_Lager(Data_Lager *r) : Handle<const Data_Lager>(r){}
};

#endif
