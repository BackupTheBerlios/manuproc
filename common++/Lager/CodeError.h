/* $Id: CodeError.h,v 1.3 2002/10/24 14:06:50 thoma Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef CODEERROR_H
#define CODEERROR_H

#include <iostream>
#include <string>
#include <exception>

class CodeError : std::exception
{
public:
 int code;
 std::string context;

 CodeError(int c, const std::string &ct)
 : code(c), context(ct) {}
 friend std::ostream &operator<<(std::ostream&,const CodeError &);
 virtual const char* what() const throw() { return "CodeError"; }
 virtual ~CodeError() throw() {}
};

inline std::ostream &operator<<(std::ostream&o,const CodeError &e)
{  return o<<e.context<<": "<<e.code;
}

#endif
