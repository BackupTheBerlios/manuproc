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

class Data_LListe : public RowDataBase
{

    cH_Lieferschein liefer;
    LieferscheinEntry entry;
    Rechnung rechnung;

 public:

   Data_LListe(const cH_Lieferschein& l,const LieferscheinEntry& e,
         const Rechnung& r)
      :  liefer(l),entry(e),rechnung(r) {}
   enum {KUNDE,ARTIKEL,BREITE,FARBE,AUFMACHUNG,STUECK,MENGE,LIEFERNR,
         LIEFERDATUM,GELIEFERTAM,RECHNUNG,RECHNUNGSDATUM,SUM_ARTIKEL};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
      {

       switch(seqnr) {
         case ARTIKEL ... AUFMACHUNG: {
            cH_ArtikelBezeichnung AB(entry.ArtikelID());
            return AB->Komponente_als_EntryValue(seqnr-ARTIKEL);
          }
         case KUNDE : return cH_EntryValueIntString(cH_Kunde(liefer->KdNr())->firma());
         case STUECK : {
              Einheit E(entry.ArtikelID());
              std::string es = E.StueckEinheit();
              return cH_EntryValueIntString( itos(entry.Stueck())+es );
           }
         case MENGE : {
              Einheit E(entry.ArtikelID());
              std::string em = E.MengenEinheit();
              return cH_EntryValueIntString( Formatiere_short(entry.Menge())+em);
           }
         case LIEFERNR :return cH_EntryValueIntString( entry.Id());
         case LIEFERDATUM : return cH_EntryValueDatum( liefer->LsDatum());
         case GELIEFERTAM : return cH_EntryValueDatum( liefer->getDatum());
         case RECHNUNG : return cH_EntryValueIntString( liefer->RngNr());
         case RECHNUNGSDATUM : return cH_EntryValueDatum(rechnung.getDatum());
         case SUM_ARTIKEL: {
            Einheit E(entry.ArtikelID());
            std::string es = E.StueckEinheit();
            std::string em = E.MengenEinheit();
            std::string s = itos(entry.Stueck())+es;
            if (entry.Menge()!=fixedpoint<3>(0)) s+=" mit "+Formatiere_short(entry.Menge())+em;
            return cH_EntryValueIntString(s);
          } 
        }
       return cH_EntryValue();
      }
   int Stueck() const {return entry.Stueck();}
   fixedpoint<3> Menge() const {return entry.Menge();}
   int Artikel_Id() const {return entry.ArtikelID();}
};


class Data_ListeNode : public TCListNode
{
   struct st_summe{int stueck;std::string seinheit; fixedpoint<3> menge;std::string meinheit;
        st_summe(int s,std::string se,fixedpoint<3> m,std::string me)
          :stueck(s),seinheit(se),menge(m),meinheit(me) {}
        st_summe()
          :stueck(0),seinheit(""),menge(0),meinheit("") {}
      };
   vector<st_summe> vec_summe;

 public:
   virtual void cumulate(const cH_RowDataBase &rd)
     { 
       ArtikelBase A(dynamic_cast<const Data_LListe &>(*rd).Artikel_Id());
       Einheit E(A);
       int stueck = (dynamic_cast<const Data_LListe &>(*rd)).Stueck();
       fixedpoint<3> menge = (dynamic_cast<const Data_LListe &>(*rd)).Menge();
       bool append_bool=true;
       for(vector<st_summe>::iterator i=vec_summe.begin();i!=vec_summe.end();++i)
         {
            if ( (*i).seinheit==E.StueckEinheit() && (*i).meinheit==E.MengenEinheit() )
             {  (*i).stueck += stueck;
                (*i).menge  += menge;
                append_bool=false;
                break;
             }
         }
       if (append_bool)
         vec_summe.push_back(st_summe(stueck,E.StueckEinheit(),menge,E.MengenEinheit()));

     }      
   const cH_EntryValue Value(guint index,gpointer gp) const
    {
      switch(index)
        {
         case Data_LListe::SUM_ARTIKEL :
            { std::string s;
              for(vector<st_summe>::const_iterator i=vec_summe.begin();i!=vec_summe.end();)
               {
                 s += itos((*i).stueck)+(*i).seinheit;
                 if ((*i).menge!=fixedpoint<3>(0)) 
                    s += " mit "+Formatiere_short((*i).menge) +(*i).meinheit;
                 ++i;
                 if(i!=vec_summe.end()) s += " und ";
               }
              return cH_EntryValueIntString(s);
            }
        }
      return cH_EntryValue();
    }
 Data_ListeNode::Data_ListeNode(guint deep,const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
   :TCListNode(deep,v,child_s_deep,child_s_data,expand) {}

  static TCListNode *create(guint col, const cH_EntryValue &v,guint child_s_deep, cH_RowDataBase child_s_data, bool expand)
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
