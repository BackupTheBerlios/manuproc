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
#include <Instanzen/ppsInstanzProduziert.h>
#include <unistd.h>


#include "auftrag_main.hh"

class Data_Lieferdaten : public RowDataBase
{
      LieferscheinEntry liefentry;
      bool zusatzinfo;
      std::string zeile;
      AufEintragBase AEB;
      std::string smenge;
      int palette;
      std::string artikel;
      std::string FormatiereMenge(ArtikelBase artikel, int stueck, LieferscheinBase::mengen_t menge);
      std::string FormatiereMenge(ArtikelBase artikel, int stueck, AuftragBase::mengen_t menge);
  public:
   Data_Lieferdaten(LieferscheinEntry _liefentry)
      : liefentry(_liefentry), zusatzinfo(false) 
      { zeile=itos(liefentry.Zeile());
        AEB=liefentry.getAufEintragBase();
        palette=liefentry.Palette();
        artikel=cH_ArtikelBezeichnung(liefentry.Artikel())->Bezeichnung();
        smenge=FormatiereMenge(liefentry.Artikel(),liefentry.Stueck(),liefentry.Menge());
      }
   Data_Lieferdaten(std::string z,LieferscheinEntry _liefentry,
                    AufEintragBase aeb,AuftragBase::mengen_t m)
      : liefentry(_liefentry), zusatzinfo(true),zeile(z),AEB(aeb)
       {  
         smenge="("+FormatiereMenge(liefentry.Artikel(),1,m)+")";
         palette=0;
       }

   enum SeqNr {LIEFZEILE_SEQ,ARTIKEL_SEQ,AUFNR_SEQ,PALETTE_SEQ,LIEFMNG_SEQ};

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
     switch(seqnr)
      {
      case AUFNR_SEQ :
         { if (AEB.valid())
           return cH_EntryValueIntString(Formatiere(AEB.Id(),0,6,"","",'0'));
           else return cH_EntryValueIntString("");
         }
      case ARTIKEL_SEQ :   return cH_EntryValueIntString(artikel);
      case PALETTE_SEQ :   return cH_EntryValueEmptyInt(palette);
      case LIEFMNG_SEQ :   return cH_EntryValueIntString(smenge);
      case LIEFZEILE_SEQ : return cH_EntryValueIntString(zeile);
      default : return cH_EntryValue();
     }
   }

  int get_Lieferschein_Id() const {return liefentry.Id();}
  LieferscheinEntry get_LieferscheinEntry() const {return liefentry;}
  AufEintragBase getAufEintragBase() const {return liefentry.getAufEintragBase();}
  bool istZusatzinfo() const {return zusatzinfo;}
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
            return cH_EntryValueIntString(AE.getRestStk().as_int());
         case GELIEF_SEQ :
            return cH_EntryValueIntString(AE.getGeliefert().as_int());
         default : return cH_EntryValueIntString();
        }
    }
   const AufEintrag &getAufEintrag() const { return AE; }
   void abschreiben(AuftragBase::mengen_t menge) 
//#ifdef MABELLA_EXTENSIONS
//        {AE.abschreiben(menge,ManuProcEntity::none_id); }   
//#else        
      { 
//          Kunde::ID kunde=Auftrag(AE).getKundennr();
//          ManuProC::st_produziert sp(kunde,AE,menge,getuid());
//          AE.Instanz()->Produziert(sp);
         AE.Produziert(menge,ManuProcEntity<>::none_id);
      
       //Produziert(AE,menge,getuid(),ManuProcEntity::none_id).NichtSelbst();
      }
//#endif      

};

class cH_Data_Lieferoffen : public Handle<const Data_Lieferoffen>
{
 protected:
   cH_Data_Lieferoffen() {}
 public:
   cH_Data_Lieferoffen(const Data_Lieferoffen *r) : Handle<const Data_Lieferoffen>(r){}
};


#endif
