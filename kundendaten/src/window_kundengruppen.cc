// $Id: window_kundengruppen.cc,v 1.3 2006/01/10 10:32:59 christof Exp $

#include "config.h"
#include "window_kundengruppen.hh"
#include <Kunde/Kundengruppe.h>
#include <expander_hook.h>

void window_kundengruppen::on_gruppenwahl_activate()
{  
}

void window_kundengruppen::neu()
{  
}

void window_kundengruppen::loeschen()
{  
}

void window_kundengruppen::entfernen()
{  
}

void window_kundengruppen::add()
{  
}

void window_kundengruppen::laden()
{ // .... 
}

enum kg_STcols
{ SP_NR, SP_NAME, SP_VORNAME, SP_ORT, SP_LAND, SP_SORTNAME, SP_PLZ, 
  SP_7, SP_8, SP_9,
  SP_ANZ
};

template <bool B>
struct kg_STprops : SimpleTreeModel_Properties
{ unsigned Columns() const { return SP_ANZ; }
  Glib::ustring Title(guint _seqnr) const
  { switch(_seqnr)
    { case SP_NR: return "Nr.";
      case SP_NAME: return "Name";
      case  SP_VORNAME: return "Vorname";
      case  SP_ORT: return "Ort";
      case  SP_LAND: return "Land";
      case  SP_SORTNAME: return "Abkz.";
      case  SP_PLZ: return "PLZ";
      default: return "";
    }
  }
  std::string ProgramName() const { return "kundendaten"; }
  std::string InstanceName() const { return B?"gruppen1":"gruppen0"; }
};

window_kundengruppen::window_kundengruppen(int id)
{ gtk_expander_hook_size_to_parent(expander_nicht->gobj());
  expander_nicht->set_expanded(false);
  gruppe->set_value(Kundengruppe::ID(id));
  kundein->setProperties(*new kg_STprops<true>(),true);
  anderekunden->setProperties(*new kg_STprops<false>(),true);
  laden();
}
