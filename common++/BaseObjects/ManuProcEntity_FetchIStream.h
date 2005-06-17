
#include <Misc/FetchIStream.h>
#include "ManuProcEntity.h"


template <class T> FetchIStream &operator>>
(FetchIStream &is, ManuProcEntity<T> &me)
{
 int _id;
 is >> _id;
 me.setId(_id); // static_cast<ManuProcEntity<T>::ID>(_id));
 return is;
}
