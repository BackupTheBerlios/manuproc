#ifndef RECHNUNGTREEDATA
#define RECHNUNGTREEDATA
#include<Misc/EntryValueIntString.h>
#include<Misc/Datum.h>
#include<Misc/Ausgabe_neu.h>
#include<Artikel/ArtikelBezeichnung.h>
#include<Lieferschein/RechnungEntry.h>
#include<Lieferschein/Lieferschein.h>
#include<rowdata.h>
//#include<Artikel/ArtikelBezeichnung.h>
//#include<Artikel/Einheiten.h>

class Data_Rechnung : public RowDataBase
{
      RechnungEntry rentry;
  public:
   Data_Rechnung(RechnungEntry _rentry)
      :rentry(_rentry) {}

   enum {LIEFZEILE_SEQ,LIEFNR_SEQ,ARTIKEL_SEQ,STUECK_SEQ,LIEFMNG_SEQ,PREIS_SEQ,GPREIS_SEQ};

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
         switch(seqnr)
           {
           case LIEFNR_SEQ :
              return cH_EntryValueIntString(
              	Formatiere((unsigned long)rentry.Lfrs().Id(),0,6,"","",'0'));
           case ARTIKEL_SEQ :
              {cH_ArtikelBezeichnung AB(rentry.Artikel());
               return cH_EntryValueIntString(AB->Bezeichnung());
              }
           case STUECK_SEQ :
              return cH_EntryValueIntString(
              	FormatiereEmptyInt((unsigned long)rentry.Stueck()));
           case LIEFMNG_SEQ :
              return cH_EntryValueIntString(rentry.Menge().String());
           case LIEFZEILE_SEQ :
              return cH_EntryValueIntString(rentry.Zeile());
           case PREIS_SEQ :
              return cH_EntryValueIntString(Formatiere_short(rentry.getPreis().Wert()));
           case GPREIS_SEQ :
              return cH_EntryValueIntString(Formatiere_short(rentry.GPreis().Wert()));
           default : return cH_EntryValueIntString();
           }

    }
  int get_Lieferschein_Id() const {return rentry.Lfrs().Id();}
  RechnungEntry get_RechnungEntry() const {return rentry;}
};

class cH_Data_Rechnung : public Handle<const Data_Rechnung>
{
 protected:
   cH_Data_Rechnung() {}
 public:
   cH_Data_Rechnung(const Data_Rechnung *r) : Handle<const Data_Rechnung>(r){}
};

/*
class Data_Lieferdaten_Node : public TreeRow
{
 
};
*/


class Data_RLieferoffen : public RowDataBase
{
    cH_Lieferschein lieferschein;
  public:
   Data_RLieferoffen(cH_Lieferschein _lieferschein)
      : lieferschein(_lieferschein) {}

   enum SeqNr {KUNDE_SEQ,LIEFNR_SEQ,LIEFDATUM_SEQ,LIEFKUNDE_SEQ};

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
    {
         switch(seqnr)
           {
           case KUNDE_SEQ :
              return cH_EntryValueIntString(lieferschein->getKunde()->getName());
           case LIEFNR_SEQ :
              return cH_EntryValueIntString(
              	Formatiere((unsigned long)lieferschein->Id(),0,6,"","",'0'));
           case LIEFDATUM_SEQ :
              return cH_EntryValueIntString(lieferschein->LsDatum().c_str());
           case LIEFKUNDE_SEQ:
              return cH_EntryValueIntString(cH_Kunde(lieferschein->KdNr())->firma());
           default : return cH_EntryValueIntString("-");
           }
    }
  cH_Lieferschein get_Lieferschein() const {return lieferschein;}
};

class cH_Data_RLieferoffen : public Handle<const Data_RLieferoffen>
{
 protected:
   cH_Data_RLieferoffen() {}
 public:
   cH_Data_RLieferoffen(const Data_RLieferoffen *r) : Handle<const Data_RLieferoffen>(r){}
};


#endif
