// -*- c++ -*-
// Generated by gtkmmproc -- DO NOT MODIFY!
#ifndef _GTKMM_DOUBLEBUTTON_H
#define _GTKMM_DOUBLEBUTTON_H

#include <glibmm.h>

/* $Id: DoubleButton.h,v 1.1 2003/04/08 08:56:54 christof Exp $ */

/* searchcombo.h
 * 
 * Copyright (C) 1999-2002 Adolf Petig GmbH & Co. KG
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <gtkmm/button.h>
#include <doublebutton.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct _doublebutton doublebutton;
typedef struct _doublebuttonClass doublebuttonClass;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


namespace ManuProC
{ class DoubleButton_Class; } // namespace ManuProC
namespace ManuProC {

//: DoubleButton widget (doublebutton is the C name)

class DoubleButton : public Gtk::Button
{
 public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  typedef DoubleButton CppObjectType;
  typedef DoubleButton_Class CppClassType;
  typedef doublebutton BaseObjectType;
  typedef doublebuttonClass BaseClassType;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

  virtual ~DoubleButton();

#ifndef DOXYGEN_SHOULD_SKIP_THIS

private:
  friend class DoubleButton_Class;
  static CppClassType doublebutton_class_;

  // noncopyable
  DoubleButton(const DoubleButton&);
  DoubleButton& operator=(const DoubleButton&);

protected:
  explicit DoubleButton(const Glib::ConstructParams& construct_params);
  explicit DoubleButton(doublebutton* castitem);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static GType get_type()      G_GNUC_CONST;
  static GType get_base_type() G_GNUC_CONST;
#endif

  doublebutton*       gobj()       { return reinterpret_cast<doublebutton*>(gobject_); }
  const doublebutton* gobj() const { return reinterpret_cast<doublebutton*>(gobject_); }


public:
  //C++ methods used to invoke GTK+ virtual functions:

protected:
  //GTK+ Virtual Functions (override these to change behaviour):

  //Default Signal Handlers::
  virtual void on_secondpressed(int mbutton);


private:


public:
  explicit DoubleButton();
  
  
  /**
   * @par Prototype:
   * <tt>void %secondpressed(int mbutton)</tt>
   */
  Glib::SignalProxy1<void,int> signal_secondpressed();
;


};

}

namespace Glib
{
  /** @relates ManuProC::DoubleButton */
  ManuProC::DoubleButton* wrap(doublebutton* object, bool take_copy = false);
}
#endif /* _GTKMM_DOUBLEBUTTON_H */

