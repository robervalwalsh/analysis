#include "Tree.h"

class TreeFactory
{
   public:
      TreeFactory();
      Tree * requestTree();
      int getNumOfTrees() const;
   protected:
      virtual Tree* createTree() = 0;
   private:
      int numOfTrees_;   
};

class JetsTreeFactory : public TreeFactory
{
   public:
      virtual Tree* createTree();
};

class MuonsTreeFactory : public TreeFactory
{
   public:
      virtual Tree* createTree();
};

