#ifndef _CLASS_DATA_LAGER_HH
#  define _CLASS_DATA_LAGER_HH

#include <Aux/EntryValueEmptyInt.h>
#include <Aux/EntryValueIntString.h>
#include <Aux/EntryValueDatum.h>
#include <Aux/itos.h>
#include <Aux/Datum.h>
#include <Aux/Ausgabe_neu.h>
#include <Auftrag/AufEintragZu.h>
#include <tclistnode.h>
//#include <Auftrag/AufEintrag.h>

//////////////////////////////////////////////////////////////////////
class Data_Lager : public RowDataBase
{
//   LagerInhalt LI;
   const AufEintrag AE ;
//   AuftragBase::mengen_t verplant,bestellt;
//   AuftragBase::mengen_t menge;
   const auftrag_main *AM ;
   bool subleaf;

public:
/*
   Data_Lager(const LagerInhalt& li,const AufEintrag &ae,
              AuftragBase::mengen_t v,AuftragBase::mengen_t b,
              const auftrag_main* am,bool sl=false)
       :LI(li),AE(ae),verplant(v),bestellt(b),AM(am),subleaf(sl) {}
*/
   Data_Lager(const AufEintrag &ae,const auftrag_main* am,bool sl=false)
       :AE(ae),AM(am),subleaf(sl) {}
//   enum SPALTEN{ARTIKEL,LIEFERDATUM,STUECK,MENGE,VERPLANT,BESTELLT,FREI};
   enum SPALTEN{ARTIKEL,MENGE,LIEFERDATUM};

 virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case ARTIKEL : {
         cH_ArtikelBezeichnung AZ(AE.Artikel());
         return cH_EntryValueIntString(AZ->Bezeichnung());
        }
/*
      case STUECK  : {
         if(subleaf) return cH_EntryValueIntString("");
         else if(LI.RestStueck())
            return cH_EntryValueIntString(FormatiereEmptyInt(LI.Stueck())+" (+"+FormatiereEmptyInt(LI.RestStueck())+")");
         else 
            return cH_EntryValueIntString(FormatiereEmptyInt(LI.Stueck()));
        }
*/ 
      case MENGE  : {
          if(subleaf) return cH_EntryValueIntString("");
//          return cH_EntryValueIntString(FormatiereEmptyInt(LI.GesamtMenge()));
          return cH_EntryValueIntString(FormatiereEmptyInt(AE.getStueck()));
        }
/*
      case VERPLANT : return cH_EntryValueIntString(FormatiereEmptyInt(verplant));
      case BESTELLT : return cH_EntryValueIntString(FormatiereEmptyInt(bestellt));
      case FREI     : 
          if(subleaf) return cH_EntryValueIntString("");
          return cH_EntryValueIntString(FormatiereEmptyInt(LI.GesamtMenge()-verplant));
*/
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

//  AufEintrag &get_KundenAufEintrag() const {return const_cast<AufEintrag&>(AE);}
  AufEintrag &getAufEintrag() const {return const_cast<AufEintrag&>(AE);}
//  int Stueck() const {return LI.Stueck();}  
//  int RestStueck() const {return LI.RestStueck();}  
//  int GesamtMenge() const {return LI.GesamtMenge();}  
//  int Verplant() const {return verplant;}
//  int Bestellt() const {return bestellt;}
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

/*
class Data_Lager_Node : public TreeRow
{
     int stueck,reststueck,gesamtmenge;
     AuftragBase::mengen_t bestellt,verplant,frei;
   public:
     virtual void cumulate(const cH_RowDataBase &rd)
      {
        cH_Data_Lager(rd)->Stueck();
        stueck=cH_Data_Lager(rd)->Stueck();   
        reststueck=cH_Data_Lager(rd)->RestStueck();   
        gesamtmenge=cH_Data_Lager(rd)->GesamtMenge();
        bestellt+=cH_Data_Lager(rd)->Bestellt();
        verplant+=cH_Data_Lager(rd)->Verplant();
        frei=gesamtmenge-verplant;
      }          
      
     const cH_EntryValue Value(guint index,gpointer gp) const
      {
        switch(index)
         {
            case Data_Lager::STUECK  : {
               if(reststueck)
                  return cH_EntryValueIntString(FormatiereEmptyInt(stueck)+" (+"+FormatiereEmptyInt(reststueck)+")");
               else 
                  return cH_EntryValueIntString(FormatiereEmptyInt(stueck));
              }
            case Data_Lager::MENGE  : return cH_EntryValueIntString(FormatiereEmptyInt(gesamtmenge));
            case Data_Lager::VERPLANT : return cH_EntryValueIntString(FormatiereEmptyInt(verplant));
            case Data_Lager::BESTELLT : return cH_EntryValueIntString(FormatiereEmptyInt(bestellt));
            case Data_Lager::FREI : return cH_EntryValueIntString(FormatiereEmptyInt(frei));
         }
       return cH_EntryValueIntString("?y?");
      }
 Data_Lager_Node(guint col, const cH_EntryValue &v, guint child_s_deep, 
 	cH_RowDataBase child_s_data, bool expand, const TreeRow &suminit)
    : TCListNode(col, v, child_s_deep, child_s_data, expand), 
       stueck(0),reststueck(0),gesamtmenge(0),frei(0) 
       {  if (suminit.Leaf()) cumulate(child_s_data);
           else 
           { stueck=dynamic_cast<const Data_Lager_Node&>(suminit).stueck;
             reststueck=dynamic_cast<const Data_Lager_Node&>(suminit).reststueck;
             gesamtmenge=dynamic_cast<const Data_Lager_Node&>(suminit).gesamtmenge;
             bestellt=dynamic_cast<const Data_Lager_Node&>(suminit).bestellt;
             verplant=dynamic_cast<const Data_Lager_Node&>(suminit).verplant;
             frei=dynamic_cast<const Data_Lager_Node&>(suminit).frei;
           }
       }
 static TCListNode *create(guint col, const cH_EntryValue &v, 
 	guint child_s_deep, cH_RowDataBase child_s_data, bool expand, const TreeRow &suminit)
    {  return new Data_Lager_Node(col,v, child_s_deep, child_s_data, expand, suminit); }
         
};

*/

#endif
