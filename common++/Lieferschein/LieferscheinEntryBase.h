/* $Id: LieferscheinEntryBase.h,v 1.1 2003/07/17 14:04:00 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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


#ifndef LIEFERENTRYBASE_H
#define LIEFERENTRYBASE_H

#include <Lieferschein/LieferscheinBase.h>
#include <BaseObjects/ManuProcEintrag.h>

class Lieferschein;

class LieferscheinEntryBase : public LieferscheinBase
{
public: 
   static const int none_znr = ManuProcEintrag::none_znr;
 protected:
	int zeilennr;

	LieferscheinEntryBase(const LieferscheinBase &lsb)
	: LieferscheinBase(lsb), zeilennr(none_znr) {}
public:
	LieferscheinEntryBase() : zeilennr(none_znr) {}
	LieferscheinEntryBase(const LieferscheinBase &lsb, int znr)
	: LieferscheinBase(lsb), zeilennr(znr) {}
	// unschön, da Ints ...
	LieferscheinEntryBase(cH_ppsInstanz inst, int ls, int znr)
	: LieferscheinBase(inst,ls), zeilennr(znr) {}

 int Zeile() const { return zeilennr; }
 int ZNr() const { return zeilennr; }
};

class ArgumentList;
ArgumentList& operator<<(ArgumentList &,const LieferscheinEntryBase &aeb);

#endif
