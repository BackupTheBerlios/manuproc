/*  Gtk--addons: a collection of gtk-- addons
 *  Copyright (C) 2001-2002 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef SEARCHCOMBOCONTENT
#define SEARCHCOMBOCONTENT

#include"SearchCombo.h"
#include<vector>
#include<string>
#include<cassert>

// g++-3.0 has problems to decide which operator to chose
#if defined(__GNUC__) && __GNUC__==3 && __GNUC_MINOR__==0
#define std_neq(x,y) std::operator!=((x),(y))
#else
#define std_neq(x,y) (x)!=(y)
#endif

template<class T>
class SearchComboContent : public Gtk::SearchCombo
{
 std::vector<T> content;
 
public:

struct ContentError : public std::exception
{  virtual const char* what() const throw() { return "SearchComboContent::ContentError";}
   ContentError() {}
};
       

 
 SearchComboContent() { gobj()->auto_narrow=false; }
 void add_item(const std::string &text, const T &item);
 void reset();
 void clear() {reset();}
 const T &Content() const;
 const T &get_value()const {return Content();}
 void setContent(const std::string &text, const T &c);
 void set_value(const std::string &text, const T &c){setContent(text,c);};

private:
 void set_text(const std::string &text);
};


template<class T>
void SearchComboContent<T>::setContent(const std::string &text, const T &item)
{
 reset();
  
 Gtk::SearchCombo::set_text(text);
 Gtk::SearchCombo::add_item(text);
 content.push_back(item);
}


template<class T>
void SearchComboContent<T>::add_item(const std::string &text, const T &item)
{
 if(size()==0)
   content.clear();
 else assert(size()==content.size());

 Gtk::SearchCombo::add_item(text);
 content.push_back(item);
 
}


template<class T>
const T &SearchComboContent<T>::Content() const
{
 if(size()==0 && std_neq(content.begin(),content.end()))
   const_cast<std::vector<T>&>(content).clear();
 else assert(size()==content.size());
 
 if(empty()) throw ContentError();
 
 if(size()==1) return content[0];

 int i = get_selected_index();
 if (0<=i && size_t(i)<content.size());
    return content[i];

 throw ContentError();
}

template<class T>
void SearchComboContent<T>::reset()
{ content.clear();
  Gtk::SearchCombo::reset();
}

#endif
