/* $Id: Ausgabe_neu.cc,v 1.2 2001/04/30 15:30:26 christof Exp $ */
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

#include "Ausgabe_neu.h"

ostream &Formatiere(ostream &os,unsigned long Zahl, 
		unsigned int Nachkommastellen,
		unsigned int Ziellaenge,
		const char *TausenderTrennzeichen,
		const char *Komma, char fuehrendesZeichen)
{  if (!TausenderTrennzeichen) TausenderTrennzeichen="";
   if (!Komma) Komma="";
   
   unsigned int stellen;
   char Zahlbuf[12];
   for (stellen=0;stellen<sizeof(Zahlbuf) && Zahl;Zahl/=10) 
   	Zahlbuf[stellen++]=Zahl%10+'0';
   for (unsigned int i=stellen;i<sizeof(Zahlbuf);i++) Zahlbuf[i]='0';
   if (stellen<Nachkommastellen+1) stellen=Nachkommastellen+1;
   
   unsigned int vorkommastellen=stellen-Nachkommastellen;
   unsigned int separatoren=(vorkommastellen-1)/3;
   if (Nachkommastellen) separatoren+=(Nachkommastellen-1)/3;
   unsigned int naechsterSeparator=vorkommastellen%3;
   if (!naechsterSeparator) naechsterSeparator=3;
   unsigned int laenge=stellen+(Nachkommastellen?strlen(Komma):0)
   	+separatoren*strlen(TausenderTrennzeichen);
   if (laenge>Ziellaenge) Ziellaenge=0;
   
   for (int i=0;i<(int)Ziellaenge-(int)laenge;i++) 
   	os<<fuehrendesZeichen;
   for (unsigned int i=0;i<vorkommastellen;i++) 
   {  if (i==naechsterSeparator) 
      {  os<<TausenderTrennzeichen;
         naechsterSeparator+=3;
      }
      os<<Zahlbuf[stellen-1-i];
   }
   if (Nachkommastellen)
   {  os<<Komma;
      naechsterSeparator=3;
      for (unsigned int i=0;i<Nachkommastellen;i++)
      {  if (i==naechsterSeparator)
         {  os<<TausenderTrennzeichen;
            naechsterSeparator+=3;
         }
         os<<Zahlbuf[Nachkommastellen-1-i];
      }
   }
   return os;
}

string string2TeX(const string s, int flags) throw()
{  unsigned int i;
   bool in_line=false;
   string ret="";

   for (i = 0; i<s.size() ; i++)
   {  switch ((unsigned char)(s[i]))
      {	 case '&':
	 case '%':
	 case '{':
	 case '}':
	 case '[':
	 case ']':
	 case '#':
	 case '_': in_line=true;
	    ret+='\\'; ret+=s[i];
	    break;
	 case '*':
	 case '<':
	 case '>':
	    in_line=true;
	    ret+=string("$")+s[i]+'$';
	    break;
	 case '"': in_line=true;
	    ret+="\\dq{}";
	    break;
	 case 160: in_line=true;
	    ret+=' ';
	    break;
	 case (unsigned char)'µ': in_line=true;
	    ret+="$\\mu$";
	    break;
	 case '\\': in_line=true;
	    ret+="$\\backslash$";
	    break;
	 case '|':
	    if (flags&BARISNEWLINE)
	    {  if ((flags&NEEDCHAR) && !in_line) ret+='~';
	       ret+="\\\\\n";
	       in_line=false;
	    }
	    else { ret+= s[i]; in_line=true; }
	    break;
	 default:
	    ret+= s[i]; in_line=true;
	    break;
      }
   }
   if (!in_line && (flags&NEEDCHAR)) ret+='~';
   return ret;
}

#include <strstream>

const string Formatiere(unsigned long Zahl,
                unsigned int Nachkommastellen,
                unsigned int Ziellaenge,
                const char *TausenderTrennzeichen,
                const char *Komma,char fuehrendesZeichen)
{  ostrstream os;
   Formatiere(os,Zahl,Nachkommastellen,Ziellaenge,TausenderTrennzeichen,Komma,fuehrendesZeichen) 
   	<< char(0);
   const string res(os.str());
   os.freeze(0);
   return res;
}
