/* $Id: SQLerror.h,v 1.5 2004/03/11 12:17:32 christof Exp $ */
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


#ifndef SQLERROR_H
#define SQLERROR_H
#include <ManuProCConfig.h>
#ifndef MPC_SQLITE
# include <Misc/SQLerror_postgres.h>
 typedef SQLerror_postgres SQLerror;
#else
# include <Misc/SQLerror_sqlite.h>
 typedef SQLerror_sqlite SQLerror;
#endif
#endif
