// $Id: Webangaben.hh,v 1.5 2002/10/09 14:48:07 thoma Exp $
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

#ifndef WEBFADEN_HH
#define WEBFADEN_HH

#include <Artikel/ArtikelBase.h>
#include <Aux/Datum.h>
#include <Aux/fixedpoint.h>
#include <Faeden/Faden.hh>
#include <Faeden/Schussfaeden.hh>

class Webangaben
{
   public:
      typedef fixedpoint<1> schussdichte_t;
   private:
      ArtikelBase artikel;
      Petig::Datum erstellt,geaendert;
      std::string riet,bemerkung;
      ArtikelBase fangfaden;
      schussdichte_t schussdichte; // in cm
      Schussfaeden schussfaeden;

   public:
      Webangaben() {}
      Webangaben(ArtikelBase a) : artikel(a) {}

      void create_if_not_exists() throw(SQLerror);
      bool exists() throw(SQLerror);
      bool Load() throw(SQLerror); // return false falls keine Webangaben existieren
      void LoadSchussfaeden() throw(SQLerror);

      void set_datum(const Petig::Datum &datum=Petig::Datum()) throw(SQLerror);      
      void set_bemerkung(const std::string &s) throw(SQLerror);
      void set_riet(const std::string &s) throw(SQLerror);
      void set_fangfaden(const ArtikelBase &ab) throw(SQLerror);
      void set_schussdichte(const schussdichte_t &f) throw(SQLerror);
      void set_Laengen(int kettlaenge,int schnittlaenge);

      const ArtikelBase Artikel() const {return artikel;}
      const Petig::Datum Erstellt() const {return erstellt;}
      const Petig::Datum Geaendert() const {return geaendert;}
      const std::string& Riet() const {return riet;}
      const std::string& Bemerkung() const {return bemerkung;}
      const ArtikelBase& Fangfaden() const {return fangfaden;}
      const schussdichte_t& Schussdichte_cm() const {return schussdichte;}
      Schussfaeden& getSchussfaeden() {return schussfaeden;}

};

#endif
