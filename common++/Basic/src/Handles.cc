// $Id: Handles.cc,v 1.5 2003/05/26 14:40:10 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <Misc/Handles.h>
#if defined DEBUG_HANDLE_CONTENT || defined DEBUG_HANDLES
#include <iostream>
#include <typeinfo>

HandleContent::~HandleContent()
{  
   if (_references) 
   {  std::cerr << "Handled Object @"<<(void*)this<<" ("<<typeid(*this).name() <<") still referenced "<<_references<<" times on destruction\n";
      if (getenv("MANUPROC_PICKY_HANDLES")) abort();
   }
#ifdef DEBUG_HANDLE_CONTENT      
   if (_watch_me)
      std::cerr << "Handled Object @"<<(void*)this<<" ("<<typeid(*this).name() <<") destruction\n";
#endif      
}

#endif
