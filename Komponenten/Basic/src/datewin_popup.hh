//

#ifndef _DATEWIN_POPUP_HH
#  include "datewin_popup_glade.hh"
#  define _DATEWIN_POPUP_HH
class datewin;

class datewin_popup : public datewin_popup_glade
{ datewin* parent;
  bool block;

        void datum_clicked();
        void woche_clicked();
        void empty_clicked();
        void on_day_selected();
public:
  datewin_popup(datewin *parent);
  ~datewin_popup();
};
#endif
