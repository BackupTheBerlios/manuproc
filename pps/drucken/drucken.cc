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
#include <Aux/ppsInstanz.h>

LR_drucken::LR_drucken(const LR_Base::typ RL_, unsigned int _auftragsnr, bool print,
   bool b_firmenpapier,bool b_kopie, cH_ppsInstanz _instanz, bool _toTeX=false)
: auftragsnr(_auftragsnr),RL(RL_),instanz(_instanz),toTeX(_toTeX)
{
 LR_drucken::drucken(print,b_firmenpapier,b_kopie);
}

void LR_drucken::drucken(bool print,bool b_firmenpapier,bool b_kopie)
{
//   if (b_kopie) kopie="Kopie ,";
   string texplotter= " -Phl1260 ";
   if (b_firmenpapier) texplotter = " -Phl1260lt ";

   FILE *f;
   if(toTeX) f=popen("cat > ./rdr$$.tex","w");
   else if (!print) f=popen("tex2prn -2 -G ","w");
   else f=popen(("tex2prn -q -2 "+texplotter).c_str(),"w");

   ofstream os(fileno(f));

   if      (RL==LR_Base::Rechnung)      
    { RechnungVoll r(auftragsnr);
      LR_Abstraktion(&r).drucken(os,b_kopie,instanz);
    }
   else if (RL==LR_Base::Lieferschein)  
    { cH_LieferscheinVoll l(auftragsnr);
      LR_Abstraktion(&*l).drucken(os,b_kopie,instanz);
    }
   else if (RL==LR_Base::Auftrag)  
    { AuftragFull a=AuftragFull(AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr));
      LR_Abstraktion(&a).drucken(os,b_kopie,instanz);
    }
   else if (RL==LR_Base::Intern)  
    { AuftragFull a=AuftragFull(AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr));
      LR_Abstraktion(&a).drucken(os,b_kopie,instanz);
    }
   else abort();
}

