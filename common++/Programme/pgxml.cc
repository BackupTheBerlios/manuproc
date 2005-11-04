// $Id: pgxml.cc,v 1.1 2005/11/04 09:56:04 christof Exp $
/*  pps: ManuProC's production planning system
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <iostream>
#include <Misc/Query.h>
#include <Misc/TagStream.h>
#include <Misc/dbconnect.h>

int main(int argc,char **argv)
{ TagStream ts;
  if (argc!=2)
  { std::cerr << "USAGE: pgxml <Query>\n";
    return 1;
  }
  try
  { ManuProC::dbconnect();
    Query("set names 'utf-8'");
    Query q(argv[1]);
    Query::Row r;
    ts.setContent(Tag("Query"));
    Tag &rows=ts.getContent();
    rows.setAttr("SQL",argv[1]);
    while ((q >> r).good())
    { Tag &row=rows.push_back(Tag("Row"));
      try
      { for (;;)
        { std::string val;
          int ind;
          std::string name=r.getFieldName();
          r >> Query::Row::WithIndicator(val,ind);
          if (ind) continue;
          row.setAttr(name,val);
        }
      }
      catch (SQLerror &e) {}
    }
    ManuProC::dbdisconnect();
    ts.write(std::cout);
    return 0;
  }
  catch (SQLerror &e)
  { std::cerr << e << '\n';
    return 1;
  }
}
