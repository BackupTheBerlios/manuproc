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

LR_drucken::LR_drucken(string RL_, unsigned int auftragsnr_, string view_plot,
   bool b_firmenpapier,bool b_kopie, cH_ppsInstanz _instanz)
: auftragsnr(auftragsnr_),RL(RL_),instanz(_instanz)
{
 LR_drucken::drucken(view_plot,b_firmenpapier,b_kopie);
}

void LR_drucken::drucken(string view_plot,bool b_firmenpapier,bool b_kopie)
{
   string kopie="";
   if (b_kopie) kopie="Kopie ,";
   string texplotter= " -Phl1260 ";
   if (b_firmenpapier) texplotter = " -Phl1260lt ";
   FILE *f;
   if (view_plot=="Preview") f=popen("tex2prn -2 -G ","w");
   if (view_plot=="Plot") f=popen(("tex2prn -q -2 "+texplotter).c_str(),"w");

   ofstream os(fileno(f));

   if      (RL=="Rechnung")      
    { RechnungVoll r(auftragsnr);
      LR_Abstraktion(&r).drucken_table(os,kopie,instanz);
    }
   else if (RL=="Lieferschein")  
    { cH_LieferscheinVoll l(auftragsnr);
      LR_Abstraktion(&*l).drucken_table(os,kopie,instanz);
    }
   else if (RL=="Auftrag")  
    { AuftragFull a=AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr);
      LR_Abstraktion(&a).drucken_table(os,kopie,instanz);
    }
   else if (RL=="Intern")  
    { AuftragFull a=AuftragBase(cH_ppsInstanz(instanz),(int)auftragsnr);
      LR_Abstraktion(&a).drucken_table(os,kopie,instanz);
    }
   else abort();
}

