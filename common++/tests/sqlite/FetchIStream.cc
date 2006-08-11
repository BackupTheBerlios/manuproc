// $Id: FetchIStream.cc,v 1.3 2006/08/11 09:33:10 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#include <Misc/Query.h>
#include <Misc/dbconnect.h>
#include <iterator>

#define X() throw SQLerror(__FILELINE__)

int main()
{  ManuProC::dbconnect(ManuProC::Connection("","template1"));

   std::cout << "simple use " << Query("select 1 from dummy").FetchOne<int>() << '\n';

// now test it   
  {Query query("select 'test','yesterday',null,null,200,20.5,'t' from dummy");
   std::string s,s2,s3,s4;
   bool b;
   int i;
   float f;
   int ind3=0;
   
   query >> s >> s2 >> Query::Row::WithIndicator(s3,ind3)
      >> Query::Row::MapNull(s4,"<NULL>")
      >> i >> f >> b;
   std::cout << s << ", " << s2 << ", " << ind3 << ", " << s4 << '\n';
   std::cout << i << ", " << f << ", " << b << '\n';
  }
  {Query query("select relname from pg_class limit 10");
   std::vector<std::string> v;
   query.FetchArray(v);
   std::copy(v.begin(),v.end(),std::ostream_iterator<std::string>(std::cout,"\n"));
  }
//  Query("select cast('test' as varchar(5))").FetchOne<std::string>();
// test nasty error? (overlapping)
{
  std::cout << "------\n";
   Query query2("select 1;");
   std::cout << query2.FetchOne<int>() << '\n';
   Query query3("select 2;");
   std::cout << query3.FetchOne<int>() << '\n';
}   
  {int a,b,c,d; std::string x,y,z;
   std::string in="\"'\\";
   Query ("select ?,?,?,'?',?, ?,?")
   	<< 1 << Query::null<int>() << "test" << in 
   		<< Query::NullIf(1,2) << Query::NullIf(2,2)
   	>> a >> Query::Row::MapNull(b,-1) >> x >> y >> z 
   		>> c >> Query::Row::MapNull(d,-1);
   std::cout << a << ' ' << b << ' ' << x << ' ' << y << ' ' << z 
   		<< ' ' << c << ' ' << d << '\n';
  }
//   Query ("analyze pg_class");
   // should never select any lines
   Query ("update pg_class set relname=relname where relname='' and oid=0");
   SQLerror::print("should be 100");
   Query ("update pg_class set relname=relname where relname='A'");
   SQLerror::test("should be no error");
   
   std::cout << (Query("select ? from pg_class limit 1")
   	.add_argument("relname",0).FetchOne<std::string>()) << '\n';
   std::cout << "this should give a warning:\n";
   Query("select ?");
   SQLerror::print("should be -202");
   
   ManuProC::dbdisconnect();
   return 0;
}
