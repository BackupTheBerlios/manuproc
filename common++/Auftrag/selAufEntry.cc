// $Id: selAufEntry.cc,v 1.2 2001/06/22 09:42:44 christof Exp $
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

#include"selAufEntry.h"


SelectedAufentry::SelectedAufentry(int _instanz, int aufid, int znr, int knr)
: instanz(_instanz), auftragid(aufid), zeilennr(znr), kdnr(knr) 
{
}

void SelectedAufList::insert(int instanz, int aid, int znr, int knr)
{
 aufidliste.push_back(SelectedAufentry(instanz,aid, znr, knr));
}

void SelectedAufList::delEntry(int instanz, int aid, int znr)
{
 vector<SelectedAufentry>::iterator i =  aufidliste.begin();

 while(i!=aufidliste.end())
  {
   if((*i).getAuftragid()==aid && (*i).Instanz()==instanz)
      if((*i).getZeilennr()==znr)  
         {aufidliste.erase(i);
          break;
         }
   ++i;
  }
}


void SelectedAufList::delAuftrag(int instanz,int aid)
{
 vector<SelectedAufentry>::iterator i;

 for(i=aufidliste.begin();;)
  {
   if(i!=aufidliste.end())
     {
      if((*i).getAuftragid()==aid && (*i).Instanz()==instanz)
       {aufidliste.erase(i);
        i = aufidliste.begin();
        }
      else
       ++i;
     }
   else
    break;
  }
}


