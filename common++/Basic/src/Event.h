// $Id: Event.h,v 1.2 2003/05/09 11:07:28 christof Exp $
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


#ifndef MISC_EVENT_H
#define MISC_EVENT_H

#include <BaseObjects/Model.h> // for SigC headers
#include <string>
#include <Misc/safemap.h>
#include <Misc/TimeStamp.h>

namespace ManuProC {  
class Event
{	typedef SigC::Signal3<void,const std::string &,const std::string &,const std::string &> fullsignal_t;
	typedef SigC::Signal2<void,const std::string &,const std::string &> filteredsignal_t;
	static safemap<std::string, filteredsignal_t> channels;
	static fullsignal_t event_sig;
	static TimeStamp last_processed;
	static bool connected;
	static void *PGconnection;
	
	static void read_notifications();
	
public:
	Event(const std::string &channel,const std::string &key="",const std::string &data="");
   	static fullsignal_t &signal_event()
   	{  return event_sig; }
   	static filteredsignal_t &signal_event(const std::string &channel)
   	{  return channels[channel]; }
   	
   	// connect to database
   	static void connect(bool ignore_old=true);
   	// implies connect()
   	// declared here but defined in Komponenten
   	static void connect_gtk(bool ignore_old=true);
   	
   	static bool look_for_notifications();

	// file descriptor to select for notifications
   	static int filedesc();
};
}

#endif
