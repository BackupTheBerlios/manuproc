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

   virtual const cH_EntryValue Value(guint seqnr,gpointer gp) const;

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
    virtual const cH_EntryValue Value(unsigned int, void*) const
    {  return cH_EntryValue();
    }
    ArtikelBase Artikel() const {return artbase;}
    ArtikelBase Artikel2() const {return artbase2;}

    Data_Node::Data_Node(const Handle<const TreeRow> &suminit)
//        :TreeRow(deep,v,child_s_deep,expand)
    {  if (suminit) cumulate(suminit.cast_dynamic<const RowDataBase>());
#if 0    
       else 
       {  artbase=dynamic_cast<const Data_Node&>(suminit).artbase;
          artbase2=dynamic_cast<const Data_Node&>(suminit).artbase2;
       }
#endif       
    }

    static TreeRow *create(const Handle<const TreeRow> &suminit)
       {  return new Data_Node(suminit);
       }
};
#endif
