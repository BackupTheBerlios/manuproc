// $Id: Trace.h,v 1.2 2002/10/31 08:29:37 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef CXX_AUX_TRACE_H
#define CXX_AUX_TRACE_H

#include <string>
#include <iostream>

#define ENABLE_TRACES

namespace ManuProC {

namespace Tracer {

	enum Channel { Auftrag,Artikel };

	void Enable(Channel c, bool an=true);
	
// internal implementation
	std::ostream &FunctionStart(const std::string &s);
	void FunctionEnd();
	bool enabled(Channel c);
}

class Trace
{	bool enabled;
public:
   ~Trace() { if (enabled) Tracer::FunctionEnd(); }

   Trace(Tracer::Channel c, const std::string &name) : enabled(Tracer::enabled(c))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << ")\n"; }
   }

template <class A> 
   Trace(Tracer::Channel c, const std::string &name, const A &a) 
   	: enabled(Tracer::enabled(c))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ")\n"; }
   }

template <class A, class B> 
   Trace(Tracer::Channel c, const std::string &name, const A &a, const B &b)
   	: enabled(Tracer::enabled(c))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b << ")\n"; }
   }

template <class A, class B,class C> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c << ")\n"; }
   }

template <class A, class B,class C,class D> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d << ")\n"; }
   }

template <class A, class B,class C,class D,class E> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e << ")\n"; }
   }

template <class A, class B,class C,class D,class E,class F> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f << ")\n"; }
   }

template <class A, class B,class C,class D,class E,class F,class G> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g << ")\n"; }
   }

template <class A, class B,class C,class D,class E,class F,class G,class H> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h << ")\n"; }
   }

template <class A, class B,class C,class D,class E,class F,class G,class H,class I> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i << ")\n"; }
   }

template <class A, class B,class C,class D,class E,class F,class G,class H,class I,class J> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i,const J &j)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i<<','<<j << ")\n"; }
   }

template <class A, class B,class C,class D,class E,class F,class G,class H,class I,class J,class K> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i,const J &j,const K &k)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i<<','<<j<<','<<k<<")\n"; }
   }

template <class A, class B,class C,class D,class E,class F,class G,class H,class I,class J,class K,class L> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i,const J &j,const K &k,const L &l)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i<<','<<j<<','<<k<<','<<l<<")\n"; }
   }


template <class A, class B,class C,class D,class E,class F,class G,class H,class I,class J,class K,class L,class M> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i,const J &j,const K &k,const L &l,const M &m)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i<<','<<j<<','<<k<<','<<l<<','<<m<<")\n"; }
   }


template <class A, class B,class C,class D,class E,class F,class G,class H,class I,class J,class K,class L,class M,class N> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i,const J &j,const K &k,const L &l,const M &m,const N &n)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i<<','<<j<<','<<k<<','<<l<<','<<m<<','<<n<<")\n"; }
   }


template <class A, class B,class C,class D,class E,class F,class G,class H,class I,class J,class K,class L,class M,class N,class O> 
   Trace(Tracer::Channel ca, const std::string &name, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f, const G &g,const H &h,const I &i,const J &j,const K &k,const L &l,const M &m,const N &n,const O &o)
   	: enabled(Tracer::enabled(ca))
   {  if (enabled) 
      { Tracer::FunctionStart(name) << a << ',' << b <<',' << c<<',' << d<<',' << e<<',' << f<<',' << g<<','<<h<<','<<i<<','<<j<<','<<k<<','<<l<<','<<m<<','<<n<<','<<o<<")\n"; }
   }



};

}

#endif
