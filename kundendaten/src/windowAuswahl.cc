/* Processed by ecpg (2.8.0) */
/* These three include files are added by the preprocessor */
#include <ecpgtype.h>
#include <ecpglib.h>
#include <ecpgerrno.h>
#line 1 "windowAuswahl.pgcc"


#include "config.h"
#include "windowAuswahl.hh"
#include <Aux/Transaction.h>
#include <Aux/SQLerror.h>

#line 1 "/usr/local/pgsql/include/sqlca.h"
#ifndef POSTGRES_SQLCA_H
#define POSTGRES_SQLCA_H

#define SQLERRMC_LEN	70

#ifdef __cplusplus
extern		"C"
{
#endif

	struct sqlca
	{
		char		sqlcaid[8];
		long		sqlabc;
		long		sqlcode;
		struct
		{
			int			sqlerrml;
			char		sqlerrmc[SQLERRMC_LEN];
		}			sqlerrm;
		char		sqlerrp[8];
		long		sqlerrd[6];
		/* Element 0: empty						*/
		/* 1: OID of processed tuple if applicable			*/
		/* 2: number of rows processed				*/
		/* after an INSERT, UPDATE or 				*/
		/* DELETE statement					*/
		/* 3: empty						*/
		/* 4: empty						*/
		/* 5: empty						*/
		char		sqlwarn[8];
		/* Element 0: set to 'W' if at least one other is 'W' 	*/
		/* 1: if 'W' at least one character string	  	*/
		/* value was truncated when it was		   	*/
		/* stored into a host variable.			   	*/
		/* 2: if 'W' a (hopefully) non-fatal notice occured 	*/		/* 3: empty									  */
		/* 4: empty						*/
		/* 5: empty						*/
		/* 6: empty						*/
		/* 7: empty						*/

		char		sqlext[8];
	};

	extern struct sqlca sqlca;


#ifdef __cplusplus
}
#endif

#endif

#line 11 "windowAuswahl.pgcc"

#include <Gtk_OStream.h>


void windowAuswahl::on_radiobuttonSortiername_clicked()
{   
   windowAuswahl::load_liste();
}

void windowAuswahl::on_radiobuttonKdNr_clicked()
{   
   windowAuswahl::load_liste();
}

void windowAuswahl::on_windowAuswahl_destroy()
{   
}

void windowAuswahl::load_liste()
{
   clistAuswahl->clear();
   /* exec sql begin declare section */
        
         
       
   
#line 33 "windowAuswahl.pgcc"
   char  SORTNAME [ 31 ]   ;
 
#line 34 "windowAuswahl.pgcc"
   unsigned int  KUNDENNR   ;
 
#line 35 "windowAuswahl.pgcc"
   char  QUERY [ 1024 ]   ;
/* exec sql end declare section */
#line 36 "windowAuswahl.pgcc"

   string ord;
   if (radiobuttonSortiername->get_active()) ord = "sortname";
   if (radiobuttonKdNr->get_active()) ord = "kundennr";
   snprintf(QUERY,sizeof(QUERY),
      "select kundennr, sortname FROM kunden order by %s",ord.c_str());

   Transaction tr;
   { ECPGprepare(__LINE__, "inp_", QUERY);}
#line 44 "windowAuswahl.pgcc"

   /* declare inp  cursor for ? */
#line 45 "windowAuswahl.pgcc"

   { ECPGdo(__LINE__, NULL, "declare inp  cursor for ?", 
	ECPGt_char_variable,(ECPGprepared_statement("inp_")),1L,1L,1*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);}
#line 46 "windowAuswahl.pgcc"

   SQLerror::test(__FILELINE__);
   Gtk::OStream os(clistAuswahl);
   while(true)
      {
         { ECPGdo(__LINE__, NULL, "fetch inp", ECPGt_EOIT, 
	ECPGt_unsigned_int,&(KUNDENNR),1L,1L,sizeof(unsigned int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(SORTNAME),31L,1L,31*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 51 "windowAuswahl.pgcc"

         SQLerror::test(__FILELINE__,100);  
         if (sqlca.sqlcode) break;
         os <<SORTNAME<<"\t"<< KUNDENNR<<"\n";
      }      
   { ECPGdo(__LINE__, NULL, "close inp", ECPGt_EOIT, ECPGt_EORT);}
#line 56 "windowAuswahl.pgcc"

   tr.close();
   for (unsigned int i=0;i<clistAuswahl->columns().size();++i)
      clistAuswahl->set_column_auto_resize(i,true);
}


windowAuswahl::windowAuswahl(windowTop* h)
{
   hauptfenster=h ;
   windowAuswahl::load_liste();
}

void windowAuswahl::on_clistAuswahl_select_row(gint row, gint column, GdkEvent *event)
{
   string sortname = clistAuswahl->get_text(row,0);
   unsigned int kndr = atoi(clistAuswahl->get_text(row,1).c_str());
   hauptfenster->load_kunde(sortname,kndr);
   destroy();
}

windowAuswahl::~windowAuswahl()
{  glademm_RadioButtonGroup("Sortierung")=Gtk::RadioButton_Helpers::Group();
}
