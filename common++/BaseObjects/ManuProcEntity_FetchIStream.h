
#include <Misc/FetchIStream.h>
#include "ManuProcEntity.h"


template <class T> FetchIStream &operator>>
(FetchIStream &is, ManuProcEntity<T> &me)
{
 int _id;
 is >> _id;
 me_id.setId((ManuProcEntity<T>::ID)_id);
 return is;
}


