/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

#include "drucken.hh"
#include <Gtk2TeX.h>
#include <cstdio>
#include "drucken_class.hh"
#include <Instanzen/ppsInstanz.h>
#include <Misc/FILEstream.h>
#include <Configuration.h>

extern bool sort_by_rownr;

LR_drucken::LR_drucken(const LR_Base::typ RL_, unsigned int _auftragsnr, 
   cH_ppsInstanz _instanz, 
   bool rueckst,bool _ean_code)
: auftragsnr(_auftragsnr),RL(RL_),instanz(_instanz),
	rueckstand(rueckst), ean_code(_ean_code)
{
 LR_drucken::drucken();
}

void LR_drucken::drucken()
{
   FILE *f=0;
   std::string fax_mail;
   std::auto_ptr<LR_Abstraktion> LRAp;
   cH_LieferscheinVoll l;
   std::auto_ptr<RechnungVoll> rng;
   std::auto_ptr<AuftragFull> auftr;
   if      (RL==LR_Base::Rechnung)      
    { 
#ifdef MABELLA_EXTENSIONS
      rng.reset(new RechnungVoll(auftragsnr,!sort_by_rownr));
#else
      rng.reset(new RechnungVoll(auftragsnr));
#endif
      LRAp.reset(new LR_Abstraktion(rng.get()));
    }
   else if (RL==LR_Base::Lieferschein || RL==LR_Base::Wareneingang)
    { 
#ifdef MABELLA_EXTENSIONS    
      if (RL==LR_Base::Lieferschein)
        l=cH_LieferscheinVoll(instanz,auftragsnr,true);
      else
#endif // keine Ahnung ob das sinnvoll ist, es war immer so ... CP
        l=cH_LieferscheinVoll(instanz,auftragsnr);
      LRAp.reset(new LR_Abstraktion(&*l,RL));
    }
   else if (RL==LR_Base::Auftrag || RL==LR_Base::Intern || RL==LR_Base::Extern)  
    { 
     auftr.reset(new AuftragFull(AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr),false));
     LRAp.reset(new LR_Abstraktion(RL,auftr.get()));
     LRAp->setRueckstand(rueckstand);
    }
   else abort();
   LRAp->setEAN(ean_code);

#ifndef MABELLA_EXTENSIONS // mabella hat _noch_ kein aktuelles tex2prn
   try
   { cH_Kunde k(LRAp->KdNr());
     std::string faxnr=k->get_first_telefon(TEL_FAX);
     std::string::size_type mussweg;
     while ((mussweg=faxnr.find(' '))!=std::string::npos)
       faxnr.erase(mussweg,1);
     while ((mussweg=faxnr.find('-'))!=std::string::npos)
       faxnr.erase(mussweg,1);
     while ((mussweg=faxnr.find('('))!=std::string::npos)
       faxnr.erase(mussweg,1);
     while ((mussweg=faxnr.find(')'))!=std::string::npos)
       faxnr.erase(mussweg,1);
     // only valid in germany ...
     if (!faxnr.empty())
     { if (faxnr[0]=='+') faxnr.replace(0,1,"00");
       fax_mail+=" --fax="+faxnr;
     }
   } catch (...) {}
#endif

   if(Configuration.toTeX)
	f=popen(("cat > ./"+Configuration.texfile).c_str(),"w");
   else if(Configuration.batch)
   	f=popen(("tex2prn -2 -i -P"+Configuration.printer).c_str(),"w");
   else
   	f=popen(("tex2prn -2 -Y"+Configuration.copies
         +(Configuration.preview_only?" -G":"")+fax_mail).c_str(),"w");

   oFILEstream os(f);
   LRAp->drucken(os,instanz);
   os.flush();
   pclose(f);
}
