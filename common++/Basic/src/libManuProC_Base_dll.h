// $Id: libManuProC_Base_dll.h,v 1.1 2003/09/18 07:35:36 christof Exp $               
/*  libManuProC_Base: ManuProC's main OO library
 *  Copyright (C) 2003 Christof Petig
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

#ifndef LIBMPC_BASE_DLL_H
#define LIBMPC_BASE_DLL_H

#ifdef __MINGW32__
#if defined(libManuProC_Base_COMPILATION) && defined(DLL_EXPORT)
#define LIBMPC_BASE_API __declspec(dllexport) 
#elif !defined(libManuProC_Base_COMPILATION)
#define LIBMPC_BASE_API __declspec(dllimport)
#else
#define LIBMPC_BASE_API
#endif /* libManuProC_Base_COMPILATION - DLL_EXPORT */
#else
#define LIBMPC_BASE_API
#endif /* __MINGW32__ */

#endif
