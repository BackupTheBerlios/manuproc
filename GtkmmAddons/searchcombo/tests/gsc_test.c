/* $Id: gsc_test.c,v 1.5 2004/04/29 14:24:40 christof Exp $ */

#include <gtk/gtk.h>
#include "gtksearchcombo.h"
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

//#define SLOW_DOWN

static gboolean reopen;

const char *names[]=
  {
    "Alan Cox <alan@lxorguk.ukuu.org.uk>",
    "Albert D. Cahalan <acahalan@cs.uml.edu>",
    "Alberto Vignani <vignani@mail.tin.it>",
    "Alistair Riddell <alistair@watsons.edin.sch.uk>",
    "Andrea Arcangeli <andrea@e-mind.com>",
    "Andreas Jellinghaus <aj@dungeon.inka.de>",
    "Andreas Kostyrka <andreas@ag.or.at>",
    "Andrew Tridgell <tridge@samba.anu.edu.au>",
    "Bill Hawes <whawes@transmeta.com>",
    "Carsten Haitzler <raster@redhat.com>",
    "Chris Wedgwood <chris@cybernet.co.nz>",
    "Christof Petig <christof.petig@wtal.de",
    "Colin Plumb <colin@nyx.net>",
    "Colin Scott <cs@iinet.com.au>",
    "Damon Chaplin <DAChaplin@email.msn.com>",
    "David Abilleira Freijeiro <odaf@nexo.es>",
    "David Campbell <campbell@torque.net>",
    "David Hinds <dhinds@zen.stanford.edu>",
    "David S. Miller <davem@dm.cobaltmicro.com>",
    "David Woodhouse <David.Woodhouse@mvhi.com>",
    "Donald Becker <becker@cesdis.gsfc.nasa.gov>",
    "Eric S. Raymond <esr@thyrsus.com>",
    "Etienne Lorrain <lorrain@fb.sony.de>",
    "Ettore Perazzoli <ettore@comm2000.it>",
    "Federico Mena Quintero <federico@nuclecu.unam.mx>",
    "Geert Uytterhoeven <geert@geert.cs.kuleuven.ac.be>",
    "Gert Scholten <gs2@dds.nl>",
    "Gregory Maxwell <linker@z.ml.org>",
    "Guillaume Laurent <glaurent@worldnet.fr>",
    "H. Peter Anvin <hpa@transmeta.com>",
    "Hannu Savolainen <hannu@opensound.com>",
    "Hans de Goede <j.w.r.degoede@ITS.tudelft.nl>",
    "Havoc Pennington <rhpennin@midway.uchicago.edu>",
    "Ionut Borcoman <borco@mailbox.ro>",
    "Jacek Jakubowski <jacek@wtal.de>",
    "Jakub Jelinek <jj@sunsite.ms.mff.cuni.cz>",
    "Jeffrey A Law <law@cygnus.com>",
    "Jon 'maddog' Hall <hall@zk3.dec.com>",
    "Linus Torvalds <torvalds@transmeta.com>",
    "Ingo Molnar <mingo@chiara.csoma.elte.hu>",
    "Mario Motta <mmotta@guest.net>",
    "Mark Galassi <rosalia@cygnus.com>",
    "Martin Mares <mj@ucw.cz>",
    "Matthias Urlichs <smurf@noris.de>",
    "Michael Elizabeth Chastain <mec@shout.net>",
    "Michael Fulbright <msf@redhat.com>",
    "Miguel de Icaza <miguel@nuclecu.unam.mx>",
    "Olaf Kirch <okir@monad.swb.de>",
    "Olivier Galibert <galibert@pobox.com>",
    "Olivier Gutknecht <gutkneco@lirmm.fr>",
    "Owen Taylor <otaylor@redhat.com>",
    "Pavel Machek <pavel@bug.ucw.cz>",
    "Philip Blundell <philb@gnu.org>",
    "Rebecca Ore <rebecca.ore@op.net>",
    "Richard Gooch <rgooch@atnf.csiro.au>",
    "Richard Stallman <rms@gnu.org>",
    "Tero Pulkkinen <terop@students.cc.tut.fi>",
    "Theodore Y. Ts'o <tytso@MIT.EDU>",
    "Tim Janik <timj@gtk.org>",
    "Tor Lillqvist <tml@hemuli.tte.vtt.fi>",
    "Trever Adams <highlander@teleteam.net>",
    "Werner Almesberger <almesber@lrc.di.epfl.ch>"
  };
const int namenum=sizeof(names)/sizeof(names[0]);

void search_cb_names(GtkSearchCombo *sc, gboolean *continue_, GtkSCContext new, gpointer   data)
{   const gchar *prefix=gtk_searchcombo_get_text(sc);
    static int nextval=0;
    int added=0;

    if (new==GTK_SEARCH_CLOSE) return;
    if (new==GTK_SEARCH_OPEN || new==GTK_SEARCH_REOPEN) 
    { nextval=0; reopen=new==GTK_SEARCH_REOPEN; }
    for (;nextval<namenum;nextval++)
    {   if (reopen || !strncasecmp(prefix,names[nextval],strlen(prefix)))
        {   gtk_searchcombo_add_item(sc,names[nextval]);
            added++;
            if (added>
#ifdef SLOW_DOWN
            		0
#else   
            		10
#endif            		
            			) 
            { nextval++; 
              *continue_=1;
#ifdef SLOW_DOWN
              sleep(1);
#endif
              return;
            }
        }
    }
}

void hello( GtkWidget *widget,
            gpointer   data )
{
    g_print ("Hello World\n");
}

gint delete_event( GtkWidget *widget,
                   GdkEvent  *event,
		   gpointer   data )
{   gtk_main_quit();
    return(TRUE);
}

#include <dirent.h> /* is this portable? */
#include <sys/stat.h>
#include <unistd.h>

#if 0
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

void search_cb(GtkSearchCombo *sc, gboolean *continue_, GtkSCContext new, gpointer   data)
{   static DIR *d;
    static char path[10240];
    static char prefix[1024];
    
    if (new==GTK_SEARCH_CLOSE)
    {  if (d)
       {  closedir(d); d=NULL; }
       DEBUG(puts("closed"));
       return;
    }
    else if (new==GTK_SEARCH_OPEN || new==GTK_SEARCH_REOPEN)
    {  char *s;
       reopen=new==GTK_SEARCH_REOPEN;
       assert(!d);
       strncpy(path,gtk_searchcombo_get_text(sc),sizeof path);
       if ((s=strrchr(path,'/'))) 
       {  strncpy(prefix,s+1,sizeof prefix);
          s[1]=0;
          d=opendir(path);
          DEBUG(printf("%p=opendir(%s)\n",d,path));
          if (!d) // up one dir
          {  DEBUG(printf("experimental up stuff\n"));
             *s=0;
             if ((s=strrchr(path,'/')))
                s[1]=0;
             if (!*path) 
             {  gtk_searchcombo_add_item(sc,"/");
                gtk_searchcombo_add_item(sc,"./");
                gtk_searchcombo_add_item(sc,"../");
             }
             else gtk_searchcombo_add_item(sc,path);
          }
       }
       else 
       {  path[0]=0;
          strncpy(prefix,gtk_searchcombo_get_text(sc),sizeof prefix);
          gtk_searchcombo_add_item(sc,"/");
          d=opendir(".");
       }
    }
    if (d)
    {  struct dirent *de=readdir(d);
       if (de)
       {  DEBUG(printf("%s=readdir(%p)\n",de->d_name,d));
          if (reopen||!strncasecmp(prefix,de->d_name,strlen(prefix)))
          {  char buf[10240];
             struct stat statb;
             g_snprintf(buf,sizeof buf,"%s%s",path,de->d_name);
             if (!stat(buf,&statb))
             {  if (S_ISDIR(statb.st_mode)) strcat(buf,"/");
             }
             else DEBUG(printf("stat(%s) failed\n",buf));
             gtk_searchcombo_add_item(sc,buf);
          }
          *continue_=TRUE;
       }
       else
       {  closedir(d); d=NULL;
       }
    }
}

void activate_cb(GtkSearchCombo *sc, gpointer   data)
{  struct stat statb;
   printf("activate_cb %p %p\n",sc,data);
   printf("text '%s'\n",gtk_searchcombo_get_text(sc));
   if (!stat(gtk_searchcombo_get_text(sc),&statb))
   {  if (S_ISDIR(statb.st_mode))
         /* trigger listing of dir's contents */
         g_signal_emit_by_name(G_OBJECT(sc->entry),"changed");
   }
} 

void change_editable(GtkButton *b, GtkSearchCombo *sc)
{  static gboolean ed=TRUE;
   ed=!ed;
   gtk_searchcombo_set_editable(sc,(gint)ed);
}

int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *searchcombo;
    GtkWidget *searchcombo2;
    GtkWidget *hbox;
    GtkWidget *button;
    GtkWidget *button2;

    gtk_init(&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);
    hbox = gtk_hbox_new (0,4);
    button = gtk_button_new_with_label("focus");
    button2 = gtk_button_new_with_label("editable");
    searchcombo = gtk_searchcombo_new ();
    searchcombo2 = gtk_searchcombo_new ();
    gtk_signal_connect (GTK_OBJECT (searchcombo), "search",
			GTK_SIGNAL_FUNC (search_cb), (gpointer)0x1234);
    gtk_signal_connect (GTK_OBJECT (searchcombo), "activate",
			GTK_SIGNAL_FUNC (activate_cb), (gpointer)0x5678);
    gtk_signal_connect (GTK_OBJECT (searchcombo2), "search",
			GTK_SIGNAL_FUNC (search_cb_names), NULL);
    gtk_signal_connect (GTK_OBJECT (searchcombo2), "activate",
			GTK_SIGNAL_FUNC (activate_cb), (gpointer)0x9876);
    gtk_signal_connect_object (GTK_OBJECT(button),"clicked", 
    			GTK_SIGNAL_FUNC (gtk_widget_grab_focus), (gpointer)searchcombo2);
    gtk_signal_connect (GTK_OBJECT(button2),"clicked", 
    			GTK_SIGNAL_FUNC (change_editable), (gpointer)searchcombo2);
    
    gtk_container_add (GTK_CONTAINER (hbox), GTK_WIDGET(searchcombo));
    gtk_container_add (GTK_CONTAINER (hbox), GTK_WIDGET(searchcombo2));
    gtk_container_add (GTK_CONTAINER (hbox), GTK_WIDGET(button));
    gtk_container_add (GTK_CONTAINER (hbox), GTK_WIDGET(button2));
    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(hbox));
    gtk_widget_show (GTK_WIDGET(searchcombo));
    gtk_widget_show (GTK_WIDGET(searchcombo2));
    gtk_widget_show (GTK_WIDGET(button));
    gtk_widget_show (GTK_WIDGET(button2));
    gtk_widget_show (GTK_WIDGET(hbox));
    gtk_widget_show (GTK_WIDGET(window));
    gtk_searchcombo_set_enable_tab(GTK_SEARCHCOMBO(searchcombo2),TRUE);

    gtk_main ();
    
    return(0);
}
