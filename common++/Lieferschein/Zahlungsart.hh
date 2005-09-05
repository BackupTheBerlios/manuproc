// $Id: Zahlungsart.hh,v 1.4 2005/09/05 16:28:37 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef ZAHLUNGSART__
#define ZAHLUNGSART__

#include <string>
#include <Misc/Handles.h>
#include <Misc/CacheStatic.h>  

#warning ich glaub die Datei kann jetzt weg MAT 15.5.02
/*
class Zahlungsart  : public HandleContent
{
        std::string kurzbezeichnung;
        std::string bezeichnung;
 protected:
        int id;
        static const int _illegal=-1;
 public:
        typedef int ID;
        static const ID none_id=_illegal;
        
        Zahlungsart() : id(none_id) {}
        Zahlungsart(int _id);

        ID Id() const {return id;}
        std::string Kurzbezeichnung() const {return kurzbezeichnung;}
        std::string Bezeichnung() const {return bezeichnung;}
};
  
class cH_Zahlungsart : public Handle<const Zahlungsart>
{
     typedef CacheStatic<Zahlungsart::ID,cH_Zahlungsart> cache_t;
     static cache_t cache;
     cH_Zahlungsart(const Zahlungsart *s) : Handle<const Zahlungsart>(s) {};
     friend class std::map<Zahlungsart::ID,cH_Zahlungsart>;
  public:
//     cH_Zahlungsart(){} //{*this=new Zahlungsart();}
     cH_Zahlungsart(){*this=new Zahlungsart();}
     cH_Zahlungsart(Zahlungsart::ID _id);

//     bool operator==(const cH_Zahlungsart &b) const {return (*this)->Id()==b->Id();}
};
*/  
  
#endif

