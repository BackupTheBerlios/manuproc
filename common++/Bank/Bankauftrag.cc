/* $Id: Bankauftrag.cc,v 1.4 2002/01/22 09:15:55 christof Exp $ */
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

#include "Bankauftrag.h"
#include "Aux/Datum.h"
#include <fcntl.h>
#include <unistd.h>
#include <Aux/string0.h>

void Bankauftrag::string2Bank(char *buf,const char *s) throw (Datenfehler)
{  int i;
   for (i=0;s[i] && i<27;i++) switch (s[i])
   {  case '0' ... '9':
      case ' ': case '.': case ',': case '&':
      case '-': case '+': case '*': case '%':
      case '/': case '$':
      case 'A' ... 'Z': buf[i]=s[i];
         break; 
      case 'a' ... 'z': buf[i]=toupper(s[i]);
         break;
      case 'ß': buf[i]=0xe1; break;
      case 'Ä': case 'ä': buf[i]=0x8e; break;
      case 'Ö': case 'ö': buf[i]=0x99; break;
      case 'Ü': case 'ü': buf[i]=0x9a; break;
      default: buf[i]='?';
         break;
   } 
   for (;i<27;i++) buf[i]=' ';
}

Bankauftrag::Bankauftrag(char _kennz, long myblz, std::string myname,long long mykonto,
			const std::string TeX_cmd) throw(IOerror,Datenfehler)
	: druckerpipe(0), dtausfile(-1), Kontosumme(0), BLZsumme(0),
	  anzahlDatensaetze(0), Betragssumme(0), Kennziffer(_kennz),
	  eigeneBLZ(0), eigenesKonto(0), eigenerName(myname), Auftragsart(0), 
	  eD_Policy(P_FORMAT|P_CHECK|P_ERASE), logfile(0)
{  
   eigeneBLZ = myblz;
   eigenesKonto = mykonto;
   // Dateien öffnen,
#warning a PID signed name in /tmp would be nicer !!!
   if ((dtausfile=creat("dtaus1.txt",0600))==-1) throw IOerror("dtaus1.txt");
   // Satz A ausgeben
	/* Datensatz A */
   Kennziffer=time(NULL); 
   struct tm *tm=localtime(&Kennziffer); 
   char buf[128];
   if (_kennz!='G' && _kennz!='L') throw Datenfehler("A2 muß 'G' oder 'L' sein");
   snprintf0(buf,sizeof buf,"0128A%cK",_kennz);
   if (myblz<10000000 || myblz>89999999) throw Datenfehler("ungültige BLZ");
   snprintf0(buf+7,sizeof(buf)-7,"%08ld",myblz); /* A4 */
   memcpy(buf+15,"00000000",8);  /* A5 */
   string2Bank(buf+23,myname); /* A6 */
   snprintf0(buf+50,sizeof(buf)-50,"%02d%02d%02d",tm->tm_mday,tm->tm_mon+1,tm->tm_year%100); /* A7 */
   memcpy(buf+56,"    ",4);  /* A8 */
   snprintf0(buf+60,sizeof(buf)-60,"%010llu",mykonto); /* A9 */
   snprintf0(buf+70,sizeof(buf)-70,"%010lu",Kennziffer);  /* A10 */
   memset(buf+80,' ',48); /* A11 */
   memset(buf+127,'1',1); /* A12 EURO */
   if (write(dtausfile,buf,128)!=128)
   {  ::close(dtausfile); dtausfile=-1;
      throw IOerror("dtausfile,write,A");
   }

   // TeX für Ausgabe öffnen
   if (!(druckerpipe=popen(TeX_cmd.c_str(),"w"))) 
   {  ::close(dtausfile); dtausfile=-1;
      throw IOerror(TeX_cmd.c_str());
   }
   
//   eigenerName=a1.c_str();
   fprintf(druckerpipe,
   "\\documentclass[12pt]{article}\n"
   "\\usepackage{isolatin1,t1enc,german,vmargin,longtable,eurosym}\n"
   "\\setmarginsrb{0.5in}{0.5in}{0.5in}{0.5in}{2\\baselineskip}{\\baselineskip}{0pt}{0pt}\n"
#if 0   
   "\\makeatletter\n"
   "\\newcommand{\\ps@Ueberws}{%%\n"
   "	\\renewcommand{\\@oddhead}{\\begin{minipage}{\\textwidth}%s \\hfill Seite %%\n"
   	,a1.c_str());
   a1.Formatiere(Kennziffer);
   fprintf(druckerpipe,
   "		\\thepage/\\ref{lastpage}\\hfill %s\\hfill %s\\\\\n"
   	,a1.c_str(),Datum(Kennziffer).c_str());
   a1.Formatiere(eigenesKonto);
   fprintf(druckerpipe,"		Überweisungen von Konto %s",a1.c_str());
   try{ BLZ2Bankname(a1,(long)eigeneBLZ); }
   catch(SQLerror &e) { std::cerr<<e; }
   fprintf(druckerpipe," bei %s",a1.c_str());
   BLZFormat(a1,eigeneBLZ);
   fprintf(druckerpipe," (BLZ %s)\\end{minipage}}%%\n",a1.c_str());
   fputs("	\\renewcommand{\\@evenhead}{\\@oddhead}%\n"
   "	\\renewcommand{\\@oddfoot}{}%\n"
   "	\\renewcommand{\\@evenfoot}{}}\n"
   "\\makeatother\n"
   "\\pagestyle{Ueberws}\n"
   "\\newlength{\\Spalteeins}\\newlength{\\Spaltezwei}\\newlength{\\Spaltedrei}\\newlength{\\Spaltevier}\n"
   "\\setlength{\\Spalteeins}{1cm}\n"
   "\\setlength{\\Spaltezwei}{6cm}\n"
   "\\setlength{\\Spaltedrei}{\\Spaltezwei}\n"
   "\\setlength{\\Spaltevier}{4cm}\n"
#endif   
   "\\parindent0pt\n"
   "\\parskip4pt\n"
   "\\begin{document}\n"
#if 0
   "\\begin{longtable}{cllr}\n"
   "\\begin{minipage}{\\Spalteeins}\\centering lfd Nr.\\\\BchgNr.\\\\[-\\baselineskip]\\end{minipage}\n"
   "& \\begin{minipage}{\\Spaltezwei}Empfänger\\\\Debitorenkonto\\end{minipage}\n"
   "& Verwendungszweck\n"
   "& \\begin{minipage}{\\Spaltevier}Betrag\\\\Skonto\\end{minipage}\n"
   "\\endhead\n",druckerpipe
#endif
	);
}


void Bankauftrag::printEinzugListe() throw(IOerror)
{
 FILE *readpipe;
 char buf[200]; 
 if(!(readpipe=popen("mtype a: | dtaus1check -v","r")))
   throw IOerror("printEinzuListe: popen");
 while(fgets(buf,200,readpipe))
   fputs(buf,stdout); 
 pclose(readpipe);
}


Bankauftrag &Bankauftrag::operator<<(const Zahlvorgang &z) throw(IOerror)
  // Satz C ausgeben
{  char buf[128];

   z.fillBuffer(buf,sizeof buf,0,eigeneBLZ,eigenesKonto,eigenerName,Auftragsart);
   if (write(dtausfile,buf,128)!=128) throw IOerror();
   z.fillBuffer(buf,sizeof buf,1,eigeneBLZ,eigenesKonto,eigenerName,Auftragsart);
   if (write(dtausfile,buf,128)!=128) throw IOerror();
   if (z.Bloecke()>2)
   {  z.fillBuffer(buf,sizeof buf,2,eigeneBLZ,eigenesKonto,eigenerName,Auftragsart);
      if (write(dtausfile,buf,128)!=128) throw IOerror();
   }
   BLZsumme+=z.BLZ; 
   Betragssumme+=z.Betrag; 
   Kontosumme+=z.Konto;
   anzahlDatensaetze++;
   
//   z.toTeX(druckerpipe);
   return *this;
}

void Bankauftrag::close() throw(IOerror)
{  /* Druckausgabe abschließen */
   if (druckerpipe)
   {  
#if 0       
      a1.Formatiere(Betragssumme,2);
      fprintf(druckerpipe,"\\cline{4-4}\n"
   "&&&Summe DM\\hfill %s\n"
   "\\\\\n"
   "\\end{longtable}\n"
   "\\label{lastpage}\n"
   "\\newpage\n",a1.c_str());
#endif

   /* Begleitzettel drucken */
   fprintf(druckerpipe,   "\\pagestyle{empty}\n"
	   "Disketten-Begleitzettel vom %s, Belegloser Datenträgeraustausch"
	   "\\hfill ReferenzNr. %s\\\\\n"
	   "\n",Petig::Datum(Kennziffer).c_str(),Formatiere(Kennziffer).c_str());
   
   fprintf(druckerpipe,"\\begin{tabular}{lr}\n");
   fprintf(druckerpipe,"\\multicolumn{2}{l}{Datei \\texttt{DTAUS1.TXT} für %s}\\\\\n",
   	Auftragsart=='G'?"Gutschriften (Überweisung)":"Lastschriften (Bankeinzug)");
   fprintf(druckerpipe,"\\multicolumn{2}{l}{Auftrag an %s}\\\\\n",BLZ2Bankname(eigeneBLZ).c_str());
   fprintf(druckerpipe,"Auftraggeber:&%s\\\\\n",string2TeX(eigenerName).c_str());
   fprintf(druckerpipe,"Konto Auftraggeber:&%s\\\\\n",Formatiere((unsigned long long)eigenesKonto).c_str());
   fprintf(druckerpipe,"BLZ Auftraggeber:&%s\\\\\n",BLZFormat(eigeneBLZ).c_str());
   
   fprintf(druckerpipe,   "\\hline\\\\[2cm]\n"
   "Disketten - Nr.&\\rule{2cm}{0.5pt}\\\\[2ex]\n"
   "Summe  Datensätze C (\\euro{}):&%s\\\\\n",Formatiere((unsigned long long)Betragssumme,2).c_str());
   fprintf(druckerpipe,"Anzahl Datensätze C:&%d\\\\\n",anzahlDatensaetze);
   fprintf(druckerpipe,"Summe der Kontonummern:&%s\\\\\n",Formatiere((unsigned long long)Kontosumme).c_str());
   fprintf(druckerpipe,"Summe der Bankleitzahlen:&%s\\\\[2cm]\n",Formatiere(BLZsumme).c_str());
   
   fprintf(druckerpipe,"Wuppertal, %s\\\\[2cm]\n",Petig::Datum(Kennziffer).c_str());
   fprintf(druckerpipe,"\\multicolumn{2}{l}{\\rule{10cm}{0.5pt}}\\\\\n"
   	"Stempel, Unterschrift\\\\\n");
   fputs("\\end{tabular}\n\\end{document}\n",druckerpipe);
   fclose(druckerpipe);
   druckerpipe=0;
   }             

   /* Satz E ausgeben */
   char buf[128];
/* Datensatz E */
   snprintf0(buf,sizeof buf,"0128E     %07d",anzahlDatensaetze);

   memset(buf+17,'0',13); /* nur EURO geht jetzt */
/*   snprintf0(buf+17,sizeof(buf)-17,"%013llu",Betragssumme);*/
   snprintf0(buf+30,sizeof(buf)-30,"%017llu",Kontosumme);
   snprintf0(buf+47,sizeof(buf)-47,"%017lu",BLZsumme);
   snprintf0(buf+64,sizeof(buf)-64,"%013llu",Betragssumme);
/*   memset(buf+64,'0',13); */
   memset(buf+77,' ',51);
   if (write(dtausfile,buf,128)!=128) throw IOerror();
   if (::close(dtausfile)) throw IOerror();

   /* Diskette erstellen */
   if(erzeugeDiskette()) throw IOerror();
   printEinzugListe();
   /* Diskette testen */
}

// 0== Ok
int Bankauftrag::erzeugeDiskette() throw(IOerror)
{  int oldfd0,oldfd1=-1,oldfd2=-1,devnull=-1;
   int r;

   oldfd0=dup(0);
   oldfd1=dup(1);
   oldfd2=dup(2);
   devnull=open("/dev/null",O_RDWR);
   if (devnull==-1) throw IOerror();
   dup2(devnull,0);
   if (logfile)
   {  dup2(fileno(logfile),1); 
      dup2(1,2);
   }
   else
   {  dup2(devnull,1);
      dup2(1,2);
   }
   if (eD_Policy&P_FORMAT) // 2do: check, erase, merge with Sfdformat
   { r=system("/usr/bin/fdformat /dev/fd0u1440");
   }
   if (eD_Policy&(P_ERASE|P_FORMAT))
   {  if (!access("/sbin/mkdosfs",X_OK))
        r+=system("/sbin/mkdosfs -v -c /dev/fd0u1440");
      else
        r+=system("/usr/bin/mformat a:");
   }
   if (eD_Policy&P_CHECK)
   {  r+=system("/usr/bin/mbadblocks a:");
   }
   system("/usr/bin/mcopy -o dtaus1.txt a:");
   
   r=system("/bin/sh -c '/usr/bin/mtype a:dtaus1.txt | dtaus1check'");
   
   if (logfile) fprintf(logfile,r?"\nDatei fehlerhaft.\n":"\nDiskette Ok.\n");
   dup2(oldfd0,0);
   dup2(oldfd1,1);
   dup2(oldfd2,2);
   ::close(devnull);
   fprintf(stderr,r?"\nDatei fehlerhaft.\n":"\nDiskette Ok.\n");
   return r;
}

const std::string Bankauftrag::BLZFormat(long BLZ) throw(Datenfehler)
{  char buf[20];
   if (BLZ<10000000 || BLZ>=90000000) throw Datenfehler();
   snprintf0(buf,sizeof buf,"%03d %03d %02d",BLZ/100000,(BLZ/100)%1000,BLZ%100);
   return buf;
} 
