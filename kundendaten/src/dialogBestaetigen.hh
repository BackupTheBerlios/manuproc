// generated 2001/3/29 11:37:38 CEST by jacek@mimi.
// using glademm V0.5_11f_cvs
//
// newer (non customized) versions of this file go to dialogBestaetigen.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _DIALOGBESTAETIGEN_HH
#  include "dialogBestaetigen_glade.hh"
#  define _DIALOGBESTAETIGEN_HH
#include "glademm_support.hh"
#include <Kunde/Kunde.h>

class dialogBestaetigen : public dialogBestaetigen_glade
{   
        
        friend class dialogBestaetigen_glade;
        void newrng_ok();
        void newrng_cancel();
public:
	const Kunde::ID getNewKdNr() const 
		{ return atoi(neuekdnr->get_text().c_str()); }
};
#endif
