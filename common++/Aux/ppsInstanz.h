// $Id: ppsInstanz.h,v 1.4 2001/06/29 09:25:49 cvs_malte Exp $
/*  libcommonc++: ManuProC's main OO library
 *  Copyright (C) 1998-2000 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#ifndef PPSINSTANZCLASS
#define PPSINSTANZCLASS

class ppsInstanz
{
public:
 typedef enum {INST_KNDAUF=1,INST_FAERB,INST_DRUCK,INST_WEB,
        INST_SCHAER,INST_SPRITZ,INST_GARNEINKAUF,INST_BANDLAGER,
        INST_ROHLAGER} ppsInstId;
 
private: 
 ppsInstId instid; 
 
public:
 ppsInstanz(ppsInstId iid) : instid(iid) {}
 ppsInstanz() : instid((ppsInstId)0) {}

 ppsInstId Id() const { return instid; }
 
 operator ppsInstId () const {return instid;}
 
  
};

#endif
 