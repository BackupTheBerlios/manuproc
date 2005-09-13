// $Id: Interval.cc,v 1.10 2005/09/13 08:01:03 christof Exp $

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
  std::string::size_type daypos=s.find("day");
  if (daypos!=std::string::npos) 
  { days=atoi(s.substr(0,daypos).c_str());
    s.erase(0,daypos+3);
    if (!s.empty() && s[0]=='s') s.erase(0,1);
    if (s.empty()) return;
  }
  std::string::size_type first_colon=s.find(':');
  std::string::size_type second_colon=std::string::npos;
  if (first_colon!=std::string::npos) second_colon=s.find(':',first_colon+1);
  int hours=atoi(s.substr(0,first_colon).c_str()),minutes=0;
  if (first_colon!=std::string::npos)
    minutes=atoi(s.substr(first_colon+1,second_colon).c_str());
  if (second_colon!=std::string::npos)
  { std::string::size_type first_dot=s.find('.',second_colon+1);
    seconds=atoi(s.substr(second_colon+1,first_dot).c_str());
    if (first_dot!=std::string::npos)
    { const char* beginptr=s.c_str()+first_dot+1;
      char* endptr;
      microseconds=strtol(beginptr,&endptr,10);
      for (int i=0;i<6-(endptr-beginptr);++i) microseconds*=10;
      if (microseconds>999999 || microseconds<0) microseconds=0;
    }
  }
  seconds+=(hours*60*60) + (minutes*60);
}

std::string ManuProC::Interval::str() const
{  std::string s;
   
   if (days) 
   { if (days==1) s+=itos(days)+" day";
     else s+=itos(days)+" days";
     if (!seconds && !microseconds) return s; 
     s+=' ';
   }
   s+=itos(seconds/(60*60))+":"+Formatiere((unsigned long)((seconds/60)%60),0,2,"","",'0');
   if (seconds%60 || microseconds) s+=":"+Formatiere((unsigned long)seconds%60,0,2,"","",'0');
   if (microseconds) s+="."+Formatiere((unsigned long)microseconds,0,6,"","",'0');
   return s;
}

bool ManuProC::Interval::operator==(const Interval &b)
{ return days==b.days && seconds==b.seconds && microseconds==b.microseconds;
}

ManuProC::Interval::Interval(const TimeStamp &a, const TimeStamp &b)
 : days(), seconds(b.diff(a,TimeStamp::seconds)), microseconds((b.Mikrosekunde()-a.Mikrosekunde()))
{ if (microseconds<0)
  { microseconds+=1000000; --seconds; }
  if (seconds>=24*60*60) 
  { days=seconds/(24*60*60); seconds-=days*(24*60*60); }
}

void ManuProC::Interval::operator+=(const Interval &b)
{ days+=b.days;
  seconds+=b.seconds;
  microseconds+=b.microseconds;
  if (microseconds>=1000000) { microseconds-=1000000; ++seconds; }
  if (seconds>=24*60*60)
  { ++days; seconds-=24*60*60; }
}

#ifdef DEFAULT_DB // actually we should test for database support
FetchIStream &operator>>(FetchIStream &is, ManuProC::Interval &v)
{  std::string s;
   is >> s;
   v=ManuProC::Interval(s);
   return is;
}

ArgumentList &operator<<(ArgumentList &q, const ManuProC::Interval &v)
{  std::string s="'";
   if (v.days) s+=itos(v.days)+" days ";
   s+=itos(v.seconds/(60*60))+":"+itos((v.seconds/60)%60)+":"+itos(v.seconds%60);
   if (v.microseconds) s+="."+Formatiere((unsigned long)v.microseconds,0,6,"","",'0');
   s+="'";
   q.add_argument(s);
   return q;
}

#endif
