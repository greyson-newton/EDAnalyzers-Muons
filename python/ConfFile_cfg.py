import FWCore.ParameterSet.Config as cms

process = cms.Process("GitTest")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('GitTest')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            'file:/afs/cern.ch/work/r/rymuelle/public/EDAnalyzerMuons/test_miniAOD.root'
                )
                            )

process.demo = cms.EDAnalyzer('Phi_Eta_Analyzer',
        muons = cms.InputTag("slimmedMuons","", "RECO")
 )

process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('TightMuonProfile.root')
                                   )
process.p = cms.Path(process.demo)
