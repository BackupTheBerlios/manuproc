// $Id: Tag.cc,v 1.4 2003/01/24 08:37:48 christof Exp $
/*  glade--: C++ frontend for glade (Gtk+ User Interface Builder)
 *  Copyright (C) 1998-2002  Christof Petig
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

#include <Misc/Tag.h>
#include <algorithm>
#include <utility> // pair
#include <locale.h>
#include <iostream>

#ifdef __MINGW32__
extern "C" { int snprintf(char *str, size_t size, const  char  *format, ...); }
#endif

const Tag *Tag::find(const std::string &tp) const
{  const_iterator i=find(begin(),tp);
   if (i==end()) return 0;
   return &*i;
}

Tag *Tag::find(const std::string &tp)
{  iterator i=find(begin(),tp);
   if (i==end()) return 0;
   return &*i;
}

static std::string quote_nonprintable(const std::string &in)
{  std::string res;
   static const char * const hex= "0123456789ABCDEF";
   for (std::string::const_iterator i=in.begin();i!=in.end();++i)
   {  if ((unsigned char)(*i)<32 || (127<=(unsigned char)(*i) && (unsigned char)(*i)<160))
         switch (*i)
         {  case '\r': res+="\\r"; break;
            case '\t': res+="\\t"; break;
            case '\n': res+="\\n"; break;
            case '\b': res+="\\b"; break;
            case '\0': res+="\\0"; break;
            default: res+=std::string("\\x")+hex[(*i>>4)&0xf]+hex[*i&0xf];
               break;
         }
      else res+=*i;
   }
   return res;
}

void Tag::debug(int recursion_depth,int indent) const
{  std::cout << std::string(indent,' ') << '"' << Type() << '"';
   if (Value()[0]) std::cout << "=\"" << quote_nonprintable(Value()) << '"';
   else std::cout << " @" << this;
   std::cout << "\n";
   for (attvec_t::const_iterator i=attributes.begin();i!=attend();++i)
      std::cout << std::string(indent,' ') << '.' << i->first << "=\"" 
      	<< quote_nonprintable(i->second) << "\"\n";
   if (recursion_depth>2 || (recursion_depth>0 && Type()!="widget"))
      for (const_iterator i=begin();i!=end();++i)
         (*i).debug(recursion_depth-1,indent+2);
}

bool Tag::hasTag(const std::string &typ) const throw()
{  return find(begin(),typ)!=end();
}

template <>
int Tag::parse_value<int>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   // 2do: check
   return atoi(value.c_str());
}

template <>
bool Tag::parse_value<bool>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   if (!strcasecmp(value.c_str(),"true")) return true;
   if (!strcasecmp(value.c_str(),"false")) return false;
   if (!strcasecmp(value.c_str(),"yes")) return true;
   if (!strcasecmp(value.c_str(),"no")) return false;
   std::cerr << "strange bool value: \"" << value << "\"\n";
   return parse_value<int>(value);
}

template <>
long Tag::parse_value<long>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   return atol(value.c_str());
} 

template <>
double Tag::parse_value<double>(const std::string &value) throw(std::out_of_range)
{  if (value.empty()) throw std::out_of_range(value);
   /* Make sure we do NOT honor the locale for numeric input */
   /* since the database gives the standard decimal point */
   std::string oldlocale= setlocale(LC_NUMERIC, NULL);
   setlocale(LC_NUMERIC, "C");
   double f=strtod(value.c_str(),0);
   setlocale(LC_NUMERIC, oldlocale.c_str());
   return f;
}

template <>
float Tag::parse_value<float>(const std::string &value) throw(std::out_of_range)
{  return parse_value<double>(value);
}

template <>
std::string Tag::parse_value<std::string>(const std::string &value) throw(std::out_of_range)
{  return value;
}

#if 0
const std::string Tag::getString(const std::string &typ,const std::string &def) const throw()
{  const_iterator t=find(begin(),typ);
   if (t!=end()) return t->Value();
   return def;
}

bool Tag::getBool(const std::string &typ,bool def) const throw()
{  const_iterator t=find(begin(),typ);
   if (t==end()) return def;
   return parse_bool_value(t->Value());
}

int Tag::getInt(const std::string &typ,int def) const throw()
{  const_iterator t=find(begin(),typ);
   if (t==end()) return def;
   return parse_int_value(t->Value());
}

float Tag::getFloat(const std::string &typ,float def) const throw()
{  const_iterator t=find(begin(),typ);
   if (t==end()) return def;
   return parse_float_value(t->Value());
}
#endif

void Tag::setValue(const std::string &tg,const std::string &value) throw()
{  iterator t=find(begin(),tg);
   if (t==end()) push_back(Tag(tg,value));
   else (*t).Value(value);
}

// this is needed for g++ 3.x 
// - still gives me strange feelings to declare inside std
namespace std {
bool operator==(const std::pair<std::string,std::string> &a,const std::string &b)
{  return a.first==b;
}
}

Tag::attvec_t::iterator Tag::attfind(const std::string &name)
{  return std::find(attributes.begin(),attend(),name); }

Tag::attvec_t::const_iterator Tag::attfind(const std::string &name) const
{  return std::find(attributes.begin(),attend(),name); }

bool Tag::hasAttr(const std::string &name) const throw()
{  return attfind(name)!=attend();
}

#if 0
const std::string &Tag::getAttr(const std::string &name, const std::string &def) const throw()
{  attvec_t::const_iterator i=attfind(name);
   if (i!=attend()) return i->second;
   return def;
}
#endif

void Tag::setAttr(const std::string &name, const std::string &value)
{  attvec_t::iterator i=attfind(name);
   if (i!=attend()) i->second=value;
   else attributes.push_back(std::pair<std::string,std::string>(name,value));
}

#if 0
bool Tag::getBoolAttr(const std::string &typ,bool def) const throw()
{  attvec_t::const_iterator i=attfind(typ);
   if (i!=attend()) return parse_bool_value(i->second,def);
   return def;
}

int Tag::getIntAttr(const std::string &typ,int def) const throw()
{  attvec_t::const_iterator i=attfind(typ);
   if (i!=attend()) return parse_int_value(i->second,def);
   return def;
}

float Tag::getFloatAttr(const std::string &typ,float def) const throw()
{  attvec_t::const_iterator i=attfind(typ);
   if (i!=attend()) return parse_float_value(i->second,def);
   return def;
}
#endif

#include <cstdio>

template<>
 std::string Tag::create_value<int>(const int &val)
{  char buf[30];
   snprintf(buf,sizeof buf,"%d",val);
   return buf; // itos?
}

template<>
 std::string Tag::create_value<double>(const double &val)
{  char buf[30];
   /* Make sure we do NOT honor the locale for numeric input */
   /* since the database gives the standard decimal point */
   std::string oldlocale= setlocale(LC_NUMERIC, NULL);
   setlocale(LC_NUMERIC, "C");
   snprintf(buf,sizeof buf,"%f",val);
   setlocale(LC_NUMERIC, oldlocale.c_str());
   return buf; // dtos
}

template<>
 std::string Tag::create_value<bool>(const bool &val)
{  return val?"true":"false"; 
}
