/* $Id: Zahlvorgang.cc,v 1.2 2001/07/05 09:23:02 christof Exp $ */
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

#include "Zahlvorgang.h"
#include "Bankauftrag.h"
#include <Aux/string0.h>

Zahlvorgang::Zahlvorgang(long zahlblz, long long zahlkonto,
                long betrag /* in Pf/Cent */, bool euro,
                std::vector<string> zahlname, std::vector<string> zweck,
                std::vector<string> myname) throw(Datenfehler)
{  BLZ=zahlblz;
   Konto=zahlkonto;
   Betrag=betrag;
   Euro=euro;
   Name.insert(Name.end(),zahlname.begin(),zahlname.end());
   Verwendungszweck.insert(Verwendungszweck.end(),zweck.begin(),zweck.end());
   Auftraggeber.insert(Auftraggeber.end(),myname.begin(),myname.end());
   // check
   if (BLZ<10000000 || BLZ >=90000000) throw Datenfehler("BLZ falsch");
   if (Euro) throw Datenfehler("Euro geht noch nicht");
   if (Name.size()>1) throw Datenfehler("Es muß (noch) genau ein Name angegeben sein");
//   if (Auftraggeber.size()!=1) throw Datenfehler("Es muß (noch) genau ein Auftraggeber angegeben sein");
   if (Verwendungszweck.size()>4) throw Datenfehler("Zu viele Verwendungszwecke");
}

Zahlvorgang::Zahlvorgang(long zahlblz, long long zahlkonto,
                long betrag /* in Pf/Cent */, bool euro,
                std::string zahlname, std::string zweck, std::string myname) throw(Datenfehler)
{  BLZ=zahlblz;
   Konto=zahlkonto;
   Betrag=betrag;
   Euro=euro;
   Name.push_back(zahlname);
   Verwendungszweck.push_back(zweck);
   Auftraggeber.push_back(myname);
}




#if 0
Zahlvorgang::
int Zahlvorgang::packeVerwendung()
{  // Verwendungszweck so dicht als möglich packen
   // gibt Anzahl der Zeilen zurück
   return 0
}
#endif

void Zahlvorgang::fillBuffer(char *buf,int num,long eigeneBLZ,
	long long eigenesKonto,std::string eigenerName,char typ) const throw()
{  switch (num)
   {  case 0:
   /* Datensatz C1 */
         snprintf0(buf,sizeof buf,"%04dC%08ld",187+29*extraZeilen(),eigeneBLZ); /* C3 */
         snprintf0(buf+13,sizeof(buf)-13,"%08ld",BLZ); /* C4 */
         snprintf0(buf+21,sizeof(buf)-21,"%010llu",Konto); /* C5 */
         memset(buf+31,'0',13); /* C6 */
         memcpy(buf+44,typ=='G'?"51000":"05000",5); /* C7a+b */
         buf[49]=' ';  /* C8 */
         snprintf0(buf+50,sizeof(buf)-50,"0%010lu",Betrag); /* C9 */
         snprintf0(buf+61,sizeof(buf)-61,"%08ld",eigeneBLZ); /* C10 */
         snprintf0(buf+69,sizeof(buf)-69,"%010llu",eigenesKonto); /* C11 */ 
         /* EURO Umrechnung - hat NULL zu enthalten */
         memset(buf+79,'0',11); /* C12 */
         memcpy(buf+90,"   ",3);  /* C13 */
         Bankauftrag::string2Bank(buf+93,Name[0]); /* C14a */
         memset(buf+120,' ',8); /* C14b */
         break;
      case 1:
   /* Datensatz C2 */
         if (Auftraggeber.size())
            Bankauftrag::string2Bank(buf,Auftraggeber[0]); /* C15 */
         else
            Bankauftrag::string2Bank(buf,eigenerName);
         Bankauftrag::string2Bank(buf+27,Verwendungszweck[0]); /* C16 */
         snprintf0(buf+54,sizeof(buf)-54,"   %02d",extraZeilen()); /* C17+C18 */
         if (Verwendungszweck.size()>1) 
         { buf[59]='0'; buf[60]='2'; Bankauftrag::string2Bank(buf+61,Verwendungszweck[1]); }
         else memset(buf+59,' ',29); /* C19-C20 */
         if (Verwendungszweck.size()>2) 
         { buf[88]='0'; buf[89]='2'; Bankauftrag::string2Bank(buf+90,Verwendungszweck[2]); }
         else memset(buf+88,' ',29); /* C21-C22 */
         memset(buf+117,' ',11); /* C23 */
         break;
      case 2:
/* Datensatz C3 */      
         buf[0]='0'; buf[1]='2'; 
         Bankauftrag::string2Bank(buf+2,Verwendungszweck[3]); /* C24+C25 */
         memset(buf+29,' ',29*3+12); 
         break;
   }
}
 
int Zahlvorgang::Bloecke() const throw()
{  return (Verwendungszweck.size()>3) ? 3 : 2;
}

int Zahlvorgang::extraZeilen() const throw()
{  return Verwendungszweck.size()>1 ? Verwendungszweck.size()-1 : 0;
}

void Zahlvorgang::toTeX(FILE *f) const throw(IOerror)
{
#if 0
lt=localtime(&u->eingegebenam);
      fprintf(f,"%4d    %-27.27s  %s  "FETT"DM%11s"_FETT"\n"
                "        %-27.27s  %s\n"
                "______  %-27.27s  %s  "FETT"Sko%10s"_FETT"\n"
                "%02d.%02d.  %20s/%6ld  %s\n\n",
                anzahlDatensaetze+1,a1,u->Verwendung[0],numformat3(u->Betrag,2,1),
                a2,u->Verwendung[1],
                a3,u->Verwendung[2],numformat(buf,u->rngBetrag-u->Betrag,2,1),
                lt->tm_mday,lt->tm_mon+1,u->Lieferantenkonto,k?k->newlong:0l,
                u->Verwendung[3]);
#endif                
} 
