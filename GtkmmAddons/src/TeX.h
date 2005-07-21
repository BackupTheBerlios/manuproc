/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Id: TeX.h,v 1.8 2005/07/21 08:58:11 christof Exp $

#include <iostream>
#include <string>
#include <glib.h>

namespace TeX
{	static const float in_cm(0.3935);
	typedef void (*Simple_cb)(std::ostream &os,gpointer user_data);
	struct HeaderFlags
	{	bool latin1:1;
	        bool utf8:1;
		bool longtable:1;
		bool header:1;
		bool footer:1;
		bool a4:1;
		bool german:1;
		bool dense:1; // \parindent0pt\parskip0pt
		bool helvetica:1; // use Helvetica
		bool landscape:1;
		bool twocolumn:1;
		int ptsize;
		// in inches
		float topmargin,leftmargin,bottommargin,rightmargin;
		std::string leftfoot,rightfoot; // fancyheadings
		std::string leftheader,rightheader; // fancyheadings
		Simple_cb preamble_cb; // called before begin{document}
		gpointer user_data;
		std::string packages;
		std::string pagestyle;
		std::string preamble;
		
		HeaderFlags()
		: latin1(true), utf8(), longtable(true), header(),
		  footer(true), a4(true), german(true), dense(true),
		  helvetica(true), landscape(), twocolumn(),
		  ptsize(10), topmargin(1*in_cm), leftmargin(1), 
		  bottommargin(1*in_cm), rightmargin(1.5*in_cm),
		  preamble_cb(), user_data()
		{}
	};
	struct StringFlags
	{	bool at_least_one_non_space:1;
		bool bar_means_newline:1;
		bool convert_space_to_tilde:1;
		
		StringFlags() : at_least_one_non_space(false), bar_means_newline(false),
			convert_space_to_tilde(false)
		{}
	};
	
	std::ostream &Header(std::ostream &os, HeaderFlags fl=HeaderFlags());
	std::ostream &Footer(std::ostream &os);
	extern bool TeX_uses_UTF8;
	std::string string2TeX(const std::string &s, const StringFlags &fl=StringFlags()) throw();
   std::string scale(const std::string& is, unsigned int maxlength, 
                     const std::string& scale,const bool use_string2TeX=true);
};
