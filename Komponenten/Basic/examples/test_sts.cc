// $Id: test_sts.cc,v 1.2 2004/05/05 13:31:53 christof Exp $
/*  ManuProcWidgets: ManuProC's GUI element library
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Christof Petig
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

#include <SimpleTreeStore.h>
#include <TreeModelDebug.h>
#include <Misc/EntryValueIntString.h>
#include <Misc/EntryValueEmptyInt.h>
#include <Misc/itos.h>
#include <gtkmm/main.h>

enum Spalten
{  SP_ATT0, SP_ATT1, SP_ATT2, SP_ATT3, SP_ATT4, 
   SP_SUM0, SP_ANZ /* , SP_SUM1, SP_SUM2 */ };

class MyRowData : public RowDataBase
{
// your data 
 int intval,i2,i3;
 std::string stringval,s1;
 
public:

 MyRowData(int i,const std::string &s,int _i2,int _i3,const std::string &_s1)
	: intval(i),i2(_i2),i3(_i3),stringval(s),s1(_s1) {}
	
 virtual const cH_EntryValue Value(guint _seqnr,gpointer gp) const
	{	
	 switch((Spalten)_seqnr)
		{case SP_ATT0 : return cH_EntryValueIntString(intval);
		 case SP_ATT1 : return cH_EntryValueIntString(stringval);
		 case SP_ATT2 : return cH_EntryValueIntString(i2);
		 case SP_ATT3 : return cH_EntryValueIntString(i3);
		 case SP_ATT4 : return cH_EntryValueIntString(s1);
 		 case SP_SUM0 : return cH_EntryValueIntString(Data(_seqnr-SP_SUM0));
 		 default : return cH_EntryValue();
		}
	}
 int Data(int i) const
 {  if (!i) return intval; 
    if (i==1) return intval*2;
    return intval*intval;
 }
 void setIntVal(int i) { intval=i; }
 void setIntVal2(int i) { i2=i; }
 int getIntVal() const { return intval; }
 int getIntVal2() const { return i2; }

#if 0
 std::vector<cH_EntryValueIntString> get_orderd(guint _seqnr) const
   {
     
   }
#endif   
};

int main(int argc,char ** argv)
{  Gtk::Main m(argc,argv);
   Glib::RefPtr<SimpleTreeStore> sts=SimpleTreeStore::create(SP_ANZ);

   std::vector <cH_RowDataBase> datavec;
#if 1
   datavec.push_back(new MyRowData(1,"X",2,3,"A"));
   datavec.push_back(new MyRowData(2,"Y",2,3,"A"));
   datavec.push_back(new MyRowData(10,"Z",2,3,"A"));
#endif
   sts->setDataVec(datavec);
   TreeModelDebug(sts);
}
