/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _AUFTRAG_MAIN_HH
#  include "auftrag_main_glade.hh"
#  define _AUFTRAG_MAIN_HH

#include <Artikel/Prozess.h>
#include <vector>
#include <Aux/EntryValueIntString.h>
#include <Aux/EntryValueDatum.h>
#include <tclistnode.h>
#include <Aux/Ausgabe_neu.h>
#include "aktAufEintrag.h" //wg. Lieferwoche

#include <Aux/ppsInstanz.h>
#include <Auftrag/AufEintragBase.h>
#include <Faeden/Schussfaeden.hh>
#include <Auftrag/AuftragFull.h>

class auftrag_main : public auftrag_main_glade
{   
 cH_ppsInstanz instanz;
 bool interne_namen, zeit_kw_bool, kunden_nr_bool,kunden_anr_bool;
// AufEintragBase2 selected_Auftrag;
 AufEintragBase *selected_AufEintrag;
// Braucht man das folgende, oder kann man aus dem 'selected_Auftrag' den 
//   'selected_Artikel' wieder herausholen? MAT
 ArtikelBase selected_Artikel;
 int showdeep;
 static const unsigned int Artikelgroesse = 4;

 

 std::vector<cH_Prozess> prozlist;
        
        friend class auftrag_main_glade;
        void on_beenden_activate();
        void on_erfassen_activate();
        void on_neuladen_activate();
        void on_main_drucken_activate();
        void on_abschreiben_activate();
    	  void on_rechnung_activate();
        void on_main_bezeichnung_activate();
        void on_zeitdarstellung_activate();
        void on_kundendarstellung_activate();
        void on_kunden_anr_activate();
        void on_mainprint_button_clicked();
	     void on_prozlistscombo_search(int *cont, GtkSCContext newsearch);
        void on_prozlistscombo_activate();
        void on_button_auftraege_clicked();
        void on_leaf_selected(cH_RowDataBase d);
        void on_node_selected(const TCListNode &node);
        void on_unselect_row(gint row, gint column, GdkEvent *event);
        void on_togglebutton_material_toggled();
        void on_button_auftrag_erledigt_clicked();
        void instanz_menge(const vector<pair<ArtikelBase,long long int> >& vec_artbase);
        map<ArtikelBase,fixedpoint<5,double,long long> > get_artikelmap(const vector<pair<ArtikelBase,long long int> >& vec_artbase);
        void fillStamm(int *cont, GtkSCContext newsearch);

        void auftrag_main::set_column_titles_of_simple_tree();
        void auftrag_main::fill_simple_tree();

        void on_button_artikeleingabe_clicked();
        void menu_instanz();
        map<int,std::string> get_all_instanz();
        void instanz_selected(int _instanz_);


public:

 bool interneNamen() const { return interne_namen; }
 bool Zeit_kw_bool() const { return zeit_kw_bool; }
 bool Kunden_nr_bool() const { return kunden_nr_bool; }
 bool Kunden_anr_bool() const {return kunden_anr_bool;}
 cH_ppsInstanz get_Instanz() const {return instanz;}
 
 auftrag_main();

private:
   // Ab hier für die Produktionsplanung ////////////////////////
   AuftragFull *instanz_auftrag;

   void neue_auftraege_beruecksichtigen(cH_ppsInstanz instanz);
   vector<AufEintragBase2> get_new_aufids(cH_ppsInstanz instanz);
   void on_searchcombo_auftragid_activate();
   void on_searchcombo_auftragid_search(int *cont, GtkSCContext newsearch) throw(SQLerror);
   void on_button_neue_anr_clicked();
   gint on_button_instanz_print_clicked(GdkEventButton *ev);
   void instanz_tree_titel_setzen();
   void neuer_auftrag_tree_titel_setzen();
   void instanz_leaf_auftrag(AufEintragBase& selected_AufEintrag);
   void show_neuer_auftrag();
   void tree_neuer_auftrag_leaf_selected(cH_RowDataBase d);
   void loadAuftrag(const AuftragBase& auftragbase);
   int get_next_entry_znr(AuftragBase& auftrag);
   void AuftragsEntryZuordnung(const AufEintragBase& AEB,const AuftragBase& AB,int znr);
};

class Data_auftrag : public RowDataBase
{
   const AufEintragBase &AB ;
   const auftrag_main *AM ;

public:
   Data_auftrag(const AufEintragBase& ab, auftrag_main* am) :
      AB(ab),AM(am) {}

   enum {KUNDE,A1,A2,A3,A4,LIEFERDATUM,AUFTRAG,VERARBEITUNG,METER,STUECK};

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case KUNDE : {
       if(AM->get_Instanz()->Id()==ppsInstanz::INST_KNDAUF)
        {
         if (AM->Kunden_nr_bool())
          { int kundenid = AB.getKdNr();
            return cH_EntryValueIntString(kundenid); 
          }
         else
          { cH_Kunde K(AB.getKdNr());
            return cH_EntryValueIntString(K->firma()); 
          }
        }
       else return cH_EntryValueIntString(AB.get_Kunde()->firma());
       }
      case A1 ... A4 : {
         int artikelid          = AB.ArtikelID();
         cH_ExtBezSchema schema = 1;
         ArtikelBase artbase=ArtikelBase(artikelid);
         cH_ArtikelBezeichnung artbez(artbase.Id(),schema);
         return cH_EntryValueIntString(artbez->Komponente(seqnr-int(A1)));
         }
      case LIEFERDATUM : {
         std::string lw;
         if (AM->Zeit_kw_bool())
           {
            int lieferwoche = AB.getLieferdatum().KW().Woche();
            int lieferjahr = AB.getLieferdatum().KW().Jahr();
            string lj=itos (lieferjahr).substr(2,2);
            lw = itos(lieferwoche)+"'"+lj;
            return cH_EntryValueIntString(lw);
           }
         else   return cH_EntryValueDatum(AB.getLieferdatum());
       }
      case AUFTRAG : {
            std::string auftrag;
            if(AM->Kunden_anr_bool()) auftrag =      AB.getYourAufNr() ;
            else                      auftrag = itos(AB.getAuftragid()) ;
            return cH_EntryValueIntString(auftrag);}
      case VERARBEITUNG : {
         std::string verarbeitung;
         try {
          verarbeitung = AB.getProzess()->getTyp()+" "+AB.getProzess()->getText();
         } catch (std::exception &e ) 
         { verarbeitung=e.what(); }
	 return cH_EntryValueIntString(verarbeitung);
	 }
      case METER : {
         int offene_meter    = AB.getRest();     
         return cH_EntryValueIntString(Formatiere(offene_meter));   }
      case STUECK : {
         int offene_stueck   = AB.getRestStk();
         return cH_EntryValueIntString(Formatiere(offene_stueck)); }
     }
   return cH_EntryValueIntString("?");
 }

   int offene_Stueck()const {return AB.getRestStk();}
   int offene_Meter() const {return AB.getRest();}
//   void delete_StueckMeter()  {deleted=true;
//      AB }
   int get_aid() const {return AB.getAuftragid();} 
   int get_zeilennr() const {return AB.getZnr();} 
   int get_Artikel_ID() const {return AB.ArtikelID();}
   Petig::Datum get_Lieferdatum() const {return AB.getLieferdatum();}
   std::string ProzessText() const {return AB.getProzess()->getTyp()+" "+AB.getProzess()->getText() ;}
   AufEintragBase& get_AufEintragBase() const {return const_cast<AufEintragBase&>(AB);}
};

class cH_Data_auftrag : public Handle<const Data_auftrag>
{
//protected:
// cH_Data_auftrag() {}
public:
 cH_Data_auftrag(const Data_auftrag *r) : Handle<const Data_auftrag>(r){}
};


class Data_Node : public TCListNode
{
 int sum_meter, sum_stueck;
 vector<pair<ArtikelBase,long long int> > vec_artbase;

public:
 virtual void cumulate(const cH_RowDataBase &rd)
   {
    guint meter = (dynamic_cast<const Data_auftrag &>(*rd)).offene_Meter();
    guint stueck= (dynamic_cast<const Data_auftrag &>(*rd)).offene_Stueck();
    sum_meter += meter;
    sum_stueck+= stueck;
    ArtikelBase AB((dynamic_cast<const Data_auftrag &>(*rd)).get_Artikel_ID());
    vec_artbase.push_back(pair<ArtikelBase,long long int>(AB,stueck));
   }
  const cH_EntryValue Value(guint index,gpointer gp) const
   {
    switch(index) 
      { case 8 : return cH_EntryValueIntString(Formatiere(sum_meter));
        case 9 : return cH_EntryValueIntString(Formatiere(sum_stueck));
        default : return cH_EntryValueIntString("-");
      }
   }
 Data_Node::Data_Node(guint deep,const cH_EntryValue &v, bool expand)
   :TCListNode(deep,v,expand), sum_meter(0),sum_stueck(0) {}

 int Sum_Stueck() const { return sum_stueck; }
 int Sum_Meter() const { return sum_meter; }

 vector<pair<ArtikelBase,long long int> > get_vec_ArtikelBase_Stueck() const {return vec_artbase;}

 static TCListNode *create(guint col, const cH_EntryValue &v, bool expand)
  {  return new Data_Node(col,v,expand);
   }
};

#endif
