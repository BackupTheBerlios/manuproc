// $Id: Bindung.cc,v 1.9 2002/11/07 07:49:57 christof Exp $
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

#include "Bindung.hh"

std::ostream& operator<< (std::ostream& os, const Bindung& b)
{
   os << b.id  << ": " << b.getCName();
   return os;
}

bool Bindung::operator== (const Bindung& b) const
{
   return ((name == b.name) && (id == b.id));
}

bool Bindung::operator!= (const Bindung& b) const
{
   return ((name != b.name) || (id != b.id));
}

bool Bindung::haveExtraFunction(ExtraFunc ex) const
{
 Efs::const_iterator i=ef.find(ex);
 if(i==ef.end()) return false;
 return true;
}

int Bindungsliste::getIdByName (const char *name) const
{
   std::vector<Bindung>::const_iterator i = find (liste.begin(), liste.end(), name);
   if (i != liste.end()) return (*i).getId();
   return 0;
}

Bindung Bindungsliste::getByName (const std::string n) const
{
   std::vector<Bindung>::const_iterator it = find (liste.begin(), liste.end(), n);
   if (it != liste.end()) return (*it);
   return Bindung();
}

Bindung Bindungsliste::getById (const int i) const
{
   std::vector<Bindung>::const_iterator it = find (liste.begin(), liste.end(), i);
   if (it != liste.end()) return (*it);
   return Bindung();
}


void Bindungsliste::print_out() const
{
   ostream_iterator<Bindung> oit (std::cout, "\n");
   copy (liste.begin(), liste.end(), oit);
}
