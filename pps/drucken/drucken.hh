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

#include <Aux/dbconnect.h>
#include <Aux/itos.h>
#include <Aux/ppsInstanz.h>
#include <string>
#include <fstream>
#include <Kunde/Kunde.h>
#include <Artikel/ArtikelBezeichnung.h>

class LR_drucken 
{
      unsigned int auftragsnr;
      const string RL;
      string kopie;
      cH_ppsInstanz instanz;
   private:
      void drucken(string view_plot,bool b_firmenpapier,bool b_kopie);
   public:
      LR_drucken(string RL, unsigned int auftragsnr,string view_plot, 
           bool b_firmenpapier,bool b_kopie,cH_ppsInstanz _instanz);
};
