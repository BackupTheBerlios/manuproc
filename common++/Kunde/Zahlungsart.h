// $Id: Zahlungsart.h,v 1.22 2003/05/05 14:25:13 jacek Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Christof Petig
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


#ifndef KUNDE_ZAHLUNGSART_HH
#define KUNDE_ZAHLUNGSART_HH
#include <string>
#include <ExtBezSchema/ExtBezSchemaHandle.h>
#include <Misc/fixedpoint.h>
#include <BaseObjects/ManuProcHandle.h>
#include <Misc/SQLerror.h>
#include <Misc/Waehrung.h>
#include <Misc/fixedpoint.h>
#include <Misc/Datum.h>
#include <Misc/multi_lang.h>
#include <vector>
#include <Misc/compiler_ports.h>

class cH_Kunde;
class FetchIStream;

class Zahlungsart : public ManuProcHandle<>
{
public:
    struct st_skonto{int skontofrist; fixedpoint<2> skontosatz;
           st_skonto(int i, fixedpoint<2> f) 
            :skontofrist(i),skontosatz(f) {} };

    typedef enum {NONE=0, DTAUS=1, RIBA=2, LCR=3} Verfahren;

    static std::string VerfahrenStr(Verfahren v);             
    static const ID default_Zahlart=1;

private:
    bool bankeinzug;
    fixedpoint<2> einzugrabatt;
    std::vector<st_skonto> vec_skonto;
    std::string kurzbezeichung,bezeichnung;
    int zahlungsfrist;
    bool monatsende;
    int frist_monate;
    Verfahren verfahren;

    void setVerfahren(const std::string v);
    friend FetchIStream &NOTGCC295(::)operator>>(FetchIStream &is, Zahlungsart &za);
    
public:
    Zahlungsart(ID _id) throw (SQLerror) ;
    
    ID Id() const {  return entityid; }
    const std::vector<st_skonto> getSkonto() const { return vec_skonto; }
    const st_skonto getSkonto(unsigned int i) const;
    const bool getBankeinzug() const { return bankeinzug; }
    const bool getMonatsende() const { return monatsende; }
    const fixedpoint<2> getEinzugrabatt() const {return einzugrabatt;} 
    const std::string Kurzbezeichung() const {return kurzbezeichung;}
    const std::string Bezeichnung() const {return bezeichnung;}
    const std::string getKurzbezeichung() const {return kurzbezeichung;}
    const std::string getBezeichnung() const {return bezeichnung;}
    const int getZahlungsfrist() const {return zahlungsfrist;}
    const std::string Name() const {return kurzbezeichung;}
    const ManuProC::Datum getZahlungstermin(const ManuProC::Datum rgdatum) const;
    Verfahren getVerfahren() const { return verfahren; }
    const int FristMonate() const { return frist_monate; }
    
    void TeX_out(std::ostream &os,
		cP_Waehrung w, 
		const fixedpoint<2> skontobetrag, 
		const fixedpoint<2> einzugrabattbetrag, 
		const ManuProC::Datum &zahlziel,
		const ManuProC::Datum &rgdatum,		
		const cH_Kunde k,
		MultiL_Dict &mld) const;
		
    void TeX_out(std::ostream &os,
		const ManuProC::Datum &zahlziel,
		const cH_Kunde k,
		const fixedpoint<2> skontobetrag,
		MultiL_Dict &mld) const;

    bool operator==(const Zahlungsart& b) const
              {return Id()==b.Id();} 
    bool operator<(const Zahlungsart& b) const
                {return Id()<b.Id();} 
};

class cH_Zahlungsart : public Handle<const Zahlungsart>
{	// cache
        typedef CacheStatic<Zahlungsart::ID,cH_Zahlungsart> cache_t;
        static cache_t cache;
        cH_Zahlungsart(const Zahlungsart *p) : Handle<const Zahlungsart>(p) {}	
#if __GNUC__ > 2
        friend class cache_t::stl_type;
#else
        friend cache_t::stl_type;
#endif
        cH_Zahlungsart() {}
public:
	typedef Zahlungsart::ID ID;
	cH_Zahlungsart(ID nr);
};

#endif
