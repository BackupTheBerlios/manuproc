/* $Id: Einheiten.h,v 1.14 2002/10/24 14:06:49 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

// $Id: Einheiten.h,v 1.14 2002/10/24 14:06:49 thoma Exp $

#ifndef ARTIKEL_EINHEITEN_H
#define ARTIKEL_EINHEITEN_H

#include <Artikel/ArtikelBase.h>
#include <Aux/SQLerror.h>
#include <Aux/CacheStatic.h>
//#include <Aux/Handles.h>
#include <vector>
#include <DynamicEnums/DynamicEnums.h>

namespace EinheitID=ManuProC::DynamicEnums::Einheiten;

class Einheit //: public HandleContent
{
public:
	typedef EinheitID::enum_t ID;
	static const ID default_id=EinheitID::St_;
	static const ID Stueck=EinheitID::St_; // Compatibility

private:
	ID einh;
	std::string bez_anzahl,tex_anzahl, bez_menge,tex_menge;
	
	typedef CacheStatic<ID,Einheit> cache_t;
	static cache_t cache;

	// nicht auskommentieren !!! wichtige Fehlerabfrage
	Einheit(int x); // not defined, use ID!
	
	Einheit() : einh(default_id) {}
	friend class std::map<ID,Einheit>;

public:
	Einheit(const ArtikelBase &ab) throw(SQLerror);
	Einheit(const ID e);
	ID Id() const { return einh; }
	operator ID() const
	{ return einh;
	}
	bool operator!=(const ID b) const
	{ return einh!=b; }
	bool operator==(const ID b) const
	{ return einh==b; }

	std::string Bezeichnung() const
	{ return (std::string)(*this); }
	operator std::string() const;
	std::string StueckEinheit() const
	{  return bez_anzahl; }
	std::string MengenEinheit() const
	{  return bez_menge; }
	std::string StueckEinheit_TeX() const
	{  return tex_anzahl; }
	std::string MengenEinheit_TeX() const
	{  return tex_menge; }
	std::string TeX() const;
	
	bool hatAnzahl() const
	{  return true; } // hmmm, sobald ich es jemals brauche wird es geändert CP 
	bool hatMenge() const
	{  return !bez_menge.empty();
	}
	
        static std::vector<Einheit> alleEinheiten();
};
/*
class cH_Einheit : public Handle<const Einheit>   
{
  typedef CacheStatic<Einheit::ID,cH_Einheit> cache_t; 
  static cache_t cache;
  cH_Einheit(const Einheit *s) : Handle<const Einheit>(s) {};
  friend class std::map<Einheit::ID,cH_Einheit>;
  cH_Einheit(){};
 public:
  cH_Einheit(const Einheit::ID id) ;
};
*/

#endif
