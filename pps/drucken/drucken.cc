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

extern bool sort_by_rownr;

LR_drucken::LR_drucken(const LR_Base::typ RL_, unsigned int _auftragsnr, bool print,
   bool b_firmenpapier,bool b_kopie, cH_ppsInstanz _instanz, bool _toTeX, 
   bool rueckst,bool ean_code)
:
auftragsnr(_auftragsnr),RL(RL_),instanz(_instanz),toTeX(_toTeX),rueckstand(rueckst)
{
 LR_drucken::drucken(print,b_firmenpapier,b_kopie,ean_code);
}

void LR_drucken::drucken(bool print,bool b_firmenpapier,bool b_kopie,bool ean_code)
{
//   if (b_kopie) kopie="Kopie ,";

   char *pr_tray1(getenv("MANUPROC_PR_TRAY1"));
   char *pr_tray2(getenv("MANUPROC_PR_TRAY2"));

   std::string texplotter;

   if(pr_tray1) texplotter=std::string(" -P")+pr_tray1; 
   else texplotter= " -Phl1260 ";
   if (b_firmenpapier)
      {if(pr_tray2) texplotter=std::string(" -P")+pr_tray2; 
	else texplotter = " -Phl1260lt ";
      }

   FILE *f;
   if(toTeX) f=popen("cat > ./rdr$$.tex","w");
   else if (!print) f=popen("tex2prn -2 -G ","w");
   else f=popen(("tex2prn -q -2 "+texplotter).c_str(),"w");

   std::ofstream os(fileno(f));


   if      (RL==LR_Base::Rechnung)      
    { 
#ifdef MABELLA_EXTENSIONS
      RechnungVoll r(auftragsnr,!sort_by_rownr);
#else
      RechnungVoll r(auftragsnr);
#endif

      LR_Abstraktion LRA(&r,b_firmenpapier);
      LRA.setEAN(ean_code);
      LRA.drucken(os,b_kopie,instanz);
    }
   else if (RL==LR_Base::Lieferschein)  
    { 
#ifdef MABELLA_EXTENSIONS    
      cH_LieferscheinVoll l(instanz,auftragsnr,true);
#else
      cH_LieferscheinVoll l(instanz,auftragsnr);
#endif
      LR_Abstraktion LRA(&*l,b_firmenpapier);
      LRA.setEAN(ean_code);
      LRA.drucken(os,b_kopie,instanz);
    }
   else if (RL==LR_Base::Auftrag || RL==LR_Base::Intern || RL==LR_Base::Extern)  
    { 
     AuftragFull a=AuftragFull(AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr),false);
     LR_Abstraktion LRA(RL,&a,b_firmenpapier);
     LRA.setEAN(ean_code);
     LRA.setRueckstand(rueckstand);
     LRA.drucken(os,b_kopie,instanz);
    }
   else abort();
}

