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
#include <tclistnode.h>
#include <Aux/Ausgabe_neu.h>
#include "aktAufEintrag.h" //wg. Lieferwoche

#include <Aux/ppsInstanz.h>
#include <Auftrag/AufEintragBase.h>

class auftrag_main : public auftrag_main_glade
{   
 ppsInstanz instanz;
 bool interne_namen, zeit_kw_bool, kunden_nr_bool,kunden_anr_bool;
AufEintragBase2 selected_Auftrag;
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
        void fillStamm(int *cont, GtkSCContext newsearch);

        void auftrag_main::set_column_titles_of_simple_tree();
        void auftrag_main::fill_simple_tree();

        void menu_instanz();
        map<int,std::string> get_all_instanz();
        void instanz_selected(int _instanz_);
public:

 bool interneNamen() const { return interne_namen; }
 bool Zeit_kw_bool() const { return zeit_kw_bool; }
 bool Kunden_nr_bool() const { return kunden_nr_bool; }
 bool Kunden_anr_bool() const {return kunden_anr_bool;}
 
 auftrag_main();

};

class Data_auftrag : public RowDataBase
{
   const AufEintragBase &AB ;
   const auftrag_main *AM ;

public:
   Data_auftrag(const AufEintragBase& ab, auftrag_main* am) :
      AB(ab),AM(am) {}

    virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const
 { 
    switch (seqnr) {
      case 0 : {
         if (AM->Kunden_nr_bool())
          { int kundenid = AB.getKdNr();
            return cH_EntryValueIntString(kundenid); 
          }
        else
          { cH_Kunde K(AB.getKdNr());
            return cH_EntryValueIntString(K->firma()); 
          }
         }
      case 1 ... 4 : {
         int artikelid          = AB.ArtikelID();
         cH_ExtBezSchema schema = 1;
         ArtikelBase artbase=ArtikelBase(artikelid);
         cH_ArtikelBezeichnung artbez(artbase,schema);
#warning horrible hack
         return (*artbez)[seqnr];
         }
      case 5 : {
         std::string lw;
         if (AM->Zeit_kw_bool())
           {
            int lieferwoche = AB.getLieferdatum().KW().Woche();
            int lieferjahr = AB.getLieferdatum().KW().Jahr();
            string lj=itos (lieferjahr).substr(2,2);
            lw = itos(lieferwoche)+"/"+lj;
           }
         else  lw =  AB.getLieferdatum().c_str()  ;
         return cH_EntryValueIntString(lw); }
      case 6 : {
         std::string auftrag;
         if(AM->Kunden_anr_bool()) auftrag =      AB.getYourAufNr() ;
         else                      auftrag = itos(AB.getAuftragid()) ;
         return cH_EntryValueIntString(auftrag);}
      case 7 : {
         std::string verarbeitung;
         try {
          verarbeitung = AB.getProzess()->getTyp()+" "+AB.getProzess()->getText() ;  
//          verarbeitung = AufEintrag(AB).getProzess()->getText() +" "+  AufEintrag(AB).getProzDat().c_str();  
         } catch (std::exception &e ) 
         { verarbeitung=e.what(); }
	 return cH_EntryValueIntString(verarbeitung);
	 }
      case 8 : {
         int offene_meter    = AB.getRest();     
         return cH_EntryValueIntString(offene_meter);   }
      case 9 : {
         int offene_stueck   = AB.getRestStk();
         return cH_EntryValueIntString(offene_stueck); }
     }
   return cH_EntryValueIntString("?");
 }

   int offene_Stueck()const {return AB.getRestStk();}
   int offene_Meter() const {return AB.getRest();}
   int get_aid() const {return AB.getAuftragid();} 
   int get_zeilennr() const {return AB.getZnr();} 
   int get_Artikel_ID() const {return AB.ArtikelID();}
   std::string ProzessText() const {return AB.getProzess()->getTyp()+" "+AB.getProzess()->getText() ;}

};

class cH_Data_auftrag : public const_Handle<Data_auftrag>
{
protected:
 cH_Data_auftrag() {}
public:
 cH_Data_auftrag(Data_auftrag *r) : const_Handle<Data_auftrag>(r){}
};


class Data_Node : public TCListNode
{
 mutable int sum_meter;
 mutable int sum_stueck;

public:
 virtual void cumulate(const cH_RowDataBase &rd)
   {
    sum_meter += (dynamic_cast<const Data_auftrag &>(*rd)).offene_Meter();
    sum_stueck+= (dynamic_cast<const Data_auftrag &>(*rd)).offene_Stueck();
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

};


class Data_Sum : public SimpleTree
{
 public:

  Data_Sum(guint cols, guint attr=8):SimpleTree(cols,attr)
   { // make sure this is not called if you derive this from class !
     init();
   }

/*
 TCListNode *NewNode(guint _seqnr, gpointer gp, const cH_RowDataBase &v, guint deep)
   { return new  Data_Node(_seqnr,gp, v,deep); }
*/
};

static TCListNode *create_MyNode(guint col, const cH_EntryValue &v, bool expand)
{  return new Data_Node(col,v,expand);
}


#endif
