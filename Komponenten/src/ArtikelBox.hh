/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// $Id: ArtikelBox.hh,v 1.1 2001/04/23 08:36:50 christof Exp $

#ifndef _ARTIKELBOX_HH
#  define _ARTIKELBOX_HH
#include <Kunde/Kunde.h>
#include <Artikel/ArtikelBase.h>
#include <Artikel/ArtikelBezeichnung.h>
#include <Aux/SQLerror.h>
#include <ExtBezSchema/ExtBezSchemaHandle.h>
#include <Aux/Transaction.h>
#include <gtk--/menu.h>
#include <gtk--/table.h>
#include <gtk--/label.h>
#include <gtk--/eventbox.h>
#include "SearchCombo.h"


class ArtikelBoxErr
{
public:
 enum ErrCode {KOMP_ANZ,NO_ART};
 
private:
 int param;
 ErrCode code;

public:
 ArtikelBoxErr(const ErrCode c,int i)
 	: param(i), code(c)
 	{}
 const string ErrMsg() const;
};

class ArtikelBox : public Gtk::EventBox
{
 friend class SigC::ObjectSlot3_<void,int *,GtkSCContext,unsigned int,ArtikelBox>;
 friend class SigC::ObjectSlot1_<void,unsigned int,ArtikelBox>;
 friend class SigC::ObjectSlot1_<int,_GdkEventButton *,ArtikelBox>;

//ALT Gtk::Table table;
 Gtk::Container *oberstes;

 vector<int> signifikanz;
 bool vertikalbool;
 bool autocompletebool;
 bool kombiniertbool;
 
 ArtikelBase artikel;
 cH_ExtBezSchema schema;
 
 guint komps; // was ist das ??? - um den Zusatnd der Eingabe im long Feld zu haben
 

 typedef vector<Gtk::SearchCombo *> t_combos;
 typedef vector<t_combos> t_combos2;
 typedef vector<Gtk::Label *> t_labels;
 typedef vector<t_labels> t_labels2;
// typedef vector<Gtk::MenuItem *> t_menuitems;
 t_combos2 combos;
 t_labels2 labels;

 Gtk::Menu *menu;
// t_menuitems menuitems;

// noch nicht implementiert 
 bool show_id:1; // ob die ID mit angezeigt wird
 Transaction tr,tr2; // internal use (search)
 void searchFunc(int *cont, GtkSCContext newsearch, guint sp, guint l) throw(SQLerror);
 void selectFunc(guint sp, guint l) throw(SQLerror);
 gint MouseButton(GdkEventButton *);
 void TypSelected(int typ);
 void fuelleMenu();
 string kombinierteAnzeige(int sig, int atyp, int id);
 void Autocomplete(Gtk::CheckMenuItem *autocomplete);
 void kombiniert(Gtk::CheckMenuItem *kombi);
 vector<EntryValue> expand_kombi_Artikel(unsigned int l);
 void set_Vertikal(Gtk::CheckMenuItem *verti);
 void Benutzerprofil_speichern();
 void Benutzerprofil_laden();
 void Neuer_Eintrag();

 void loadArtikel(unsigned int l) throw(SQLerror);
 static gint try_grab_focus(GtkWidget *w,gpointer gp);
 
 void init();
 Gtk::Container *init_table(int l);
 void setzeTyp(int t);
 void setzeSignifikanz(int t);

public:
//        typedef enum {SHOW_DETAIL,SHOW_KOMPL} ShowType;

	ArtikelBox(const cH_ExtBezSchema &_schema) throw(SQLerror);
	~ArtikelBox();

	const string operator[](guint i) const { return combos[i][0]->get_text(); }
	const cH_ExtBezSchema getBezSchema() const { return schema; }
//	int getSignif() const { return schema->size(signifikanz[0]); }
	
	void setExtBezSchema(const cH_ExtBezSchema &_schema);

   void reset()

   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->reset(); }  

	// war editable
	void set_editable(bool edit)
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_editable(edit); }  

	// CP: I don''t like this function too much
//        void set_focus(uint idx=0)
//	{ combos[idx]->grab_focus(); }

	// war set_expand
	void set_autoexpand(bool exp)
//	{  for(uint j=0;j<combos.size();++j)
//         for(uint i=0;i<combos[j].size();++i) combos[j][i]->set_autoexpand(exp);	}
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_autoexpand(exp); }  

	// war set_fill
	void set_always_fill(bool fill)
//	{  for(uint j=0;j<combos.size();++j)
//         for(uint i=0;i<combos[j].size();++i) combos[j][i]->set_always_fill(fill);}
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_always_fill(fill); }  

   const ArtikelBase &get_value() const
        {  return artikel; }
	void set_value(const ArtikelBase &art) throw(SQLerror,ArtikelBoxErr);

	// ich bin für activate als standard
	SigC::Signal0<void> activate;
	
};
#endif
