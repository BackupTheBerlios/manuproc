// $Id: TooltipView.cc,v 1.2 2004/05/03 14:17:45 christof Exp $
/*  libKomponenten: ManuProC's Widget library
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

#include <TooltipView.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

namespace {
struct TooltipAssociation_bool : TooltipAssociation
{	Model_ref<bool> model;
	std::string falsetip,truetip;

	// first push back then access	
	TooltipAssociation_bool() {}
	void changed_cb(gpointer x) const;
};
}

void TooltipAssociation_bool::changed_cb(gpointer x) const
{  if (model.matches(x)) 
      tips->set_tip(*widget,model.Value()?truetip:falsetip);
}

void TooltipViewList::Associate(Gtk::Widget &w,const Model_ref<bool> &m,
		const std::string &falsetip,
		const std::string &truetip)
{  TooltipAssociation_bool *tta=0;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
   connections.push_back(tta=new TooltipAssociation_bool());
#else
   connections.push_back(tta=SigC::manage(new TooltipAssociation_bool()));
#endif
   tta->tips=tips;
   tta->widget=&w;
   tta->model=m;
   tta->falsetip=falsetip;
   tta->truetip=truetip;
   m.signal_changed().connect(SigC::slot(*tta,&TooltipAssociation_bool::changed_cb));
   tta->changed_cb(0);
}

void TooltipViewList::Disassociate(Gtk::Widget &w)
{  for (list_t::iterator i=connections.begin();i!=connections.end();++i)
      if ((*i)->widget==&w) 
      {  
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
	 delete *i;
#endif
         i=connections.erase(i);
      }
   tips->unset_tip(w);
}

#if 0
template <> void TooltipViewList::Associate<int>(Gtk::Widget &w,
		const Model_ref<int> &m,
		const std::list<std::pair<int,std::string> > &tips)
{
}
#endif
