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
#include <Artikel/ArtikelMengeSumme.h>
#include <Lieferschein/LieferscheinEntry.h>
#include <Auftrag/AuftragBase.h>
#include "auftrag_lieferschein_classes.h"

class petig_we : public petig_we_glade
{
 struct we_entry {
   ArtikelBase	artikel;
   std::vector<std::string> artbez;
   int stueck;
   bool zi;
//   LieferscheinEntry::zusaetze_t zinfo;
   LieferscheinEntryBase lseb;	// Lieferschein des Lieferanten
   ArtikelMenge::mengen_t menge;
   std::map<std::string,int> auftrag_referenz; // youraufnr zu menge
                                               // kommt vom Zusatzinfo
   std::string first_auf_ref;		// from lieferschentry, not zusatzinfo                                               
   we_entry() : stueck(0),zi(false),menge(0) {}
   };
 std::vector<struct we_entry> we_ls;
 
        void on_petig_we_ok_clicked();
        void on_petig_we_cancel_clicked();  
 void identify_article() throw(SQLerror); 

 cH_Data_Lieferoffen getHandleForAufEntry(AuftragBase::ID abid, 
                         ArtikelBase::ID artid,
                         const std::string youraufnr);
};
#endif
