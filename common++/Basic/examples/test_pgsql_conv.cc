// $Id: test_pgsql_conv.cc,v 1.5 2002/07/05 12:35:01 christof Exp $
// $ID$
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


#include <Misc/Zeitpunkt_new.h>
#include <ostream/c++ostream.h>

int main()
{  const char *ts="1999-08-20 06:33:50+02";
//   std::cout << ts << " = " 
//   	<< time_t_wrapper(Zeitpunkt_new(PostgresTimestamp(ts))) << '\n';
   return 0;
}
