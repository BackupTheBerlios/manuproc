/* $Id: LieferscheinBase.h,v 1.18 2006/10/31 16:03:12 christof Exp $ */
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



#ifndef LIEFERBASE
#define LIEFERBASE
#include<Instanzen/ppsInstanz.h>
#include <Misc/fixedpoint.h>
#include <Misc/UniqueValue.h>

class ArtikelBase;
class LieferscheinBase
{
 protected:
   cH_ppsInstanz instanz;
  	int lieferid;	
	static const int _illegal=-1;
	
public:	
 	typedef enum {LART_LS='L',LART_RUECK='R',LART_NONE=0} LsArt; 
 	
	static const UniqueValue::value_t trace_channel;

 	typedef fixedpoint<ManuProC::Precision::LieferscheinMenge> mengen_t;
	typedef int ID;
	static const ID none_id=_illegal;
	
	LieferscheinBase() : instanz(ppsInstanzID::None), lieferid(none_id),
				lagerid(1) {}
	LieferscheinBase(const cH_ppsInstanz& _instanz,int lid) 
	     : instanz(_instanz), lieferid(lid), lagerid(1) {}

	ID Id() const {return lieferid;}
   cH_ppsInstanz Instanz() const {return instanz; }
   bool valid() const { return Id()!=none_id; }
   
   int lagerid; // Lager aus dem die Lieferung abgebucht werden soll.
};

#endif
