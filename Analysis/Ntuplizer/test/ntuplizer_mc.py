import FWCore.ParameterSet.Config as cms

process = cms.Process("MssmHbb")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '76X_mcRun2_asymptotic_v12')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

output_file = 'ntuple.root'

## TFileService
process.TFileService = cms.Service("TFileService",
	fileName = cms.string(output_file)
)

## ============ TRIGGER FILTER ===============
## Enable below at cms.Path if needed 
process.triggerSelection = cms.EDFilter( "TriggerResultsFilter",
    triggerConditions = cms.vstring(
#                          "HLT_ZeroBias_v*",
                          "HLT_BTagMu_DiJet20_Mu5_v*",
    ),
    hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
    l1tResults = cms.InputTag( "" ),
    l1tIgnoreMask = cms.bool( False ),
    l1techIgnorePrescales = cms.bool( False ),
    daqPartitions = cms.uint32( 1 ),
    throw = cms.bool( True )
)


## ============ EVENT FILTER COUNTER ===============
## Filter counter (maybe more useful for MC)
process.TotalEvents    = cms.EDProducer("EventCountProducer")
process.FilteredEvents = cms.EDProducer("EventCountProducer")

## ============ PRIMARY VERTEX FILTER ===============
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlineSlimmedPrimaryVertices"), # primary vertex collection name
   cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"), # ndof>thr=4 corresponds to sum(track_weigths) > (thr+3)/2 = 3.5 so typically 4 good tracks
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)

## ============  THE NTUPLIZER!!!  ===============
process.MssmHbb     = cms.EDAnalyzer("Ntuplizer",
    MonteCarlo      = cms.bool(True),
    CrossSection    = cms.double(1),  # in pb
    UseFullName     = cms.bool(False),
    ## Monte Carlo only
    GenFilterInfo   = cms.InputTag("genFilterEfficiencyProducer"),
    GenRunInfo      = cms.InputTag("generator"),
    GenJets         = cms.VInputTag(cms.InputTag("slimmedGenJets")),
    GenParticles    = cms.VInputTag(cms.InputTag("prunedGenParticles")),
    ###################
    TotalEvents     = cms.InputTag("TotalEvents"),
    FilteredEvents  = cms.InputTag("FilteredEvents"),
    PatJets         = cms.VInputTag(
                                    cms.InputTag("slimmedJetsPuppi","","PAT"),
                                    cms.InputTag("slimmedJetsAK8PFCHSSoftDropPacked","SubJets","PAT")
                                    ), 
    PatMETs         = cms.VInputTag(
                                    cms.InputTag("slimmedMETs","","PAT"),
                                    cms.InputTag("slimmedMETsPuppi","","PAT")
                                    ), 
    PatMuons        = cms.VInputTag(
                                    cms.InputTag("slimmedMuons","","PAT")
                                    ), 
    PrimaryVertices = cms.VInputTag(
                                    cms.InputTag("offlineSlimmedPrimaryVertices","","PAT")
                                    ), 
    BTagAlgorithms = cms.vstring   (
                                    "pfCombinedInclusiveSecondaryVertexV2BJetTags",
#                                     "combinedSecondaryVertexBJetTags",
#                                     "pfJetBProbabilityBJetTags",
#                                     "pfJetProbabilityBJetTags",
#                                     "pfTrackCountingHighPurBJetTags",
#                                     "pfTrackCountingHighEffBJetTags",
#                                     "pfSimpleSecondaryVertexHighEffBJetTags",
#                                     "pfSimpleSecondaryVertexHighPurBJetTags",
#                                     "pfCombinedSecondaryVertexV2BJetTags",
#                                     "pfCombinedSecondaryVertexSoftLeptonBJetTags",
#                                     "pfCombinedMVABJetTags",
                                   ),
    BTagAlgorithmsAlias = cms.vstring   (
                                         "btag_csvivf",
#                                          "btag_csv",
#                                          "btag_jetbprob",
#                                          "btag_jetprob",
#                                          "btag_tchp",
#                                          "btag_tche",
#                                          "btag_svhe",
#                                          "btag_svhp",
#                                          "btag_csvv2",
#                                          "btag_csvlep",
#                                          "btag_csvmva",
                                        ),
    TriggerResults  = cms.VInputTag(cms.InputTag("TriggerResults","","HLT")),
    TriggerPaths    = cms.vstring  (
    ## I recommend using the version number explicitly to be able to compare 
    ## however for production one has to be careful that all versions are included.
    ## Thinking of a better solution...
    											"HLT_BTagMu_DiJet20_Mu5_v",
    											"HLT_DoubleJetsC100_DoubleBTagCSV0p9_DoublePFJetsC100MaxDeta1p6_v",
    											"HLT_DoubleJetsC100_DoubleBTagCSV0p85_DoublePFJetsC160_v",
                                   ),
    TriggerObjectStandAlone  = cms.VInputTag(
                                             cms.InputTag("selectedPatTrigger","","PAT"),
                                             ),
    TriggerObjectLabels    = cms.vstring  (
                                    "hltL1sL1Mu3JetC16WdEtaPhi2",
                                    "hltBDiJet20L1FastJetCentral",
                                    "hltBSoftMuonDiJet20L1FastJetMu5L3FilterByDR",
                                   ),
    L1ExtraJets     = cms.VInputTag(
                                    cms.InputTag("l1extraParticles","Central","RECO"),
#                                    cms.InputTag("l1extraParticles","Forward","RECO"),
#                                    cms.InputTag("l1extraParticles","Tau","RECO")
                                    ),
    L1ExtraMuons    = cms.VInputTag(
                                    cms.InputTag("l1extraParticles","","RECO")
                                    ),
)

process.p = cms.Path(
                      process.TotalEvents *
#                      process.triggerSelection *
                      process.primaryVertexFilter *
                      process.FilteredEvents *
                      process.MssmHbb
                    )


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
#       '/store/mc/RunIISpring15DR74/SUSYGluGluToBBHToBB_M-300_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/50000/38C86330-3F13-E511-86E6-002590A37106.root',
       '/store/mc/RunIIFall15MiniAODv2/SUSYGluGluToBBHToBB_M-200_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/0E7138F8-EBB8-E511-BCCE-3417EBE64CD5.root',
] );


secFiles.extend( [
               ] )

