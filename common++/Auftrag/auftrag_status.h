/* $Id: auftrag_status.h,v 1.5 2006/06/26 07:53:12 christof Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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


#ifndef AUFSTATUS
#define AUFSTATUS

typedef enum {NOSTAT=-1, UNCOMMITED=0, OPEN, CLOSED, STORNO, OPEN_AND_UNCOM} AufStatVal;
typedef enum {REQUEST=0, CONFIRM, PROBLEM} AufChangeStatVal;
typedef enum {MENGE='M', LIEFDATUM='D', STATUS='S'} AufChangeTypeVal;

#endif
