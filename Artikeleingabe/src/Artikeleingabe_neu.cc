// $Id: Artikeleingabe_neu.cc,v 1.4 2005/10/19 13:39:50 christof Exp $
/*  Artikeleingabe: ManuProC's article management program
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

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
      { standard_instanz->set_value(ppsInstanz::ID(a.bestellen_bei));
        standard_instanz->set_sensitive(true);
      }

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
                const std::map<int,std::vector<std::string> >&entries_map)
{  std::string gruppe=(Query("select bezeichnung from artikelgruppen "
        "where (warengruppe,schema)=(?,?) order by bezeichnung limit 1")
        << warengruppe << schema
	).FetchOne<std::string>();
   was_tun->set_value(gruppe);
   on_was_tun_activate();
   ArtikelBoxAnzeige anz(cH_ExtBezSchema(schema,warengruppe));
   for (::std::map<int,std::vector<std::string> >::const_iterator j=entries_map.begin();
         j!=entries_map.end();++j)
     anz.Sichtbar(j->first,true);
   artikelbox_neu->setzeAnzeige(anz);
   for (::std::map<int,std::vector<std::string> >::const_iterator j=entries_map.begin();
         j!=entries_map.end();++j)
   { std::vector<cH_EntryValue> evv;
     for (std::vector<std::string>::const_iterator i=j->second.begin();i!=j->second.end();++i)
       evv.push_back(cH_EntryValueIntString(*i));
     try
     { artikelbox_neu->set_content(evv,artikelbox_neu->getSignifikanzPos(j->first));
     } catch (...) {}
   }
   artikelbox_neu->grab_focus();
}
