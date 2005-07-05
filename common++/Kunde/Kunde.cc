// $Id: Kunde.cc,v 1.59 2005/07/05 17:03:39 jacek Exp $
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

#include "Kunde/Kunde.h"
#include <Misc/Ausgabe_neu.h>
#include <Misc/Transaction.h>
#include <Kunde/Telefon.h>
#include <DynamicEnums/DynamicEnums.h>
#include <algorithm> // find on 2.95
#include <Artikel/ArtikelStamm.h>

std::ostream &operator<<(std::ostream &o,const cH_Kunde &k)
{return o<<k->firma()<<"("<<k->Id()<<")";}


cH_Kunde::cache_t cH_Kunde::cache;
H_Kunde::cache_t H_Kunde::cache;

cH_Kunde::cH_Kunde(Kunde::ID id)
{  cH_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   { 
    *this=cH_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}

H_Kunde::H_Kunde(Kunde::ID id)
{  H_Kunde *cached(cache.lookup(id));
   if (cached) *this=*cached;
   else
   {
     *this=H_Kunde(new Kunde(id));
      cache.Register(id,*this);
   }
}



const std::string Kunde::getFullName() const
{
 std::string ret;

 ret=adresse.firma;
#ifdef MANUPROC_DYNAMICENUMS_CREATED 
 if(isInGrp(KundengruppeID::Personen))
   ret += " "+adresse.name2;
#endif
 return ret;
}


const std::string Kunde::LaTeX_von() const
{
  std::string s="\\underline{\\scriptsize ";
  s+= string2TeX(getName()+", ");

#ifndef PETIG_EXTENSIONS 
  // eigentlich ist das ein Relikt aus der Zeit, als man das Land nicht
  // drunterschreiben musste, sondern stattdessen innerhalb der EU das 
  // KFZ Kennzeichen vor die PLZ schreiben konnte
  std::string lkz=adresse.land->LKZ();
  if (!lkz.empty()) lkz+='-';
#else
  std::string lkz;
#endif

  if(!adresse.postfach.empty())
    s+= string2TeX("Postfach "+adresse.postfach+", "+
		lkz+adresse.postfachplz+" "+adresse.ort); 
  else
    s+= string2TeX(adresse.strasse +" "+adresse.hsnr+", "+
		lkz+adresse.plz+" "+adresse.ort);
//  if(kundennummer_von!=none_id) s+="\\qquad KdNr: "+Kunde(kundennummer_von).UnsereKundenNr(); 
  s+= "}\\\\[3mm]\n";
  return  s;
}

const std::string Kunde::LaTeX_von_gross(const ID kid, const std::string width) const
{
  std::string s=std::string("\\parbox[t]{")+width+"}{\\footnotesize\\raggedleft ";
  s+= string2TeX(getName())+"\\\\\n";
  s+= string2TeX(strasse()+" "+hausnr())+"\\\\\n";
  s+= string2TeX(plz()+" "+ort())+"\\\\\n";
  s+= string2TeX("Tel. "+get_first_telefon(TEL_TEL))+"\\\\\n";
  s+= string2TeX("Fax. "+get_first_telefon(TEL_FAX))+"\\\\[1ex]\n";
  s+= "\\normalsize\n";
  s+= string2TeX("Kd.-Nr. "+cH_Kunde(kid)->UnsereKundenNr())+"\\\\\n";
  s+= "}\\\\[6mm]\n";
  return  s;
}

std::string Kunde::get_first_telefon(const TelArt& art) const
{
 std::list<cH_Telefon> L=getTelefon();
 for(std::list<cH_Telefon>::const_iterator i=L.begin();i!=L.end();++i)
  {
    if((*i)->TelefonArt()==art)
      switch(art) {
         case TEL_TEL:
         case TEL_FAX: return (*i)->NummerStr(); break;
         default : return (*i)->Text();
         }
  }
 return "";
}



const std::string Kunde::LaTeX_an(bool liefer,TelArt telart,
				const std::string width,
				const std::string telwidth) const
{
  std::string strasse_postfach;
  std::string lkz_plz_ort;

#ifdef MABELLA_EXTENSIONS  // meines Wissens nicht mehr üblich, CP (s.o.)
  if (!adresse.land->LKZ().empty()) 
	lkz_plz_ort += adresse.land->LKZ()+" - ";
#endif  

  if ((!liefer && !adresse.postfach.empty() && rng_an_postfach) || adresse.strasse.empty())
     {
      strasse_postfach = "Postfach "+adresse.postfach;
      lkz_plz_ort +=  adresse.postfachplz+" "+adresse.ort;
     }
     else
     { 
      strasse_postfach = adresse.strasse+ " " +adresse.hsnr;
      lkz_plz_ort += adresse.plz+" "+adresse.ort;
     }

  std::string s=std::string("\\parbox[t]{")+width+"}{\n";
#ifdef MABELLA_EXTENSIONS
  if(Anrede()->Id()!=ManuProcEntity<>::none_id)
    s+=Anrede()->getAnrede(MultiL_Dict(Sprache()))+"~\\\\\n";
#else
  s+="\\large ";
  s+="Firma \\\\";
#endif

#ifdef MANUPROC_DYNAMICENUMS_CREATED
  if(isInGrp(KundengruppeID::Personen))
   { 
   if(!getName().empty())
     s+=string2TeX(getName(),NEEDCHAR)+" ";
    s+=string2TeX(getName2(),NEEDCHAR)+ "~\\\\";
   }
  else
   {
    s+=string2TeX(getName(),NEEDCHAR)+"~\\\\\n";
    if(!getName2().empty())
      s+=string2TeX(getName2(),NEEDCHAR)+"~\\\\\n";
   }
#endif
  
  if (!postanwvor().empty()) s+= string2TeX(postanwvor(),NEEDCHAR) +"~\\\\";
  s += string2TeX(strasse_postfach,NEEDCHAR); 
  if (!postanwnach().empty()) s+= "~\\\\" + string2TeX(postanwnach(),NEEDCHAR) ;
  s += "\\\\[1ex]" + string2TeX(lkz_plz_ort,NEEDCHAR);
#ifndef MABELLA_EXTENSIONS
  if (Auslaender()) s+="\\\\[1ex]"+string2TeX(adresse.land->Name());
#endif
  s += "}\n";
  if(telart==TEL_NONE)
    return s;


  std::string s2;
  std::list<cH_Telefon> T=getTelefon();
  s2+="\\parbox[t]{"+telwidth+"}{\\scriptsize~\\\\\\vfill\n";
  for(std::list<cH_Telefon>::const_iterator i=T.begin();i!=T.end();++i)
     {s2+= (*i)->ArtString() +" "+(*i)->NummerStr()+"\\\\\n"; }
  s2 +="}";

  return  std::string("\\parbox[t]{")+telwidth+"}{"+s+s2+"}\n";
}



const PreisListe::ID Kunde::preisliste() const
{
 if(!prlist_valid) Preislisten();
 
 return preislisten.empty() ? PreisListe::none_id : preislisten.front().second;
}


bool Kunde::isInGrp(const Kundengruppe::ID gid) const 
{
#ifdef MANUPROC_DYNAMICENUMS_CREATED
 
 if(gruppen.empty())
   load_Gruppen();

 if(gruppen.empty()) return false;

 for(std::vector<Kundengruppe::ID>::const_iterator f=gruppen.begin(); 
	f!=gruppen.end(); ++f)
    {if(*f == gid) return true;
//std::cout << "KgGrp" << *f << "gid:" << gid <<"\n";
    }

 return false;
#else
 return false;
#endif
}

void Kunde::putInGrp(const Kundengruppe::ID gid) 
{
#ifdef MANUPROC_DYNAMICENUMS_CREATED
   Query("insert into ku_gruppen_map (kundennr,grpnr)"
       " values (?,?)") << Id() << gid;

  gruppen.push_back(gid); 
#endif       
}


void Kunde::pullFromGrp(const Kundengruppe::ID gid) 
{
#ifdef MANUPROC_DYNAMICENUMS_CREATED
   Query("delete from ku_gruppen_map "
       " where (kundennr,grpnr) = (?,?)") << Id() << gid;

 load_Gruppen();
#endif       
}


void Kunde::isLieferadresse(bool is) 
{
#ifdef MANUPROC_DYNAMICENUMS_CREATED
 if(is)
   putInGrp(KundengruppeID::Lieferadresse);
 else       
   pullFromGrp(KundengruppeID::Lieferadresse);
#endif       
}

void Kunde::isRechnungsadresse(bool is) 
{
#ifdef MANUPROC_DYNAMICENUMS_CREATED
 if(is)
   putInGrp(KundengruppeID::Rechnungsadresse);
 else       
   pullFromGrp(KundengruppeID::Rechnungsadresse);
#endif       
}


void Kunde::isAuftragsadresse(bool is) 
{
#ifdef MANUPROC_DYNAMICENUMS_CREATED
 if(is)
   putInGrp(KundengruppeID::Auftragsadresse);
 else       
   pullFromGrp(KundengruppeID::Auftragsadresse);
#endif       
}




void Kunde::load_Gruppen() const throw(SQLerror)
{
 gruppen.erase(gruppen.begin(),gruppen.end());

 Query q("select grpnr from ku_gruppen_map"
 	" where kundennr=?");
 
 q << Id();

 int ret;

 FetchIStream fi=q.Fetch();

 while(fi.good())
   {fi >> ret;
    gruppen.push_back((Kundengruppe::ID)ret);
    fi=q.Fetch();
   }

// for(std::vector<Kundengruppe::ID>::const_iterator f=gruppen.begin(); 
//	f!=gruppen.end(); ++f)
//    std::cout << "geladen:" << (*f) << "\n";

}



void Kunde::AB_an_rngadresse(bool b) throw(SQLerror)
{
 Transaction tr;
 
 Query q("update kunden set ab_an_rngadresse=? where kundennr=?");
 
 q << b << Id();
 
 tr.commit();
 ab_an_rngadresse=b;
 
}



// weak test for UST-ID integrity
bool Kunde::idnr_valid() const
{
 if(IDnr.size()<8) return false; // wahrscheinlich zu kurz
 if(atoi(IDnr.substr(0,2).c_str())>0) return false; // keine LKZ am Anfang
 return true;   // wahrscheinlich OK 
}



bool Kunde::MwSt(const ManuProcEntity<>::ID rngid) const throw(SQLerror)
{
 bool ret=true;
 
 if(rngid!=ManuProcEntity<>::none_id)
   {Query("select case when coalesce(steuersatz,16)>0"
       "  then true else false end from rechnung"
       "  where rngid=?") << rngid >> ret; 
     return ret;
    }

 if(mwst==false) return false; // überhaupt keine MwSt

 // oder
 // MwSt erst ermitteln
 if(!Auslaender()) return true;
 if(EU())
   if(!idnr_valid()) return true;
   
 return false;
}

void Kunde::set_idnr(const std::string& s)
{
 std::string t(s);

 while(t.find(" ")!=std::string::npos)
  t.replace(t.find(" "),1,"");

 IDnr = t;

}


fixedpoint<2> Kunde::getProvSatz_Artikel(const ArtikelBase art,
  const fixedpoint<2> rabatt) const throw(SQLerror)
{
 cH_Kunde verk(VerkNr());
 ArtikelStamm as(art);
// int t; // ArtikelTyp;
 // Jacek: Warum hast du nicht den Cache von ArtikelStamm genommen?
// Query("select warengruppe from artikelstamm where id=?")   
//               << art.Id() >> t;
 ArtikelTyp at(as.Warengruppe());

 std::string tabelle="artbez_"+
        itos(at)+"_"+
        itos(ExtBezSchema::default_id);

 fixedpoint<2> to_set=0;
#if defined(MABELLA_EXTENSIONS) && defined(MANUPROC_DYNAMICENUMS_CREATED)
 int signatur=1;
 int bezkomptype=1;
 fixedpoint<2> rab;
       
    if(VerkNr()!=Kunde::none_id &&
       ArtikelTyp::hasAttribute(at,ArtikelTypAttr::provision))
      {
       fixedpoint<2> ps1=0.0,ps2=0.0;

       Query q("select provsatz1, provsatz2, "
          " rabatt "
          " from prov_verkaeufer where"
	  " verknr=? and kundennr=? and rabatt>=? order by rabatt asc limit 1");
       q << verk->Rngan() << Id() << rabatt; // provsatz vom mainvertreter
       SQLerror::test(__FILELINE__,100);

       if(q.Result()!=100)
         q  >> ps1 >> ps2 >> rab;

       if(ps1==ps2) to_set=ps1;
       else
	{
        cH_ExtBezSchema ebz(ExtBezSchema::default_id,at);

 	const ExtBezSchema::BezKomp bk=(*ebz)
			[std::pair<int,int>(bezkomptype,signatur)];
	std::string first_komp_col=bk.spaltenname;

	int psnr=0;
	Query q("select provsatznr from prov_config p join "+tabelle+
		" b on (b."+first_komp_col+"=p.artikel and b.id=?) "
		"where kundennr=?");
	q << art.Id() << Id();
	SQLerror::test(__FILELINE__,100);	
	
	if(q.Result()!=100)
	  {q >> psnr; 
	   if(psnr==0) to_set=0.0;		// no Provision at all
	   else if(psnr==1) to_set=ps2;		// excepted Provision
	   else to_set=0.0;		// what to do else ?
	  } // not found means default provision
	else to_set=ps1;	// default ProvSatz

 	}
       	
      }

#endif      

 return to_set;

}


std::string Kunde::Kontakt(const TelArt& art, Kunde::ID firmaid,
		bool forcereload) const
{
 if(kontakt.valid() && !forcereload)
   return kontakt.get_first_kontakt(art,firmaid,Id());
 kontakt.reload(firmaid,Id());
 return kontakt.get_first_kontakt(art,firmaid,Id());   
}

#if !defined(__GNUC__) || __GNUC__ > 2
const Kunde::ID Kunde::eigene_id;
#endif


