// $Id: Webangaben.cc,v 1.13 2004/02/26 10:46:08 christof Exp $
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


#include "Webangaben.hh"
#include <Misc/FetchIStream.h>
#include <Misc/itos.h>

void Webangaben::set_Laengen(int kettlaenge,int schnittlaenge)
{ Query("update webangaben set kettlaenge=?, schnittlaenge=? where artikel=?")
      << kettlaenge << schnittlaenge << artikel.Id();
}

void Webangaben::create_if_not_exists() throw(SQLerror)
{  if(!exists())
        Query("insert into webangaben (artikel) values (?)")
        	<< artikel.Id();
}

bool Webangaben::exists() throw(SQLerror)
{  int DUMMY;
   
   try
   {  Query("select artikel from webangaben where artikel=?")
   	<< artikel.Id()
   	>> DUMMY;
   }
   catch (SQLerror &e)
   {  return false;
   }
   return true;
}


bool Webangaben::Load() throw(SQLerror)
{  try
   {  Query("select erstellt, geaendert, riet, bemerkungen,"
	"fangfaden,schussdichte "
	"from webangaben where artikel=?")
	<< artikel.Id()
	>> erstellt >> geaendert
	>> FetchIStream::MapNull(riet)
	>> FetchIStream::MapNull(bemerkung)
	>> FetchIStream::MapNull(fangfaden)
	>> FetchIStream::MapNull(schussdichte);
      return true;
   }
   catch (SQLerror &e)
   {  erstellt=geaendert=ManuProC::Datum();
      riet="";
      bemerkung="";
      fangfaden=ArtikelBase();
      schussdichte=0;
   }
   return false;
}

void Webangaben::set_datum(const Petig::Datum &datum) throw(SQLerror)
{  Query("update webangaben set erstellt=?, geaendert=now() "
   	"where artikel=?")
   	<< datum
   	<< Artikel().Id();
   geaendert=datum;
}

void Webangaben::set_bemerkung(const std::string &s) throw(SQLerror)
{  Query("update webangaben set bemerkungen=?, geaendert=now() "
         "where artikel=?")
         << s << Artikel().Id();
   bemerkung=s;
}

void Webangaben::set_riet(const std::string &s) throw(SQLerror)
{  Query("update webangaben set riet=?, geaendert=now() where artikel=?")
   	<< s
   	<< Artikel().Id();
   riet=s;  
}

void Webangaben::set_fangfaden(const ArtikelBase &ab) throw(SQLerror)
{  Query("update webangaben set fangfaden=?, geaendert=now() where artikel=?")
   	<< ab.Id()
   	<< Artikel().Id();
   fangfaden=ab;
}

void Webangaben::set_schussdichte(const fixedpoint<1> &f) throw(SQLerror)
{  Query("update webangaben set schussdichte=?, geaendert=now() "
		"where artikel=?")
	<< f
	<< Artikel().Id();
   schussdichte=f;
}
