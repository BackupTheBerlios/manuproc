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

#include"auftragbase.h"
#include"MainTree.hh"
#include<stdio.h>
#include<map>

extern char Meldung[];

AUFENTRYMAP aufentrymap;

extern bool stuecksumme;/* ob stueck summiert werden sollen */
extern bool auftragflag;/* zeigt an ab wann auftragid in den Zeilen */
			/* gespeichert werden soll */	


const string MainTree::getColTitle(int attr, int seq)
{
 string nr;
 string str;

 if(seq) nr=itos(seq);

 switch(attr)
  {
	case KNDNR_SEQ : str="Kunde\n"; return str+nr; break;  
	case ART_SEQ : str="Artikel\n"; return str+nr; break;  
	case BR_SEQ : str="Breite\n"; return str+nr; break;  
	case FB_SEQ : str="Farbe\n"; return str+nr; break;  
	case AUFM_SEQ : str="Aufmachung\n"; return str+nr; break;  
	case KW_SEQ : str="Lieferwoche\n"; return str+nr; break;  
	case AUFID_SEQ : str="Auftrag\n"; return str+nr; break;
	case PROZ_SEQ : str="Verarbeitung\n"; return str+nr; break;
	default : str="-\n";
  }
 return str;
}

#warning weg mit diesem Code !!!
void MainTree::fill(SelectedFullAufList &allids, int deep=0)
{
 vector<AufEintragBase>::iterator i = allids.aufidliste.begin();

 if((!tmpq.empty()) && (tmpq.size()<ATTRCOUNT))
   for(size_t j=1;j<tmpvec.size();j++) {if(tmpvec[j]<0) tmpq.push_back(-1*tmpvec[j]);}
 while(!tmpvec.empty()) tmpvec.pop_back();

 deque<int> seqtitle = tmpq;

 currseq.erase(currseq.begin(),currseq.end());

 while(!seqtitle.empty())
   {
    set_column_title(ATTRCOUNT-seqtitle.size(),
			getColTitle(seqtitle.front(),0));
    currseq.push_back(seqtitle.front());
//cout << "currseq = "<< seqtitle.front()<<"\n";
    seqtitle.pop_front();
    }

 set_column_title(ATTRCOUNT,"offen Meter");
 set_column_justification(ATTRCOUNT, GTK_JUSTIFY_RIGHT);
 set_column_title(ATTRCOUNT+1,"offen Stück");
 set_column_justification(ATTRCOUNT+1, GTK_JUSTIFY_RIGHT);
 set_column_title(ATTRCOUNT+2," ");
 column_titles_show();

 for(size_t i=0; i<columns().size(); ++i)
   set_column_auto_resize(i,true);

 for(; i!=allids.aufidliste.end(); ++i)
   {	int d=deep;
	stuecksumme=auftragflag=false;
	
	deque<int> selseq  = tmpq;

	TCListRow::iterator l = begin();
	
	try
   	{ int mk=(*i).mapKey();
	  AUFENTRYMAP::const_iterator af=aufentrymap.find(mk);
          if(af==aufentrymap.end()) 
	     aufentrymap[mk]=*i;
 	    
	  AufEintrag &tmpentry=aufentrymap[mk];	

	  int tmp = selseq.front();
	  cH_EntryValue val = tmpentry.getSeqValue(tmp);
	  while((l!=end()) &&
		(*val > *(((MyRow*)(*l).get_user_data())->getValue())))
	  	l++;
	
	  if((tmp==AUFM_SEQ) && *(val)) stuecksumme=true;
	  if(tmp==AUFID_SEQ) auftragflag=true;
	  if(l!=end())
	    {
	    if(*val == *(((MyRow*)(*l).get_user_data())->getValue()))
	      {selseq.pop_front();
       	       d = d ? d-1 : 0;
      	       ((MyRow*)(*l).get_user_data())->summeter+=tmpentry.getRest();
      	       if(stuecksumme)
		 ((MyRow*)(*l).get_user_data())->sumstueck+=tmpentry.getRestStk();
	      if(auftragflag)
		 {((MyRow*)(*l).get_user_data())->auftragid=tmpentry.getAuftragid();	
		  ((MyRow*)(*l).get_user_data())->zeilennr=tmpentry.getZnr();	
		  ((MyRow*)(*l).get_user_data())->youraufnr=tmpentry.getYourAufNr();
		  ((MyRow*)(*l).get_user_data())->prozessdatum=tmpentry.getProzDat();
		  ((MyRow*)(*l).get_user_data())->prozess=tmpentry.getProzess();
		  ((MyRow*)(*l).get_user_data())->artbase=ArtikelBase(tmpentry.ArtikelID());
		 }	
      	       ((MyRow*)(*l).get_user_data())->insertAufEintrag(tmpentry,selseq,d);
  	      }
	    else
	      goto insertnew;
	    }
	  else
	    {
insertnew:
	     TCListRow *tclr;
	     MyRow *r = new MyRow(val, selseq.empty());
	     r->summeter+=tmpentry.getRest();
	     if(stuecksumme)
	       r->sumstueck+=tmpentry.getRestStk();
	     if(auftragflag)
	       {r->auftragid=tmpentry.getAuftragid();
                r->zeilennr=tmpentry.getZnr();
                r->youraufnr=tmpentry.getYourAufNr();
                r->prozessdatum=tmpentry.getProzDat();
		r->prozess=tmpentry.getProzess();
                r->artbase=ArtikelBase(tmpentry.ArtikelID());
	       }
	     r->seqnr = tmp;
	     r->toshow = (d>0);
	     d = d ? d-1 : 0;
             tclr = new TCListRow(l,r->getColEntries(selseq.size()),
           			(ATTRCOUNT-selseq.size())+1,r->toshow);
	     tclr->set_user_data(r);
	     r->tclistrow = tclr;           			
	     selseq.pop_front();
	     r->insertAufEintrag(tmpentry, selseq, d);           			
	     }
   	}
   	catch (SQLerror &e)
   	{ meldung->Show(e);
	  if(e.Code() == 100) continue;
	  else break;
   	}
        catch (Petig::Datumsfehler &d)
	{cerr << d; return;}
        catch (...)
	{cerr << "unbehandelte Exception\n"; exit(0);}
   }

}


void MainTree::drucken(FILE *f,vector<int> &len,const vector<int>&maxlen)
{for (TCListRow::iterator l = begin();l!=end();++l) 
   ((MyRow*)(*l).get_user_data())->drucken(f,len,maxlen);
}



void MainTree::on_click_column(int col)
{
 static int sequence=1;

 if(col >= ATTRCOUNT) return;

 if(tmpvec.empty())
   {while(tmpvec.size()<ATTRCOUNT+1)
     tmpvec.push_back(-1 * tmpvec.size());
    sequence=1;
    while(!tmpq.empty())
      tmpq.pop_front();
   }

 if(sequence>ATTRCOUNT) return;

 int seq=currseq[col];

 if(tmpvec[seq]<0)
   {tmpvec[seq]=sequence;
    tmpq.push_back(seq);
    set_column_title(col,getColTitle(seq,sequence));
    sequence++;
   }

 showdeep = tmpq.size();
}


void MainTree::setDefaultSeq()
{
 for(size_t i=0;i<currseq.size();i++)
    set_column_title(i,	getColTitle(currseq[i],0));

 tmpq.push_back(KNDNR_SEQ);
 tmpq.push_back(ART_SEQ);
 tmpq.push_back(BR_SEQ);
 tmpq.push_back(FB_SEQ);
 tmpq.push_back(AUFM_SEQ);
 tmpq.push_back(KW_SEQ);
 tmpq.push_back(AUFID_SEQ);
 tmpq.push_back(PROZ_SEQ);
 
 while(!tmpvec.empty()) tmpvec.pop_back();

}


MainTree::MainTree(int cols) : TCList(cols)
{
 showdeep=0;
 delete_user.connect(SigC::slot(this,&MainTree::onrowdelete));
 click_column.connect(SigC::slot(this,&MainTree::on_click_column));

 tmpq.push_back(KNDNR_SEQ);
 tmpq.push_back(ART_SEQ);
 tmpq.push_back(BR_SEQ);
 tmpq.push_back(FB_SEQ);
 tmpq.push_back(AUFM_SEQ);
 tmpq.push_back(KW_SEQ);
 tmpq.push_back(AUFID_SEQ);
 tmpq.push_back(PROZ_SEQ);

}
