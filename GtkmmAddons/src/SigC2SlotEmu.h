/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2001-2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Id: SigC2SlotEmu.h,v 1.6 2004/05/03 09:47:17 christof Exp $

// compatibility header for sigc 2.0 <-> sigc 1.2

#ifndef F8A1CFD6_43A4_4DBC_9F30_826BBC4F0650
#define F8A1CFD6_43A4_4DBC_9F30_826BBC4F0650
#include <gtkmmconfig.h>

#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
// don't ask me why this does not work ...
//#  include <sigc++/compatibility.h>

#warning Using this header is discouraged, replace with (see following line)
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

#if 0
#include <sigc++/slot.h>
#include <sigc++/bind.h>

namespace SigC
{
   template <class R,class T,class U> 
    Slot0<R> slot(T &t, R (U::*p)())
	{ return sigc::mem_fun(t,p); }
   template <class R,class T,class U,class A> 
    Slot1<R,A> slot(T &t, R (U::*p)(A)) 
    	{ return sigc::mem_fun(t,p); }
   template <class R,class T,class U,class A,class B> 
    Slot2<R,A,B> slot(T &t, R (U::*p)(A,B)) 
    	{ return sigc::mem_fun(t,p); }
   template <class R,class T,class U,class A,class B,class C> 
    Slot3<R,A,B,C> slot(T &t, R (U::*p)(A,B,C)) 
    	{ return sigc::mem_fun(t,p); }
}
#endif
#else // sigc 2.0 compatibility layer for SigC 1.2
#include <sigc++/slot.h>

namespace sigc
{  template <class T,class U,class R> 
    SigC::Slot0<R> mem_fun(T &t, R (U::*p)())
	{ return SigC::slot(t,p); }
   template <class R,class T,class U,class A> 
    SigC::Slot1<R,A> mem_fun(T &t, R (U::*p)(A)) 
    	{ return SigC::slot(t,p); }
   template <class R,class T,class U,class A,class B> 
    SigC::Slot2<R,A,B> mem_fun(T &t, R (U::*p)(A,B)) 
    	{ return SigC::slot(t,p); }
   template <class R,class T,class U,class A,class B,class C> 
    SigC::Slot3<R,A,B,C> mem_fun(T &t, R (U::*p)(A,B,C)) 
    	{ return SigC::slot(t,p); }
}

#endif

#endif
