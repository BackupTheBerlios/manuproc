/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include"myRow.h"
#include"auftragbase.h"
#include<stdio.h>
#include<Aux/Ausgabe_neu.h>
#include"auftrag_main.hh"

extern bool stuecksumme;/* ob stueck summiert werden sollen */
extern bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
			/* gespeichert werden soll */	

extern auftrag_main *auftragmain;

const string MyRow::getColText() const 
{

 if(seqnr==KW_SEQ)
   {
    return value.Datum().c_str();}
 else
  if(seqnr==AUFID_SEQ)
     {if(auftragmain->interneNamen())
	 return Formatiere(value.getIntVal(),0,6,"","",'0')/*+"."+youraufnr*/;
      else
        return youraufnr/*+"."+str*/;
     }
 else
   if(seqnr==PROZ_SEQ)
     return value.getStrProzVal();
  else
    return value.getStrVal();
}


const string &MyRow::getStkSumText() const 
{
 static string ret;

 if(sumstueck)
   ret=Formatiere((ulong)sumstueck,0,0,"","");
 else ret = "";
 return ret;
}


void MyRow::insertAufEintrag(const AufEintrag& entry, deque<int> &seq, int deep=0)
{
	int tmp = seq.front();
	EntryValue val = entry.getSeqValue(tmp);
	TCListRow::iterator i = tclistrow->begin();

	while((i!=tclistrow->end()) &&
		(val > ((MyRow*)(*i).get_user_data())->getValue())) i++;
	
	if((tmp==AUFM_SEQ) && val) stuecksumme=true;
	if(tmp==AUFID_SEQ) auftragflag=true; 
		
	if(i!=tclistrow->end())
	  {
	   if(val == ((MyRow*)(*i).get_user_data())->getValue())
	     {seq.pop_front();
	      deep = deep ? deep-1 : 0;
	      ((MyRow*)(*i).get_user_data())->summeter+=entry.getRest(); 
	      if(stuecksumme)
	        ((MyRow*)(*i).get_user_data())->sumstueck+=entry.getRestStk(); 
	      if(auftragflag)
		{((MyRow*)(*i).get_user_data())->auftragid=entry.getAuftragid();	
		 ((MyRow*)(*i).get_user_data())->zeilennr=entry.getZnr();
		 ((MyRow*)(*i).get_user_data())->youraufnr=entry.getYourAufNr();
		 ((MyRow*)(*i).get_user_data())->prozessdatum=entry.getProzDat();
		 ((MyRow*)(*i).get_user_data())->prozess=entry.getProzess();
	         ((MyRow*)(*i).get_user_data())->artbase=ArtikelBase(entry.ArtikelID());
		}	
	      if(!(((MyRow*)(*i).get_user_data())->leaf))
	      	((MyRow*)(*i).get_user_data())->insertAufEintrag(entry, seq, deep);
	     }      		
	   else
	     goto insertnew2;  	
	  }
	else
	  {
insertnew2:
	   TCListRow *tclr;
	   MyRow *r = new MyRow(val, (seq.size()==1));
	   r->summeter += entry.getRest(); 
	   if(stuecksumme)
	     r->sumstueck += entry.getRestStk();
	   if(auftragflag)
	     {r->auftragid=entry.getAuftragid();	
	      r->zeilennr=entry.getZnr();
	      r->youraufnr=entry.getYourAufNr();
              r->prozessdatum=entry.getProzDat();
	      r->prozess=entry.getProzess();
              r->artbase=ArtikelBase(entry.ArtikelID());
	     }	
	   r->seqnr = tmp;
	   r->toshow = (deep>0);
	   deep = deep ? deep-1 : 0;
           tclr = new TCListRow(i,r->getColEntries(seq.size()),
           			!r->leaf ? (signed int)(ATTRCOUNT-seq.size())+1:-1,
					r->toshow);
	   tclr->set_user_data(r);
	   r->tclistrow = tclr;
	   if(!r->leaf)
	     {
	      seq.pop_front();
	      if(!seq.empty())
	      r->insertAufEintrag(entry, seq, deep);
	     }
	   }
}


void MyRow::updateSummen()
{
 TCListRow::iterator i = tclistrow->begin();

 tclistrow->relabel(ATTRCOUNT, Formatiere((ulong)summeter));
 if(sumstueck)
    tclistrow->relabel(ATTRCOUNT+1, Formatiere((ulong)sumstueck));
 while(i!=tclistrow->end())
   {((MyRow*)(*i).get_user_data())->updateSummen();i++;}
}


#warning weg mit dem Code!
void MyRow::stutzeBaum(TCList &tcliste, int deep, int showdeep)
{
 string tmpstr;
 while(tclistrow->size()==1)
   {(*(tclistrow->begin())).reparent_children(*tclistrow);
    tclistrow->relabel(deep,((MyRow*)(*(tclistrow->begin())).get_user_data())->getColText()); 
    tclistrow->relabel(ATTRCOUNT+1,((MyRow*)(*(tclistrow->begin())).get_user_data())->getStkSumText());

    sumstueck=((MyRow*)(*(tclistrow->begin())).get_user_data())->sumstueck;
    auftragid=((MyRow*)(*(tclistrow->begin())).get_user_data())->auftragid;
    zeilennr=((MyRow*)(*(tclistrow->begin())).get_user_data())->zeilennr;
    youraufnr=((MyRow*)(*(tclistrow->begin())).get_user_data())->youraufnr;
    prozessdatum=((MyRow*)(*(tclistrow->begin())).get_user_data())->prozessdatum;
    prozess=((MyRow*)(*(tclistrow->begin())).get_user_data())->prozess;
    artbase=((MyRow*)(*(tclistrow->begin())).get_user_data())->artbase;
    leaf=((MyRow*)(*(tclistrow->begin())).get_user_data())->leaf;

/*    if(prozessdatum.valid())
      tclistrow->relabel(ATTRCOUNT+2,prozessdatum.c_str());
    else
      tclistrow->relabel(ATTRCOUNT+2,"-");
*/
    deep++;
    tcliste.erase(tclistrow->begin());
   }
 if(deep<ATTRCOUNT) tclistrow->set_expanding_column(deep);  
 else tclistrow->set_expanding_column(-1);
 if(deep > showdeep) tclistrow->collapse();
 if(tclistrow->size()>1)
   {TCListRow::iterator i = tclistrow->begin();
    deep++;
    while(i!=tclistrow->end())
      {
       ((MyRow*)(*i).get_user_data())->stutzeBaum(tcliste,deep,showdeep); i++;
      }
   }  
 
}


void MyRow::drucken(FILE *f,vector<int> &len,const vector<int>&maxlen)
{const vector<string> &colstr = tclistrow->get_columns();
 if (!f)
 {  for (vector<int>::size_type i=0;i<len.size();++i) 
       if ((unsigned int)(len[i])<colstr[i].size())
       {  if (i<maxlen.size() && colstr[i].size()>(unsigned int)(maxlen[i])) len[i]=maxlen[i];
          else len[i]=colstr[i].size();
       }
 }
 else 
 {  for (vector<int>::size_type i=0;i<len.size();++i) 
    {  if (!len[i] && i!=len.size()-1) 
    	  continue; // suppress '""+" "' but not '""+"\n"'
       fprintf(f,"%*.*s%c",len[i],len[i],colstr[i].c_str()
       		,i==len.size()-1?'\n':' ');
    }
 }
 for (TCListRow::iterator l = tclistrow->begin();l!=tclistrow->end();++l)
    ((MyRow*)(*l).get_user_data())->drucken(f,len,maxlen);
}



const string MyRow::Description(const cH_ExtBezSchema &s) const
{
 string str;
 cH_ArtikelBezeichnung artbez(artbase,s->Id(),ArtikelBezeichnung::dont_throw());

 str=artbez->Bezeichnung();
 return str;   
}

#warning weg damit!
vector<string> &MyRow::getColEntries(int seqdeep)
{
 static vector<string> v;
 int i=ATTRCOUNT;
 
  v.erase(v.begin(),v.end());
 while(i>seqdeep) {v.push_back(""); --i;}
 if(seqnr==KW_SEQ)
   {
    v.push_back(value.Datum().c_str());
   }
 else 
   if(seqnr==AUFID_SEQ)
     {if(auftragmain->interneNamen())
        v.push_back(Formatiere(value.getIntVal(),0,6,"","",'0')/*+"."+youraufnr*/);
      else
        v.push_back(youraufnr/*+"."+str*/);
     }
 else
   if(seqnr==PROZ_SEQ)
     v.push_back(value.getStrProzVal());
 else
   v.push_back(value.getStrVal());



 while(i>1) {v.push_back(""); --i;}
 v.push_back(Formatiere((ulong)summeter,0,0,","));

 if(sumstueck)
   v.push_back(Formatiere((ulong)sumstueck,0,0,","));
 else v.push_back("");

 v.push_back("");

 return v;
}  
