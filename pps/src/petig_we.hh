// generated 2004/8/30 13:51:10 CEST by jacek@jaceksdell.(none)
// using glademm V2.6.0_cvs
//
// newer (non customized) versions of this file go to petig_we.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _PETIG_WE_HH
#  include "petig_we_glade.hh"
#  define _PETIG_WE_HH
#include <Artikel/ArtikelBase.h>
#include <vector>
#include <string>

class petig_we : public petig_we_glade
{
 struct we_entry {
   ArtikelBase	artikel;
   std::vector<std::string> artbez;
   int stueck;
   std::string auftrag_referenz;
   we_entry(const std::vector<std::string> a, int s, 
            const std::string auf) : artbez(a),stueck(s),
              auftrag_referenz(auf) {}
   we_entry() : stueck(0) {}
   };
 std::vector<struct we_entry> we_ls;
  
        void on_petig_we_ok_clicked();
        void on_petig_we_cancel_clicked();  
};
#endif
