// generated 2004/2/7 19:56:13 CET by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to buchen_dialog.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _BUCHEN_DIALOG_HH
#  include "buchen_dialog_glade.hh"
#  define _BUCHEN_DIALOG_HH
class buchen_dialog : public buchen_dialog_glade
{  
public:
 buchen_dialog(bool lief) 
   {
    if(lief)
      text->set_text("Der Lieferschein ist noch nicht im Lager gebucht."
		" Wollen Sie die Mengen jetzt vom Lager abbuchen ?");
    else
      text->set_text("Der Wareneingang ist noch nicht im Lager gebucht."
		" Wollen Sie die Mengen jetzt dem Lager zubuchen ?");
   
   text2->set_text("WARNUNG: 'nicht mehr fragen' markiert die ungebuchten\n"
" Positionen als erledigt. Danach wird der Lieferschein nicht mehr"
" zum Buchen vorgeschlagen. Es bleiben ggf. offene Positionen in Aufträgen.");
   }
};
#endif
