/* $Id: ProvAbrechnung.cc,v 1.9 2005/05/03 15:52:51 jacek Exp $ */
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include <Vertrieb/ProvAbrechnung.h>
#include <Misc/FetchIStream.h>

const ProvAbrechnung::ID ProvAbrechnung::getNextAbrNr(
	const Kunde::ID verkid, const ManuProC::Datum vom) throw(SQLerror)
{
 ProvAbrechnung::ID abrid;
 int jahr=vom.Jahr();

 cH_Kunde verk(verkid);
 if(verkid!=verk->Rngan())
   {Query("select max(provnr) from prov_abrechnung"
 	" where verknr=?")
	<< itos(verk->Rngan()) >> abrid; 
   }	
 else
    {   
    Query("select coalesce(max(provnr),?*100+0)+1 from prov_abrechnung"
 	" where verknr=? and date_part('y',vom)=?")
	<< jahr << itos(verkid) << jahr >> abrid;    
    }

 return abrid;
}

const ManuProC::Datum ProvAbrechnung::getNextVomDate(const Kunde::ID verkid) throw(SQLerror)
{
 ManuProC::Datum newvom;
 
 Query("select coalesce(max(bis)+1,date('1970-01-01')) from prov_abrechnung"
       " where verknr=?")
	<< itos(verkid) >> newvom; 


 return newvom;
}


Preis::geldbetrag_out ProvAbrechnung::getAuszahlung() const
{
 fixedpoint<2> summe=0;
 fixedpoint<2> rng_summe=0;

 RechnungBase::ID tmprid=RechnungBase::none_id;
 float rabatt;

 for(std::vector<ProvAbrechnungEntry*>::const_iterator i=entries.begin();
	i!=entries.end(); ++i)
    {
     if(tmprid != (*i)->RngEntry().Id())
      {
       if(tmprid != RechnungBase::none_id)
         {
	  float rabfkt = (100 - rabatt)/100.0;
          rng_summe = rng_summe * rabfkt;
	  summe+=rng_summe;
	 }
       rng_summe=0;
       tmprid=(*i)->RngEntry().Id();
       rabatt=(*i)->getRngRabattsatz();
      }

     rng_summe+=(*i)->Provision();   
    }

 if(tmprid!=RechnungBase::none_id)
   {
    float rabfkt = (100 - rabatt)/100.0;
    rng_summe = rng_summe * rabfkt;
    summe+=rng_summe;
   }


 cH_Kunde vk(verkaeufer);
 if(vk->MwSt())
   summe = summe * (fixedpoint<2>(1) + ((float)MWST/100));

 return summe; 

}


