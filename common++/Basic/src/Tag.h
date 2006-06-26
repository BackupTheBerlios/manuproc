// $Id: Tag.h,v 1.13 2006/06/26 07:53:19 christof Exp $
/*  ManuProC_Base: Main ManuProC Library
 *  Copyright (C) 1998-2003  Christof Petig
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

#ifndef TAG_HH
#define TAG_HH
#include <ManuProCConfig.h>
#include <string>
#include <list>
#include <map>
#include <algorithm> // for find
#include <stdexcept>

class Tag {
    	std::string type;
    	std::string value;
    	typedef std::map<std::string,std::string> attvec_t;
    	attvec_t attributes;
    	std::list<Tag> sub_specifications;
    	Tag *parent_ptr;

	void repair_back_pointer() throw();

public: // nice to have for custom parsing
    	typedef std::list<Tag>::difference_type difference_type;
    	template <class T>
    	 static T parse_value(const std::string &val) throw(std::out_of_range);
    	template <class T>
    	 static T parse_value_def(const std::string &val, const T &def);
    	template <class T>
    	 static std::string create_value(const T &val);
    	 
public:
    	Tag(const std::string &t,const std::string &v=std::string()) throw()
		: type(t), value(v), parent_ptr(0)
	{}
	// this makes a copy !!!
	Tag &push_back(const Tag t) throw();
	Tag &push_back(std::string const& t, std::string const& v=std::string()) throw()
	{ return push_back(Tag(t,v)); }
	typedef std::list<Tag>::iterator iterator;
	typedef std::list<Tag>::const_iterator const_iterator;
	const_iterator begin() const throw()
	{  return sub_specifications.begin(); }
	const_iterator end() const throw()
	{  return sub_specifications.end(); }
	const Tag &back() const throw()
	{  return sub_specifications.back(); }
	std::list<Tag>::size_type size() const throw()
	{  return sub_specifications.size(); }
	void reserve(std::list<Tag>::size_type n) throw()
	{  } // return sub_specifications.reserve(n); }
	iterator begin() throw()
	{  return sub_specifications.begin(); }
	iterator end() throw()
	{  return sub_specifications.end(); }
	Tag &back() throw()
	{  return sub_specifications.back(); }
	Tag *parent() throw()
	{  return parent_ptr; }
	const Tag *parent() const throw()
	{  return parent_ptr; }
	
	std::string const& Type() const throw()
	{  return type; }
	void Type(const std::string &val) throw()
	{  type=val; }
	std::string const& Value() const throw()
	{  return value; }
	void Value(const std::string &val) throw()
	{  value=val; }

	// slow variants
	const Tag *find(const std::string &type) const;
	Tag *find(const std::string &type);
	// please prefer these finds, they are faster!
	const_iterator find(const_iterator it,const std::string &type) const
	{  return std::find(it,end(),type); }
	const_iterator find(const_iterator it,const std::string &type)
	{  return std::find(it,const_cast<const Tag*>(this)->end(),type); }
	iterator find(iterator it,const std::string &type)
	{  return std::find(it,end(),type); }

	// values of attributes
	typedef attvec_t::const_iterator const_attiterator;
	const_attiterator attbegin() const { return attributes.begin(); }
	attvec_t::iterator attend() { return attributes.end(); }
	const_attiterator attend() const { return attributes.end(); }
	attvec_t::iterator attfind(const std::string &name);
	const_attiterator attfind(const std::string &name) const;
	
	template <class T>
	 T getAttr(const std::string &name) const throw(std::out_of_range);
	template <class T>
	 T getAttr_def(const std::string &name, const T &def=T()) const throw()
    	{  try { return getAttr<T>(name); } 
    	   catch (std::out_of_range &e) { return def; }
    	}
	bool hasAttr(const std::string &name) const throw();
	void setAttr(const std::string &name, const std::string &value);
	template <class T>
	 void setAttr(const std::string &name, T const& val)
	{ setAttr(name,create_value<T>(val));
	}
	
	// values of substructures
	bool hasTag(const std::string &typ) const throw();
	void setValue(const std::string &key,const std::string &value) throw();
	template <class T>
	 T getValue(const std::string &key) const;
	template <class T>
	 T getValue_def(const std::string &key, const T &def) const throw()
	{  try { return getValue<T>(key); } 
    	   catch (std::out_of_range &e) { return def; }
        }
	
	bool operator==(const std::string &tp) const
	{  return type==tp; }
	bool operator!=(const std::string &tp) const
	{  return type!=tp; }
	
	void debug(int recursion_depth=2,int indent=0) const;

	// template specialization declarations
	
#ifndef TAG_OMIT_DEPRECATED
	const std::string getAttr(const std::string &name, const std::string &def=std::string()) const throw()
	{  return getAttr_def<std::string>(name,def); }
	bool getBoolAttr(const std::string &typ,bool def=false) const throw()
	{  return getAttr_def<bool>(typ,def); }
	int getIntAttr(const std::string &typ,int def=0) const throw()
	{  return getAttr_def<int>(typ,def); }
	float getFloatAttr(const std::string &typ,float def=0) const throw()
	{  return getAttr_def<float>(typ,def); }
	void setIntAttr(const std::string &name, int val);
	void setFloatAttr(const std::string &name, double val);
	void setBoolAttr(const std::string &name, bool val);
	// Spezialfall: nur setzen wenn !=""
	void setAttr_ne(const std::string &name, const std::string &value)
	{  if (!value.empty()) setAttr(name,value); }
	void setIntAttr_nn(const std::string &name, int val)
	{  if (val) setIntAttr(name,val); }

	const std::string getString(const std::string &typ,const std::string &def=std::string()) const throw()
	{  return getValue_def<std::string>(typ,def); }
	bool getBool(const std::string &typ,bool def=false) const throw()
	{  return getValue_def<bool>(typ,def); }
	int getInt(const std::string &typ,int def=0) const throw()
	{  return getValue_def<int>(typ,def); }
	float getFloat(const std::string &typ,float def=0) const throw()
	{  return getValue_def<float>(typ,def); }
	void mark(const std::string &key,const std::string &value) throw()
	{  setValue(key,value); }
	
#endif
};

// some nice macros

#define FOR_EACH_CONST_TAG(variable,parent) \
	for (Tag::const_iterator variable=(parent).begin(); \
		(variable)!=(parent).end(); ++(variable))

#define FOR_EACH_CONST_TAG_OF(variable,parent,type) \
	for (Tag::const_iterator variable= \
			(parent).find((parent).begin(),(type)); \
		(variable)!=(parent).end(); \
		variable=(parent).find(++(variable),(type)))

#define FOR_EACH_CONST_TAG_OF_5(variable,parent,begin,end,type) \
	for (Tag::const_iterator variable= \
			(parent).find((begin),(type)); \
		(variable)!=(end); \
		variable=(parent).find(++(variable),(type)))

#define FOR_EACH_TAG(variable,parent) \
	for (Tag::iterator variable=(parent).begin(); \
		(variable)!=(parent).end(); ++(variable))

#define FOR_EACH_TAG_OF(variable,parent,type) \
	for (Tag::iterator variable= \
			(parent).find((parent).begin(),(type)); \
		(variable)!=(parent).end(); \
		variable=(parent).find(++(variable),(type)))

#define FOR_EACH_TAG_OF_5(variable,parent,begin,end,type) \
	for (Tag::iterator variable= \
			(parent).find((begin),(type)); \
		(variable)!=(end); \
		variable=(parent).find(++(variable),(type)))

// you can skip these declarations while reading this file
template <> std::string Tag::create_value<int>(const int &val);
template <> std::string Tag::create_value<double>(const double &val);
template <> std::string Tag::create_value<bool>(const bool &val);
template <> int Tag::parse_value<int>(const std::string &value) throw(std::out_of_range);
template <> bool Tag::parse_value<bool>(const std::string &value) throw(std::out_of_range);
template <> long Tag::parse_value<long>(const std::string &value) throw(std::out_of_range);
template <> double Tag::parse_value<double>(const std::string &value) throw(std::out_of_range);
template <> float Tag::parse_value<float>(const std::string &value) throw(std::out_of_range);
template <> std::string Tag::parse_value<std::string>(const std::string &value) throw(std::out_of_range);

#ifndef TAG_OMIT_DEPRECATED
inline void Tag::setIntAttr(const std::string &name, int val)
{  setAttr(name,create_value<int>(val)); }
inline void Tag::setFloatAttr(const std::string &name, double val)
{  setAttr(name,create_value<double>(val)); }
inline void Tag::setBoolAttr(const std::string &name, bool val)
{  setAttr(name,create_value<bool>(val)); }
#endif

// g++ 2.95 does not like these inlined
template <class T>
 T Tag::parse_value_def(const std::string &val, const T &def)
{  try { return parse_value<T>(val); } 
   catch (std::out_of_range &e) { return def; }
}

template <class T>
 T Tag::getAttr(const std::string &name) const throw(std::out_of_range)
{  const_attiterator t=attfind(name);
   if (t==attend()) throw std::out_of_range(name);
   return parse_value<T>(t->second);
}

template <class T>
 T Tag::getValue(const std::string &key) const
{  const_iterator t=find(begin(),key);
   if (t==end()) throw std::out_of_range(key);
   return parse_value<T>(t->Value());
}

#endif
