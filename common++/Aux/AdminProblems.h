// $Id: AdminProblems.h,v 1.8 2002/10/24 14:14:30 christof Exp $
/*  libc++: 
 *  Copyright (C) 1998-2001 Adolf Petig GmbH & Co. KG
 *                             written by Malte Thoma
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

#include <string>

class AdminProblems
{
   public:
      AdminProblems(){}

      static void create(const std::string &programm,const std::string &text);
};
