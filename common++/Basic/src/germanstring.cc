// $Id: germanstring.cc,v 1.5 2004/04/29 13:17:23 christof Exp $            
/*  Indexerstellung
 *  Copyright (C) 2002 Christof Petig
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

#include <Misc/germanstring.h>
#include <cctype>

namespace {
typedef const char *str;
typedef const str twostr[2];
typedef twostr *replacement_iter;

class myiterator
{	std::string::const_iterator sit,end;
	const char *replaced;
	static const twostr replacements[];

	void set_replaced();
public:
	myiterator(const std::string::const_iterator &i,const std::string::const_iterator &e) 
		: sit(i), end(e), replaced(0)
	{  set_replaced();
	}
	myiterator(const std::string::const_iterator &i) 
		: sit(i), end(i), replaced(0)
	{}
	unsigned char operator*()
	{  if (replaced) return *replaced;
	   return tolower((unsigned char)*sit);
	}
	void operator++();
	bool operator!=(const myiterator &b)
	{  return sit!=b.sit || replaced!=b.replaced; }
};
}

// this is sorted!

const twostr myiterator::replacements[] = 
	{ { "AE", "ae0" },
	  { "Ae", "ae0" }, 
	  { "OE", "oe0" },
	  { "Oe", "oe0" }, 
	  { "UE", "ue0" },
	  { "Ue", "ue0" }, 
	  { "ae", "ae0" },
	  { "oe", "oe0" },
	  { "ss", "ss0" },
	  { "ue", "ue0" },
#if defined(__MINGW32__) || MPC_SIGC_VERSION>=0x120 // UTF-8
	  { "Ã„", "ae1" }, 
	  { "Ã–", "oe1" }, 
	  { "Ãœ", "ue1" }, 
	  { "ÃŸ", "ss1" }, 
	  { "Ã¤", "ae1" }, 
	  { "Ã¶", "oe1" }, 
	  { "Ã¼", "ue1" }, 
#else // ISO 8859
	  { "Ä", "ae1" }, 
	  { "Ö", "oe1" }, 
	  { "Ü", "ue1" }, 
	  { "ß", "ss1" }, 
	  { "ä", "ae1" }, 
	  { "ö", "oe1" }, 
	  { "ü", "ue1" }, 
#endif	  
	};
static const char dummy[16]={0};

static const char *find(std::string::const_iterator &a,
	const std::string::const_iterator &aend,
	replacement_iter b,replacement_iter bend)
{  for (replacement_iter i=b; i!=bend; ++i)
   {  const char *j=(*i)[0];
      std::string::const_iterator k=a;
      for (;k!=aend && *j==*k;++k,++j);
      if (!*j) { a=k; return (*i)[1]; }
   }
   return 0;
#if 0 // forget about bsearch for now
 replacement_iter med=b+(bend-b)/2;
#endif
}

void myiterator::set_replaced()
{  if (sit!=end) 
      replaced=find(sit,end,replacements,
   		replacements+(sizeof(replacements)/sizeof(replacements[0])));
   else replaced=0;
}
	  
void myiterator::operator++()
{  if (replaced)
   {  ++replaced;
      if (*replaced) return;
      set_replaced();
      return;
   }
   if (sit==end) return; 
   ++sit;
   set_replaced();
}

bool germanstring::operator<(const germanstring &b) const
{  myiterator a=myiterator(begin(),end());
   myiterator i=myiterator(b.begin(),b.end());
   while (a!=myiterator(end()) && i!=myiterator(b.end()))
   {  // std::cout << *a << ',' << *i << '\n';
      if (*a<*i) return true;
      if (*a>*i) return false;
      ++a;
      ++i;
   }
   if (i!=b.end()) return true;
   return false;
}

#if 0
bool germanstring::operator==(const germanstring &b) const
{  myiterator a=myiterator(begin(),end());
   myiterator i=myiterator(b.begin(),b.end());
   while (a!=myiterator(end()) && i!=myiterator(b.end()))
   {  if (*a!=*i) return false;
      ++a;
      ++i;
   }
   return a==end() && i==b.end();
}
#endif

#ifdef TEST

#include <iostream>

// compile via
// g++ -o test -DTEST -I. germanstring.cc

#define DO(x) std::cout << #x << '=' << ((x)?"true":"false") << '\n'
#define COMP2(x,y) DO(germanstring(x)<germanstring(y)); DO(germanstring(y)<germanstring(x))
int main()
{  DO(germanstring("Ätsch")<germanstring("Af"));
   DO(germanstring("Ae")<germanstring("Ätsch"));
   COMP2("Ätsch","äTSCH");
   COMP2("Ätsch","Aetsch");
   COMP2("Wasser","Waßer");
   COMP2("Pfütze","Pfuetze");
   DO(germanstring("ÄÖÜäöüßaeoeuess")<germanstring("ÄÖÜäöüßaeoeuess"));
   COMP2("Wasser","Wasserpflanze");
   return 0;
}

#endif
