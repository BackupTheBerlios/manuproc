#include <Aux/EntryValueIntString.h>
#include <Aux/EntryValueDatum.h>
#include <Aux/Datum.h>
#include <Lieferschein/LieferscheinList.h>
#include <Lieferschein/LieferscheinEntry.h>
#include <Lieferschein/Lieferschein.h>
#include <Lieferschein/Rechnung.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <tclistnode.h>
#include <Artikel/Einheiten.h>
#include <typeinfo>
#include <Aux/Ausgabe_neu.h>
#include <Artikel/ArtikelMengeSumme.h>

class Data_LListe : public RowDataBase
{

    cH_Lieferschein liefer;
    LieferscheinEntry entry;
    Rechnung rechnung;
    ArtikelMenge menge;

 public:

   Data_LListe(const cH_Lieferschein& l,const LieferscheinEntry& e,
         const Rechnung& r)
      :  liefer(l),entry(e),rechnung(r),
         menge(entry.ArtikelID(),entry.Stueck(),entry.Menge()) {}

   enum {KUNDE,ARTIKEL,BREITE,FARBE,AUFMACHUNG,LIEFERNR,
         LIEFERDATUM,GELIEFERTAM,RECHNUNG,RECHNUNGSDATUM,SUM_MENGE,SUM_AMENGE};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
      {

       switch(seqnr) {
         case ARTIKEL ... AUFMACHUNG: {
            cH_ArtikelBezeichnung AB(entry.ArtikelID());
            return AB->Komponente_als_EntryValue(seqnr-ARTIKEL);
          }
         case KUNDE : return cH_EntryValueIntString(cH_Kunde(liefer->KdNr())->firma());
         case LIEFERNR :return cH_EntryValueIntString( entry.Id());
         case LIEFERDATUM : return cH_EntryValueDatum( liefer->LsDatum());
         case GELIEFERTAM : return cH_EntryValueDatum( liefer->getDatum());
         case RECHNUNG : return cH_EntryValueIntString( liefer->RngNr());
         case RECHNUNGSDATUM : return cH_EntryValueDatum(rechnung.getDatum());
         case SUM_MENGE: 
              return cH_EntryValueIntString(menge.Menge());
         case SUM_AMENGE: 
              return cH_EntryValueIntString(menge.abgeleiteteMenge());
        }
       return cH_EntryValue();
      }
   const ArtikelMenge getArtikelMenge() const { return menge; }
};


class Data_ListeNode : public TreeRow
{
   ArtikelMengeSumme sum;
 public:
   virtual void cumulate(const cH_RowDataBase &rd)
     { 
       const ArtikelMenge &am=dynamic_cast<const Data_LListe &>(*rd).getArtikelMenge();
       sum.cumulate(am);
     }      
   const cH_EntryValue Value(guint index,gpointer gp) const
    {
      switch(index)
        {
         case Data_LListe::SUM_MENGE :
              return cH_EntryValueIntString(sum.Menge());
         case Data_LListe::SUM_AMENGE :
              return cH_EntryValueIntString(sum.abgeleiteteMenge());
        }
      return cH_EntryValue();
    }
 Data_ListeNode::Data_ListeNode(guint deep,const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
   :TreeRow(deep,v,child_s_deep,child_s_data,expand) {}

  static TreeRow *create(guint col, const cH_EntryValue &v,guint child_s_deep, cH_RowDataBase child_s_data, bool expand)
  {  return new Data_ListeNode(col,v,child_s_deep,child_s_data,expand);
  }
};

class cH_Data_LListe : public Handle<const Data_LListe>
{
protected:
 cH_Data_LListe() {}
public:   
 cH_Data_LListe(Data_LListe *r) : Handle<const Data_LListe>(r) {}
};
