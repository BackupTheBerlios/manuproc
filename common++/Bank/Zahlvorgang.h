/* $Id: Zahlvorgang.h,v 1.1 2001/04/23 08:11:58 christof Exp $ */
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

#ifndef ZAHLVORGANG_H
#define ZAHLVORGANG_H
#include "Bank/Datenfehler.h"
#include <vector>
#include <string>
#include <stdio.h>
#include "Aux/IOerror.h"

class Zahlvorgang
{  long BLZ;
   long long Konto;
   long Betrag;
   bool Euro;
   vector<string> Name;
   vector<string> Verwendungszweck;
   vector<string> Auftraggeber;
   
   void fillBuffer(char *buf,int num,long eigeneBLZ,long long eigenesKonto,
   		string eigenerName,char typ) const throw();
   int Bloecke() const throw();
   int extraZeilen() const throw();
   void toTeX(FILE *f) const throw (IOerror);
   
   friend class Bankauftrag;
   
public:
	Zahlvorgang(long zahlblz, long long zahlkonto, 
		long betrag /* in Pf/Cent */, bool euro,
		vector<string> zahlname, vector<string> zweck, 
		vector<string> myname) throw(Datenfehler);
	Zahlvorgang(long zahlblz, long long zahlkonto, 
		long betrag /* in Pf */,
		vector<string> zahlname, vector<string> zweck, 
		vector<string> myname) throw(Datenfehler)
	{  Zahlvorgang(zahlblz,zahlkonto,betrag,false,zahlname,zweck,myname);
	}
	Zahlvorgang(long zahlblz, long long zahlkonto, long betrag, bool euro,
		string zahlname, string zweck, string myname) throw(Datenfehler); 
};
#endif
