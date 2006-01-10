// $Id: window_kundengruppen.cc,v 1.15 2006/01/10 10:33:35 christof Exp $

#include "config.h"
#include "window_kundengruppen.hh"
#include <Kunde/Kundengruppe.h>
#include <Kunde/Kunde.h>
#include <Misc/EntryValueIntString.h>
#include <expander_hook.h>
#include <Misc/Transaction.h>
#include <Misc/i18n.h>
#include "windowTop.hh"
#include <pwd.h>
#include <unistd.h>
#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
//#include <gtkmm/box.h>

enum kg_STcols
{ SP_NR, SP_NAME, SP_VORNAME, SP_ORT, SP_LAND, SP_SORTNAME, SP_PLZ, 
  SP_KINDER, SP_8, SP_9,
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

void window_kundengruppen::on_gruppenwahl_activate()
{ laden();
}

static std::string Benutzername()
{ struct passwd *p=getpwuid(getuid());
  if (!p) return "?";
  // oder gecos?
  return p->pw_name;
}

void window_kundengruppen::neu()
{ Gtk::Dialog d(_("Neue Kundengruppe anlegen"),*this,true);
  d.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
  d.add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);
  d.get_vbox()->add(*Gtk::manage(new Gtk::Label(_("Wie soll die neue Kundengruppe heißen"))));
  Gtk::Entry e;
  d.get_vbox()->add(e);
  e.set_activates_default();
  d.show_all();
  d.set_default_response(Gtk::RESPONSE_OK);
  if (d.run()==Gtk::RESPONSE_OK)
  { int id=KundengruppeID::None;
    try
    { // why isn't there a class method for this
      Query("select grpnr from ku_gruppe where name=?")
         << e.get_text() >> id;
    }
    catch (SQLerror &er)
    { Transaction tr;
      Query("lock table ku_gruppe in exclusive mode");
      Query("select coalesce(max(grpnr+1),1) from ku_gruppe") >> id;
      Query("insert into ku_gruppe (grpnr,name,obergruppe,kommentar) "
        "values (?,?,?,?)") << id
        << e.get_text() << "Benutzergruppe" 
        << ("erzeugt am "+ManuProC::Datum::today().write_euro()+" durch "
            +Benutzername());
      tr.commit();
      // View anlegen?
      gruppe->register_value(Kundengruppe::ID(id),e.get_text());
    }
    gruppe->set_value(Kundengruppe::ID(id));
    laden();
  }
}

void window_kundengruppen::bearbeiten()
{ cH_RowDataBase sel=kundein->getMenuContext();
  if (!!sel)
  { main->laden(sel.cast_dynamic<const KGdata>()->k);
    main->get_window()->raise();
  }
}

void window_kundengruppen::loeschen()
{ Query("delete from ku_gruppen_map where grpnr=?") << gruppe->get_value();
  Query("delete from ku_gruppe where grpnr=?") << gruppe->get_value();
  gruppe->set_value(KundengruppeID::None);
  laden();
}

void window_kundengruppen::entfernen()
{ kundein->nodes_select_leaves();
  std::vector<cH_RowDataBase> v=kundein->getSelectedRowDataBase_vec(false);
  for (std::vector<cH_RowDataBase>::const_iterator i=v.begin();i!=v.end();++i)
  { kundein->getModel().erase(*i);
    anderekunden->getModel().push_back(*i);
    i->cast_dynamic<const KGdata>()->k.cast_const<Kunde>()->pullFromGrp(gruppe->get_value());
  }
  kundein->get_selection()->unselect_all();
}

void window_kundengruppen::add()
{ anderekunden->nodes_select_leaves();
  std::vector<cH_RowDataBase> v=anderekunden->getSelectedRowDataBase_vec(false);
  for (std::vector<cH_RowDataBase>::const_iterator i=v.begin();i!=v.end();++i)
  { anderekunden->getModel().erase(*i);
    kundein->getModel().push_back(*i);
    i->cast_dynamic<const KGdata>()->k.cast_const<Kunde>()->putInGrp(gruppe->get_value());
  }
  anderekunden->get_selection()->unselect_all();
}

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
  toolbutton_delete->set_sensitive(gruppe->get_value()!=KundengruppeID::None 
      && cH_Kundengruppe(gruppe->get_value())->Obergruppe()=="Benutzergruppe");
}

class kg_SumNode : public TreeRow
{ int kinder;
public:
  virtual void cumulate(const cH_RowDataBase &rd) { ++kinder; }
  virtual void deduct(const cH_RowDataBase &rd) { --kinder; }
  virtual cH_EntryValue Value(guint col,gpointer gp) const
  { if (col!=SP_KINDER) return cH_EntryValue();
    return cH_EntryValueIntString(kinder);
  }
  kg_SumNode(const Handle<const TreeRow> &suminit) : kinder()
  { if (suminit) kinder=suminit.cast_dynamic<const kg_SumNode>()->kinder;
  }
};

template <bool B>
struct kg_STprops : SimpleTreeModel_Properties
{ unsigned Columns() const { return SP_ANZ; }
  Glib::ustring Title(guint _seqnr) const
  { switch(_seqnr)
    { case SP_NR: return _("Nr.");
      case SP_NAME: return _("Name");
      case  SP_VORNAME: return _("Vorname");
      case  SP_ORT: return _("Ort");
      case  SP_LAND: return _("Land");
      case  SP_SORTNAME: return _("Abkz.");
      case  SP_PLZ: return _("PLZ");
      case SP_KINDER: return _("Anzahl");
      default: return "";
    }
  }
  std::string ProgramName() const { return "kundendaten"; }
  std::string InstanceName() const { return B?"gruppen1":"gruppen0"; }
  Handle<TreeRow> create_node(const Handle<const TreeRow> &suminit) const
  { return new kg_SumNode(suminit);
  }
};

window_kundengruppen::window_kundengruppen(windowTop *m, int id) : main(m)
{ gtk_expander_hook_size_to_parent(expander_nicht->gobj());
  expander_nicht->set_expanded(false);
  gruppe->set_value(Kundengruppe::ID(id));
  kundein->setProperties(*new kg_STprops<true>(),true);
  kundein->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
  anderekunden->setProperties(*new kg_STprops<false>(),true);
  anderekunden->get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);
  kundein->addMenuItem(_("Kunde bearbeiten"))
      .connect(sigc::mem_fun(*this,&window_kundengruppen::bearbeiten));
  laden();
}
