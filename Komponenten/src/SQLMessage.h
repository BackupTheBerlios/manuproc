/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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
#ifndef _SQLMESSAGE_HH
#  define _SQLMESSAGE_HH


#include <Message.h>
#include <Aux/SQLerror.h>
#include <Aux/itos.h>

class SQLMessage : public Message
{
public:
 void Show(const SQLerror &e)
 {std::string _msg="DB-Error Code: "+itos(e.Code())+"\n";
  _msg+="DB-Error Message: "+e.Message()+"\n";
  _msg+="Context: "+e.Context()+"\n";
  Message::Show((char *)_msg.c_str());
 }
 void Show(const std::string &s) {Message::Show(s);}
 void Show(const exception &s) {Message::Show(s.what());}
};

#endif
