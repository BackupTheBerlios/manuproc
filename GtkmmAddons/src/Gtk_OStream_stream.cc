// $Id: Gtk_OStream_stream.cc,v 1.4 2004/04/29 14:24:40 christof Exp $
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998-2002  Adolf Petig GmbH. & Co. KG
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

std::streamsize Gtk::OStream::data_stream(const char *s,std::streamsize n)
{   (*handler_data.stream.os).write(s,n);
    return (*handler_data.stream.os).good()?n:0;
};
