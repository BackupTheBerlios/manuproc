// $Id: TagStream.cc,v 1.10 2004/06/04 10:36:32 christof Exp $
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

#include <Misc/TagStream.h>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <Misc/inbetween.h>
#if defined(__GNUC__) && __GNUC__<3
#  include <strstream.h>
#else
#  include <sstream>
#endif
#include <fstream>

//#define REMEMBER_EMPTY_SPACE

static void iso2utf8(std::string &s)
{  for (unsigned i = 0; i<s.size() ; ++i)
   {  unsigned char x=s[i];
      if (x>127) 
      {  char buf[2];
         buf[0]=0xc0|(x>>6);
         buf[1]=0x80|(x&0x3f);
         s.replace(i,1,buf,2);
         ++i;
      }
   }
}

void TagStream::utf82iso(std::string &s)
{  for (unsigned i = 0; i+1<s.size() ; ++i)
   {  unsigned char x=s[i];
      if ((x&0xe0)==0xc0) 
      {  unsigned char y=s[i+1];
         unsigned char r=(x<<6)|(y&0x3f);
         s.replace(i,2u,1u,char(r));
      }
      else if (x>=0x80)
         std::cout << "UTF8 decoding error " << x << '\n';
   }
}

std::string TagStream::host_encoding="UTF-8";

void TagStream::de_xml(std::string &cont)
{  unsigned i(0);
   while (i<cont.size())
   {  std::string::iterator verbatim(std::find(cont.begin()+i,cont.end(),'&'));
      if (verbatim!=cont.end())
      {  std::string::iterator endtag(std::find(verbatim,cont.end(),';'));
         if (endtag!=cont.end()) ++endtag;
         std::string tag(verbatim,endtag);
         if (tag[1]=='#' && tag[2]=='x')
         {  int c=0;  // hex coded
            for (std::string::const_iterator j=tag.begin()+3; 
               *j!=';' && j!=tag.end();++j)
            {  if ('0' <= *j && *j<='9') c=(c<<4)+(*j-'0');
               else c=(c<<4)+((*j-'A'+10)&0xf);
            }
            cont.replace(verbatim,endtag,1,char(c));
            ++i;
         }
         else if (tag[1]=='#' && '0'<=tag[2] && tag[2]<='9')
         {  int c=0;  // decimal coded
            for (std::string::const_iterator j=tag.begin()+2; 
               *j!=';' && j!=tag.end();++j)
               c=c*10+(*j-'0');
            cont.replace(verbatim,endtag,1,char(c));
            ++i;
         }
         else if (tag=="&amp;") { cont.replace(verbatim,endtag,1,'&'); ++i; }
         else if (tag=="&lt;") { cont.replace(verbatim,endtag,1,'<'); ++i; }
         else if (tag=="&gt;") { cont.replace(verbatim,endtag,1,'>'); ++i; }
         else if (tag=="&quot;") { cont.replace(verbatim,endtag,1,'"'); ++i; }
         else if (tag=="&auml;") 
         {  std::string nw="ä"; // assumes host_encoding=="UTF-8"
            if (recode_load_vfunc) (*recode_load_vfunc)(nw);
            cont.replace(verbatim,endtag,nw); i+=nw.size(); 
         }
         	// and so on ... but glade simply passes them
         else
         {  i=endtag-cont.begin(); 
         }
      }
      else break;
   }
}

TagStream::TagStream(const std::string &path) 
	: Tag(std::string()), read_again(), pointer(), end_pointer(), is(), 
	  ifs(), iss(), recode_load_vfunc(), recode_save_vfunc()
{  ifs=new std::ifstream(path.c_str());
   is=ifs;
   is->read(buffer,GB_BUFFER_SIZE);
   end_pointer=is->gcount();
   load_project_file(this);
}

TagStream::TagStream(const char *str) 
	: Tag(std::string()), read_again(), pointer(), end_pointer(), is(), 
	  ifs(), iss(), recode_load_vfunc(), recode_save_vfunc()
{  
#if defined(__GNUC__) && __GNUC__<3
   iss=new std::istrstream(str,strlen(str));
#else
   iss=new std::istringstream(str);
#endif
   is=iss;
   is->read(buffer,GB_BUFFER_SIZE);
   end_pointer=is->gcount();
   load_project_file(this);
}

TagStream::TagStream() 
	: Tag(std::string()), read_again(), pointer(), end_pointer(), is(), 
	  ifs(), iss(), encoding(host_encoding), recode_load_vfunc(), recode_save_vfunc() 
{}
 
void TagStream::setEncoding(const std::string &s)
{  if (s.empty()) encoding=host_encoding;
   else encoding=s;
   if (encoding==host_encoding) 
   {  recode_load_vfunc=recode_save_vfunc=0; }
   else if (encoding=="UTF-8" && host_encoding=="ISO-8859-1")
   {  recode_load_vfunc=&utf82iso; recode_save_vfunc=&iso2utf8; }
   else if (host_encoding=="UTF-8" && encoding=="ISO-8859-1")
   {  recode_save_vfunc=&utf82iso; recode_load_vfunc=&iso2utf8; }
   else
   {  std::cerr << "don't know how to convert " << encoding << "<->" << host_encoding << '\n';
   }
}

TagStream::TagStream(std::istream &i) 
	: Tag(std::string()), read_again(), pointer(), end_pointer(), is(), 
	  ifs(), iss(), recode_load_vfunc(), recode_save_vfunc()
{  is=&i;
   is->read(buffer,GB_BUFFER_SIZE);
   end_pointer=is->gcount();
   load_project_file(this);
}

TagStream::~TagStream()
{  if (ifs) {  delete static_cast<std::ifstream*>(ifs); ifs=0; }
   if (iss) {  delete iss; iss=0; }
   is=0;
}

void TagStream::load_project_file(Tag *top)
{  setEncoding(host_encoding);
   if (end_pointer>3 && buffer[0]==0xef && buffer[1]==0xbb && buffer[2]==0xbf)
      set_pointer(buffer+3);
   while (good() && next_tag(top));
}

bool TagStream::good()
{  return pointer<end_pointer;
}

static bool more_than_space(const char *b,const char *e)
{  while (b!=e)
   {  if (*b!=' ' && *b!='\t' && *b!='\r' && *b!='\n') return true;
      ++b;
   }
   return false;
}

// winding
char *TagStream::next_tag_pointer(Tag *parent)
{  if (pointer>GB_BUFFER_SIZE/2 && end_pointer==GB_BUFFER_SIZE)
   {  memmove(buffer,buffer+pointer,GB_BUFFER_SIZE-pointer);
      end_pointer-=pointer;
      pointer=0;
      is->read(buffer+end_pointer,GB_BUFFER_SIZE-end_pointer);
      end_pointer+=is->gcount();
   }
   char *bra=find('<');
   char *result=bra;
   if (!bra) bra=buffer+end_pointer;
   if (bra>buffer+pointer && more_than_space(buffer+pointer,bra)) 
   {  std::string cont=std::string(buffer+pointer,bra-(buffer+pointer));
      de_xml(cont);
      if (recode_load_vfunc) (*recode_load_vfunc)(cont);
      parent->push_back(Tag(std::string(),cont));
   }
   set_pointer(bra);
   return result;
}

#if 0
#define ERROR3(name,ptr,end) \
	{ std::cerr << name " @'"; \
	  std::cerr.write(ptr,ptr+10>end?end-ptr:10); \
	  std::cerr << "'\n"; \
	  return 0; \
	}
#endif	
#define ERROR2(name,ptr) \
	({ std::cerr << name " @'"; \
	  std::cerr.write(ptr,10); \
	  std::cerr << "'\n"; \
	  return 0; \
	})

// hacked to accept UTF-8 literals, this is not a generally good idea

static bool isword(unsigned char x)
// accepting d7 and f7 violates standard ... who cares
{  return isalnum(x)||in<char>(x,'.','-','_',':')||x>=0x80;
}

static bool isword0(unsigned char x)
// accepting d7 and f7 violates standard ... who cares
{  return isalnum(x)||in<char>(x,'_',':')||x>=0x80;
}

char *TagStream::find_wordend(char *ptr)
{  while (ptr<buffer+end_pointer && isword(*ptr)) ++ptr;
   return ptr>=buffer+end_pointer ? 0 : ptr;
}

char *TagStream::find_wordend2(char *ptr)
{  while (ptr<buffer+end_pointer && *ptr!='>' && *ptr!=' ') ++ptr;
   return ptr>=buffer+end_pointer ? 0 : ptr;
}

char *TagStream::find_tagend2(char *ptr)
{  while (ptr<buffer+end_pointer && *ptr!='>')
   {  if (*ptr=='"')
      {  ++ptr;
         while (ptr<buffer+end_pointer && *ptr!='"') ++ptr;
         if (ptr>=buffer+end_pointer) return 0;
      }
      ++ptr;
   }
   return ptr>=buffer+end_pointer ? 0 : ptr;
}

// returns unmatched </tag> or 0
// set pointer should wind
// unify the errors
char *TagStream::next_tag(Tag *parent)
{  char *tag;
   while ((tag=next_tag_pointer(parent)))
   {  if (tag[1]=='?') // meta tag
      {  char *tagend=find_wordend(tag+2);
         if (!tagend) ERROR2("tag doesn't end",tag);
         
         std::string newtagtype=std::string(tag+1,tagend-(tag+1));
	 if (recode_load_vfunc) (*recode_load_vfunc)(newtagtype);         
         Tag *newtag(&parent->push_back(Tag(newtagtype,"")));
         while (tagend)
         {  while (isspace(*tagend)) tagend++;
            if (*tagend=='?')
            {  if (tagend[1]!='>') ERROR2("strange tag end (?[^>])",tag);
               set_pointer(tagend+2);
               if (newtag->Type()=="?xml") 
               {  setEncoding(newtag->getAttr("encoding"));
               }
               goto continue_outer;
            }
            if (isword0(*tagend))
            {  char *attrend=find_wordend(tagend);
               if (!attrend || *attrend!='=' || 
               	(attrend[1]!='"' && attrend[1]!='\''))
                  ERROR2("strange attribute",tagend);
               char *valuestart(attrend+2);
               char *valueend(find(valuestart,attrend[1]));
               if (valueend)
               {  std::string cont=std::string(valuestart,valueend-valuestart);
                  std::string name=std::string(tagend,attrend-tagend);
                  de_xml(cont);
                  if (recode_load_vfunc) 
                  {  (*recode_load_vfunc)(name);
                     (*recode_load_vfunc)(cont);
                  }
                  newtag->setAttr(name,cont);
                  tagend=valueend+1;
               }
               else ERROR2("value does not end",valuestart);
            }
            else ERROR2("strange attribute char",tag);
         }         
         continue; // outer
      }
      if (tag[1]=='-' && tag[2]=='-')
      {  char *endcomment=find(tag+3,'-');
         while (endcomment && endcomment[1]!='-' && endcomment[2]!='>')
            endcomment=find(endcomment+1,'-');
         if (!endcomment) ERROR2("Comment does not end",tag);
         std::string cont=std::string(tag+3,endcomment-(tag+3));
         de_xml(cont);
	 if (recode_load_vfunc) (*recode_load_vfunc)(cont);         
         parent->push_back(Tag("--",cont));
         set_pointer(endcomment+3);
         continue; // outer
      }
      if (tag[1]=='/') 
         return tag; // unmatched </tag>
      if (tag[1]=='!') // special tag
      {  char *tagend=find_wordend2(tag+1);
         if (!tagend) ERROR2("tag doesn't end",tag);
         
         char *value=tagend,*valueend=tagend;
         if (*tagend!='>') 
         { value=tagend+1;
           valueend=find_tagend2(value);
           if (!valueend) ERROR2("tag doesn't end",tag);
         }
         
         std::string cont=std::string(value,valueend-value);
         std::string name=std::string(tag+1,tagend-(tag+1));
         de_xml(cont);
         if (*recode_load_vfunc)
         {  (*recode_load_vfunc)(name);
            (*recode_load_vfunc)(cont);
         }
         parent->push_back(Tag(name,cont));
         set_pointer(valueend+1);
         continue; // outer
      }
      // normal tag
      {  char *tagend=find_wordend(tag+1);
         if (!tagend) ERROR2("tag doesn't end",tag);
         
         std::string newtagtype=std::string(tag+1,tagend-(tag+1));
	 if (*recode_load_vfunc) (*recode_load_vfunc)(newtagtype);
         Tag *newtag(&parent->push_back(Tag(newtagtype,"")));
         // read attributes
         while (tagend)
         {  while (isspace(*tagend)) tagend++;
            if (*tagend=='/')
            {  if (tagend[1]!='>') ERROR2("strange tag end (/[^>])",tag);
               set_pointer(tagend+2);
               goto continue_outer;
            }
            if (*tagend=='>') break;
            if (isword0(*tagend))
            {  char *attrend=find_wordend(tagend);
               if (!attrend || *attrend!='=' || 
               	(attrend[1]!='"' && attrend[1]!='\''))
                  ERROR2("strange attribute",tagend);
               char *valuestart(attrend+2);
               char *valueend(find(valuestart,attrend[1]));
               if (valueend)
               {  std::string cont=std::string(valuestart,valueend-valuestart);
                  std::string name=std::string(tagend,attrend-tagend);
                  de_xml(cont);
                  if (recode_load_vfunc)
                  {  (*recode_load_vfunc)(name);
                     (*recode_load_vfunc)(cont);
                  }
                  newtag->setAttr(name,cont);
                  tagend=valueend+1;
               } else ERROR2("value does not end",valuestart);
            }
            else ERROR2("strange attribute char",tagend);
         }
         char *tagvalue=tagend+1;
         char *valueend=find(tagvalue,'<');
         if (!valueend) ERROR2("premature value end",tagvalue);
         if (more_than_space(tagvalue,valueend))
         {  std::string cont=std::string(tagvalue,valueend-tagvalue);
            de_xml(cont);
            if (recode_load_vfunc) (*recode_load_vfunc)(cont);
            newtag->Value(cont);
         }
         set_pointer(valueend);
         if (valueend[1]!='/') tagvalue=next_tag(newtag); // recurse
         else tagvalue=valueend;
         
         if (!tagvalue) ERROR2("premature nested value end","?? EOF ???");
         if (tagvalue[1]!='/') ERROR2("not ending?",tagvalue);
         char *endtagend=find(tagvalue+1,'>');
         if (!endtagend) ERROR2("endtag doesn't end",valueend);
         std::string endtype=std::string(tagvalue+2,endtagend-(tagvalue+2));
	 if (recode_load_vfunc) (*recode_load_vfunc)(endtype);         
         if (endtype!=newtag->Type())
         {  std::cerr << "tag <" << newtag->Type() << "> ended with </" 
         		<< endtype << ">\n";
         }
         set_pointer(endtagend+1);
       }
       continue_outer: ;
   }
   return 0;
}

const Tag &TagStream::getContent() const
{  FOR_EACH_CONST_TAG(i,*this)
   {  if (!i->Type().empty() && i->Type()[0]!='?'
   		 && i->Type()[0]!='!' && i->Type()[0]!='-')
         return *i;
   }
   throw std::exception();
}

Tag &TagStream::getContent()
{  FOR_EACH_TAG(i,*this)
   {  if (!i->Type().empty() && i->Type()[0]!='?'
   		 && i->Type()[0]!='!' && i->Type()[0]!='-')
         return *i;
   }
   throw std::exception();
}

static const char HEX[]="0123456789ABCDEF";

static void toXML(std::string &s)
{  // for (std::string::iterator i=s.begin();i!=s.end();)
   for (unsigned i=0;i<s.size();)
   {  if (isalnum(s[i]) 
		|| (unsigned char)s[i]>=160
		|| strchr("# @^+-*/.,?!$'|~[]{}()_:;=",s[i])) 
         ++i;
      else if (s[i]=='&') { s.replace(i,1,"&amp;"); i+=5; }
      else if (s[i]=='<') { s.replace(i,1,"&lt;"); i+=4; }
      else if (s[i]=='>') { s.replace(i,1,"&gt;"); i+=4; }
      else if (s[i]=='"') { s.replace(i,1,"&quot;"); i+=6; }
      else 
      {  s.replace(i,1,std::string("&#x")+HEX[(s[i]>>4)&0xf]+HEX[s[i]&0xf]+';');
         i+=6;
      }
   }
}

void TagStream::write(std::ostream &o, const Tag &t, int indent,bool indent_first,bool compact) const
{  if (indent_first && !compact) o << '\n' << std::string(indent,' ');
   if (!t.Type().empty()) 
   {  std::string ttype(t.Type());
      if (recode_save_vfunc) (*recode_save_vfunc)(ttype);
      o << "<" << ttype;
      // save attributes 
      for (Tag::const_attiterator i=t.attbegin();i!=t.attend();++i)
      {  std::string attname=i->first, attval=i->second;
         if (recode_save_vfunc) 
         {  (*recode_save_vfunc)(attname);
            (*recode_save_vfunc)(attval);
         }
         toXML(attval);
         o << ' ' << attname << "=\"" << attval << '\"';
      }
      // save content ...
      if (t.begin()!=t.end() || !t.Value().empty())
      {  indent++;
         o << '>';
         std::string tval(t.Value());
         if (recode_save_vfunc) (*recode_save_vfunc)(tval);
         toXML(tval);
         o << tval;
         bool indent_next=t.Value().empty();
         for (Tag::const_iterator i=t.begin();i!=t.end();++i) 
         {  write(o,*i,indent,indent_next,compact);
            indent_next=!i->Type().empty();
         }
         if (indent_next && !compact) o << '\n' << std::string(indent-1,' ');
         std::string ttype(t.Type());
         if (recode_save_vfunc) (*recode_save_vfunc)(ttype);
         o << "</" << ttype << '>';
      }
      else o << "/>";
   }
   else 
   {  std::string tval(t.Value());
      if (recode_save_vfunc) (*recode_save_vfunc)(tval);
      toXML(tval);
      o << tval;
   }
}

void TagStream::write(std::ostream &o,bool compact) const
{  if (encoding=="UTF-8") // BOM
      o << "﻿"; // BOM
   o << "<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>";
   if (!compact) o << '\n';
   write(o, getContent(),0,true,compact);
   if (!compact) o << '\n';
}

bool TagStream::write(const std::string &filename,const std::string &_encoding)
{  if (!filename.empty()) setFileName(filename);
   if (!_encoding.empty()) setEncoding(_encoding);
   std::ofstream os(filename.c_str());
   write(os);
   return os.good();
}
