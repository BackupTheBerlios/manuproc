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

// $Id: exception.cc,v 1.4 2002/05/06 13:41:22 christof Exp $
// long explantion at the end

#include <iostream>
#include <Aux/exception.h>
#include <unistd.h>

// taken from gcc/eh_common.h

typedef void * (*__eh_matcher) (void *, void *, void *);

typedef struct __eh_info
{
  __eh_matcher match_function;
  short language;
  short version;
} __eh_info;

// taken from gcc/cp/exception.cc

#include <typeinfo>
//#include <stddef.h>

/* C++-specific state about the current exception.
   This must match init_exception_processing().

   Note that handlers and caught are not redundant; when rethrown, an
   std::exception can have multiple active handlers and still be considered
   uncaught.  */

struct cp_eh_info
{
  __eh_info eh_info;
  void *value;
  void *type;
  void (*cleanup)(void *, int);
  bool caught;
  cp_eh_info *next;
  long handlers;
  void *original_value;
};

/* Language-specific EH info pointer, defined in libgcc2. */

extern "C" cp_eh_info **__get_eh_info ();       // actually void **

static void print_exception_u()
{  std::cerr << "unexpected std::exception: ";
   std::cerr << ((std::type_info*)((*__get_eh_info ())->type))->name();
   std::cerr << '\n';
   std::cerr.flush();
   _exit(1);
}

static void print_exception_t()
{  
   std::cerr << "uncaught std::exception: ";
   std::cerr << ((std::type_info*)((*__get_eh_info ())->type))->name();
   std::cerr << '\n';
   std::cerr.flush();
   _exit(1);
}

void Petig::PrintUncaughtExceptions() throw()
{  std::set_unexpected(print_exception_u);
   std::set_terminate(print_exception_t);
}

#if 0
What does a throw do?

it calls 
	void * __eh_alloc(size_t size)
uses a copy ctor of the std::exception and then destructs all
instances, then calls 
	void __cp_push_exception(void *value, void *type, void (*cleanup)(void *, int))
and (__eh_pc ?)
	void __throw()

#endif
