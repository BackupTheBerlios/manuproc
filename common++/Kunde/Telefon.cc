// $Id: Telefon.cc,v 1.4 2002/09/27 06:43:29 thoma Exp $
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

#include"Telefon.h"
#include<Aux/Transaction.h>
#include<Aux/itos.h>

cH_Telefon::cH_Telefon(const Telefon::ID _tnr)
{
 *this = cH_Telefon(new Telefon(_tnr));
}

std::string Telefon::ArtString() const 
{
 switch(nummer.art)
	{
	case TEL_TEL : return("Telefon"); break;
	case TEL_FAX : return("Fax"); break;
	case TEL_MOB : return("Mobil"); break;
	case TEL_E_MAIL : return("e-mail"); break;
	case TEL_HOMEPAGE : return("Homepage"); break;
	default : return("-");
	}
 return("-");
}

std::string Telefon::NummerStr() const
{
  return string("(+")+itos(nummer.land)+" "+itos(nummer.vorwahl)+") "+itos(nummer.nummer)+
  	((nummer.durchwahl ==-1) ? "" : ("-"+itos(nummer.durchwahl)));
}



std::vector<pair<std::string,TelArt> > Telefon::getTelArtVec()
{
  vector<pair<std::string,TelArt> > V;
  V.push_back(pair<std::string,TelArt>("Telefon",TEL_TEL));
  V.push_back(pair<std::string,TelArt>("Fax",TEL_FAX));
  V.push_back(pair<std::string,TelArt>("Mobil",TEL_MOB));
  V.push_back(pair<std::string,TelArt>("e-mail",TEL_E_MAIL));
  V.push_back(pair<std::string,TelArt>("Homepage",TEL_HOMEPAGE));
  return V;
}


