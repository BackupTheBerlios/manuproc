// $Id: TagStream.cc,v 1.4 2003/10/17 14:31:31 christof Exp $
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

#include <Misc/TagStream.h>
#include <cstring>
#include <unistd.h>
#include <algo.h>

//#define REMEMBER_EMPTY_SPACE

std::string iso2utf8(const std::string &s)
{  std::string ret="";

   for (std::string::const_iterator i = s.begin(); i!=s.end() ; i++)
   {  if ((unsigned char)(*i)>127) 
      {  ret+=(unsigned char)(0xc0|(((unsigned char)(*i))>>6));
         ret+=(unsigned char)(0x80|((*i)&0x3f));
      }
      else ret+=*i;
   }
   return ret;
}

std::string utf82iso(const std::string &s)
{  std::string ret="";

   for (std::string::const_iterator i = s.begin(); i!=s.end() ; i++)
   {  if (((*i)&0xe0)==0xc0)
      {  unsigned char first=*i;
         unsigned char second=*++i;
         ret+=(unsigned char)((first<<6)|(second)&0x3f);
      }
      else if ((unsigned char)(*i)>=0x80)
         std::cout << "UTF8 error " << int(*i) << '\n';
      else ret+=*i;
   }
   return ret;
}

std::string TagStream::host_encoding="UTF-8";

std::string TagStream::recode_load(const std::string &in) const
{  if (encoding==host_encoding) return in;
   if (encoding=="ISO-8859-1" && host_encoding=="UTF-8") return iso2utf8(in);
   if (encoding=="UTF-8" && host_encoding=="ISO-8859-1") return utf82iso(in);
   return encoding+"->"+host_encoding;
}

std::string TagStream::recode_save(const std::string &in) const
{  if (encoding==host_encoding) return in;
   if (host_encoding=="UTF-8" && encoding=="ISO-8859-1") return utf82iso(in);
   if (host_encoding=="ISO-8859-1" && encoding=="UTF-8") return iso2utf8(in);
   return host_encoding+"->"+host_encoding;
}

std::string TagStream::de_xml(const std::string &cont)
{  std::string ret;
   std::string::const_iterator i(cont.begin());
   while (i!=cont.end())
   {  std::string::const_iterator verbatim(::find(i,cont.end(),'&'));
      ret+=std::string(i,verbatim);
      if (verbatim!=cont.end())
      {  std::string::const_iterator endtag(::find(verbatim,cont.end(),';'));
         if (endtag!=cont.end()) ++endtag;
         std::string tag(verbatim,endtag);
         if (tag[1]=='#' && tag[2]=='x')
         {  int c=0;  // hex coded
            for (std::string::const_iterator j=tag.begin()+3; 
               *j!=';' && j!=tag.end();++j)
            {  if ('0' <= *j && *j<='9') c=(c<<4)+(*j-'0');
               else c=(c<<4)+((*j-'A'+10)&0xf);
            }
            ret+=char(c);
         }
         else if (tag[1]=='#' && '0'<=tag[2] && tag[2]<='9')
         {  int c=0;  // decimal coded
            for (std::string::const_iterator j=tag.begin()+3; 
               *j!=';' && j!=tag.end();++j)
               c=c*10+(*j-'0');
            ret+=char(c);
         }
         else if (tag=="&amp;") ret+='&';
         else if (tag=="&lt;") ret+='<';
         else if (tag=="&gt;") ret+='>';
         else if (tag=="&quot;") ret+='"';
         else if (tag=="&auml;") ret+="ä"; // and so on ... but glade simply passes them
         else
         {  ret+=tag;
         }
         i=endtag;
      }
      else break;
   }
   return ret;
}

TagStream::TagStream(const std::string &path) 
	: Tag(""), read_again(false), pointer(0), end_pointer(0), is(0), ifs(0), iss(0)
{  ifs=new std::ifstream(path.c_str());
   is=ifs;
   is->read(buffer,GB_BUFFER_SIZE);
   end_pointer=is->gcount();
   load_project_file(this);
}

TagStream::TagStream(const char *str) 
	: Tag(""), read_again(false), pointer(0), end_pointer(0), is(0), ifs(0), iss(0)
{  iss=new std::istrstream(str,strlen(str));
   is=iss;
   is->read(buffer,GB_BUFFER_SIZE);
   end_pointer=is->gcount();
   load_project_file(this);
}

TagStream::TagStream() 
	: Tag(""), read_again(false), pointer(0), end_pointer(0), is(0), ifs(0), iss(0), encoding(host_encoding) {}
 
TagStream::TagStream(std::istream &i) 
	: Tag(""), read_again(false), pointer(0), end_pointer(0), is(0), ifs(0), iss(0)
{  is=&i;
   is->read(buffer,GB_BUFFER_SIZE);
   end_pointer=is->gcount();
   load_project_file(this);
}

TagStream::~TagStream()
{  if (ifs) {  delete ifs; ifs=0; }
   if (iss) {  delete iss; iss=0; }
   is=0;
}

void TagStream::load_project_file(Tag *top)
{  encoding=host_encoding;
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
      parent->push_back(Tag("",recode_load(de_xml(std::string(buffer+pointer,bra-(buffer+pointer))))));
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
{  return isalnum(x)||strchr(".-_:",x)||x>=0x80;
}

static bool isword0(unsigned char x)
// accepting d7 and f7 violates standard ... who cares
{  return isalnum(x)||strchr("_:",x)||x>=0x80;
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
         
         Tag *newtag(&parent->push_back(Tag(recode_load(std::string(tag+1,tagend-(tag+1))),"")));
         while (tagend)
         {  while (isspace(*tagend)) tagend++;
            if (*tagend=='?')
            {  if (tagend[1]!='>') ERROR2("strange tag end (?[^>])",tag);
               set_pointer(tagend+2);
               if (newtag->Type()=="?xml") 
               {  encoding=newtag->getAttr("encoding");
                  if (encoding.empty()) encoding=host_encoding;
                  if (encoding!=host_encoding)
                     std::cout << "Recoding " << encoding << "->" << host_encoding << '\n';
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
               {  newtag->setAttr(recode_load(std::string(tagend,attrend-tagend)),
               		recode_load(de_xml(std::string(valuestart,valueend-valuestart))));
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
         parent->push_back(Tag("--",recode_load(std::string(tag+3,endcomment-(tag+3)))));
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
         
         parent->push_back(Tag(recode_load(std::string(tag+1,tagend-(tag+1))),
         	std::string(value,valueend-value)));
         set_pointer(valueend+1);
         continue; // outer
      }
      // normal tag
      {  char *tagend=find_wordend(tag+1);
         if (!tagend) ERROR2("tag doesn't end",tag);
         
         Tag *newtag(&parent->push_back(Tag(recode_load(std::string(tag+1,tagend-(tag+1))),"")));
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
               {  newtag->setAttr(recode_load(std::string(tagend,attrend-tagend)),
               		recode_load(de_xml(std::string(valuestart,valueend-valuestart))));
                  tagend=valueend+1;
               } else ERROR2("value does not end",valuestart);
            }
            else ERROR2("strange attribute char",tagend);
         }
         char *tagvalue=tagend+1;
         char *valueend=find(tagvalue,'<');
         if (!valueend) ERROR2("premature value end",tagvalue);
         if (more_than_space(tagvalue,valueend))
            newtag->Value(recode_load(de_xml(std::string(tagvalue,valueend-tagvalue))));
         set_pointer(valueend);
         if (valueend[1]!='/') tagvalue=next_tag(newtag); // recurse
         else tagvalue=valueend;
         
         if (!tagvalue) ERROR2("premature nested value end","?? EOF ???");
         if (tagvalue[1]!='/') ERROR2("not ending?",tagvalue);
         char *endtagend=find(tagvalue+1,'>');
         if (!endtagend) ERROR2("endtag doesn't end",valueend);
         if (recode_load(std::string(tagvalue+2,endtagend-(tagvalue+2)))!=newtag->Type())
         {  std::cerr << "tag <" << newtag->Type() << "> ended with </";
            std::cerr.write(tagvalue+2,endtagend-(tagvalue+2)) << ">\n";
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

static std::string toXML(const std::string &s)
{  std::string res;
   for (std::string::const_iterator i=s.begin();i!=s.end();++i)
   {  if (isalnum(*i)) res+=*i;
      else if ((unsigned char)*i>=160) res+=*i;
      else if (strchr("# @^+-*/.,?!$'`|~[]{}()_:;=",*i)) res+=*i;
      else if (*i=='&') res+="&amp;";
      else if (*i=='<') res+="&lt;";
      else if (*i=='>') res+="&gt;";
      else if (*i=='"') res+="&quot;";
      else res+=std::string("&#x")+HEX[(*i>>4)&0xf]+HEX[*i&0xf]+';';
   }
   return res;
}

void TagStream::write(std::ostream &o, const Tag &t, int indent,bool indent_first,bool compact) const
{  if (indent_first && !compact) o << '\n' << std::string(indent,' ');
   if (!t.Type().empty()) 
   {  o << "<" << recode_save(t.Type());
      // save attributes 
      for (Tag::const_attiterator i=t.attbegin();i!=t.attend();++i)
      {  o << ' ' << recode_save(i->first) << "=\"" << toXML(recode_save(i->second)) << '\"';
      }
      // save content ...
      if (t.begin()!=t.end() || !t.Value().empty())
      {  indent++;
         o << '>';
         o << toXML(recode_save(t.Value()));
         bool indent_next=t.Value().empty();
         for (Tag::const_iterator i=t.begin();i!=t.end();++i) 
         {  write(o,*i,indent,indent_next,compact);
            indent_next=!i->Type().empty();
         }
         if (indent_next && !compact) o << '\n' << std::string(indent-1,' ');
         o << "</" << recode_save(t.Type()) << '>';
      }
      else o << "/>";
   }
   else o << t.Value();
}

void TagStream::write(std::ostream &o,bool compact) const
{  o << "<?xml version=\"1.0\" encoding=\"" << encoding << "\"?>";
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
