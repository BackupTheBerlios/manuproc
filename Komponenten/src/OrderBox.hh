// $Id: OrderBox.hh,v 1.1 2006/04/03 10:02:14 christof Exp $
/*  libKomponenten: ManuProC's Widget library
 *  Copyright (C) 2002 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski, Christof Petig, Malte Thoma
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

#ifndef EDDDAF68_BCFA_49DD_BF8C_A343F4609264
#define EDDDAF68_BCFA_49DD_BF8C_A343F4609264

#include <SearchComboContent.hh>
#include <Misc/Transaction.h>
class Query;

struct OrderBox : SearchComboContent<int>
{
  enum what_t { Order, Delivery, Invoice };
  
private:
  int customer;
  what_t what;
  Query *query;
  Transaction tr;
  int instanz;
  bool extra_string;
  
  void on_search(int *_continue, GtkSCContext newsearch) throw();
public:
  OrderBox(int _instanz=1, what_t w=Order);
};

#endif
