// $Id: Rundungsmacros.h,v 1.3 2002/05/09 12:46:00 christof Exp $
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

// Adabas liefert falsch gerundete Werte in floats
// normalisierende Funktionen
#define ROUND0(x) ((int)((x)+0.5))
/* this macro compensates losses on Pentiums (8.7/4) */
#define ROUND0U(x) ((int)((x)+0.5001))
#define ROUND1(x) ((int)((x)*10+0.5)/10.0)
#define ROUND2(x) ((int)((x)*100+0.5)/100.0)
#define ROUND2U(x) ((int)((x)*100+0.5001)/100.0)
#define ROUND3(x) ((int)((x)*1000+0.5)/1000.0)
