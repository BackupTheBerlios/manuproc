#include "config.h"
#include "Artikeleingabe.hh"
#include <Misc/Query.h>
#include <Misc/EntryValueIntString.h>

void Artikeleingabe::on_standard_einheit_activate()
{ if (aktuelle_gruppe.valid() && Einheit::ID(aktuelle_gruppe.einheit)!=standard_einheit->get_value())
  {  was_tun->set_value(""); aktuelle_gruppe=Artikelgruppe(); 
  }
  artikelbox_neu->set_Default_Einheit(standard_einheit->get_value());  
}

void Artikeleingabe::on_optionmenu_standardinstanz_activate()
{ if (aktuelle_gruppe.valid() && ppsInstanz::ID(aktuelle_gruppe.bestellen_bei)!=standard_instanz->get_value()->Id())
  {  was_tun->set_value(""); aktuelle_gruppe=Artikelgruppe(); }
  artikelbox_neu->set_Default_Instanz(standard_instanz->get_value()->Id());
}

void Artikeleingabe::artikelbox_neu_activate()
{  if(aktuelle_gruppe.valid() 
	&& (ExtBezSchema::ID(aktuelle_gruppe.schema)!=artikelbox_neu->getBezSchema()->Id()
	|| ArtikelTyp::ID(aktuelle_gruppe.warengruppe)!=artikelbox_neu->getBezSchema()->Typ()))
   {  was_tun->set_value(""); aktuelle_gruppe=Artikelgruppe(); 
   }
   artikelbox->set_value(artikelbox_neu->get_value());
   artikelbox_activate();
}

FetchIStream &operator>>(FetchIStream &is,struct Artikelgruppe &ag)
{  return is >> ag.schema >> ag.warengruppe >> ag.bestellen_bei >> ag.einheit;
}

void Artikeleingabe::on_was_tun_activate()
{  try
   {  Artikelgruppe a=Query("select schema,warengruppe,bestellen_bei,einheit from "
	"artikelgruppen where bezeichnung='"+was_tun->get_value()+"'")
	.FetchOne<Artikelgruppe>();
      artikelbox_neu->setExtBezSchema(cH_ExtBezSchema(ExtBezSchema::ID(a.schema),ArtikelTyp::ID(a.warengruppe)));
      standard_einheit->set_value(Einheit::ID(a.einheit));
      artikelbox_neu->set_Default_Einheit(Einheit::ID(a.einheit));  
      if(a.bestellen_bei==ppsInstanzID::None)
        {standard_instanz->set_sensitive(false);
         no_instanz->set_active(true);
        }
      else
        standard_instanz->set_value(ppsInstanz::ID(a.bestellen_bei));

      artikelbox_neu->set_Default_Instanz(ppsInstanz::ID(a.bestellen_bei));
      if (standard_instanz->get_value()->Id()!=ppsInstanz::ID(a.bestellen_bei))
      {  standard_instanz->set_mode(Optionmenu_Instanz::allesAnzeigen);
         standard_instanz->set_value(ppsInstanz::ID(a.bestellen_bei));
      }
      aktuelle_gruppe=a;
      artikelbox_neu->grab_focus();
   }
   catch (SQLerror &e)
   {  std::cerr << "on_was_tun_activate: " << e << '\n';
   }
}

void Artikeleingabe::neuenArtikelAnlegen(unsigned warengruppe,unsigned schema,
                const std::vector<std::string>&entries)
{  std::string gruppe=(Query("select bezeichnung from artikelgruppen "
        "where (warengruppe,schema)=(?,?) order by bezeichnung limit 1")
        << warengruppe << schema
	).FetchOne<std::string>();
   was_tun->set_value(gruppe);
   on_was_tun_activate();
   std::vector<cH_EntryValue> evv;
   for (std::vector<std::string>::const_iterator i=entries.begin();i!=entries.end();++i)
     evv.push_back(cH_EntryValueIntString(*i));
   artikelbox_neu->set_content(evv,0);
   artikelbox_neu->grab_focus();
}
