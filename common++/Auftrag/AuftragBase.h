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

#ifndef AUFTRAGBASE_H
#define AUFTRAGBASE_H
#include<Aux/ppsInstanz.h>
#include <Aux/Datum.h>
#include <Artikel/ArtikelBase.h>

// hier sollten noch viel mehr Funktionen aus Auftrag rein !!!

class AuftragBase
{
 protected:
        cH_ppsInstanz instanz; 	
 	int auftragid;	
 public:
	AuftragBase() : instanz(ppsInstanz::INST_NONE), auftragid(0) {}
	AuftragBase(cH_ppsInstanz _instanz, int aufid=0) throw() : instanz(_instanz), auftragid(aufid) {}
	int Id() const {return auftragid;}
        void set_Id(int i) {auftragid = i;}
        ppsInstanz::ID Instanz() const {return instanz->Id(); }
	bool valid() const { return auftragid!=0; }

        // gibt Zeilennummer zur�ck, MAT
        int insertNewEntry(unsigned long int bestellt, 
                Petig::Datum lieferdatum, ArtikelBase::ID artid) throw(SQLerror);
};

#endif
