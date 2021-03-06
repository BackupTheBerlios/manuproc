// diese Klassen sollten _nur_ von auftrag_lieferschein_classes_sql.pgcc 
// verwendet werden, also ist diese H File Unsinn

#ifndef LIEFERSCHEINTREEDATA
#define LIEFERSCHEINTREEDATA
#include <Misc/EntryValueIntString.h>
#include <Misc/EntryValueEmptyInt.h>
#include <Misc/EntryValueDatum.h>
#include <Misc/Datum.h>
#include <Misc/Ausgabe_neu.h>
#include <Lieferschein/LieferscheinEntry.h>
#include <Lieferschein/Lieferschein.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Artikel/Einheiten.h>
#include <Auftrag/AufEintrag.h>
#include <unistd.h>
#include <Lager/Lager.h>
#include <functional>

#include "auftrag_main.hh"

class Data_Lieferdaten : public RowDataBase
{
      LieferscheinEntry liefentry;
      bool zusatzinfo;
      std::string zeile;
      AufEintragBase AEB;
      std::string smenge;
      std::string lagerbez;
      std::string FormatiereMenge(ArtikelBase artikel, int stueck, LieferscheinBase::mengen_t menge);
      std::string FormatiereMenge(ArtikelBase artikel, int stueck, AuftragBase::mengen_t menge);
  public:
   Data_Lieferdaten(LieferscheinEntry _liefentry)
      : liefentry(_liefentry), zusatzinfo(false) 
      {  zeile=itos(liefentry.Zeile());
         smenge=FormatiereMenge(liefentry.Artikel(),liefentry.Stueck(),liefentry.Menge());
         if (liefentry.getZusatzInfos().size()<2) AEB=liefentry.getZusatzInfos()[0].aeb;
      }
   Data_Lieferdaten(std::string z,LieferscheinEntry _liefentry,
                    AufEintragBase aeb,AuftragBase::mengen_t m)
      : liefentry(_liefentry), zusatzinfo(true),zeile(z),AEB(aeb)
       {  
         smenge="("+FormatiereMenge(liefentry.Artikel(),1,m)+")";
       }

   enum SeqNr {LIEFZEILE_SEQ,ARTIKEL_SEQ,AUFNR_SEQ,PALETTE_SEQ,
   		LIEFMNG_SEQ,PROZ_BEST,VOMLAGER_SEQ,TEXT};

   virtual cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
     switch(seqnr)
      {
      case AUFNR_SEQ :
         { std::string auftr=liefentry.getRefOrder();
           if (!auftr.empty()) auftr+="/";
           if (AEB.valid())
             return cH_EntryValueIntString(auftr+AuftragBase::ID2string(AEB.Id()));
           else return cH_EntryValueIntString(auftr);
         }
      case ARTIKEL_SEQ :   return cH_EntryValueIntString(zusatzinfo?std::string()
      		:cH_ArtikelBezeichnung(liefentry.Artikel())->Bezeichnung());
      case PALETTE_SEQ :   return zusatzinfo?cH_EntryValue()
      		:cH_EntryValueEmptyInt(liefentry.Palette());
      case LIEFMNG_SEQ :   return cH_EntryValueIntString(smenge);
      case LIEFZEILE_SEQ : return cH_EntryValueIntString(zeile);
      case VOMLAGER_SEQ : 
      	if((liefentry.Status()==(AufStatVal)UNCOMMITED) ||
      	   (liefentry.lagerid==FertigWarenLager::none_lagerid))
      	  return cH_EntryValueIntString(std::string(" UNGEBUCHT :")+
      	  					itos(liefentry.lagerid));
      	else
      	  return cH_EntryValueIntString(liefentry.lagerid);
      case PROZ_BEST:
        {  fixedpoint<1> genutzt=liefentry.DurchAuftraegeAbgedeckt();
           if (genutzt<1) return cH_EntryValueIntString(genutzt.String()+"%");
           return cH_EntryValue();
        }
      case TEXT: return cH_EntryValueIntString(liefentry.Text());
      default : return cH_EntryValue();
     }
   }

  int get_Lieferschein_Id() const {return liefentry.Id();}
  const LieferscheinEntry &get_LieferscheinEntry() const {return liefentry;}
  bool istZusatzinfo() const {return zusatzinfo;}
  const AufEintragBase &getAufEintragBase() const { return AEB; }
  int getLiefMenge() const { return liefentry.Stueck(); }
};

class Lief_ref_Auftrag : public std::unary_function<AufEintragBase,bool>
{
 AufEintragBase aeb;

public:
 explicit Lief_ref_Auftrag(const AufEintragBase _aeb):aeb(_aeb) {}
 bool operator() (cH_RowDataBase l) const 
   {
    Handle<const Data_Lieferdaten> h_ld=
    	l.cast_dynamic<const Data_Lieferdaten>();
    if(aeb.Id() != h_ld->getAufEintragBase().Id()) return false;
    if(aeb.ZNr() != h_ld->getAufEintragBase().ZNr()) return false;
    return true;
   }
};



class LiefHasStatus : public std::unary_function<LieferscheinEntry,bool>
{
 AufStatVal status;

public:
 explicit LiefHasStatus(const AufStatVal st):status(st) {}
 bool operator() (cH_RowDataBase l) const 
   {
    Handle<const Data_Lieferdaten> h_ld=
    	l.cast_dynamic<const Data_Lieferdaten>();
    if(status == h_ld->get_LieferscheinEntry().Status())
      {if(status!=(AufStatVal)UNCOMMITED) return true;
       if(h_ld->get_LieferscheinEntry().lagerid==ManuProcEntity<>::none_id)
         return true;
      }
    return false;
   }
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
      bool valid;
public:
   Data_Lieferoffen(const AufEintrag& ae,const auftrag_main* am) 
      : AE(ae),AM(am),valid(true) {}
   Data_Lieferoffen() 
      : AM(NULL),valid(false) {}
      
   enum SeqNr {AUFNR_SEQ=0,ARTIKEL_SEQ,LIEFDAT_SEQ,OFFMNG_SEQ,
   		GELIEF_SEQ,IMLAGER_SEQ};
   virtual cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
       switch(seqnr)
        {
         case AUFNR_SEQ :
             {std::string artnr;
              artnr=AuftragBase::ID2string(AE.Id());
#ifdef MABELLA_EXTENSIONS              
              if(AM->get_Instanz()->Id()==ppsInstanzID::Einkauf &&
                     !AE.getYourAufNr().empty())
                    artnr=AE.getYourAufNr()+" - "+artnr;
#endif                    
              return cH_EntryValueIntString(artnr);
              }
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
         case IMLAGER_SEQ : 
            return cH_EntryValueIntString(AE.getAmLager().as_int());
         default : return cH_EntryValueIntString();
        }
    }
   const AufEintrag &getAufEintrag() const { return AE; }
   const ArtikelBase getArtikel() const { return AE.Artikel(); }   
   bool Valid() const { return valid; }
   void abschreiben(AuftragBase::mengen_t menge) 
//#ifdef MABELLA_EXTENSIONS
//        {AE.abschreiben(menge,ManuProcEntity::none_id); }   
//#else        
      { 
//          Kunde::ID kunde=Auftrag(AE).getKundennr();
//          ManuProC::st_produziert sp(kunde,AE,menge,getuid());
//          AE.Instanz()->Produziert(sp);
         AE.ProduziertNG(menge,LieferscheinEntryBase());
      
       //Produziert(AE,menge,getuid(),ManuProcEntity::none_id).NichtSelbst();
      }
//#endif      

};

class Auftrag_ref_Lief : public std::unary_function<AufEintragBase,bool>
{
 AufEintragBase aeb;

public:
 explicit Auftrag_ref_Lief(const AufEintragBase _aeb):aeb(_aeb) {}
 bool operator() (cH_RowDataBase l) const 
   {
    Handle<const Data_Lieferoffen> h_ld=
    	l.cast_dynamic<const Data_Lieferoffen>();
    if(aeb.Id() != h_ld->getAufEintrag().Id()) return false;
    if(aeb.ZNr() != h_ld->getAufEintrag().ZNr()) return false;
    return true;
   }
};


class cH_Data_Lieferoffen : public Handle<const Data_Lieferoffen>
{
 protected:
   cH_Data_Lieferoffen() {}
 public:
   cH_Data_Lieferoffen(const Data_Lieferoffen *r) : Handle<const Data_Lieferoffen>(r){}
};


#endif
