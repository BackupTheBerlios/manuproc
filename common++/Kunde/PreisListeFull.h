// $Id: PreisListeFull.h,v 1.21 2005/07/06 10:26:51 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef KUNDE_PREISLISTEFULL_HH
#define KUNDE_PREISLISTEFULL_HH

#include<Misc/SQLerror.h>
#include<Kunde/PreisListe.h>
#include<Artikel/ArtikelBase.h>
#include <Artikel/Preis.h>
#include <Artikel/ArtikelTyp.h>
#include<map>
#include <Misc/EntryValueIntString.h>
#include <Misc/Datum.h>

class Artikelpreis;

class PreisListeFull : public PreisListe
{
public:
 typedef std::pair<ArtikelBase::ID,int> UniqPreis; // Artikel mit Mindestmenge

 struct PayOff {
       Preis p;
       ID pl_parent;
       std::vector<cH_EntryValueIntString> b;
       int sort_sequence;
     };
     
private:
 std::map<UniqPreis,struct PayOff> payoff;
 std::map<int,struct PayOff> sorted_payoff; 
 std::vector<std::string> bez_columns;
 
 void load_preisliste(bool with_artbez,ID id, bool art_in_list, ArtikelTyp at);

public:
  PreisListeFull(ID id, bool art_in_list=true, 
  		ArtikelTyp at=ArtikelTyp::default_ID) throw(SQLerror);
  PreisListeFull(bool with_artbez,ID id, bool art_in_list=true, 
	ArtikelTyp at=ArtikelTyp::default_ID) throw(SQLerror); 
  PreisListeFull() {}
  const std::map<UniqPreis,struct PayOff > &PayOff() const { return payoff; }
  const std::map<int, struct PayOff > &Sort_PayOff() const { return sorted_payoff; }  
  const std::vector<std::string> &Bez_Columns() const { return bez_columns; }  
};


#endif
