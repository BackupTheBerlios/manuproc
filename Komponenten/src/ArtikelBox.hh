// $Id: ArtikelBox.hh,v 1.39 2006/08/03 11:57:22 christof Exp $
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

// $Id: ArtikelBox.hh,v 1.39 2006/08/03 11:57:22 christof Exp $

#ifndef _ARTIKELBOX_HH
#  define _ARTIKELBOX_HH
#include <Kunde/Kunde.h>
#include <Aux/EntryValueBase.h>
#include <Aux/SQLerror.h>
#include <Aux/Transaction.h>
#include <gtkmm/menu.h>
#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <gtkmm/eventbox.h>
#include "SearchCombo.h"
#include <gtkmm/image.h>
#include <gtkmm/tooltips.h>
#include <Artikel/Einheiten.h>
#include <Artikel/Artikelpreis.h>
#include <Instanzen/ppsInstanz.h>


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
{	int anzeige_pos,anzeige_neg;
public:
	int auswahl;
	bool autocomplete:1,kombiniert:1,reset_on_focus:1;
	cH_ExtBezSchema schema;

	ArtikelBoxAnzeige(cH_ExtBezSchema s)
	: anzeige_pos(), anzeige_neg(), auswahl(),
	  autocomplete(), kombiniert(), reset_on_focus(), schema(s)
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

class ArtikelBezeichnung;

class ArtikelBox : public Gtk::EventBox
{
// friend class SigC::ObjectSlot3_<void,int *,GtkSCContext,unsigned int,ArtikelBox>;
// friend class SigC::ObjectSlot1_<void,unsigned int,ArtikelBox>;
// friend class SigC::ObjectSlot1_<int,_GdkEventButton *,ArtikelBox>;

 struct BreitenSuche_switch_arg
 {  cH_ExtBezSchema s;
    unsigned i,sp,l;
    BreitenSuche_switch_arg(cH_ExtBezSchema s2,int i2,unsigned sp2,unsigned l2)
    	: s(s2), i(i2), sp(sp2), l(l2) {}
 };

 bool vertikalbool:1;
 bool autocompletebool:1;
 bool kombiniertbool:1;
 bool labelbool:1;
// bool einheiten_instanzen_bool:1;
 bool automatisch_anlegen_bool:1;
 bool eingeschraenkt:1;
// bool alle_artikel_anzeigen_bool:1;
// bool alle_artikel_anzeigen_mit_id_bool:1;
 bool artikel_automatisch_finden:1;
 bool reset_on_focus:1;
 bool block_selection:1;

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
 
 std::vector<std::pair<int,int> > suchvorgaben;
 
// ---- widgets ( GUI part ) ----
 typedef std::vector<Gtk::SearchCombo *> t_combos;
 typedef std::vector<t_combos> t_combos2;
 typedef std::vector<Gtk::Label *> t_labels;
 typedef std::vector<t_labels> t_labels2;
 
 std::vector<Gtk::Widget*> dependant_widgets;

 Gtk::Container *oberstes;
 t_combos2 combos;
 t_labels2 labels;

 Gtk::Menu *menu;
 Gtk::Image *pixmap;
 Gtk::Label *label_typ;
 Gtk::CheckMenuItem *label;
 Gtk::SearchCombo *active_sc;
 
 Gtk::Tooltips tooltips;
 
 typedef ArtikelBase (*artikel_anlegen_funcptr_t)(gpointer user_data,
 		cH_ExtBezSchema const&s,
	 	std::map<int,std::vector<cH_EntryValue> > const& felder);
 artikel_anlegen_funcptr_t artikel_anlegen_funcptr;
 gpointer artikel_anlegen_gptr;

 // ---- internal methods ----
 void searchFunc(int *cont, GtkSCContext newsearch, guint sp, guint l) throw(SQLerror);
 void selectFunc(guint sp, guint l) throw(SQLerror);
 void searchFunc_alle_artikel(int *cont, GtkSCContext newsearch, guint sp) throw(SQLerror) {};
 void selectFunc_alle_artikel(guint sp) throw(SQLerror){};
 bool FocusInFunc(GdkEventFocus*, guint sp, guint l); 
 bool MouseButton(GdkEventButton *);
 void TypSelected(int typ);
 void fuelleMenu();
 void adjust_combo_behaviour();
 static std::string kombinierterName(cH_ExtBezSchema schema, int signifikanz);
 void Autocomplete(Gtk::CheckMenuItem *autocomplete);
 void ResetOnFocusCMI(Gtk::CheckMenuItem *rof);
 void kombiniert(Gtk::CheckMenuItem *kombi);
 void set_Label(Gtk::CheckMenuItem *label);

// Gtk::HBox *createEIBox(); 
 void set_Vertikal(Gtk::CheckMenuItem *verti);
 void einschraenken_cb(Gtk::CheckMenuItem *einschr_mi);
 void Benutzerprofil_speichern();
 void Benutzerprofil_laden();
 void Neuer_Eintrag();
 static ArtikelBase Neuer_Eintrag_ext(gpointer ud, cH_ExtBezSchema const&s,
	    std::map<int,std::vector<cH_EntryValue> > const& felder);
 void Neuer_Eintrag_automatisch(Gtk::CheckMenuItem *cmi);
 void AlleArtikelAnzeigen(Gtk::CheckMenuItem *cmi);
 void AlleArtikelAnzeigenId(Gtk::CheckMenuItem *cmi);
 unsigned intern_id(int typ);
 void where_what(std::string& where, std::string& what,bool jumbo);
 bool neuanlegen() const;
 ArtikelBase::ID artikel_exist(bool jumbo);
 double get_menge_from_artikelbox();

 bool loadArtikel(unsigned l) throw(SQLerror);
 static gint try_grab_focus(GtkWidget *w,gpointer gp);

 
 void artbox_start();
 void init();
 void init2();
 Gtk::Container *init_table(int l);
 Gtk::Container *init_table_alle_artikel(int l);
 void setzeSchemaId(int t);
 void setzeSchemaTyp(int t2);
 void setzeSignifikanz(int t);
 void pixmap_setzen(bool);
 bool BreitenSuche(GtkSCContext newsearch,unsigned sp=0,unsigned l=0,int *cont=0);
 // this is for an idle triggered callback
 bool BreitenSuche_switch(BreitenSuche_switch_arg a);
 bool set_value_idle(Handle<const ArtikelBezeichnung> ab);

// Kombiniert/nicht kombiniert: Zugriff auf Combos
 std::vector<cH_EntryValue> get_content(unsigned l=0,unsigned spmax=G_MAXINT) const;
public:  // public for Artikeleingabe
protected: 
 std::vector<cH_EntryValue> expand_kombi_Artikel(unsigned l,std::string text) const;
 static std::string Kombinieren(cH_ExtBezSchema schema, unsigned signif,const std::vector<std::string> &v);
 static std::string Kombinieren(cH_ExtBezSchema schema, unsigned signif,const std::vector<cH_EntryValue> &v);
 enum enum_art_label {ARTIKEL,LABEL};
 std::vector<cH_EntryValue> expand_kombi(unsigned l,enum_art_label eal) const;
 
 // false=failed
 bool determineFocus(guint &sigidx_out, guint &entryidx_out) const;

	SigC::Signal0<void> activate;
//	SigC::Signal1<void,ArtikelBase::ID> new_article_inserted;
	SigC::Signal1<void,gpointer> MenueAusgewaehlt;

public:
	ArtikelBox(const cH_ExtBezSchema &_schema) throw(SQLerror);
	ArtikelBox(const std::string& _program,const std::string& _position) throw(SQLerror);
	~ArtikelBox();

	const cH_ExtBezSchema getBezSchema() const { return schema; }
	void setExtBezSchema(const cH_ExtBezSchema &_schema);

	void setExtBezSchemaID(ExtBezSchema::ID id); // ein Schema vorgeben, aber keinen ArtikelTyp
	
	// Schema und Sichtbarkeit setzen (für Artikeleingabe)
	void setzeAnzeige(const ArtikelBoxAnzeige &anz);
	unsigned getSignifikanzPos(int sig);
        void set_content(const std::vector<cH_EntryValue> &v,unsigned l=0);

   void reset();
   void set_editable(bool edit);
	void set_autoexpand(bool exp);
	void set_always_fill(bool fill);
   void set_automatisch_anlegen(bool b);


   const ArtikelBase &get_value() const
        {  return artikel; }

   const ArtikelBase &get_next_value(
	const std::vector<ExtBezSchema::BezKomp> bv=
		std::vector<ExtBezSchema::BezKomp>()) 
        {set_value(QueryForNext(bv));
	 return artikel; 
	}

   const ArtikelBase &get_prev_value() 
        {set_value(QueryForPrev());
	 return artikel; 
	}

private:
   ArtikelBase QueryForNext(const std::vector<ExtBezSchema::BezKomp> bv) throw(SQLerror);
   ArtikelBase QueryForPrev() throw(SQLerror);

public:        
	void set_value(const ArtikelBase &art) throw(SQLerror,ArtikelBoxErr);
   void show_label(bool b);
   
	void Einschraenken(const std::string &e, bool an=true);
	// dies kann nicht Einschraenken heißen (char* -> bool geht vor)
	void Einschraenken_b(bool an);

/*
   void set_EinheitenInstanz(Gtk::CheckMenuItem *einheiten_instanz);
	void show_einheiten_instanz(bool b);
*/	
	void AddUserMenu(const std::string &text, gpointer data);
	// sinnvollere Variante ...
//	sigc::signal0<void> AddUserMenu(const Glib::ustring &text);
	void ClearUserMenus();

	void EinWarenkorb(cH_PreisListe p);
	void NichtWarenkorb(PreisListe::ID pid);
	void AlleWarenkorb(Kunde::ID kid);

	// -1 means the last, -2 the last but one ... 0 means first, 1 second ...
	void set_focus(int sig, int field);
	void set_reset_on_focus(bool b) { reset_on_focus=b; }

	// ----- Signale -----
	SigC::Signal0<void> &signal_activate() { return activate; }
//	SigC::Signal1<void,ArtikelBase::ID> &signal_new_article_inserted() { return new_article_inserted; }
	SigC::Signal1<void,gpointer> &signal_MenueAusgewaehlt() { return MenueAusgewaehlt; }
	
	// nette Dinge für jeden (?)
	static std::string Tabellenname(cH_ExtBezSchema s);
	
	void AnlegenCall(artikel_anlegen_funcptr_t f,gpointer ud=0)
	{ artikel_anlegen_funcptr=f;
	  artikel_anlegen_gptr=ud;
	}
	
	void depends(Gtk::Widget *w);
};
#endif
