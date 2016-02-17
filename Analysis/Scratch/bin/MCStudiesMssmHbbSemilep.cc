#include <string>
#include <iostream>
#include <vector>

#include "TFile.h" 
#include "TFileCollection.h"
#include "TChain.h"
#include "TH1.h" 

#include "Analysis/MssmHbb/interface/MssmHbb.h"

using namespace std;
using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;


// =============================================================================================   
int main(int argc, char * argv[])
{
   std::cout << "MCStudiesMssmHbbSemilep *** STARTED ***" << std::endl; 
   TH1::SetDefaultSumw2();  // proper treatment of errors when scaling histograms
   
   // Input files list
   std::string inputList = "rootFileList.txt";
   MssmHbb analysis(inputList);
   
   // Input collections
   analysis.addTree<Jet>         ("Jets"        , "MssmHbb/Events/slimmedJetsPuppi"              );
   analysis.addTree<Muon>        ("Muons"       , "MssmHbb/Events/slimmedMuons"                  );
   
   // Histograms
   std::map<std::string, TH1F*> h1_;
   
   // Jets
   h1_["ptb1"]   = new TH1F("ptb1"  ,"",200,0,1000);
   h1_["ptb2"]   = new TH1F("ptb2"  ,"",200,0,1000);
   h1_["ptb3"]   = new TH1F("ptb3"  ,"",200,0,1000);
   
   h1_["etab1"]  = new TH1F("etab1" ,"",50,0,2.5);
   h1_["etab2"]  = new TH1F("etab2" ,"",50,0,2.5);
   h1_["etab3"]  = new TH1F("etab3" ,"",50,0,2.5);
   
   h1_["m12"]    = new TH1F("m12"   ,"",100, 0,500);
   
   // Muon jets
   h1_["ptb1_mu15"]   = new TH1F("ptb1_mu15"  ,"",200,0,1000);
   h1_["ptb2_mu15"]   = new TH1F("ptb2_mu15"  ,"",200,0,1000);
   h1_["ptb3_mu15"]   = new TH1F("ptb3_mu15"  ,"",200,0,1000);
   
   h1_["etab1_mu15"]  = new TH1F("etab1_mu15" ,"",50,0,2.5);
   h1_["etab2_mu15"]  = new TH1F("etab2_mu15" ,"",50,0,2.5);
   h1_["etab3_mu15"]  = new TH1F("etab3_mu15" ,"",50,0,2.5);
   
   h1_["m12_mu15"]    = new TH1F("m12_mu15"   ,"",100, 0,500);
   
   h1_["ptmu1"]       = new TH1F("ptmu1","",200,0,200);
   h1_["ptmu2"]       = new TH1F("ptmu2","",200,0,200);
   
   // Analysis of events
   std::cout << "  Total number of events analysed: " << analysis.size() << std::endl; 
   for ( int i = 0 ; i < analysis.size() ; ++i )
   {
      analysis.event(i);
      
      // Jets
      auto jets = analysis.collection<Jet>("Jets");
      int njets = jets->size();
      
      // Muons
      auto muons = analysis.collection<Muon>("Muons");
      int nmuons = muons->size();
      
      analysis.match<Jet,Muon>("Jets","Muons",0.3);
      
      if ( njets < 3 || nmuons < 1 ) continue;
      
      // triple b jet pre-selection
      // 3 leading jets must have pt > 30 GeV and btag > 0.95
      Jet jet1 = jets->at(0);
      Jet jet2 = jets->at(1);
      Jet jet3 = jets->at(2);
      if ( jet1.p4().DeltaR(jet2.p4()) < 1. || jet1.p4().DeltaR(jet3.p4()) < 1. || jet2.p4().DeltaR(jet3.p4()) < 1. ) continue;
      if ( jet1.pt() < 50 || fabs(jet1.eta()) > 2.1 ) continue;
      if ( jet2.pt() < 40 || fabs(jet2.eta()) > 2.1 ) continue;
      if ( jet3.pt() < 20 || fabs(jet3.eta()) > 2.4) continue;
      if ( jet1.btag() < 0.95 || jet2.btag() < 0.95 || jet3.btag() < 0.95 ) continue;
      
      float m12 = (jet1.p4()+jet2.p4()).M();
      
      h1_["ptb1"]  -> Fill(jet1.pt());
      h1_["ptb2"]  -> Fill(jet2.pt());
      h1_["ptb3"]  -> Fill(jet3.pt());
      
      h1_["etab1"] -> Fill(fabs(jet1.eta()));
      h1_["etab2"] -> Fill(fabs(jet2.eta()));
      h1_["etab3"] -> Fill(fabs(jet3.eta()));
      
      h1_["m12"]   -> Fill(m12);
      
      // at least one muon jet with muon pt > 15
      bool muonInJet1 = false;
      bool muonInJet2 = false;
      // muon jet
      auto mujet1 = jet1.matched("Muons");
      auto mujet2 = jet2.matched("Muons");
      if ( mujet1 )
         if ( mujet1->pt() > 10. && fabs(mujet1->eta()) < 2.1 ) muonInJet1 = true;
      if ( mujet2 ) 
         if ( mujet2->pt() > 10. && fabs(mujet2->eta()) < 2.1 ) muonInJet2 = true;
      
//      if ( ! muonInJet1 ) continue;
//      if ( ! muonInJet2 ) {}
      if ( ! (muonInJet1 || muonInJet2 ) ) continue;
      
      if ( muonInJet1 ) 
         h1_["ptmu1"] -> Fill(mujet1->pt());
      if ( muonInJet2 ) 
         h1_["ptmu2"] -> Fill(mujet2->pt());
     
      h1_["ptb1_mu15"]  -> Fill(jet1.pt());
      h1_["ptb2_mu15"]  -> Fill(jet2.pt());
      h1_["ptb3_mu15"]  -> Fill(jet3.pt());
      
      h1_["etab1_mu15"] -> Fill(fabs(jet1.eta()));
      h1_["etab2_mu15"] -> Fill(fabs(jet2.eta()));
      h1_["etab3_mu15"] -> Fill(fabs(jet3.eta()));
      
      h1_["m12_mu15"]   -> Fill(m12);
      
   }
   
   TFile out("output_histograms.root","recreate");
   
   for ( auto & h : h1_ )
   {
      h.second -> Write();
   }
   
   out.Close();
   
      
//    
}

