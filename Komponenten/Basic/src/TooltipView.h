// $Id: TooltipView.h,v 1.2 2004/05/03 14:17:45 christof Exp $
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

#ifndef MANUPROC_WIDGETS_TOOLTIPVIEW_H
#define MANUPROC_WIDGETS_TOOLTIPVIEW_H

#include <Misc/Model.h>
#include <list>
#include <gtkmm/tooltips.h>
#include <sigc++/object.h>

struct TooltipAssociation : SigC::Object
{	Gtk::Widget *widget;
	Gtk::Tooltips *tips;
	TooltipAssociation() : widget(), tips() {}
};

class TooltipViewList
{
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
	typedef std::list<TooltipAssociation *> list_t;
#else
	typedef std::list<SigC::Ptr<TooltipAssociation> > list_t;
#endif
        list_t connections;
	Gtk::Tooltips *tips;
public:
	TooltipViewList(Gtk::Tooltips &t) : tips(&t) {}
	void Associate(Gtk::Widget &w,const Model_ref<bool> &m,
		const std::string &falsetip,
		const std::string &truetip);
	template <class T>
	 void Associate(Gtk::Widget &w,const Model_ref<T> &m,
	 	const std::list<std::pair<T,std::string> > &tips);
	void Disassociate(Gtk::Widget &w);
};

template <> void TooltipViewList::Associate<int>(Gtk::Widget &w,
		const Model_ref<int> &m,
		const std::list<std::pair<int,std::string> > &tips);
#endif
