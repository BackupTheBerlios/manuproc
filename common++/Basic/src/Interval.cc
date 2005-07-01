// $Id: Interval.cc,v 1.1 2005/07/01 06:38:00 christof Exp $

#include <ManuProCConfig.h>
#include <Misc/Interval.h>
#ifdef DEFAULT_DB // actually we should test for database support
#include <Misc/Query.h>
#endif
#include <Misc/itos.h>
#include <cassert>

ManuProC::Interval::Interval(const std::string &_s) : days(), seconds(), microseconds()
{ std::string s=_s;
  if (s.size()==5) s+=":00";
  assert(s.size()==8);
  assert(s[2]==':' && s[5]==':');
  seconds=(atoi(s.substr(0,2).c_str())*60*60) + (atoi(s.substr(3,2).c_str())*60)
    + atoi(s.substr(6,2).c_str());
}

#ifdef DEFAULT_DB // actually we should test for database support
FetchIStream &operator>>(FetchIStream &is, ManuProC::Interval &v)
{  std::string s;
   is >> s;
   v=ManuProC::Interval(s);
   return is;
}

ArgumentList &operator<<(ArgumentList &q, const ManuProC::Interval &v)
{  assert(!v.days);
   assert(!v.microseconds); // not implemented yet
   std::string s="'"+itos(v.seconds/(60*60))+":"+itos((v.seconds/60)%60)+":"+itos(v.seconds%60)+"'";
   q.add_argument(s);
   return q;
}

#endif
