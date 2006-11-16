// $Id: auftrag_serialize.cc,v 1.4 2006/11/16 15:31:38 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 1998-2002 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include <iostream>
#include <Misc/TagStream.h>
#include <Auftrag/AuftragFull.h>
#include <Misc/dbconnect.h>
#include <Misc/exception.h>

int main(int argc,char *argv[])
{  if (argc<3) 
   {  std::cerr << "Usage: " << argv[0] << " <instanz> <id> [<erteilung>]\n";
      return 1;
   }
   ppsInstanz::ID instanz=ppsInstanz::ID(atoi(argv[1]));
   int id=atoi(argv[2]);
   if (int(instanz)<1 || id<0) return 2;

   ManuProC::PrintUncaughtExceptions();
   ManuProC::dbconnect();

   AuftragFull auf(AuftragBase(instanz,id));
   TagStream ts;
   ts.setContent(Tag("serialize"));
   serialize(ts.getContent(),auf,argc>3);
   ts.write(std::cout);
   ManuProC::dbdisconnect();
   return 0;
}
