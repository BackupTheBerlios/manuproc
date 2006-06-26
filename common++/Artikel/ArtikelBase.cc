/* $Id: ArtikelBase.cc,v 1.7 2006/06/26 07:53:01 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include<Artikel/ArtikelBase.h>
#include<Artikel/ArtikelBezeichnung.h>
#include <Misc/Query.h>

Query::Row& operator>>(Query::Row& is,ArtikelBase &AB)
{
  int a;
  int ind;
  is >> Query::Row::WithIndicator(a,ind);
  if (!ind) AB=ArtikelBase(a);
  else AB=ArtikelBase();
  return is;
}

ArgumentList& operator<<(ArgumentList &is,const ArtikelBase &a)
{  if (!a) return is << Query::null_s(Query::NullIf_s<int>::postgres_type);
   return is << a.Id();
}

std::ostream &operator<<(std::ostream &o,const ArtikelBase &a)
{
  o << cH_ArtikelBezeichnung(a)->Bezeichnung();
  return o;
}
