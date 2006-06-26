
#include <Misc/Query.h>
#include "ManuProcEntity.h"


template <class T> Query::Row &operator>>
(Query::Row &is, ManuProcEntity<T> &me)
{
 int _id;
 is >> _id;
 me.setId(_id); // static_cast<ManuProcEntity<T>::ID>(_id));
 return is;
}
