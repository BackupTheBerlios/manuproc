// $Id: Event.cc,v 1.2 2003/05/09 11:07:28 christof Exp $
/*  libcommonc++: ManuProC's main OO library
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

#include <Misc/Event.h>
#include <Misc/FetchIStream.h>
#include <libpq-fe.h>
#include <Misc/Transaction.h>

safemap<std::string, ManuProC::Event::filteredsignal_t> ManuProC::Event::channels;
ManuProC::Event::fullsignal_t ManuProC::Event::event_sig;
bool ManuProC::Event::connected;
ManuProC::TimeStamp ManuProC::Event::last_processed;
void *ManuProC::Event::PGconnection;

bool ManuProC::Event::look_for_notifications()
{  bool consumed=false;
retry:
   PQconsumeInput((PGconn *)PGconnection);
   PGnotify *n;
   bool pending=false;
   while ((n=PQnotifies((PGconn *)PGconnection)))
   {  if (n->relname==std::string("events")) pending=true;
   }
   if (pending) 
   {  consumed=true;
      read_notifications();
      goto retry;
   }
   return consumed;
}
	
void ManuProC::Event::read_notifications()
{  Transaction tr;
   ManuProC::TimeStamp bis;
   Query("select max(zeit) from events") >> bis;
   if (!bis.valid()) return;
   if (!last_processed.valid()) last_processed=ManuProC::TimeStamp("1970-1-1");
   Query q("select channel,key,data from events "
   	"where ? < zeit and zeit <= ? order by zeit");
   q << last_processed << bis;
   FetchIStream is;
   while ((q >> is).good())
   {  std::string channel,key,data;
      is >> channel 
      	 >> FetchIStream::MapNull(key)
      	 >> FetchIStream::MapNull(data);
      event_sig(channel,key,data);
      channels[channel](key,data);
   }
   last_processed=bis;
}

ManuProC::Event::Event(const std::string &channel,const std::string &key,const std::string &data)
{  Query("insert into events (zeit,channel,key,data) values (now(),?,?,?)")
	<< channel << Query::NullIf(key) << Query::NullIf(data);
   Query("NOTIFY events");
}

namespace {
struct ecpg_connection // stolen from ecpg internals
{   char	   *name;
    PGconn	   *connection;
    // ...
};

extern "C" ecpg_connection *ECPGget_connection(const char *);
}

void ManuProC::Event::connect(bool ignore_old)
{  Query("LISTEN events");
   connected=true;
   PGconnection=ECPGget_connection(0)->connection;
   if (ignore_old)
   {  Query("select now()") >> last_processed;
      while (PQnotifies((PGconn *)PGconnection));
   }
   else read_notifications();
}

int ManuProC::Event::filedesc()
{  if (!connected) return -1;
   return PQsocket((PGconn *)PGconnection);
}

// connect_gtk() :
//   register a read callback which registers an idle method which reads
//   events [PQsocket()]
