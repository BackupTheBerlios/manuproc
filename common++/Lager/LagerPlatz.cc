/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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
 
#include "LagerPlatz.hh"
#include <cstdio>

void LagerPlatz::DarstellungSetzen()
{
#if defined PETIG_EXTENSIONS && defined MANUPROC_DYNAMICENUMS_CREATED
  if(instanz == ppsInstanzID::Bandlager) darstellung=eInt;
  else if(instanz == ppsInstanzID::Rohwarenlager) darstellung=eString;
  else
#endif  
       assert(!"LagerPlatz::DarstellungSetzen: Falsche Instanz");
}


bool LagerPlatz::eString_valid()
{
#ifdef PETIG_EXTENSIONS
  int regal; char platz; int ebene;
  std::string s=s_position;
  const char *sp(s.c_str()),*np;
   // ignore whitespace???
   if (!isdigit((unsigned char)*sp)) return false;
   regal=strtol(sp,(char**)&np,10);
   if (!isalpha((unsigned char)*np)) return false;
   platz=toupper((unsigned char)*np);
   sp=np+1;
   if (!isdigit((unsigned char)*sp)) return false;
   ebene=strtol(sp,(char**)&np,10);
   while (isspace((unsigned char)*np)) np++;
   if (*np) return false;
   if (regal<1 || regal>15) return false;
   if (ebene<1 || ebene>6) return false;
   if (platz<'A' || platz>'P') return false;
   char buf[30];
   snprintf(buf,sizeof buf,"%02d%c%d",regal,platz,ebene);
   s_position=buf;
   return true;
#endif
}

bool LagerPlatz::eInt_valid()
{
  return true;
}
