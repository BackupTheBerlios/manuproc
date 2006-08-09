#ifndef _LIEFERSCHEINLISTE_HH
#  include "lieferscheinliste_glade.hh"
#  define _LIEFERSCHEINLISTE_HH
#include <Lieferschein/LieferscheinList.h>

class lieferscheinliste : public lieferscheinliste_glade
{   
        LieferscheinList LL;
        ArtikelBase artbase; // eigentlich durch artikelbox->get_value() abgedeckt
        int kundenid; // eigentlich durch kundenbox->get_value() abgedeckt
        cH_ppsInstanz instanz;
        
        void kundenbox_activate();
        void kundengruppe_activate();
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
        void drucken_clicked();
        static std::string scale(const Gtk::TreeModel::const_iterator &r,int col,
            const std::string &text,gpointer user_data);
        static void praeambel(std::ostream &os,gpointer user_data);
        
        enum menu_selection { M_Auftrag, M_Lieferschein, M_Rechnung };
        void on_menu_selection(menu_selection m);
  public:
        lieferscheinliste(const cH_ppsInstanz& _instanz, 
              Kunde::ID k=Kunde::none_id, ArtikelBase art=ArtikelBase());
};


#endif
