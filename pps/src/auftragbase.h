/*  pps: ManuProC's ProductionPlanningSystem
 *  Copyright (C) 2001 Adolf Petig GmbH & Co. KG, written by Jacek Jakubowski
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

#include<Auftrag/auftrag_status.h>

#ifndef AUFTRAGBASE
#define AUFTRAGBASE
#include"MyMessage.h"

#define ART_SEQ		1
#define BR_SEQ		2
#define FB_SEQ		3
#define AUFM_SEQ	4
#define KW_SEQ		5
#define AUFID_SEQ	6
#define KNDNR_SEQ	7
#define PROZ_SEQ	8

#define ATTRCOUNT	8


#define LOADED		0
#define INSERTED	1

#define STKCOL		4
#define UEBERCOL	8

/* Stati für Aufträge und Auftr.Einträge */
#define OFFEN	"offen"
#define FERTIG	"fertig"
#define STORN	"storno"


typedef enum {SEL_STATUS=1} SelType;

extern MyMessage *meldung;

#define MAPKEY(_aid,_znr)       ((_aid)* 10000 + (_znr))

#endif
