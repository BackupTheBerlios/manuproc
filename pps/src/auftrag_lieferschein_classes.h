#ifndef LIEFERSCHEINTREEDATA
#define LIEFERSCHEINTREEDATA
#include<Aux/EntryValueIntString.h>
#include<Aux/EntryValueDatum.h>
#include<Aux/Datum.h>
#include<Aux/Ausgabe_neu.h>
#include<Lieferschein/LieferscheinEntry.h>
#include<Lieferschein/Lieferschein.h>
#include<rowdata.h>
#include<Artikel/ArtikelBezeichnung.h>
#include<Artikel/Einheiten.h>
#include<Auftrag/AufEintragBase.h>

class Data_Lieferdaten : public RowDataBase
{
      LieferscheinEntry liefentry;
  public:
   Data_Lieferdaten(LieferscheinEntry _liefentry)
      :liefentry(_liefentry) {}

   enum SeqNr {LIEFZEILE_SEQ,ARTIKEL_SEQ,AUFNR_SEQ,LIEFMNG_SEQ};

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
      case LIEFMNG_SEQ :
              {  //int stueck(GeliefertS());
                 int stueck = liefentry.Stueck();
//                 fixedpoint<3> menge(GeliefertM());
                 fixedpoint<3> menge = liefentry.Menge();
                      std::string a;
                      if (stueck!=1)
                      {  a=Formatiere(stueck)
                        + Einheit(liefentry.ArtikelID()).StueckEinheit();
                      }
                      if (menge.Scaled()!=0)
                      {  if (stueck!=1) a+="*";
                         a+=Formatiere_short(menge)
                     + Einheit(liefentry.ArtikelID()).MengenEinheit();
                      }
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
};

class cH_Data_Lieferdaten : public Handle<const Data_Lieferdaten>
{
 protected:
   cH_Data_Lieferdaten() {}
 public:
   cH_Data_Lieferdaten(const Data_Lieferdaten *r) : Handle<const Data_Lieferdaten>(r){}
};

/*
class Data_Lieferdaten_Node : public TCListNode
{
 
};
*/


class Data_Lieferoffen : public RowDataBase
{
      AufEintragBase auftrag;
      cH_ArtikelBezeichnung artbez;
      int offen;
      int geliefert;
      Petig::Datum lieferdatum;
      
  public:
   Data_Lieferoffen(AufEintragBase& _auftrag, cH_ArtikelBezeichnung _artbez,
         int _offen,int& _geliefert,Petig::Datum _lieferdatum    )
      :auftrag(_auftrag),artbez(_artbez),offen(_offen),geliefert(_geliefert),
        lieferdatum(_lieferdatum) {}

   enum SeqNr {AUFNR_SEQ=0,ARTIKEL_SEQ,LIEFDAT_SEQ,OFFMNG_SEQ,GELIEF_SEQ,};

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
       switch(seqnr)
        {
         case AUFNR_SEQ :
            return cH_EntryValueIntString(Formatiere(auftrag.AufId(),0,6,"","",'0'));
         case ARTIKEL_SEQ :
              return cH_EntryValueIntString(artbez->Bezeichnung());
         case LIEFDAT_SEQ :
            return cH_EntryValueDatum(lieferdatum);
         default : return cH_EntryValueIntString();
        }
    }

   int get_Artikel_Id() const {return artbez->getArtid();}
   int Offen() const { return offen;}
   int get_Auftrag_Id() const { return auftrag.AufId(); }
   const AufEintragBase2 &AuftragEntry() const { return auftrag; }
   void abschreiben(int menge);
//   static void abschreiben(const AufEintragBase2 &auf,int menge) throw(SQLerror);   
};

class cH_Data_Lieferoffen : public Handle<const Data_Lieferoffen>
{
 protected:
   cH_Data_Lieferoffen() {}
 public:
   cH_Data_Lieferoffen(const Data_Lieferoffen *r) : Handle<const Data_Lieferoffen>(r){}
};


#endif
