/* $Id: itos.h,v 1.10 2004/02/12 11:26:43 jacek Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#ifndef AUX_ITOS_H
#define AUX_ITOS_H
#include <string>
std::string itos(int i);
std::string itos0p(int i, int c=0,bool show_plus=false);
std::string itos0pad(int i, int p);
std::string dtos(double i);
std::string ulltos(unsigned long long i);
std::string btos(bool b);
#endif
