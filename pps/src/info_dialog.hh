// generated 2004/2/19 16:50:40 CET by jacek@jaceksdell.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to info_dialog.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _INFO_DIALOG_HH
#  include "info_dialog_glade.hh"
#  define _INFO_DIALOG_HH
class info_dialog : public info_dialog_glade
{
public:
 info_dialog(const std::string t) { text->set_text(t); }  
};
#endif
