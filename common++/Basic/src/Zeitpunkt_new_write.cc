// $Id: Zeitpunkt_new_write.cc,v 1.4 2002/05/06 13:41:22 christof Exp $
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


#include <Aux/Zeitpunkt_new.h>
#include <Aux/string0.h>

void Zeitpunkt_new::write(PostgresTimestamp a) const
{  snprintf0((char*)a,a.Size(),"%04d-%d-%d %d:%02d:%02d%+d",
	datum.Jahr(),datum.Monat(),datum.Tag(),
	prec>=hours?hour:0,prec>=minutes?minute:0,
	prec>=seconds?second:0,minutes_from_gmt/60);
}
