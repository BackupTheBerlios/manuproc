/* $Id: Message.h,v 1.1 2003/01/06 15:07:33 christof Exp $ */
/*  Gtk--addons: a collection of gtk-- addons
    Copyright (C) 1998  Adolf Petig GmbH. & Co. KG
    Developed by Jacek Jakubowski <jacek@wtal.de>
 
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

#ifndef MESSAGEH
#define MESSAGEH
#include<gtkmm/dialog.h>
#include<gtkmm/label.h>
#include<gtkmm/button.h>
#include<gtkmm/box.h>
#include<gtkmm/main.h>
#include <string>

class Message : public Gtk::Dialog
{
        Gtk::Label message;
        Gtk::Button okbutton;
        Gtk::HBox hbox;

    public :
        Message() : message( "" ), okbutton( "OK" ), hbox( false,0 )
        {
            get_vbox()->pack_start( hbox,false,false,20 );
            hbox.pack_start( message,false,false,20 );
            get_action_area()->pack_start( okbutton,false,false,0 );

            okbutton.show();
            message.show();
            hbox.show();

            okbutton.clicked.connect(SigC::slot(this,&Message::Hide));
        }

        void Show( const std::string &m )
        {
            Gtk::Main::instance()->grab_add( *this );
            message.set( m );
            show();
        }

    private :
        void Hide()
        {
            ( Gtk::Main::instance() )->grab_remove( *this );
            hide();
        }
};

#endif
