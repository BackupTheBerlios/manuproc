

#ifndef LLISTE_CLASSES
#define LLISTE_CLASSES

#include <Misc/EntryValueBase.h>

#include <Misc/Datum.h>
#include <Lieferschein/LieferscheinList.h>
#include <Lieferschein/LieferscheinEntry.h>
#include <Lieferschein/Lieferschein.h>
#include <Lieferschein/Rechnung.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <tclistnode.h>
#include <Artikel/Einheiten.h>
#include <typeinfo>
#include <Misc/Ausgabe_neu.h>
#include <Artikel/ArtikelMengeSumme.h>

class Data_LListe : public RowDataBase
{
public:
 typedef enum {KUM_DATUM,KUM_WOCHE,KUM_MONAT,KUM_QUARTAL,KUM_JAHR} KumVal;

private:

    cH_Lieferschein liefer;
    LieferscheinEntry entry;
    Rechnung rechnung;
    ArtikelMenge menge;
    KumVal option_timecumulate;

 public:



   Data_LListe(const cH_Lieferschein& l,const LieferscheinEntry& e,
         const Rechnung& r, KumVal kv=KUM_DATUM)
      :  liefer(l),entry(e),rechnung(r),
         menge(ArtikelBase(entry.ArtikelID()),entry.Stueck(),entry.Menge()),
	 option_timecumulate(kv) {}

   typedef enum {KUNDE,AUFTRAG,ARTIKEL,BREITE,FARBE,AUFMACHUNG,LIEFERNR,
         LIEFERDATUM,RECHNUNG,RECHNUNGSDATUM,SUM_MENGE,SUM_AMENGE} Spalten;

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const;
   const ArtikelMenge getArtikelMenge() const { return menge; }
   void setTimeCumulate(KumVal opt) { option_timecumulate=opt;}
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
 Data_ListeNode::Data_ListeNode(guint deep,const cH_EntryValue &v, guint child_s_deep, 
 	cH_RowDataBase child_s_data,bool expand, const TreeRow &suminit)
   :TreeRow(deep,v,child_s_deep,child_s_data,expand) 
   {   if (suminit.Leaf()) cumulate(child_s_data);
       else sum=dynamic_cast<const Data_ListeNode&>(suminit).sum;
   }

  static TreeRow *create(guint col, const cH_EntryValue &v,guint child_s_deep,
  	 cH_RowDataBase child_s_data, bool expand, const TreeRow &suminit)
  {  return new Data_ListeNode(col,v,child_s_deep,child_s_data,expand,suminit);
  }
};

class cH_Data_LListe : public Handle<const Data_LListe>
{
protected:
 cH_Data_LListe() {}
public:   
 cH_Data_LListe(Data_LListe *r) : Handle<const Data_LListe>(r) {}
};

#endif

