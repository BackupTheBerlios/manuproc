// $Id: delayed_reclaim.cc,v 1.7 2004/02/16 15:29:05 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2003 Adolf Petig GmbH & Co. KG
 *  written by Jacek Jakubowski & Christof Petig
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

#include <Auftrag/AufEintrag_delayedreclaim.h>
#include <Misc/TraceNV.h>
#include <Misc/Transaction.h>
#include <Misc/FetchIStream.h>
#include <Lager/Lager.h>
#include <Auftrag/VerfuegbareMenge.h>
#include <Auftrag/selFullAufEntry.h>
//#include <Auftrag/AufEintragZu.h>

bool AufEintrag::delayed_reclaim::active;
std::list<std::pair<cH_ppsInstanz,ArtikelBase> > 
	AufEintrag::delayed_reclaim::delayed;

// ähnlich move_to_Move, allerdings wird 2er
class dr_Planen_dp_cb : public distribute_parents_cb
{       AufEintrag &quelle,&ziel,&A2er;
public:
        dr_Planen_dp_cb(AufEintrag &q,AufEintrag &z,AufEintrag &a2) 
		: quelle(q), ziel(z), A2er(a2) {}
        virtual AuftragBase::mengen_t operator()(const AufEintragBase &, 
                                AuftragBase::mengen_t) const;
};

AuftragBase::mengen_t dr_Planen_dp_cb::operator()(const AufEintragBase &ae,
                                AuftragBase::mengen_t M) const
{  ManuProC::Trace _t(AuftragBase::trace_channel, std::string("dr_Planen_dp_cb::")+__FUNCTION__,ae,M);
   quelle.MengeAendern(-M,true,ae);
   AufEintragZu(ae).Neu(ziel,M);
   if (A2er.valid()) A2er.MengeAendern(-M,false,ziel);
   return M;
}

struct dr_Planen_cb : distribute_children_cb
{  AufEintrag &A0er;
   AufEintrag &A2er;
   mutable AuftragBase::mengen_t rest;

   dr_Planen_cb(AufEintrag &a,AufEintrag &a2) : A0er(a),A2er(a2) {}
   AuftragBase::mengen_t getRest() const { return rest; }
   AuftragBase::mengen_t operator()(const ArtikelBase &,
                           const AufEintragBase &,AuftragBase::mengen_t) const;
   void operator()(const ArtikelBase &,AuftragBase::mengen_t m) const
   {  rest=m; }
};

// wir können move_to nicht verwenden, da dies delayed_reclaim nutzt
AuftragBase::mengen_t dr_Planen_cb::operator()(const ArtikelBase &art,
	const AufEintragBase &ziel,AuftragBase::mengen_t m) const
{  AufEintrag z(ziel);
   return distribute_parents(A0er,m,dr_Planen_dp_cb(A0er,z,A2er));
}

struct FreieMengeNutzen_cb : auf_positionen_verteilen_cb
{	AufEintrag &A2er;

	FreieMengeNutzen_cb(AufEintrag &a) : A2er(a) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t M) const
	{  ManuProC::Trace _t(AuftragBase::trace_channel, 
			std::string("FreieMengeNutzen_cb::")+__FUNCTION__,
			ae,M,NV("2er",A2er),
			A2er.getLieferdatum());
	   if (ae.getLieferdatum()>=A2er.getLieferdatum())
	   {  dr_Planen_cb cb(ae,A2er); 
	      distribute_children(A2er,M,A2er.Artikel(),cb);
	      return cb.getRest();
	   }
	   return 0;
	}
};

struct FreieMengeNutzen2_cb : auf_positionen_verteilen_cb
{	AufEintrag &A3er;

	FreieMengeNutzen2_cb(AufEintrag &a) : A3er(a) {}
	AuftragBase::mengen_t operator()(AufEintrag &ae, AuftragBase::mengen_t M) const
	{  if (ae.getLieferdatum()>=A3er.getLieferdatum())
	   {  AufEintrag A2er;
	      return distribute_parents(ae,M,dr_Planen_dp_cb(ae,A3er,A2er));
	   }
	   return 0;
	}
};

AufEintrag::mengen_t AufEintrag::delayed_reclaim::MengeNutzen(AufEintrag &ae, mengen_t m)
{   ManuProC::Trace _t(trace_channel, __FUNCTION__,ae,m);
    SQLFullAuftragSelector waiting_orders(make_value(SQLFullAuftragSelector::
      	 	sel_Artikel_Planung_id(ae.Instanz()->Id(),Kunde::eigene_id,
      	 		ae.Artikel(),AuftragBase::ungeplante_id)));
    return auf_positionen_verteilen(waiting_orders,m,FreieMengeNutzen2_cb(ae));
}

void AufEintrag::delayed_reclaim::reclaim()
{  ManuProC::Trace _t(trace_channel, __FUNCTION__);
   
   Transaction tr;
   Query::Execute("lock table auftragentry in exclusive mode");
   while (!delayed.empty())
   {  std::pair<cH_ppsInstanz,ArtikelBase> act=delayed.front();
      AuftragBase ab(act.first,dispo_auftrag_id);
      if (act.first->LagerInstanz()) // wegen Lagerdatum sonst Liste
      {  AuftragBase::mengen_t m;
         AufEintragBase neuerAEB(ab,
              ab.existEntry(act.second,LagerBase::Lagerdatum(),OPEN,m));
         // verteilen
         if (neuerAEB.valid())
         {  MengeVormerken(act.first,act.second,m,true,ProductionContext());
            AufEintrag(neuerAEB).MengeAendern(-m,false,AufEintragBase());
         }
      }
      else
      {  // alle 2er suchen
         VerfuegbareMenge vfm(act.first,act.second,ManuProC::Datum::Infinity());
         for (VerfuegbareMenge::iterator i=vfm.getDispoAuftraege().begin();
         		i!=vfm.getDispoAuftraege().end();++i)
         {  if (!i->getRestStk()) continue;
            SQLFullAuftragSelector waiting_orders(make_value(SQLFullAuftragSelector::
      	 	sel_Artikel_Planung_id(act.first->Id(),Kunde::eigene_id,act.second,
	      	 	AuftragBase::ungeplante_id)));
            auf_positionen_verteilen(waiting_orders,
            		i->getRestStk(),FreieMengeNutzen_cb(*i));
         }
      }
      delayed.pop_front();
   }
   tr.commit();
}

AufEintrag::delayed_reclaim::delayed_reclaim()
  : nested(active)
{  if (!nested) active=true;
}

AufEintrag::delayed_reclaim::~delayed_reclaim()
{  if (!nested) 
   { reclaim(); active=false; }
}

void AufEintrag::delayed_reclaim::delay(cH_ppsInstanz inst,const ArtikelBase &ab)
{  delayed.push_back(std::pair<cH_ppsInstanz,ArtikelBase>(inst,ab));
}
