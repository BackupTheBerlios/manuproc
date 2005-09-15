// $Id: Interval.cc,v 1.12 2005/09/15 10:53:32 christof Exp $

#include <ManuProCConfig.h>
#include <Misc/Interval.h>
#ifdef DEFAULT_DB // actually we should test for database support
#include <Misc/Query.h>
#endif
#include <Misc/itos.h>
#include <cassert>
#include <Misc/Ausgabe_neu.h>

static const int million=1000000;
static const int secs_per_day=24*60*60;

// this does not check very strict for well-formedness
ManuProC::Interval::Interval(const std::string &_s) : days(), seconds(), microseconds()
{ std::string s=_s;
  std::string::size_type daypos=s.find("day");
  if (daypos!=std::string::npos) 
  { days=atoi(s.substr(0,daypos).c_str());
    s.erase(0,daypos+3);
    if (!s.empty() && s[0]=='s') s.erase(0,1);
    if (s.empty()) return;
  }
  std::string::size_type minus=s.find('-');
  if (minus!=std::string::npos) s.erase(0,minus+1);
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
  if (minus!=std::string::npos) 
  { seconds=-seconds;
    microseconds=-microseconds;
  }
  normalize();
}

ManuProC::Interval::Interval(int d,int s,int m)
  : days(d), seconds(s), microseconds(m)
{ normalize();
}

ManuProC::Interval ManuProC::Interval::operator-() const
{ return ManuProC::Interval(-days,-seconds,-microseconds);
}

std::string ManuProC::Interval::str() const
{  std::string s;
   
   if (days) 
   { if (days==1 || days==-1) s+=itos(days)+" day";
     else s+=itos(days)+" days";
     if (!seconds && !microseconds) return s; 
     s+=' ';
   }
   
   if (negative()) 
     return s+"-"+Interval(0,-seconds,-microseconds).str();
   s+=itos(seconds/(60*60))+":"+Formatiere((unsigned long)((seconds/60)%60),0,2,"","",'0');
   if (seconds%60 || microseconds) s+=":"+Formatiere((unsigned long)seconds%60,0,2,"","",'0');
   if (microseconds) s+="."+Formatiere((unsigned long)microseconds,0,6,"","",'0');
   return s;
}

bool ManuProC::Interval::operator==(const Interval &b) const
{ return days==b.days && seconds==b.seconds && microseconds==b.microseconds;
}

bool ManuProC::Interval::negative() const
{ return days<0 || (!days && (seconds<0 || (!seconds && microseconds<0)));
}

ManuProC::Interval::Interval(const TimeStamp &a, const TimeStamp &b)
 : days(), seconds(b.diff(a,TimeStamp::seconds)), microseconds((b.Mikrosekunde()-a.Mikrosekunde()))
{ normalize();
}

static void normalize_one(int &what, int &next, int factor, bool negative)
{ if (what<=-factor || what>=factor)
  // this is also valid if to_add gets negative
  { int to_add=what/factor;
    next+=to_add; 
    what-=factor*to_add; 
  }
  if (what<0 && !negative)
  { --next;
    what+=factor;
  }
  else if (what>0 && negative) 
  { ++next;
    what-=factor;
  }
}

void ManuProC::Interval::normalize()
{ normalize_one(microseconds, seconds, million, negative());
  bool was_negative=negative();
  normalize_one(seconds, days, secs_per_day, was_negative);
  if (was_negative!=negative())
    normalize_one(microseconds, seconds, million, negative());
}

void ManuProC::Interval::operator+=(const Interval &b)
{ days+=b.days;
  seconds+=b.seconds;
  microseconds+=b.microseconds;
  normalize();
}

ManuProC::Interval ManuProC::Interval::operator-(const Interval &b) const
{ return ManuProC::Interval(days-b.days,seconds-b.seconds,microseconds-b.microseconds);
}

ManuProC::Interval ManuProC::Interval::operator+(const Interval &b) const
{ return ManuProC::Interval(days+b.days,seconds+b.seconds,microseconds+b.microseconds);
}

#ifdef DEFAULT_DB // actually we should test for database support
FetchIStream &operator>>(FetchIStream &is, ManuProC::Interval &v)
{  std::string s;
   is >> s;
   v=ManuProC::Interval(s);
   return is;
}

ArgumentList &operator<<(ArgumentList &q, const ManuProC::Interval &v)
{  q.add_argument("'"+v.str()+"'");
   return q;
}

#endif
