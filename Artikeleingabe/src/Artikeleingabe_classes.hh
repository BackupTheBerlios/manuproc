#ifndef _ARTIKELEINGABE_CLASSES_HH
#  define _ARTIKELEINGABE_CLASSES_HH

#include <tclistnode.h>

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
         artbase = (dynamic_cast<const Data_tree &>(*rd)).Artikel();
         artbase2 = (dynamic_cast<const Data_tree &>(*rd)).Artikel2();
      }
    ArtikelBase Artikel() const {return artbase;}
    ArtikelBase Artikel2() const {return artbase2;}

    Data_Node::Data_Node(guint deep,const cH_EntryValue &v, guint child_s_deep,  const Handle<const TreeRow> const TreeRow &suminitsuminit)
        :TreeRow(deep,v,child_s_deep,child_s_data,expand) 
    {  if (suminit.Leaf()) cumulate(child_s_data);
       else 
       {  artbase=dynamic_cast<const Data_Node&>(suminit).artbase;
          artbase2=dynamic_cast<const Data_Node&>(suminit).artbase2;
       }
    }

    static TreeRow *create(  const Handle<const TreeRow> const TreeRow &suminitsuminit)
       {  return new Data_Node(col,v,child_s_deep,child_s_data,expand,suminit);
       }
};
#endif
