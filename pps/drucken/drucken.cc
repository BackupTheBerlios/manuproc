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
   FILE *f;

   if(Configuration.toTeX)
	f=popen(("cat > ./"+Configuration.texfile).c_str(),"w");
   else
     if(Configuration.batch)
   	f=popen(("tex2prn -2 -i -P"+Configuration.printer).c_str(),"w");
   else
   	f=popen(("tex2prn -2 -Y"+Configuration.copies
         +(Configuration.preview_only?" -G":"")).c_str(),"w");

   oFILEstream os(f);

   if      (RL==LR_Base::Rechnung)      
    { 
#ifdef MABELLA_EXTENSIONS
      RechnungVoll r(auftragsnr,!sort_by_rownr);
#else
      RechnungVoll r(auftragsnr);
#endif

      LR_Abstraktion LRA(&r);
      LRA.setEAN(ean_code);
      LRA.drucken(os,instanz);
    }
   else if (RL==LR_Base::Lieferschein)  
    { 
#ifdef MABELLA_EXTENSIONS    
      cH_LieferscheinVoll l(instanz,auftragsnr,true);
#else
      cH_LieferscheinVoll l(instanz,auftragsnr);
#endif
      LR_Abstraktion LRA(&*l,RL);
      LRA.setEAN(ean_code);
      LRA.drucken(os,instanz);
    }
   else if (RL==LR_Base::Wareneingang)  
    { 
      cH_LieferscheinVoll l(instanz,auftragsnr);
      LR_Abstraktion LRA(&*l,RL);
      LRA.setEAN(ean_code);
      LRA.drucken(os,instanz);
    }    
   else if (RL==LR_Base::Auftrag || RL==LR_Base::Intern || RL==LR_Base::Extern)  
    { 
     AuftragFull a=AuftragFull(AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr),false);
     LR_Abstraktion LRA(RL,&a);
     LRA.setEAN(ean_code);
     LRA.setRueckstand(rueckstand);
     LRA.drucken(os,instanz);
    }
   else abort();
   pclose(f);
}

