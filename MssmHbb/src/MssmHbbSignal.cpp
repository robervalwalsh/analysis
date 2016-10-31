/*
 * MssmHbbSignal.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: shevchen
 */

#include "Analysis/MssmHbb/interface/MssmHbbSignal.h"

using namespace analysis;
using namespace analysis::tools;
using namespace analysis::mssmhbb;

std::string constructTemplateName(const std::string & name);

MssmHbbSignal::MssmHbbSignal(const std::string & inputFilelist,const double & dataLumi, const bool & lowM, const bool & test) :
								selectionDoubleB(inputFilelist,dataLumi,lowM,test)
{
	nJets_ = 3;
        if(lowM){
                triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v";
                triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoublePFJetsC100","hltDoubleBTagCSV0p9","hltDoublePFJetsC100MaxDeta1p6"};
                pt1_ = 100.; pt2_ = 100.; pt3_ = 40;
                eta1_ = 2.2; eta2_ = 2.2; eta3_ = 2.2;
                btag1_ = 0.8; btag2_ = 0.8; btag3_ = 0.8;
                btagOP1_ = 1; btagOP2_ = 1; btagOP3_ = 1;
                dR_ = 1; dEta_ = 1.55; mHat_ = 0.7;
        }
        else {
                triggerLogicName_ = "HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v";
                triggerObjectName_ = {"hltL1sL1DoubleJetC100","hltDoubleJetsC100","hltDoubleBTagCSV0p85","hltDoublePFJetsC160"};
                pt1_ = 160.; pt2_ = 160.; pt3_ = 40;
                eta1_ = 2.2; eta2_ = 2.2; eta3_ = 2.2;
                btag1_ = 0.8; btag2_ = 0.8; btag3_ = 0.8;
                btagOP1_ = 1; btagOP2_ = 1; btagOP3_ = 1;
                dR_ = 1; dEta_ = 100.; mHat_ = 0.7;
        }
	baseOutputName_ = "MssmHbbSignal";
}

MssmHbbSignal::~MssmHbbSignal() {
}

//Member methods

const bool MssmHbbSignal::leadingJetSelection(const std::shared_ptr<tools::Collection<tools::Jet> > & offlineJets){

	//To avoid code duplication frind class were used
	if(!selectionDoubleB::leadingJetSelection(offlineJets)) return false;

//	if(TEST) std::cout<<"I'm in MssmHbbSignal::leadingJetSelection"<<std::endl;

	//Selection of good Leading Jets:
	//Only jets that pass Loose identification will be considered
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);
	Jet jet3 = offlineJets->at(2);



	//Eta requirements


	//deltaR requirements


	//Pt requirements
	if(!cuts_.check("pt3",jet3.pt() >= pt3_)) return false;

	//Eta requirements
	if(!cuts_.check("eta3",std::abs(jet3.eta()) <= eta3_)) return false;

	//BTag requirements
	if(!cuts_.check("btag3",jet3.btag() >= btag3_)) return false;

	//deltaR requirements
	if(!cuts_.check("dR23",jet2.deltaR(jet3) > dR_)) return false;
	if(!cuts_.check("dR13",jet1.deltaR(jet3) > dR_)) return false;

	return true;
}

void MssmHbbSignal::fillHistograms(const std::shared_ptr<Collection<Jet> > &offlineJets, const double & weight){

//	if(TEST) std::cout<<"I'm in MssmHbbSignal::fillHistograms"<<std::endl;
	Jet jet1 = offlineJets->at(0);
	Jet jet2 = offlineJets->at(1);
	Jet jet3 = offlineJets->at(2);
	TLorentzVector obj12;
	obj12 = jet1.p4() + jet2.p4();

	if(JESshift_ == 0 && JERshift_ == 0){
//		std::cout<<"JERshift: "<<JERshift_<<" Match: "<<jet1.matched(genJets_->name())<<" sf: "<<jet1.JerSf()<<" pt: "<<jet1.pt()<<std::endl;
		selectionDoubleB::fillHistograms(offlineJets,weight);

		(histo_.getHisto())["data_Mbb_230"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["data_Mbb_220"]->Fill(obj12.M(),weight);

		(histo_.getHisto())["jet_pt3"]->Fill(jet3.pt(),weight);
		(histo_.getHisto())["jet_eta3"]->Fill(jet3.eta(),weight);
		(histo_.getHisto())["jet_phi3"]->Fill(jet3.phi(),weight);
		(histo_.getHisto())["jet_btag_csv3"]->Fill(jet3.btag(),weight);
		(histo_.getHisto())["jet_btag_cmva3"]->Fill(jet3.btag("btag_csvmva"),weight);
		(histo_.getHisto())["jet_dR13"]->Fill(jet1.deltaR(jet3),weight);
		(histo_.getHisto())["jet_dR23"]->Fill(jet2.deltaR(jet3),weight);

		if(signalMC_){
			(histo_.getHisto())["template_Mbb"]->Fill(obj12.M(),weight);
			(histo_.getHisto())["template_SFb_up"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_up"]);
			(histo_.getHisto())["template_SFb_down"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_down"]);

			(histo_.getHisto())["template_SFl_up"]->Fill(obj12.M(),weight/weight_["SFl_central"] * weight_["SFl_up"]);
			(histo_.getHisto())["template_SFl_down"]->Fill(obj12.M(),weight/weight_["SFl_central"] * weight_["SFl_down"]);

			(histo_.getHisto())["template_PU_up"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_up"]);
			(histo_.getHisto())["template_PU_down"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_down"]);

			(histo_.getHisto())["template_PtEff_up"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_up"]);
			(histo_.getHisto())["template_PtEff_down"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_down"]);

			//visualisation
			(histo_.getHisto())["template_Mbb_VIS"]->Fill(obj12.M(),weight);
			(histo_.getHisto())["template_SFb_VIS_up"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_up"]);
			(histo_.getHisto())["template_SFb_VIS_down"]->Fill(obj12.M(),weight/weight_["SFb_central"] * weight_["SFb_down"]);

			(histo_.getHisto())["template_SFl_VIS_up"]->Fill(obj12.M(),weight/weight_["SFl_central"] * weight_["SFl_up"]);
			(histo_.getHisto())["template_SFl_VIS_down"]->Fill(obj12.M(),weight/weight_["SFl_central"] * weight_["SFl_down"]);

			(histo_.getHisto())["template_PU_VIS_up"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_up"]);
			(histo_.getHisto())["template_PU_VIS_down"]->Fill(obj12.M(),weight/weight_["PU_central"] * weight_["PU_down"]);

			(histo_.getHisto())["template_PtEff_VIS_up"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_up"]);
			(histo_.getHisto())["template_PtEff_VIS_down"]->Fill(obj12.M(),weight/weight_["PtEff_central"] * weight_["PtEff_down"]);
		}
	}
	else if (JESshift_ < 0 && JERshift_ == 0){
		(histo_.getHisto())["template_JES_down"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["template_JES_VIS_down"]->Fill(obj12.M(),weight);
	}
	else if (JESshift_ > 0 && JERshift_ == 0){
		(histo_.getHisto())["template_JES_up"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["template_JES_VIS_up"]->Fill(obj12.M(),weight);
	}
	else if (JERshift_ > 0 && JESshift_ == 0){
		(histo_.getHisto())["template_JER_up"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["template_JER_VIS_up"]->Fill(obj12.M(),weight);
//		std::cout<<"JERshift: "<<JERshift_<<" Match: "<<jet1.matched(genJets_->name())<<" sf: "<<jet1.JerSfUp()<<" pt: "<<jet1.pt()<<std::endl;
	}
	else if (JERshift_ < 0 && JESshift_ == 0){
		(histo_.getHisto())["template_JER_down"]->Fill(obj12.M(),weight);
		(histo_.getHisto())["template_JER_VIS_down"]->Fill(obj12.M(),weight);
//		std::cout<<"JERshift: "<<JERshift_<<" Match: "<<jet1.matched(genJets_->name())<<" sf: "<<jet1.JerSfDown()<<" pt: "<<jet1.pt()<<std::endl;
	}
//	if(TEST) std::cout<<"I'm out of MssmHbbSignal::fillHistograms"<<std::endl;
}

void MssmHbbSignal::writeHistograms(){
	std::string full_name;

	outputFile_->mkdir("distributions","general distributions of Jets and di-Jet objects");
	outputFile_->mkdir("templates","templates with a full range");
	for(const auto & h: (histo_.getHisto())){
		if(h.second->GetEntries() == 0) continue; 			//skip empty histograms
		if(isMC()) h.second->Scale(weight_["Lumi"]);
		if(h.first.find("template") != std::string::npos){
			full_name = constructTemplateName(h.first);
			if(h.first.find("VIS") != std::string::npos) outputFile_->cd("templates");
			h.second->Write(full_name.c_str());
			outputFile_->cd("");
		}
		else {
			outputFile_->cd("distributions");
			h.second->Write();
			outputFile_->cd("");
		}
	}
//	outputFile_->Close();
}

const double MssmHbbSignal::assignWeight(){
double weight = 1;
	if(isMC()) {
		weight = weight_["PtEff_central"] * weight_["PU_central"] * weight_["SFb_central"] * weight_["SFl_central"];// * weight_["Signal_Shape"];
	}
	//std::cout<<" weight = "<<weight<<" "<<weight_["dEta"]<<" "<<weight_["Lumi"]<<" "<<weight_["2DPt"]<<" "<<weight_["BTag"]<<" "<<weight_["PU_central"]<<" "<<weight_["SFb_central"]<<" "<<weight_["SFl_central"]<<std::endl;
	return weight;
}

std::string constructTemplateName(const std::string & name){
	std::string process = "bbH";
	std::string channel	= "Mbb";
	std::string energy	= "13TeV";
	std::string collaboration = "CMS";
	if(name.find("template_") == std::string::npos) {
		std::cerr<<"Wrong input at constructTemplateName finction. Interrupt"<<std::endl;
		exit(1);
	}
	std::string full_name = name.std::string::substr(name.find("template_") + std::string("template_").std::string::length());
	if(name.find("_up") != std::string::npos){
		full_name = full_name.substr(0,full_name.size() - 3);
		full_name = process + "_" + channel + "_" + collaboration + "_" + full_name + "_" + energy + "Up";
	}
	else if(name.find("_down") != std::string::npos){
		full_name = full_name.substr(0,full_name.size() - 5);
		full_name = process + "_" + channel + "_" + collaboration + "_" + full_name + "_" + energy + "Down";
	}
	else if(name.find("Mbb") != std::string::npos){
		full_name = process + "_" + full_name;
	}

	return full_name;
}

std::shared_ptr<tools::Collection<tools::Jet> > MssmHbbSignal::modifyJetCollection(tools::Jet & jet,
						std::shared_ptr<tools::Collection<tools::Jet> > & initialJets
						){
	Jet shiftedJet = jet;
	TLorentzVector p4Shift = jet.p4() * (1 + 2.* JESshift_ * jet.jecUncert());
	jet.p4(p4Shift);
	initialJets->add(jet);
	return initialJets;
}

void MssmHbbSignal::runAnalysis(const std::string &json, const std::string &output, const int &size){

	this->SetupStandardOutputFile(output);
	this->setupAnalysis(json);
	std::cout<<"Total number of events: "<<this->size()<<std::endl;
	this->makeHistograms(size);

	if(signalMC_){
		for(int i = 0; i < 3 ; ++i){
			JERshift_ = 0;
			if(i==0) JESshift_ = 0;
			else if (i == 1){
				JESshift_ = 1;
			}
			else {
				JESshift_ = - 1;
			}

			this->applySelection();
		}

		for(int i = 1; i < 3; ++i){
			JESshift_ = 0;
			if (i == 1){
				JERshift_ = 2;
			}
			else {
				JERshift_ = -2;
			}
				this->applySelection();

		}
	}
	else{
		JERshift_ = 0;
		JESshift_ = 0;
		this->applySelection();
	}
	this->writeHistograms();


}

void MssmHbbSignal::addStatErrorsTemplates(const double & relative_threshold){

	if(relative_threshold < 0 || relative_threshold > 1){
		std::cerr<<"Exception in MssmHbbSignal::addStatErrorsTemplates - threshold should be >= 0 && <= 1"<<std::endl;
	}

}

void MssmHbbSignal::addStatErrorsTemplates(const int & nbins){

	if((histo_.getHisto())["template_Mbb"] == nullptr) {
		std::cerr<<"No template_Mbb histogram. Should create it first"<<std::endl;
		exit(-1);
	}

	int nTot = (histo_.getHisto())["template_Mbb"]->GetNbinsX();

	if(nbins < 0 || nbins > nTot){
		std::cerr<<"Exception in MssmHbbSignal::addStatErrorsTemplates - number of bins should be >= 0 && <= nMax"<<std::endl;
		exit(-1);
	}
	//vector that contains bin errors
	std::vector<std::pair<int, double> > bin_errors;
	for (int i=1;i<=nTot;++i){
		std::pair<int,double> pr = std::make_pair(i,(histo_.getHisto())["template_Mbb"]->GetBinError(i));
		bin_errors.push_back(pr);
	}
	//sort vector with default i<j to find highest
	std::sort(bin_errors.begin(),bin_errors.end(),great_second<int,double>());

	for(int i=0;i<nbins;++i){

		outputFile_->cd("");
		//name of the future histogram
		std::string name = "template_Bin"+std::to_string(i+1)+"_up";
		//Create a new histogram that will contain highest stat. error
		TH1D *h_up = (TH1D*) (histo_.getHisto())["template_Mbb"] -> Clone(name.c_str());
		h_up->SetMarkerStyle(20);
		std::pair<int,double> pbin(bin_errors[i]);
		h_up->SetBinContent(pbin.first,h_up->GetBinContent(pbin.first)+pbin.second);
		h_up->SetBinError(pbin.first,0.);
		name = constructTemplateName(name);
		h_up->Write(name.c_str());


		name = "template_Bin"+std::to_string(i+1)+"_down";
		TH1D *h_down = (TH1D*) (histo_.getHisto())["template_Mbb"] -> Clone(name.c_str());
		h_down->SetBinContent(pbin.first,h_down->GetBinContent(pbin.first)-pbin.second);
		h_down->SetBinError(pbin.first,0.);
		h_down->SetMarkerStyle(20);
		name = constructTemplateName(name);
		h_down->Write(name.c_str());

	}

}

