// $Id: TagStream.h,v 1.9 2004/11/24 08:16:34 christof Exp $
/*  glade--: C++ frontend for glade (Gtk+ User Interface Builder)
 *  Copyright (C) 1998-2004  Christof Petig
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

#ifndef _TAGSTREAM_HH
#define _TAGSTREAM_HH

#include <ManuProCConfig.h>
#include <Misc/Tag.h>
#include <iostream>

class TagStream : public Tag
{	// ---- for reading -----
	// copied from _GbWidgetLoadData
	static const int GB_BUFFER_SIZE=40960;
	char buffer[GB_BUFFER_SIZE];
	bool read_again;
	int pointer,end_pointer;

	std::istream *is;
	std::istream *ifs;
	std::istream *iss;

	char *next_tag(Tag *parent);
	char *next_tag_pointer(Tag *parent);

	char *find_wordend(char *ptr);
	char *find_wordend2(char *ptr);
	char *find_tagend2(char *ptr);
	void set_pointer(char *ptr) // test for valid range
	{  pointer=ptr-buffer; }

	char *find(const char *start,char what)
	{  return (char*)memchr(start,what,end_pointer-(start-buffer)); }
	char *find(char what)
	{  return (char*)memchr(buffer+pointer,what,end_pointer-pointer); }

	void de_xml(std::string &cont);
	void load_project_file(Tag *top);

	// ------ normal operation --------
	std::string encoding;
	std::string file_name;
	
	// encoding -> host_encoding
//	void recode_load(std::string &in) const;
//	void recode_save(std::string &in) const;
	void (*recode_load_vfunc)(std::string &in);
	void (*recode_save_vfunc)(std::string &in);
	
	// ------ debugging -------
	void write(std::ostream &o,const char *ptr)
	{  int size=end_pointer-(ptr-buffer);
	   if (size>40) size=40;
	   o.write(ptr,size);
	}
public:
	// this is the encoding used in the internal data structures
	// if you do not want UTF-8 set this variable to "ISO-8859-1"
	static std::string host_encoding;

	TagStream(const std::string &path);
	TagStream(const char *buf);
	TagStream(std::istream &i);
	TagStream();
	~TagStream();
	
	// deprecated, use getContent !
	const Tag *find(const std::string &type) const
	{  return Tag::find(type); }
	Tag::const_iterator find(Tag::const_iterator it,const std::string &type) const
	{  return Tag::find(it,type); }
	
	// throws std::exception if not found
	const Tag &getContent() const;
	Tag &getContent();
	void setContent(const Tag &c)
	{  try { getContent()=c; } catch (std::exception) { push_back(c); } }
	
	bool good();
	
	// writing Tags to a file
	void setFileName(const std::string &s)
	{  file_name=s; }
	void setEncoding(const std::string &s);
	bool write(const std::string &filename="",const std::string &_encoding="");
	void write(std::ostream &o,bool compact=false) const;
	
	static void utf82iso(std::string &s);
	static std::string base64(const std::string &s, unsigned linelen=0);
	static std::string unbase64(const std::string &s);
private:
	void write(std::ostream &o, const Tag &t, int indent=0, bool indent_first=true,bool compact=false) const;
};

#endif
