// $Id: EAN.cc,v 1.7 2002/10/24 14:08:59 thoma Exp $
/*  libKomponenten: GUI components for ManuProC's libcommon++
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG
 *                             written by Christof Petig and Malte Thoma
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
    
#include "EAN.h"
#include <Aux/itos.h>

std::string EAN::add_Pruefsumme(std::string ean,unsigned int length)
{
  assert(ean.size()==length); 
  int s=0;
  for(int i=length-1;i>=0;i--)
    {
      s+= (ean[i]-'0') * (i & 1 ? 3 : 1); 
    }
  if (!(s %= 10)) s = 10;
  ean+= '0' + 10 - s;
  return ean;                
}    
                 