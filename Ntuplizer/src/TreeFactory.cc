#include "Analysis/Ntuplizer/interface/TreeFactory.h"

TreeFactory::TreeFactory() : numOfTrees_(0)
{
}

Tree * TreeFactory::requestTree()
{
   numOfTrees_++;
   std::cout << "New tree created" << std::endl;
   return createTree();
}

int TreeFactory::getNumOfTrees() const
{
   return numOfTrees_;
}

Tree* JetsTreeFactory::createTree()
{
   return new JetsTree();
}

Tree* MuonsTreeFactory::createTree()
{
   return new MuonsTree();
}

