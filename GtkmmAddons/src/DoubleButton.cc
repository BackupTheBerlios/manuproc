// Generated by gtkmmproc -- DO NOT MODIFY!

#include <gtkmm/DoubleButton.h>
#include <gtkmm/private/DoubleButton_p.h>

/* $Id: DoubleButton.cc,v 1.5 2004/01/29 14:59:55 christof Exp $ */

/* searchcombo.h
 * 
 * Copyright (C) 2003 Adolf Petig GmbH & Co. KG
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

#include <doublebutton.h>

ManuProC::DoubleButton::DoubleButton() : Glib::ObjectBase(0), //Mark this class as gtkmmproc-generated, rather than a custom class, to allow vfunc optimisations.
  Gtk::Button(Glib::ConstructParams(doublebutton_class_.init(), (char*) 0))
{  }


namespace
{

void DoubleButton_signal_secondpressed_callback(doublebutton* self, int button,void* data)
{
  using namespace ManuProC;
  typedef SigC::Slot1<void,int> SlotType;

  // Do not try to call a signal on a disassociated wrapper.
  if(Glib::ObjectBase::_get_current_wrapper((GObject*) self))
  {
    try
    {
      if(SigC::SlotNode *const slot = Glib::SignalProxyNormal::data_to_slot(data))
        (*(SlotType::Proxy)(slot->proxy_))
            (button, slot);
    }
    catch(...)
    {
      Glib::exception_handlers_invoke();
    }
  }
}

const Glib::SignalProxyInfo DoubleButton_signal_secondpressed_info =
{
  "secondpressed",
  (GCallback) &DoubleButton_signal_secondpressed_callback,
  (GCallback) &DoubleButton_signal_secondpressed_callback
};

} // anonymous namespace


namespace Glib
{

ManuProC::DoubleButton* wrap(doublebutton* object, bool take_copy)
{
  return dynamic_cast<ManuProC::DoubleButton *> (Glib::wrap_auto ((GObject*)(object), take_copy));
}

} /* namespace Glib */

namespace ManuProC
{


/* The *_Class implementation: */

const Glib::Class& DoubleButton_Class::init()
{
  if(!gtype_) // create the GType if necessary
  {
    // Glib::Class has to know the class init function to clone custom types.
    class_init_func_ = &DoubleButton_Class::class_init_function;

    // TODO: This is currently just optimized away, apparently with no harm.
    // Is it actually necessary?
    // Make sure that the parent type has been created.
    CppClassParent::CppObjectType::get_type();

    // Create the wrapper type, with the same class/instance size as the base type.
    register_derived_type(doublebutton_get_type());

    // Add derived versions of interfaces, if the C type implements any interfaces:
  }

  return *this;
}

void DoubleButton_Class::class_init_function(void* g_class, void* class_data)
{
  BaseClassType *const klass = static_cast<BaseClassType*>(g_class);
  CppClassParent::class_init_function(klass, class_data);

  klass->secondpressed = &secondpressed_callback;
}


void DoubleButton_Class::secondpressed_callback(doublebutton* self, int button)
{
  CppObjectType *const obj = dynamic_cast<CppObjectType*>(
      Glib::ObjectBase::_get_current_wrapper((GObject*)self));

  // Non-gtkmmproc-generated custom classes implicitly call the default
  // Glib::ObjectBase constructor, which sets is_derived_. But gtkmmproc-
  // generated classes can use this optimisation, which avoids the unnecessary
  // parameter conversions if there is no possibility of the virtual function
  // being overridden:
  if(obj && obj->is_derived_())
  {
    try // Trap C++ exceptions which would normally be lost because this is a C callback.
    {
      // Call the virtual member method, which derived classes might override.
      obj->on_secondpressed(button);
    }
    catch(...)
    {
      Glib::exception_handlers_invoke();
    }
  }
  else
  {
    BaseClassType *const base = static_cast<BaseClassType*>(
        g_type_class_peek_parent(G_OBJECT_GET_CLASS(self)) // Get the parent class of the object class (The original underlying C class).
    );

    // Call the original underlying C function:
    if(base && base->secondpressed)
      (*base->secondpressed)(self, button);
  }
}


Glib::ObjectBase* DoubleButton_Class::wrap_new(GObject* o)
{
  return manage(new DoubleButton((doublebutton*)(o)));

}


/* The implementation: */

DoubleButton::DoubleButton(const Glib::ConstructParams& construct_params)
:
  Gtk::Button(construct_params)
{
  }

DoubleButton::DoubleButton(doublebutton* castitem)
:
  Gtk::Button((GtkButton*)(castitem))
{
  }

DoubleButton::~DoubleButton()
{
  destroy_();
}

DoubleButton::CppClassType DoubleButton::doublebutton_class_; // initialize static member

GType DoubleButton::get_type()
{
  return doublebutton_class_.init().get_type();
}

GType DoubleButton::get_base_type()
{
  return doublebutton_get_type();
}


Glib::SignalProxy1<void,int> DoubleButton::signal_secondpressed()
{
  return Glib::SignalProxy1<void,int>(this, &DoubleButton_signal_secondpressed_info);
}


void ManuProC::DoubleButton::on_secondpressed(int mbutton)
{
  BaseClassType *const base = static_cast<BaseClassType*>(
      g_type_class_peek_parent(G_OBJECT_GET_CLASS(gobject_)) // Get the parent class of the object class (The original underlying C class).
  );

  if(base && base->secondpressed)
    (*base->secondpressed)(gobj(),mbutton);
}


} // namespace ManuProC


