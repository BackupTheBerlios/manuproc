/* $Id: AdressenIterator.h,v 1.4 2003/01/08 09:46:57 christof Exp $ */
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

#include <Misc/SQLerror.h>

#define ORTSEP ';'

struct AdressenIterator
{  int kundennr;
   char firma[61];
   char ort[31];
#ifdef __cplusplus
   AdressenIterator(const char *pattern) throw(SQLerror);
   ~AdressenIterator() throw(SQLerror);
   void get_next() throw(SQLerror);
#endif
};

#ifdef __cplusplus
extern "C" {
#else
typedef struct AdressenIterator AdressenIterator;
#endif
AdressenIterator *AdressenIterator_by_pattern(const char *pattern) throw();
AdressenIterator *AdressenIterator_get_next(AdressenIterator *ai) throw();
void free_AdressenIterator(AdressenIterator *ai) throw();
#ifdef __cplusplus
}
#endif
