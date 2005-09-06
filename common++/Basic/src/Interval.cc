// $Id: Interval.cc,v 1.6 2005/09/06 10:36:00 christof Exp $

#include <ManuProCConfig.h>
#include <Misc/Interval.h>
#ifdef DEFAULT_DB // actually we should test for database support
#include <Misc/Query.h>
#endif
#include <Misc/itos.h>
#include <cassert>
#include <Misc/Ausgabe_neu.h>

ManuProC::Interval::Interval(const std::string &_s) : days(), seconds(), microseconds()
{ std::string s=_s;
  std::string::size_type first_colon=s.find(':');
  std::string::size_type second_colon=std::string::npos;
  if (first_colon!=std::string::npos) second_colon=s.find(':',first_colon+1);
  int hours=atoi(s.substr(0,first_colon).c_str()),minutes=0;
  if (first_colon!=std::string::npos)
    minutes=atoi(s.substr(first_colon+1,second_colon).c_str());
  if (second_colon!=std::string::npos)
    seconds=atoi(s.substr(second_colon+1).c_str());
  seconds+=(hours*60*60) + (minutes*60);
}

std::string ManuProC::Interval::str() const
{  assert(!days);
   assert(!microseconds); // not implemented yet
   std::string s=itos(seconds/(60*60))+":"+Formatiere((unsigned long)((seconds/60)%60),0,2,"","",'0');
   if (seconds%60 || microseconds) s+=":"+Formatiere((unsigned long)seconds%60,0,2,"","",'0');
   return s;
}

bool ManuProC::Interval::operator==(const Interval &b)
{ return days==b.days && seconds==b.seconds && microseconds==b.microseconds;
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
