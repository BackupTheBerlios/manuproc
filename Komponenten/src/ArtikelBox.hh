// $Id: ArtikelBox.hh,v 1.17 2002/07/08 08:26:54 christof Exp $
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

// $Id: ArtikelBox.hh,v 1.17 2002/07/08 08:26:54 christof Exp $

#ifndef _ARTIKELBOX_HH
#  define _ARTIKELBOX_HH
#include <Kunde/Kunde.h>
#include <Aux/EntryValueBase.h>
#include <Aux/SQLerror.h>
#include <Aux/Transaction.h>
#include <gtk--/menu.h>
#include <gtk--/table.h>
#include <gtk--/label.h>
#include <gtk--/eventbox.h>
#include "SearchCombo.h"
#include <gtk--/pixmap.h>
#include <gtk--/tooltips.h>

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

class ArtikelBoxAnzeige
{	int anzeige_pos,anzeige_neg,auswahl;
	bool autocomplete,kombiniert;
	cH_ExtBezSchema schema;
public:
	ArtikelBoxAnzeige(cH_ExtBezSchema s)
	: anzeige_pos(0), anzeige_neg(0), auswahl(0),
	  autocomplete(false), kombiniert(false), schema(s)
	{}
	
	static ArtikelBoxAnzeige Laden(cH_ExtBezSchema schema);
	void Speichern() const;
	
	cH_ExtBezSchema Schema() const { return schema; }
	void Schema(cH_ExtBezSchema s) { schema=s; }
	
	bool Sichtbar(int signifikanz) const
	{  if (!signifikanz) return false;
	   else if (signifikanz<0) return anzeige_neg&(1<<(-signifikanz-1));
	   else return anzeige_pos&(1<<(signifikanz-1));
	}
	void Sichtbar(int signifikanz,bool ja)
	{  if (signifikanz<0) 
		anzeige_neg=(anzeige_neg&~(1<<(-signifikanz-1)))|(ja?(1<<(-signifikanz-1)):0);
	   else if (signifikanz>0) 
		anzeige_pos=(anzeige_pos&~(1<<(signifikanz-1)))|(ja?(1<<(signifikanz-1)):0);
	}
	int Auswahl() const { return auswahl; }
	void Auswahl(int a) { auswahl=a; }
	bool AutoComplete() const { return autocomplete; }
	void AutoComplete(bool ja) { autocomplete=ja; }
	bool Kombiniert() const { return kombiniert; }
	void Kombiniert(bool ja) { kombiniert=ja; }
};

class ArtikelBox : public Gtk::EventBox
{
// friend class SigC::ObjectSlot3_<void,int *,GtkSCContext,unsigned int,ArtikelBox>;
// friend class SigC::ObjectSlot1_<void,unsigned int,ArtikelBox>;
// friend class SigC::ObjectSlot1_<int,_GdkEventButton *,ArtikelBox>;

 bool vertikalbool:1;
 bool autocompletebool:1;
 bool kombiniertbool:1;
 bool labelbool:1;
 bool automatisch_anlegen_bool;
 bool eingeschraenkt:1;
 bool alle_artikel_anzeigen_bool:1;
 bool alle_artikel_anzeigen_mit_id_bool:1;
 bool artikel_automatisch_finden:1;

 std::string sprogram,sposition;
 cH_ExtBezSchema schema;
 // Idee: dieses Schema will man für neue ArtikelTypen verwenden auch wenn
 // zwischendurch andere aktiv waren (z.B. 1)
 ExtBezSchema::ID gewaehltesSchema;
 std::string einschraenkung;
 std::string joinstring;

 ArtikelBase artikel;

 Transaction tr,tr2; // internal use (search)
 std::vector<int> signifikanz;

 typedef std::pair<std::string,gpointer> user_menu_t;
 typedef std::vector<user_menu_t> user_menus_t;
 user_menus_t user_menus;
 
// ---- widgets ( GUI part ) ----
 typedef std::vector<Gtk::SearchCombo *> t_combos;
 typedef std::vector<t_combos> t_combos2;
 typedef std::vector<Gtk::Label *> t_labels;
 typedef std::vector<t_labels> t_labels2;

 Gtk::Container *oberstes;
 t_combos2 combos;
 t_labels2 labels;

 Gtk::Menu *menu;
 Gtk::Pixmap *pixmap;
 Gtk::Label *label_typ;
 Gtk::CheckMenuItem *label;
 
 Gtk::Tooltips tooltips;

 // ---- internal methods ----
 void searchFunc(int *cont, GtkSCContext newsearch, guint sp, guint l) throw(SQLerror);
 void selectFunc(guint sp, guint l) throw(SQLerror);
 void searchFunc_alle_artikel(int *cont, GtkSCContext newsearch, guint sp) throw(SQLerror) {};
 void selectFunc_alle_artikel(guint sp) throw(SQLerror){};
 gint MouseButton(GdkEventButton *);
 void TypSelected(int typ);
 void fuelleMenu();
 static std::string kombinierterName(cH_ExtBezSchema schema, int signifikanz);
 void Autocomplete(Gtk::CheckMenuItem *autocomplete);
 void kombiniert(Gtk::CheckMenuItem *kombi);
 void set_Label(Gtk::CheckMenuItem *label);
 
 void set_Vertikal(Gtk::CheckMenuItem *verti);
 void einschraenken_cb(Gtk::CheckMenuItem *einschr_mi);
 void Benutzerprofil_speichern();
 void Benutzerprofil_laden();
 void Neuer_Eintrag();
 void Neuer_Eintrag_sql(int db_id, std::string where, std::string what,int bestellen_bei);
 void Neuer_Eintrag_automatisch(Gtk::CheckMenuItem *cmi);
 void AlleArtikelAnzeigen(Gtk::CheckMenuItem *cmi);
 void AlleArtikelAnzeigenId(Gtk::CheckMenuItem *cmi);
 unsigned int intern_id(int typ);
 void where_what(std::string& where, std::string& what,bool jumbo);
 bool neuanlegen();
 ArtikelBase::ID artikel_exist(bool jumbo);
 void insert_into_artikelzusammensetzung(int id, int id_jumbo, int prozess,double menge);
 double get_menge_from_artikelbox();

 void loadArtikel(unsigned int l) throw(SQLerror);
 static gint try_grab_focus(GtkWidget *w,gpointer gp);
 
 void artbox_start();
 void init();
 Gtk::Container *init_table(int l);
 Gtk::Container *init_table_alle_artikel(int l);
 void setzeSchemaId(int t);
 void setzeSchemaTyp(int t2);
 void setzeSignifikanz(int t);
 void pixmap_setzen(bool);
 bool BreitenSuche(GtkSCContext newsearch,unsigned int sp=0,unsigned int l=0,int *cont=0);

// Kombiniert/nicht kombiniert: Zugriff auf Combos
 std::vector<cH_EntryValue> get_content(unsigned int l=0,unsigned int spmax=G_MAXINT) const;
 void set_content(const std::vector<cH_EntryValue> &v,unsigned int l=0);
 std::vector<cH_EntryValue> expand_kombi_Artikel(unsigned int l,std::string text) const;
 static std::string Kombinieren(cH_ExtBezSchema schema, unsigned int signif,const std::vector<std::string> &v);
 static std::string Kombinieren(cH_ExtBezSchema schema, unsigned int signif,const std::vector<cH_EntryValue> &v);
 enum enum_art_label {ARTIKEL,LABEL};
 std::vector<cH_EntryValue> expand_kombi(unsigned int l,enum_art_label eal) const;
 
 // false=failed
 bool determineFocus(guint &sigidx_out, guint &entryidx_out) const;

public:
	ArtikelBox(const cH_ExtBezSchema &_schema) throw(SQLerror);
	ArtikelBox(const std::string& _program,const std::string& _position) throw(SQLerror);
	~ArtikelBox();

	const cH_ExtBezSchema getBezSchema() const { return schema; }
	void setExtBezSchema(const cH_ExtBezSchema &_schema);
	
	void setExtBezSchemaID(ExtBezSchema::ID id); // ein Schema vorgeben, aber keinen ArtikelTyp

   void reset()
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->reset(); 
     eingeschraenkt=false;
     einschraenkung="";
     joinstring="";
   }  

	void set_editable(bool edit)
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_editable(edit); }  

	void set_autoexpand(bool exp)
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_autoexpand(exp); }  

	void set_always_fill(bool fill)
   { for (t_combos2::iterator j=combos.begin();j!=combos.end();++j)  
      for (t_combos::iterator i=j->begin();i!=j->end();++i)
        (*i)->set_always_fill(fill); }  

   const ArtikelBase &get_value() const
        {  return artikel; }
        
	void set_value(const ArtikelBase &art) throw(SQLerror,ArtikelBoxErr);
   void show_label(bool b);
   
	void Einschraenken(const std::string &e, bool an=true);
	// dies kann nicht Einschraenken heißen (char* -> bool geht vor)
	void Einschraenken_b(bool an);
	
	void AddUserMenu(const std::string &text, gpointer data);
	void ClearUserMenus();

#ifdef MABELLA_EXTENSIONS
	void NurWarenkorb(Kunde::ID kid)
	  {joinstring = " join artikelpreise on (id=artikelid) ";
	   einschraenkung=" kundennr="+itos(kid);
	  }
#endif

	// ----- Signale -----
	SigC::Signal0<void> activate;
	SigC::Signal1<void,ArtikelBase::ID> new_article_inserted;
	SigC::Signal1<void,gpointer> MenueAusgewaehlt;
	
	// nette Dinge für jeden (?)
	static std::string Tabellenname(cH_ExtBezSchema s);
};
#endif
