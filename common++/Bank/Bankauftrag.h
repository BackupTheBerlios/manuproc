/* $Id: Bankauftrag.h,v 1.9 2003/11/07 11:12:50 jacek Exp $ */
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

#include <vector>
#include <string>
#include <stdio.h>
#include "Misc/IOerror.h"
#include "Bank/Zahlvorgang.h"
#include "Misc/Ausgabe_neu.h"
#include <Misc/SQLerror.h>

#ifndef BANKAUFTRAGH
#define BANKAUFTRAGH

class Bankauftrag
{	FILE *druckerpipe;
	int dtausfile;
	
	long long Kontosumme;
	long long BLZsumme;
	int anzahlDatensaetze;
	long long Betragssumme;
	
	long Kennziffer;
	unsigned long eigeneBLZ;
	long long eigenesKonto;
	std::string eigenerName;
	char Auftragsart; /* 'G' oder 'L' */
	
	int eD_Policy;
	FILE *logfile;

	/// erzeugeDiskette
	int erzeugeDiskette() throw(IOerror);
public :
	Bankauftrag(char _kennz, long myblz, std::string myname,long long mykonto,
						const std::string TeX_cmd)
						throw(IOerror,Datenfehler);
			
	Bankauftrag &operator<<(const Zahlvorgang &z) throw(IOerror);
	void printEinzugListe() throw(IOerror);
	void close(bool nodisk=false) throw(IOerror);
	static void string2Bank(char *buf,const char *s) throw (Datenfehler);
	static void string2Bank(char *buf,std::string s) throw (Datenfehler)
	{  string2Bank(buf,s.c_str()); }

	static const int P_FORMAT=0x4;
	static const int P_CHECK=0x2;
	static const int P_ERASE=0x1;
	void setze_eD_Policy(int pol) { eD_Policy=pol; }
	void setze_logfile(FILE *f) { logfile=f; }
	static const std::string BLZFormat(long BLZ) throw(Datenfehler);
	static const std::string BLZ2Bankname(long BLZ) throw(SQLerror);
	long getKennziffer() const { return Kennziffer; }
};

#endif
