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
#include"auftragentry.h"
#include<Aux/Ausgabe_neu.h>

AufEintrag::AufEintrag(const SelectedAufentry &aufentry,
			const cH_ExtBezSchema &schema)
 :AufEintragBase(aufentry,schema)
{};	



const EntryValue AufEintrag::getSeqValue(int seqnr) const
{

 switch(seqnr)
  {
#warning : seqnr=bezkomptype for 1-4 (hack)
	case ART_SEQ :return (*artikel)[ART_SEQ];
          break;
	case BR_SEQ :return (*artikel)[BR_SEQ];
          break;
	case FB_SEQ :return (*artikel)[FB_SEQ];
          break;
	case AUFM_SEQ :return (*artikel)[AUFM_SEQ];
          break;
	case KW_SEQ : return lieferdatum; 
	case AUFID_SEQ : return auftragid; 
        case KNDNR_SEQ : return kdnr;  
	case PROZ_SEQ : 
           { EntryValue tmpev(prozess,prozess->Id() ? SUBID_SPAWN : 0, getProzDat());
            return tmpev;
           }
	default : return -1;
  }
 return -1;
}


Gtk::OStream &operator<<(Gtk::OStream &o,const AufEintrag &a)
{
 o 	<< a.getStueck() << "\t"
	<< a.getSeqValue(ART_SEQ).getStrVal() << "\t"
	<< a.getSeqValue(BR_SEQ).getStrVal() << "\t"
	<< a.getSeqValue(FB_SEQ).getStrVal() << "\t"   
	<< a.getSeqValue(AUFM_SEQ).getStrVal() << "\t"
	<< a.getMeter() << "\t";

 int rest=a.getRest();  	    	    
 if(rest>0) o << rest << "\t";	    	    	    	    
 else o << "" << "\t";
 o	<< Formatiere(a.GPreis().Wert(),2,0,".") << "\t";
 try {    
        Petig::Datum d(a.getSeqValue(KW_SEQ).Datum());
	o << d.KW().Woche()<<"'"<<d.Jahr();
 } catch (Petig::Datumsfehler &e)
 {  o << "Fehler"; }
 o << '\t' << a.getEntryStatusStr() << '\t';
 try { o << a.LastEditDate().c_str();
 } catch (Petig::Datumsfehler &e)
 {  o << "Fehler"; }
 o << '\n';
 return o;
}
