// $Id: AufEintragZuMengenAenderung.h,v 1.8 2003/09/02 12:10:52 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Malte Thoma
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

//#include <Auftrag/AufEintragZu.h>  
//#include <Misc/Transaction.h>
#include <Auftrag/AufEintragBase.h>
#include <Misc/compiler_ports.h>

#warning deprecated header included

namespace AufEintragZuMengenAenderung {

  // Beim 'vormerken' eines Artikels 
  __deprecated void change_parent(const AufEintragBase &old_parent,
                              const AufEintragBase &new_parent,
                              const AuftragBase::mengen_t &menge) throw(SQLerror);

  // Beim 'schnappen' eines anderen geplanten Auftrags werden die 
  // Eltern-'0er' des 'child_aeb=1er' erhöht, die
  // Zuordnung wird reduzieren
  __deprecated void increase_parents__reduce_assingments(
                     const AufEintragBase &child_aeb,
                     AuftragBase::mengen_t menge) throw(SQLerror);

  __deprecated void Change_Zuordnung_to_Children(const bool child,const AufEintrag &AE,
                        const AuftragBase::mengen_t &menge) throw(SQLerror);

  // beim 'planen' eines AufEintrags
  __deprecated void move_zuordnung_zu_geplantem(
                                   AufEintrag &AE0er, 
                                   AufEintrag &AE1er,
                                   AuftragBase::mengen_t menge,
                                   ManuProC::Auftrag::Action reason) throw(SQLerror);

   // ElternAEB erhält Menge vom 2er
   __deprecated void freie_dispomenge_verwenden(
         const AufEintrag &AE2er,AuftragBase::mengen_t menge,const AufEintragBase &ElternAEB) throw(SQLerror);
}

