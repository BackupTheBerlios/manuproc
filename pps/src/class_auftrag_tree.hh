#include <tclistnode.h>
#include <Artikel/ArtikelMengeSumme.h>
#include <Aux/EntryValueDatum.h>
#include <Aux/EntryValueFixed.h>
#include <Aux/EntryValueIntString.h>
#include <Auftrag/AufEintragZu.h>

class Data_auftrag : public RowDataBase
{
   const AufEintrag &AB ;
   const auftrag_main *AM ;
   ArtikelMenge menge;

public:
   Data_auftrag(const AufEintrag& ab, auftrag_main* am) :
      AB(ab),AM(am),menge(AB.ArtId(),AB.getRestStk()) {}

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case auftrag_main::KUNDE : {
       if(AM->get_Instanz()->Id()==ppsInstanzID::Kundenauftraege)
        {
         if (AM->Kunden_nr_bool()) return cH_EntryValueIntString(AB.getKdNr()); 
         else  return cH_EntryValueIntString(cH_Kunde(AB.getKdNr())->sortname()); 
        }
       else { 
#if 0       
	 std::string k;
         std::list<cH_Kunde> LK=AufEintragZu(AB).get_Referenz_Kunden();
         // Bei mehr als einem Kunden nur noch die ID anzeigen
         if(LK.size()==1)
          {
            if (AM->Kunden_nr_bool()) return cH_EntryValueIntString((*LK.begin())->Id());
            else return cH_EntryValueIntString((*LK.begin())->firma());
          }
         else
          for (std::list<cH_Kunde>::const_iterator i=LK.begin();i!=LK.end();)
           { //k+=(*i)->firma();
             //if (++i != LK.end()) k+=", ";
               k+=itos((*i)->Id());
               if (++i != LK.end()) k+=", ";
           }
         return  cH_EntryValueIntString(k);
#endif
         return cH_EntryValue();
         }
       }
      case auftrag_main::A1 ... auftrag_main::A4 : {
         cH_ExtBezSchema schema(1,ExtBezSchema::default_Typ);
         if (!AM->interneNamen_bool()) schema = cH_ExtBezSchema(AB.getKdNr(),ExtBezSchema::default_Typ);
         cH_ArtikelBezeichnung artbez(AB.ArtId(),schema);
         return artbez->Komponente_als_EntryValue(seqnr-int(auftrag_main::A1));
         }
      case auftrag_main::LIEFERDATUM : {
         if (AM->Zeit_kw_bool())
            return cH_EntryValueKalenderwoche(AB.getLieferdatum().KW());
         else   
            return cH_EntryValueDatum(AB.getLieferdatum());
       }
      case auftrag_main::LETZTELIEFERUNG : {
          return cH_EntryValueDatum(AB.LetzteLieferung());
       }
/*
      case auftrag_main::LIEFERUNGAM : {
         AuftragBase::mengen_t menge =AB.getGeliefert();
         if(menge==AuftragBase::mengen_t(0)) return cH_EntryValueIntString("");
         cH_Lieferschein LE=AB.getLieferschein();
         std::string s=LE->LsDatum().c_str();         
         return cH_EntryValueIntString(itos(menge)+" am "+s);
       }
*/
      case auftrag_main::AUFTRAG : {
            std::string auftrag;
            if(AM->Kunden_anr_bool()) auftrag =      AB.getYourAufNr() ;
            else                      auftrag = itos(AB.getAuftragid()) ;
            return cH_EntryValueIntString(auftrag);}
      case auftrag_main::LETZEPLANINSTANZ :{
            std::string s = cH_ppsInstanz(AB.LetztePlanInstanz())->shortName()+" ";
            if (AB.getProzDat().valid()) s+=AB.getProzDat().Short();
            return cH_EntryValueIntString(s);
         }
      case auftrag_main::VERARBEITUNG : {
         std::string verarbeitung;
         try {
            verarbeitung = AB.Planung();
         } catch (std::exception &e ) 
         { verarbeitung=e.what(); }
	 return cH_EntryValueIntString(verarbeitung);
	 }
#ifdef PETIG_EXTENSIONS	 
      case auftrag_main::METER : 
         if (AM->Instanz()==ppsInstanzID::Rohwarenlager ||
         	AM->Instanz()==ppsInstanzID::_Garn__Einkauf)
            return cH_EntryValueIntString(menge.abgeleiteteMenge());
         else
         {ArtikelMenge::mengen_t m=menge.getMenge(EinheitID::m);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen>(m);
         }
      case auftrag_main::STUECK : 
         if (AM->Instanz()==ppsInstanzID::Rohwarenlager ||
         	AM->Instanz()==ppsInstanzID::_Garn__Einkauf)
            return cH_EntryValueIntString(menge.Menge());
         else
         {ArtikelMenge::mengen_t m=menge.getMenge(Einheit::Stueck);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen>(m);
         }
#else
      case auftrag_main::METER : 
      	  return cH_EntryValueIntString(menge.abgeleiteteMenge());
      case auftrag_main::STUECK : 
          return cH_EntryValueIntString(menge.Menge());
#endif          
     }
   return cH_EntryValue();
 }

   int offene_Stueck()const {return AB.getRestStk();}
   int get_aid() const {return AB.getAuftragid();} 
   int get_zeilennr() const {return AB.getZnr();} 
   int get_Artikel_ID() const {return AB.ArtId();}
   ManuProC::Datum get_Lieferdatum() const {return AB.getLieferdatum();}
   std::string ProzessText() const {return AB.getProzess()->getTyp()+" "+AB.getProzess()->getText() ;}
   AufEintrag& get_AufEintrag() const {return const_cast<AufEintrag&>(AB);}
   const ArtikelMenge getArtikelMenge() const { return menge; }
   void redisplayMenge(TreeBase *maintree_s) 
   {  menge=ArtikelMenge(AB.ArtId(),AB.getRestStk());
      maintree_s->redisplay(this,auftrag_main::METER);
      maintree_s->redisplay(this,auftrag_main::STUECK);
   }
   void redisplayLetzePlanInstanz(TreeBase *maintree_s,const AufEintrag &ae)
    {
      const_cast<AufEintrag&>(AB)=ae;
      maintree_s->redisplay(this,auftrag_main::LETZEPLANINSTANZ);
    }
};

struct cH_Data_auftrag : public Handle<const Data_auftrag>
{  cH_Data_auftrag(const Data_auftrag *r) : Handle<const Data_auftrag>(r){}
};


class Data_Node : public TreeRow
{
  ArtikelMengeSumme sum;
// std::map<ArtikelBase,double> map_artbase;

public:
 virtual void cumulate(const cH_RowDataBase &rd)
   {  const ArtikelMenge &am=dynamic_cast<const Data_auftrag &>(*rd).getArtikelMenge();
      sum.cumulate(am);
//      map_artbase[am.Artikel()] += am.getMenge();
   }
  const cH_EntryValue Value(guint index,gpointer gp) const
   {
    switch(index) 
      { 
#ifdef PETIG_EXTENSIONS	 
      case auftrag_main::METER : 
         {ArtikelMengeSumme::mengen_t m=sum.Summe(EinheitID::m);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen,double,long long>(m);
         }
      case auftrag_main::STUECK : 
         {ArtikelMengeSumme::mengen_t m=sum.Summe(Einheit::Stueck);
          if (!m) return cH_EntryValue();
          else return cH_EntryValueFixed<ArtikelMenge::nachkommastellen,double,long long>(m);
         }
#else
        case auftrag_main::METER : return cH_EntryValueIntString(sum.abgeleiteteMenge());
        case auftrag_main::STUECK : return cH_EntryValueIntString(sum.Menge());
#endif
        default : return cH_EntryValue();
      }
   }
 Data_Node::Data_Node(guint deep,const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
   :TreeRow(deep,v,child_s_deep,child_s_data,expand) {}
 static TreeRow *create(guint col, const cH_EntryValue &v, guint child_s_deep, cH_RowDataBase child_s_data,bool expand)
   {  return new Data_Node(col,v,child_s_deep,child_s_data,expand);
   }

// std::vector<pair<ArtikelBase,long long int> > get_vec_ArtikelBase_Stueck() const {return vec_artbase;}
// std::map<ArtikelBase,double> get_map_ArtikelBase_Stueck() const {return map_artbase;}
//  std::map<AufEintrag,double> get_map_AufEintrag_Stueck() const;
};

