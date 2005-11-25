// $Id: Artikeleingabe_neu.cc,v 1.5 2005/11/25 15:24:46 christof Exp $
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
#include <Artikel/ArtikelStamm.h>

void Artikeleingabe::on_standard_einheit_activate()
{ if (aktuelle_gruppe.valid() && Einheit::ID(aktuelle_gruppe.einheit)!=standard_einheit->get_value())
  {  was_tun->set_value(""); aktuelle_gruppe=Artikelgruppe(); 
  }
}

void Artikeleingabe::on_optionmenu_standardinstanz_activate()
{ if (aktuelle_gruppe.valid() && ppsInstanz::ID(aktuelle_gruppe.bestellen_bei)!=standard_instanz->get_value()->Id())
  {  was_tun->set_value(""); aktuelle_gruppe=Artikelgruppe(); }
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
      if(a.bestellen_bei==ppsInstanzID::None)
        {standard_instanz->set_sensitive(false);
         no_instanz->set_active(true);
        }
      else
      { standard_instanz->set_value(ppsInstanz::ID(a.bestellen_bei));
        standard_instanz->set_sensitive(true);
      }

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
{  
   top_notebook->set_current_page(0);
   std::string gruppe=(Query("select bezeichnung from artikelgruppen "
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

ArtikelBase Artikeleingabe::new_adaptor(gpointer _this,cH_ExtBezSchema const& s,
            std::map<int,std::vector<cH_EntryValue> > const& felder)
{ std::map<int,std::vector<std::string> > m;
  for (std::map<int,std::vector<cH_EntryValue> >::const_iterator i=felder.begin();
        i!=felder.end();++i)
  { m[i->first].reserve(i->second.size());
    for (std::vector<cH_EntryValue>::const_iterator j=i->second.begin();
        j!=i->second.end();++j)
      m[i->first].push_back((*j)->getStrVal());
  }
  static_cast<Artikeleingabe*>(_this)->neuenArtikelAnlegen(s->Typ().Id(),s->Id(),m);
}

ArtikelBase Artikeleingabe::Anlegen(gpointer udata,cH_ExtBezSchema const& s,
            std::map<int,std::vector<cH_EntryValue> > const& _felder)
{ std::map<int,std::vector<cH_EntryValue> > felder=_felder;
  Artikeleingabe *_this=static_cast<Artikeleingabe*>(udata);
  try
  { // weed out all empty parts
   reloop:
    for (std::map<int,std::vector<cH_EntryValue> >::iterator i=felder.begin();
      i!=felder.end();++i)
    { bool empty=true;
      for (std::vector<cH_EntryValue>::const_iterator j=i->second.begin();
                j!=i->second.end();++j)
      { if (!(*j)->getStrVal().empty())
        { empty=false;
          break;
        }
      }
      if (empty) { felder.erase(i); goto reloop; }
    }
    
    for (std::map<int,std::vector<cH_EntryValue> >::const_iterator i=felder.begin();
      i!=felder.end();++i)
    { for (ExtBezSchema::const_sigiterator j=s->sigbegin(i->first);j!=s->sigend(i->first);++j)
      { 
// hier (und weiter unten) sollte man irgendwann mal 'ID' und 'EAN-Code' in die 
// Tabellenspalten der jeweiligen Schemata umwandeln.
// Dazu brauchte man dann wohl ein get_[id|ean]_spaltenname()
// in ExtBezSchema, oder?
 /* testen ob ID oder EAN eingetragen => FEHLER */
 // sollte wohl eher angezeigt werden CP
        if (j->spaltenname=="id") 
        { std::cerr<<"FEHLER: ID darf nicht vorgegeben werden: Feld freilassen\n";
          return ArtikelBase();
        }
#ifdef MABELLA_EXTENSIONS      
        if (j->spaltenname=="ean")
        { std::cerr<<"FEHLER: EAN darf nicht vorgegeben werden: Feld freilassen\n";
          return ArtikelBase();
        }
#endif
      }
    }
    ArtikelBase db_id;
    Transaction tr;

    db_id=ArtikelStamm::Anlegen(s->Typ().Id(),s->Id(),
  	_this->standard_instanz->get_value()->Id(),
  	_this->standard_einheit->get_value().Id());
  	
    std::vector<std::string> columns,values;
    for (std::map<int,std::vector<cH_EntryValue> >::const_iterator i=felder.begin();
      i!=felder.end();++i)
    { for (ExtBezSchema::const_sigiterator j=s->sigbegin(i->first);j!=s->sigend(i->first);++j)
        columns.push_back(j->spaltenname);
      for (std::vector<cH_EntryValue>::const_iterator j=i->second.begin();j!=i->second.end();++j)
        values.push_back((*j)->getStrVal());
      assert(columns.size()==values.size());
    }
    ArtikelBezeichnung::Anlegen(s,db_id,columns,values);
#if 0

#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED // Jumborollen anlegen ?
 if (Default_IEP.bestelle_bei==ppsInstanzID::Rollerei)
 {  ArtikelBase::ID jumbo_id=artikel_exist(true);
    if(!jumbo_id)
      {
        db_id_jumbo=ArtikelStamm::Anlegen(schema->Typ().Id(),schema->Id(),
	  	ppsInstanzID::Bandlager,EinheitID::m).Id();
	where=what="";
        where_what(where,what,true);
        assert(where[where.size()-1]==')' && what[what.size()-1]==')');
        where[where.size()-1]=' ';
        what[what.size()-1]=' ';
        ArtikelBezeichnung::Anlegen(schema,ArtikelBase(db_id_jumbo),where,what);
      }
    if (jumbo_id && db_id!=jumbo_id)
       ArtikelBaum::Anlegen(ArtikelBase(db_id),ArtikelBase(jumbo_id),ProzessID::Rollen,get_menge_from_artikelbox());
  }
#endif
#endif
    tr.commit();
    _this->artikelbox_neu->set_value(db_id);
    _this->artikelbox->set_value(db_id);
    _this->artikelbox_activate();
    
#if 0
  new_article_inserted(db_id);
#endif  
    return db_id;
  }catch (SQLerror &e)   {  /*std::cerr << e << '\n';*/}
  return ArtikelBase();
}
