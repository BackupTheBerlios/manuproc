// generated 2000/1/28 11:31:10 MET by jacek@mimi.
// using glademm V0.5.5
//
// newer (non customized) versions of this file go to wlagermain.hh_glade

// you might replace
//    class Foo : public Foo_Glade { ... };
// by
//    typedef Foo_Glade Foo;
// if you didn't make any modifications to the widget

#ifndef _WLAGERMAIN_HH
#  include "wlagermain_glade.hh"
#  define _WLAGERMAIN_HH

//#include "glademm_support.hh"
#include<Artikel/ArtikelBase.h>
#include <Lager/FertigWaren.h>
#include<Gtk_OStream.h>

class wlagermain : public wlagermain_glade
{   
 friend class wlagermain_glade;
 std::vector<FertigWaren> vfw;
 int lagerid;
 
 struct st_artmenge {ArtikelBase artikel;int menge;
         st_artmenge() :artikel(0),menge(0){}
         st_artmenge(ArtikelBase a,int m) :artikel(a),menge(m){} 
	};

 st_artmenge artmenge;

        friend class wlagermain_Glade;
        void on_beenden();
	gint on_delete_event(GdkEventAny*);
        void on_abmelden();

        void on_mengespin_activate();

        void on_artbox_activate();
        void on_artreset_activate();
	void on_mengespin_changed();
        void on_button_einlagern_clicked();
        void on_button_auslagern_clicked();
        void on_button_inventur_clicked();
        void on_button_reload_clicked();
        void on_lagerwahl_activate();

        void on_einlagern_activate();
        void on_auslagern_activate();
        void on_inventur_activate();
        void on_keineaktion_activate();

        bool Check();
	void append_to_CList(const FertigWaren &fw,Gtk::OStream &os);
	void fill_CList(const ArtikelBase &a);
	void show_CList();
	void fillLagerMenu();
public:
	wlagermain();
};
#endif

