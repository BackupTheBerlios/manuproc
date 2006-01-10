// $Id: window_kundengruppen.cc,v 1.6 2006/01/10 10:33:11 christof Exp $

#include "config.h"
#include "window_kundengruppen.hh"
#include <Kunde/Kundengruppe.h>
#include <Kunde/Kunde.h>
#include <Misc/EntryValueIntString.h>
#include <expander_hook.h>
#include <Misc/Transaction.h>

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

enum kg_STcols
{ SP_NR, SP_NAME, SP_VORNAME, SP_ORT, SP_LAND, SP_SORTNAME, SP_PLZ, 
  SP_7, SP_8, SP_9,
  SP_ANZ
};

struct KGdata : RowDataBase
{ cH_Kunde k;
  KGdata(cH_Kunde const& d) : k(d) {}
  cH_EntryValue Value(guint _seqnr, gpointer gp) const
  { switch (_seqnr)
    { case SP_NR: return cH_EntryValueIntString(k->Id());
      case SP_NAME: return cH_EntryValueIntString(k->firma());
      case SP_VORNAME: return cH_EntryValueIntString(k->name2());
      case SP_PLZ: return cH_EntryValueIntString(k->plz().empty()?k->postfachplz():k->plz());
      case SP_LAND: return cH_EntryValueIntString(k->land()->Name());
      case SP_SORTNAME: return cH_EntryValueIntString(k->sortname());
      case SP_ORT: return cH_EntryValueIntString(k->ort());
      default: return cH_EntryValue();
    }
  }
};

void window_kundengruppen::laden()
{ // .... 
  Transaction t;
  Query q("gruppen","select kundennr,exists(select true from ku_gruppen_map "
        "where kunden.kundennr=ku_gruppen_map.kundennr and grpnr=?) from kunden");
  q << gruppe->get_value();
  kundein->getModel().clear();
  anderekunden->getModel().clear();
  Query::Row is;
  while ((q>>is).good())
  { int kdnr; bool drin;
    is >> kdnr >> drin;
    if (drin) kundein->getModel().push_back(new KGdata(kdnr));
    else anderekunden->getModel().push_back(new KGdata(kdnr));
  }
}

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
