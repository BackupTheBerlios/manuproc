// register a GType for an cH_EntryValue

#include <Misc/EntryValueBase.h>
#include <glib-object.h>
#include <Misc/ctime_assert.h>

static inline cH_EntryValue &cH_from_GValue(GValue *value)
{  typedef ctime_assert<sizeof(value->data[0].v_pointer)==sizeof(cH_EntryValue)>::_true failed;
   return *static_cast<cH_EntryValue*>(
   	static_cast<void*>(&value->data[0].v_pointer));
}

static inline const cH_EntryValue &cH_from_GValue(const GValue *value)
{  return *static_cast<const cH_EntryValue*>(
   	static_cast<const void*>(&value->data[0].v_pointer));
}

static void cH_value_init(GValue       *value)
{  new (&cH_from_GValue(value)) cH_EntryValue();
}

static void cH_value_free(GValue       *value)
{  cH_from_GValue(value).~cH_EntryValue();
}

static void cH_value_copy(const GValue *src, GValue *dest)
{  cH_from_GValue(dest)=cH_from_GValue(src);
}

static gconstpointer cH_value_peek(const GValue *src)
{  return &*cH_from_GValue(src);
}

static GTypeValueTable value_table =
{  .value_init = &cH_value_init,
   .value_free = &cH_value_free,
   .value_copy = &cH_value_copy,

   .value_peek_pointer = &cH_value_peek,

// _perhaps_ lcopy might be interesting to prevent superflous referencing
};

static GType_Info info=
{  /* value handling */
  .value_table = &value_table;
};

static GTypeFundamentalInfo finfo =
{ .type_flags = 0, // not instantiable, non derivable
};

GType 
cH_entry_value_get_type (void)
{
  static GType type = 0;
  if (!type) {
	  g_assert (G_TYPE_MAKE_FUNDAMENTAL (G_TYPE_RESERVED_USER_FIRST) <= g_type_fundamental_next ());
          type = g_type_register_fundamental (g_type_fundamental_next (), "cH_EntryValue", &info, &finfo, 0);
  }
  return type;
}
