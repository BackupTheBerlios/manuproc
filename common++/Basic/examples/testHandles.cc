// $Id: testHandles.cc,v 1.8 2003/11/26 14:06:29 christof Exp $
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


#include <Misc/Handles.h>
#include <iostream>

class A : public HandleContent
{  	int val;
	friend class Handle<A>;
public:
	A(int x) : val(x) 
	{  std::cerr << "ctor " << val << '/' << this << '\n';
	}
	~A()
	{  std::cerr << "dtor " << val << '/' << this << '\n';
	}
	bool operator==(const A &b)
	{  return val==b.val;
	}
	friend std::ostream &operator<<(std::ostream &o,const A &a);
};

struct B : public A
{  B() : A('B') {}
};

std::ostream &operator<<(std::ostream &o,const A &a)
{  return o << a.val;
}

typedef Handle<A> Ah2;

class Ah : public Handle<A>
{public:
	Ah(int x=-1) : Handle<A>(new A(x)) {};
	Ah(A *a) : Handle<A>(a) {}; // dangerous, you pass ownership
	~Ah() { std::cerr << "Handle " << **this << '/' << this << " dtor\n"; }
};

class Storage
{	Ah2 member;
public:
	Storage() : member(new A(-2)) {}
	void Store(const Ah2 &arg)
	{  member=arg; }
};

void a(const Ah2 &v)
{  std::cerr << "a(" << *v << ")\n";
}

void b(Ah2 v)
{  std::cerr << "b(" << *v << ") -> ";
}

void c(const Ah &v)
{  std::cerr << "c(" << *v << ")\n";
}

void d(Ah v)
{  std::cerr << "d(" << *v << ") -> ";
}

void e(Handle<B> b)
{  Handle<A> a=b; // downcast
   Handle<B> b2=a.cast_dynamic<B>();
   Handle<const A> ca=a; // adding constness
   Handle<A> a2=ca.cast_const<A>();
   Handle<B> b3=a.cast_static<B>();

#if ! defined(__GNUC__) || __GNUC__>2
   Handle<B> b4=Handle<B>::cast_dynamic(a);
   Handle<B> b5=Handle<B>::cast_static(a);
   Handle<A> a3=Handle<A>::cast_const(ca);
#if 0 // these give errors   
   Handle<B> b6=Handle<B>::cast_const(ca);
   Handle<B> b7=a;
#endif
#endif
}

// demonstrate static elements

A static_a(42);
bool dummy=static_a.is_static(true);

Ah static_test(&static_a);



int main()
{  Ah y(2);
   Ah z(new A(1));
   c(z); c(y);
   d(z); d(y);

   {  Ah2 x(new A(3));
      a(x); b(x);
   }
   
   a(Ah(4)); b(Ah(5)); c(6); d(7);
   std::cerr << "new + Ownership passing\n";
   c(new A(8)); a(new A(9));
   std::cerr << "Storage class -> ";
   Storage s;
   s.Store(y);
   s.Store(new A(10));
   std::cerr << "Assignment -> ";
   y=z;
   c(y); c(z);
   std::cerr << "Self Assignment (no op)\n";
   y=y;
   c(y);
   std::cerr << "static object Assignment -> ";
   y=&static_a;
   std::cerr << "static self Assignment (no op)\n";
   y=y;
   std::cerr << "uninitialized Handle\n";
   { Handle<A> w;
     w=y;
   }
   std::cerr << "main() ending -> ";
}
