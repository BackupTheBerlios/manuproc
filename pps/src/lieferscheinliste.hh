// generated 2001/9/13 14:54:32 CEST by thoma@Tiger.
// using glademm V0.6.2_cvs
//
// newer (non customized) versions of this file go to lieferscheinliste.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _LIEFERSCHEINLISTE_HH
#  include "lieferscheinliste_glade.hh"
#  define _LIEFERSCHEINLISTE_HH
//#include <Aux/EntryValueIntString.h>
//#include <Aux/Datum.h>
#include <Lieferschein/LieferscheinList.h>
//#include <Lieferschein/LieferscheinEntry.h>
//#include <Lieferschein/Lieferschein.h>
//#include <Lieferschein/Rechnung.h>
//#include <Artikel/ArtikelBezeichnung.h>
//#include <tclistnode.h>
//#include <typeinfo>
//#include <fstream>


class lieferscheinliste : public lieferscheinliste_glade
{   
        LieferscheinList *LL;
        ArtikelBase artbase;
        int kundenid;
        
        friend class lieferscheinliste_glade;
        void kundenbox_activate();
        void artikelbox_activate();
        void datum_von_activate();
        void datum_bis_activate();
        void set_titles();
        void on_radiobutton_kunde_toggled();
        void on_radiobutton_zeit_toggled();
        void on_radiobutton_zeit_2_toggled();
        void on_radiobutton_artikel_toggled();
        void on_button_close_clicked();
        void on_button_show_clicked();
        void fill_tree();
        void on_button_drucken_clicked();
        static std::string scale(const Gtk::CList::Row &r,int col,
            const std::string &text,gpointer user_data);
        static void praeambel(std::ostream &os,gpointer user_data);

  public:
        lieferscheinliste();
};


#endif
