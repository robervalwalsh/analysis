import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)

##  Using MINIAOD. GlobalTag just in case jet re-clustering, L1 trigger filter  etc is needed to be done
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '76X_dataRun2_v15')
#process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '80X_dataRun2_Prompt_v9')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
for pset in process.GlobalTag.toGet.value():
    pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
## fix for multi-run processing
process.GlobalTag.RefreshEachRun = cms.untracked.bool( False )
process.GlobalTag.ReconnectEachRun = cms.untracked.bool( False )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.demo = cms.EDAnalyzer('TriggerInfoAnalyzer',
    TriggerResults  = cms.InputTag("TriggerResults","","HLT")
)


process.p = cms.Path(process.demo)


readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)

readFiles.extend( [
       '/store/data/Run2015D/BTagCSV/MINIAOD/16Dec2015-v1/50000/A6D829FA-F6AB-E511-BC84-0090FAA581F4.root',
#       '/store/data/Run2016C/BTagCSV/MINIAOD/PromptReco-v2/000/275/659/00000/125192CF-A33B-E611-9A83-02163E011E37.root',
#       '/store/data/Run2016B/BTagCSV/MINIAOD/PromptReco-v2/000/275/376/00000/069D7EC6-AF39-E611-95F1-02163E01261C.root'
] );


secFiles.extend( [
               ] )
