/* $Id: nsc_test.cc,v 1.5 2004/04/29 15:41:48 christof Exp $ */
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998  Adolf Petig GmbH. & Co. KG
    Developed by Christof Petig <christof.petig@wtal.de>
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <string>
#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <SearchCombo.h>
#include <unistd.h>
#include <iostream>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#  include <sigc++/compatibility.h>
#endif

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

class comboArtikel : public SigC::Object
{
        int nextval;
        Gtk::SearchCombo *sc;
    public:
        void suchfunc(gboolean *cont, GtkSCContext newsearch)
        {   int added=0;
            const gchar *prefix=sc->get_text().c_str();

            if (newsearch==GTK_SEARCH_CLOSE) return;
            else if (newsearch==GTK_SEARCH_OPEN) nextval=0;
            for (;nextval<namenum;nextval++)
            {   if (!strncasecmp(prefix,names[nextval],strlen(prefix)))
                {   sc->add_item(names[nextval]);
                    added++;
                    if (added>10) 
                    { nextval++; *cont=true; return; }
                }
            }
        }
        void selectfunc()
        {
            std::cerr << "activate: "<<sc->get_text()<<"\n";
        }
        comboArtikel(Gtk::SearchCombo *s) : nextval(0), sc(s)
    {}
};


class testwindow : public Gtk::Window
{
        Gtk::VBox vbox;
        Gtk::SearchCombo scombo,scombo2,scombo3;
        comboArtikel comboart,comboart2,comboart3;
        Gtk::Entry entry;

    public:

        testwindow(bool alwfill, bool autocomp) 
           : vbox(false,0), scombo(alwfill, autocomp), scombo2(alwfill, autocomp),
             scombo3(alwfill, autocomp), comboart(&scombo),
             comboart2(&scombo2), comboart3(&scombo3)
        {
            vbox.add( entry);
            vbox.add( scombo);
            vbox.add( scombo2);
            vbox.add( scombo3);
            add(vbox);
            vbox.show();
            entry.show();
            scombo.show();
            scombo2.show();
            scombo3.show();

            scombo.signal_search().connect(SigC::slot(comboart,&comboArtikel::suchfunc));
            scombo.signal_activate().connect(SigC::slot(comboart,&comboArtikel::selectfunc));
            scombo2.signal_search().connect(SigC::slot(comboart2,&comboArtikel::suchfunc));
            scombo2.signal_activate().connect(SigC::slot(comboart2,&comboArtikel::selectfunc));
            scombo3.signal_search().connect(SigC::slot(comboart3,&comboArtikel::suchfunc));
            scombo3.signal_activate().connect(SigC::slot(comboart3,&comboArtikel::selectfunc));
        }
        bool delete_event_impl(GdkEventAny *)
        {
            std::cout << "Close Window" << std::endl;
            Gtk::Main::instance()->quit();
            return 0;
        }
};


int main( int argc, char **argv )
{   int opt;
    bool autocomplete=true;
    bool alwaysfill=true;
    Gtk::Main m(argc, argv );
    while ((opt=getopt(argc,argv,"gadf"))!=EOF)
    switch (opt)
    {  case 'a': autocomplete=!autocomplete; break;
       case 'f': alwaysfill=!alwaysfill; break;
       case '?': std::cout << "USAGE sc_test [-g] [-a] [-f]\n"; break;
    }
    std::cout << "alwaysfill("<<alwaysfill<<") autocomplete("<<autocomplete<<")\n";
    testwindow w(alwaysfill,autocomplete);

    w.show();

    m.run(w);
    return 0;
}
