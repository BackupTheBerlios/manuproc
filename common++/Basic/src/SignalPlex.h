// $Id: SignalPlex.h,v 1.1 2003/11/20 08:05:58 christof Exp $
/*  libcommon++: ManuProC's OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#ifndef SIGNALPLEX_H
#define SIGNALPLEX_H

// Signal multiplexer

#include <ManuProCConfig.h>

#ifndef SIGC1_2
#error this class needs SigC 1.2+
#endif
#include <sigc++/signal.h>
#include <sigc++/object_slot.h>

// one way variant - two way variant should be similar to ModelPlex
template <class S>
 class SignalPlex1 : public SigC::Object, public S
{	typedef SignalPlex1<S> this_t;
	
	SigC::Connection con;
	S *actback;

	// forbid assignment
	void operator=(const this_t &b);
	SignalPlex1(const this_t &b);
public:
	SignalPlex1(S &s) : actback()
	{  set_signal(s);
	}
	SignalPlex1() : actback() {}

	void set_signal(S &s)
	{  con.disconnect();
	   actback=&s;
	   con=s->connect(slot());
	}
	void unset_signal()
	{  con.disconnect();
	   actback=0;
	}
	
	void operator=(S &s)
	{  set_signal(s); }
};

#endif
