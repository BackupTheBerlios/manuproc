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

// $Id: ArtikelBox.hh,v 1.5 2001/07/05 12:58:06 christof Exp $

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
#include <gtk--/pixmap.h>

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
 const std::string ErrMsg() const;
};

class ArtikelBox : public Gtk::EventBox
{
 friend class SigC::ObjectSlot3_<void,int *,GtkSCContext,unsigned int,ArtikelBox>;
 friend class SigC::ObjectSlot1_<void,unsigned int,ArtikelBox>;
 friend class SigC::ObjectSlot1_<int,_GdkEventButton *,ArtikelBox>;

 typedef std::vector<Gtk::SearchCombo *> t_combos;
 typedef std::vector<t_combos> t_combos2;
 typedef std::vector<Gtk::Label *> t_labels;
 typedef std::vector<t_labels> t_labels2;

 Gtk::Container *oberstes;
 std::vector<int> signifikanz;
 bool vertikalbool;
 bool autocompletebool;
 bool kombiniertbool;
 
 ArtikelBase artikel;
 cH_ExtBezSchema schema;
 
 t_combos2 combos;
 t_labels2 labels;

 Gtk::Menu *menu;
 Gtk::Pixmap *pixmap;
 Transaction tr,tr2; // internal use (search)

 void searchFunc(int *cont, GtkSCContext newsearch, guint sp, guint l) throw(SQLerror);
 void selectFunc(guint sp, guint l) throw(SQLerror);
 gint MouseButton(GdkEventButton *);
 void TypSelected(int typ);
 void fuelleMenu();
 std::string kombinierteAnzeige(int sig, int atyp, int id);
 void Autocomplete(Gtk::CheckMenuItem *autocomplete);
 void kombiniert(Gtk::CheckMenuItem *kombi);
 std::vector<EntryValue> expand_kombi_Artikel(unsigned int l);
 void set_Vertikal(Gtk::CheckMenuItem *verti);
 void Benutzerprofil_speichern();
 void Benutzerprofil_laden();
 void Neuer_Eintrag();
 unsigned int intern_id(int typ);
 void where_what(std::string& where, std::string& what);

 void loadArtikel(unsigned int l) throw(SQLerror);
 static gint try_grab_focus(GtkWidget *w,gpointer gp);
 
 void init();
 Gtk::Container *init_table(int l);
 void setzeTyp(int t);
 void setzeTyp2(int t2);
 void setzeSignifikanz(int t);

public:
	ArtikelBox(const cH_ExtBezSchema &_schema) throw(SQLerror);
	~ArtikelBox();

	const std::string operator[](guint i) const { return combos[i][0]->get_text(); }
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

	// war set_expand
	void set_autoexpand(bool exp)
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_autoexpand(exp); }  

	// war set_fill
	void set_always_fill(bool fill)
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
