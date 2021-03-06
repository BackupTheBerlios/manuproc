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


#include <Instanzen/ppsInstanz.h>
#include <string>
#include "lr_base.h"


class LR_drucken 
{
      unsigned int auftragsnr;
      const LR_Base::typ RL;
      std::string kopie;
      cH_ppsInstanz instanz;
      bool rueckstand;
      bool ean_code;
   private:
      void drucken();
      std::string get_Zahlungsziel(int);
   public:
      LR_drucken(const LR_Base::typ RL, unsigned int _auftragsnr,
		cH_ppsInstanz _instanz,
		bool rueckst=false,bool ean_code=false);
};

