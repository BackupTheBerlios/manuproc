// diese Klassen sollten _nur_ von auftrag_lieferschein_classes_sql.pgcc 
// verwendet werden, also ist diese H File Unsinn

#ifndef LIEFERSCHEINTREEDATA
#define LIEFERSCHEINTREEDATA
#include<Aux/EntryValueIntString.h>
#include<Aux/EntryValueEmptyInt.h>
#include<Aux/EntryValueDatum.h>
#include<Aux/Datum.h>
#include<Aux/Ausgabe_neu.h>
#include<Lieferschein/LieferscheinEntry.h>
#include<Lieferschein/Lieferschein.h>
#include<rowdata.h>
#include<Artikel/ArtikelBezeichnung.h>
#include<Artikel/Einheiten.h>
#include<Auftrag/AufEintrag.h>

#include "auftrag_main.hh"

class Data_Lieferdaten : public RowDataBase
{
      LieferscheinEntry liefentry;
  public:
   Data_Lieferdaten(LieferscheinEntry _liefentry)
      :liefentry(_liefentry) {}

   enum SeqNr {LIEFZEILE_SEQ,ARTIKEL_SEQ,AUFNR_SEQ,PALETTE_SEQ,LIEFMNG_SEQ};

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
     switch(seqnr)
      {
      case AUFNR_SEQ :
         {  int aufid=liefentry.AufId();
            if (aufid)
          return cH_EntryValueIntString(Formatiere(aufid,0,6,"","",'0'));
            else return cH_EntryValueIntString("");
         }
      case ARTIKEL_SEQ :
         {
           if (liefentry.ZusatzInfo()) return cH_EntryValueIntString("");
           cH_ArtikelBezeichnung AB(liefentry.ArtikelID());
           return cH_EntryValueIntString(AB->Bezeichnung());
         }
      case PALETTE_SEQ :
         {
           if (liefentry.ZusatzInfo()) return cH_EntryValueIntString("");
           return cH_EntryValueEmptyInt(liefentry.Palette());
         }
      case LIEFMNG_SEQ :
        {  
           int stueck = liefentry.Stueck();
           fixedpoint<3> menge = liefentry.Menge();
           std::string a;
           if (stueck!=1)
             {  a=Formatiere(stueck)
                 + Einheit(ArtikelBase(liefentry.ArtikelID())).StueckEinheit();
             }
           if (menge.Scaled()!=0)
             {  if (stueck!=1) a+="*";
                a+=Formatiere_short(menge)
                 + Einheit(ArtikelBase(liefentry.ArtikelID())).MengenEinheit();
             }
           if (stueck==1 && menge.Scaled()==0)
               a=Formatiere(stueck)+Einheit(ArtikelBase(liefentry.ArtikelID())).MengenEinheit();
           if (liefentry.ZusatzInfo()) a="("+a+")";
             return cH_EntryValueIntString(a);
         }
      case LIEFZEILE_SEQ :
         return cH_EntryValueIntString(liefentry.Zeile());
      default : return cH_EntryValue();
     }
   }

  int get_Lieferschein_Id() const {return liefentry.Id();}
  LieferscheinEntry get_LieferscheinEntry() const {return liefentry;}
  AufEintragBase getAufEintragBase() const {return liefentry.getAufEintragBase();}
};

class cH_Data_Lieferdaten : public Handle<const Data_Lieferdaten>
{
 protected:
   cH_Data_Lieferdaten() {}
 public:
   cH_Data_Lieferdaten(const Data_Lieferdaten *r) : Handle<const Data_Lieferdaten>(r){}
};

/*
class Data_Lieferdaten_Node : public TreeRow
{
 
};
*/


class Data_Lieferoffen : public RowDataBase
{
      AufEintrag AE;
      const auftrag_main *AM ;

  public:
   Data_Lieferoffen(const AufEintrag& ae,const auftrag_main* am) 
      : AE(ae),AM(am) {}
   enum SeqNr {AUFNR_SEQ=0,ARTIKEL_SEQ,LIEFDAT_SEQ,OFFMNG_SEQ,GELIEF_SEQ,};
   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
       switch(seqnr)
        {
         case AUFNR_SEQ :
            return cH_EntryValueIntString(Formatiere(AE.Id(),0,6,"","",'0'));
         case ARTIKEL_SEQ :
            return cH_EntryValueIntString(cH_ArtikelBezeichnung(AE.Artikel())->Bezeichnung());
         case LIEFDAT_SEQ :
           {
             if (AM->Zeit_kw_bool())
               return cH_EntryValueKalenderwoche(AE.getLieferdatum().KW());
             else   
               return cH_EntryValueDatum(AE.getLieferdatum());
           }
         case OFFMNG_SEQ :
            return cH_EntryValueIntString(AE.getRestStk());
         case GELIEF_SEQ :
            return cH_EntryValueIntString(AE.getGeliefert());
         default : return cH_EntryValueIntString();
        }
    }
   const AufEintrag &getAufEintrag() const { return AE; }
   void abschreiben(AuftragBase::mengen_t menge) 
        {AE.abschreiben(menge,ManuProcEntity::none_id); }
};

class cH_Data_Lieferoffen : public Handle<const Data_Lieferoffen>
{
 protected:
   cH_Data_Lieferoffen() {}
 public:
   cH_Data_Lieferoffen(const Data_Lieferoffen *r) : Handle<const Data_Lieferoffen>(r){}
};


#endif
