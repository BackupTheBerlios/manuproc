// $Id: intmap.cc,v 1.2 2003/09/17 09:07:07 christof Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 2003 Adolf Petig GmbH & Co. KG, written by Christof Petig
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// 0: C array  1: intmap (vector prealloc)  2: intmap (vector resize)
// 3: vectormap_u  4: vectormap_s  5: std::map  6: filled vectormap_s
// 7: filled map
#ifndef WHAT
#define WHAT 1
#endif
#ifndef DIMENSION
#define DIMENSION 65536
#endif
// 1 or 100
#ifndef SPARSE
#define SPARSE 1
#endif

#ifndef LOOPS
#define LOOPS 20*DIMENSION
#endif

#if WHAT==0
typedef int type[DIMENSION*SPARSE];
#elif WHAT==1 || WHAT==2
#include <Misc/intmap.h>
typedef intmap<int,int> type;
#elif WHAT==3
#include <Misc/vectormap.h>
typedef vectormap_u<int,int> type;
#elif WHAT==4 || WHAT==6
#include <Misc/vectormap.h>
typedef vectormap_s<int,int> type;
#elif WHAT==5 || WHAT==7
#include <map>
typedef std::map<int,int> type;
#endif

//#define MEM_BORDER (char*)sbrk(0)
//#define MEM_BORDER (char*)malloc(1)
#define MEM_BORDER new char

#include <iostream>
#include <sys/times.h>
#include <cstdlib>
#include <sys/resource.h>

int main()
{  //struct rusage rusage;
//   getrusage(RUSAGE_SELF,&rusage);
//   long mem_start=rusage.ru_idrss;
   srand(time(0));
   char *init_mem=MEM_BORDER;
   char *mem_start=init_mem;
//   while ((mem_start=MEM_BORDER)==init_mem) malloc(4);
   struct tms tms;
   
   times(&tms);
   clock_t start=tms.tms_utime;

#if WHAT==0
   int *var=(int*)malloc(DIMENSION*SPARSE*sizeof(int));
#elif WHAT==1
   type var(DIMENSION*SPARSE);
#else
   type var;
#endif
						
//   getrusage(RUSAGE_SELF,&rusage);
//   long mem_creation=rusage.ru_idrss;
   char *mem_creation=MEM_BORDER;

#if WHAT==6 || WHAT==7
   for (int i=0;i<DIMENSION;++i) var[i*SPARSE]=rand();
#else   
   for (int i=0;i<DIMENSION;++i) var[(rand()%DIMENSION)*SPARSE]=rand();
#endif
   times(&tms);
   clock_t firstD=tms.tms_utime;
//   getrusage(RUSAGE_SELF,&rusage);
//   long mem_firstD=rusage.ru_idrss;
   char *mem_firstD=MEM_BORDER;
   
   for (int i=0;i<LOOPS;++i) 
      var[(rand()%DIMENSION)*SPARSE]+=rand();
   times(&tms);
   clock_t loop=tms.tms_utime;
//   getrusage(RUSAGE_SELF,&rusage);
//   long mem_loop=rusage.ru_idrss;
   char *mem_loop=MEM_BORDER;
   
   std::cout << "what=" << WHAT << " dimension=" << DIMENSION << " sparse=" << SPARSE << " loops=" << LOOPS << '\n';
   std::cout << "creation\t\t" << (mem_creation-mem_start)/1024 << "kb\n";
   std::cout << "population\t" << (firstD-start) <<"ticks\t" << (mem_firstD-mem_creation)/1024 << "kb\n";
   std::cout << "loop\t\t" << (loop-firstD) <<"ticks\t" << (mem_loop-mem_firstD)/1024 << "kb\n";
   
#if WHAT==0
   // free(var);
#else
   // create a memory leak to measure memory footprint
   new type(var);
#endif   
}
