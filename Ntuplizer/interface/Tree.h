#ifndef Analysis_Ntuplizer_Tree_h
#define Analysis_Ntuplizer_Tree_h 1

#include <iostream>

class Tree
{
   public:
         virtual void info() = 0;
};

class JetsTree : public Tree
{
   public:
      virtual void info()
      {
         std::cout << "That's the jet tree of the ntuple" << std::endl;
      }
};

class MuonsTree : public Tree
{
   public:
      virtual void info()
      {
         std::cout << "That's the muon tree of the ntuple" << std::endl;
      }
};



#endif



