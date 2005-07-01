// $Id: Interval.h,v 1.1 2005/07/01 06:38:00 christof Exp $

#include <string>

namespace ManuProC {
struct Interval
{ int days;
  int seconds;
  int microseconds;
  
  Interval(const std::string &i);
  Interval() : days(), seconds(), microseconds() {}
  
};
}

class FetchIStream;
FetchIStream &operator>>(FetchIStream &is, ManuProC::Interval &v);
class ArgumentList;
ArgumentList &operator<<(ArgumentList &, const ManuProC::Interval &v);
