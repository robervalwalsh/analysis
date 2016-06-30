// -*- C++ -*-
//
// Package:    Analysis/Tests
// Class:      TriggerInfoAnalyzer
// 
/**\class TriggerInfoAnalyzer TriggerInfoAnalyzer.cc Analysis/Tests/plugins/TriggerInfoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Roberval Walsh
//         Created:  Wed, 29 Jun 2016 14:15:21 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

//
// class declaration
//

class TriggerInfoAnalyzer : public edm::EDAnalyzer  {
   public:
      explicit TriggerInfoAnalyzer(const edm::ParameterSet&);
      ~TriggerInfoAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
      edm::InputTag triggerResultsCollection_;
      HLTConfigProvider hltConfig_;
      edm::EDGetTokenT<edm::TriggerResults> triggerResultsTokens_;
      std::shared_ptr<HLTPrescaleProvider> hltPrescale_;

};

TriggerInfoAnalyzer::TriggerInfoAnalyzer(const edm::ParameterSet& config)
{
   triggerResultsCollection_ = config.getParameter<edm::InputTag>("TriggerResults");
   triggerResultsTokens_ = consumes<edm::TriggerResults>(triggerResultsCollection_);
   hltPrescale_ = std::shared_ptr<HLTPrescaleProvider>(new HLTPrescaleProvider(config, consumesCollector(), *this));;
}


TriggerInfoAnalyzer::~TriggerInfoAnalyzer()
{
}


// ------------ method called for each event  ------------
void TriggerInfoAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
   using namespace edm;

   std::cout << "Size of the HLT configuration = " << hltConfig_.size() << std::endl;
   std::cout << "Global Tag = " << hltConfig_.globalTag() << std::endl;
   for ( size_t j = 0 ; j < hltConfig_.size() ; ++j )
   {
//      if ( hltConfig_.triggerName(j) != "HLT_DoubleJetsC100_DoubleBTagCSV_p014_DoublePFJetsC100MaxDeta1p6_v2" ) continue;
      std::cout << "trigger: " << hltConfig_.triggerName(j) << std::endl;
      const std::pair<std::vector<std::pair<std::string,int> >,int> prescalesInDetail(hltPrescale_->prescaleValuesInDetail(event,setup,hltConfig_.triggerName(j)));      
      std::cout << " prescale HLT: " << prescalesInDetail.second << std::endl;
      for (unsigned int i=0; i<prescalesInDetail.first.size(); ++i)
      {
         std::cout << "     " << i << ":" << prescalesInDetail.first[i].first << "/" << prescalesInDetail.first[i].second << std::endl; ;
      }
   }

}


// ------------ method called once each job just before starting event loop  ------------
void TriggerInfoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerInfoAnalyzer::endJob() 
{
}

void TriggerInfoAnalyzer::beginRun(edm::Run const& run, edm::EventSetup const& setup)
{
   // Initialize HLTConfig every lumi block
   bool changed;
   hltPrescale_->init(run, setup, triggerResultsCollection_.process(), changed);
   hltConfig_ = hltPrescale_->hltConfigProvider();
   
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerInfoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerInfoAnalyzer);
