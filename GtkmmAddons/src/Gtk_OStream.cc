// $Id: Gtk_OStream.cc,v 1.7 2004/11/08 14:59:29 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 2002  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof.petig@wtal.de>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "Gtk_OStream.h"
#include <cassert>

std::streamsize Gtk::OStreamBase::data_cb(const char_type* s, std::streamsize n)
{  if (data_impl) 
      return (dynamic_cast<Gtk::OStream*>(this)->*data_impl)(s,n);
   return 0;
}

Gtk::OStreamBase::OStreamBase(line_cbt l,close_cbt c)
	:  std::ostream(0), 
	   flush_impl(0), close_impl(c), data_impl(&OStream::default_data),
	   line_impl(l), 
	   user_data(0), notify(0), buf(*this,&OStreamBase::data_cb)
{  this->init(&buf);
}

Gtk::OStreamBase::OStreamBase(data_cbt d)
	:  std::ostream(0), 
	   flush_impl(0), close_impl(0), data_impl(d), line_impl(0),
	   user_data(0), notify(0), buf(*this,&OStreamBase::data_cb)
{
}

std::streamsize Gtk::OStream::default_data(const char *s,std::streamsize n)
{  data.append(s,n);
   if (line_impl) //  && memchr(s,'\n',n))
   {  std::string::size_type end=std::string::npos;
      while ((end=data.find('\n'))!=std::string::npos)
      {  (this->*line_impl)(data.substr(0,end));
         data.erase(0,end!=std::string::npos?end+1:end);
      }
   }
   return n;
}

Gtk::OStreamBase::~OStreamBase(void)
{   // flush(); can't help any more, this is destroyed
}

void Gtk::OStreamBase::flush(gpointer _user_data,GtkDestroyNotify d)
{  if (_user_data || d)
   {  user_data=_user_data; notify=d; }

   if (flush_impl) (dynamic_cast<Gtk::OStream*>(this)->*flush_impl)();
   else if (line_impl && data_impl==&OStream::default_data && !data.empty())
   {  (dynamic_cast<Gtk::OStream*>(this)->*line_impl)(data);
      data="";
   }
   flushed();
}

Gtk::OStream::~OStream()
{  flush(); // flush before we destroy ourselves
   if (close_impl) (dynamic_cast<Gtk::OStream*>(this)->*close_impl)();
}
