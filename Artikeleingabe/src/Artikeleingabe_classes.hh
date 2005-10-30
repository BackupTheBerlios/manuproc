// $Id: Artikeleingabe_classes.hh,v 1.9 2005/10/30 01:00:42 christof Exp $
/*  Artikeleingabe: ManuProC's article management program
 *  Copyright (C) 2004 Adolf Petig GmbH & Co. KG
 *  written by Christof Petig
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

#ifndef _ARTIKELEINGABE_CLASSES_HH
#  define _ARTIKELEINGABE_CLASSES_HH

#include <TreeRow.h>

class Data_tree : public RowDataBase
{
   static const unsigned int nachkommastellen=5;
   typedef fixedpoint<nachkommastellen> menge_t;
   typedef std::pair<cH_ArtikelBezeichnung,menge_t > zeile_t;
   typedef std::vector<zeile_t > vec_zeile_t; 
   vec_zeile_t vec_zeile;

  public : 
   Data_tree(vec_zeile_t _vec_zeile) 
      : vec_zeile(_vec_zeile) {}

   virtual cH_EntryValue Value(guint seqnr,gpointer gp) const;

  ArtikelBase Artikel()  const; //  {return ArtikelBase(vec_zeile[0].first->Id()) ;}
  ArtikelBase Artikel2() const; //  {
/*     if (vec_zeile.size()>1) return ArtikelBase(vec_zeile[1].first->Id()) ;
     else return ArtikelBase();
   } */
};

typedef Handle<const Data_tree> cH_Data_tree;

class Data_Node : public TreeRow
{
    ArtikelBase artbase,artbase2;
 public:
    virtual void cumulate(const cH_RowDataBase &rd)
      {
         artbase = rd.cast_dynamic<const Data_tree>()->Artikel();
         artbase2 =  rd.cast_dynamic<const Data_tree>()->Artikel2();
      }
    virtual void deduct(const cH_RowDataBase &rd)
      {
      }
    virtual cH_EntryValue Value(unsigned int, void*) const
    {  return cH_EntryValue();
    }
    ArtikelBase Artikel() const {return artbase;}
    ArtikelBase Artikel2() const {return artbase2;}

    Data_Node::Data_Node(const Handle<const TreeRow> &suminit)
//        :TreeRow(deep,v,child_s_deep,expand)
    {  if (suminit) // cumulate(suminit.cast_dynamic<const RowDataBase>());
#if 0    
       else 
#endif       
       {  artbase=suminit.cast_dynamic<const Data_Node>()->artbase;
          artbase2=suminit.cast_dynamic<const Data_Node>()->artbase2;
       }
//#endif       
    }

    static Handle<TreeRow> create(const Handle<const TreeRow> &suminit)
       {  return new Data_Node(suminit);
       }
};
#endif
