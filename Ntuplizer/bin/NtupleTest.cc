#include<vector>
#include <memory>

#include "Analysis/Ntuplizer/interface/TreeFactory.h"



int main(int argc, char * argv[])
{
   MuonsTreeFactory myMuonFactory;
   Tree * myMuons = myMuonFactory.requestTree();
   myMuons -> info();

//std::cout << " -----" << std::endl;

   JetsTreeFactory myJetFactory;
   Tree * myJets = myJetFactory.requestTree();
   myJets -> info();
   
   
}
