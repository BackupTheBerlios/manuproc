// $Id: Interval.h,v 1.7 2005/09/08 10:08:16 christof Exp $

#ifndef TA734B8C_4F66_4FEF_BEA5_8A6C8FC2C017
#define TA734B8C_4F66_4FEF_BEA5_8A6C8FC2C017

#include <string>
#include <Misc/TimeStamp.h>

namespace ManuProC {
struct Interval
{ int days;
  int seconds;
  int microseconds;
  
  Interval(const std::string &i);
  Interval() : days(), seconds(), microseconds() {}
  Interval(const TimeStamp &a, const TimeStamp &b); // a<b (normally)
  std::string str() const;
  bool operator==(const Interval &b);
};
}

class FetchIStream;
FetchIStream &operator>>(FetchIStream &is, ManuProC::Interval &v);
class ArgumentList;
ArgumentList &operator<<(ArgumentList &, const ManuProC::Interval &v);

#endif

