// -*- C++ -*-
//
// Package:    EDAnalyzer2D/Phi_Eta_Analyzer
// Class:      Phi_Eta_Analyzer
//
/**\class Phi_Eta_Analyzer Phi_Eta_Analyzer.cc EDAnalyzer2D/Phi_Eta_Analyzer/plugins/Phi_Eta_Analyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Greyson Nikolai Newton
//         Created:  Fri, 14 Jun 2019 22:32:43 GMT
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
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1D.h"
#include "TH2F.h"
#include <iostream>
#include <iomanip>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using pat::MuonCollection;

class Phi_Eta_Analyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit Phi_Eta_Analyzer(const edm::ParameterSet&);
      ~Phi_Eta_Analyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
    
      // ----------member data ---------------------------
      edm::EDGetTokenT<pat::MuonCollection> muonToken_;
      unsigned int minTracks;
      TH1D *th1d_tight_muon;
      TH2D *th2d_phi_eta;
      float nAllEvents;
      float nGlobal;
      float nStandAlone;
      float nPT;
      float nMatchedStations;
      float nChi2;
      float nDB;
      float nTightMuon;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
Phi_Eta_Analyzer::Phi_Eta_Analyzer(const edm::ParameterSet& iConfig)
 :
  muonToken_(consumes<pat::MuonCollection>(edm::InputTag("slimmedMuons")))


{
   edm::Service<TFileService> fs;
   th1d_tight_muon = fs->make<TH1D>("TightMuons", "Tight Muons", 7  , 0 , 7);
   th2d_phi_eta = fs->make<TH2D>("phi_eta_", "Tight Muons;Phi;Eta", 1, -3.14, 3.14, 1, -3.14, 3.14);
   th1d_tight_muon->GetXaxis()->SetBinLabel(1, "Global");
   th1d_tight_muon->GetXaxis()->SetBinLabel(2, "StandAlone");
   th1d_tight_muon->GetXaxis()->SetBinLabel(3, "PT");
   th1d_tight_muon->GetXaxis()->SetBinLabel(4, "Stations");
   th1d_tight_muon->GetXaxis()->SetBinLabel(5, "ValidDB");
   th1d_tight_muon->GetXaxis()->SetBinLabel(6, "Chi2");
// now do what ever initialization is needed
    nAllEvents = 0.0f;
    nGlobal = 0.0f;
    nStandAlone = 0.0f;
    nPT = 0.0f;
    nMatchedStations = 0.0f;
    nChi2 = 0.0f;
    nDB = 0.0f;
    nTightMuon = 0.0f;
}


Phi_Eta_Analyzer::~Phi_Eta_Analyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

   void
Phi_Eta_Analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using pat::MuonCollection;
   edm::Handle<pat::MuonCollection> muons;
   iEvent.getByToken(muonToken_, muons);
   for (const pat::Muon &mu : *muons) {
	nAllEvents++;
/*	
	if(mu.isTightMuon())
	{	
		th1d_tight_muon->Fill(5.5);
		nTightMuon++;
	}
*/
	if(mu.isGlobalMuon())
	{
		continue;
	}		
		th1d_tight_muon->Fill(0.5);
		nGlobal++;
		th2d_phi_eta->SetFillColor(1);
		th2d_phi_eta->Fill(mu.phi(), mu.eta());;

	if(mu.isStandAloneMuon())
	{
		continue;
	}
		th1d_tight_muon->Fill(1.5);
		nStandAlone++;
		th2d_phi_eta->SetFillColor(2);
		th2d_phi_eta->Fill(mu.phi(), mu.eta());

	if((mu.pt()>20) && (mu.pt()<200))
	{
		continue;}
		th1d_tight_muon->Fill(2.5);
		nPT++;
		th2d_phi_eta->SetFillColor(3);
		th2d_phi_eta->Fill(mu.phi(), mu.eta());
	if(mu.numberOfMatchedStations() >= 2)
	{
		continue;
	}
		th1d_tight_muon->Fill(3.5);
		nMatchedStations++;
		th2d_phi_eta->SetFillColor(4);
		th2d_phi_eta->Fill(mu.phi(), mu.eta());

        if(mu.dB() < 0.2)
        {
                continue;
        }

                th1d_tight_muon->Fill(4.5);
                nDB++;
		th2d_phi_eta->SetFillColor(5);
		th2d_phi_eta->Fill(mu.phi(), mu.eta());
/*	if(mu.globalTrack()->normalizedChi2() < 10)
	{
		continue;
	}
		th1d_tight_muon->Fill(5.5);
		nChi2++;
*/
 
} 
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}
// ------------ method called once each job just before starting event loop  ------------
void
Phi_Eta_Analyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
Phi_Eta_Analyzer::endJob()
{
   std::cout << std::fixed;
   std::cout << std::setprecision(2);
   std::cout << "All Events " << nAllEvents << std::endl;
   std::cout << "Muon Conditions: " << std::endl; 
   std::cout << "Global             " << nGlobal << " | "<< ((nGlobal/nAllEvents)*100) << "%" << std::endl; 
   std::cout << "Stand Alone        "<< nStandAlone << " | "<< ((nStandAlone/nAllEvents)*100) << "%" << std::endl; 
   std::cout << "Tranverse Momentum " << nPT  << " | "<< ((nPT/nAllEvents)*100) << "%" << std::endl;
   std::cout << "Matched Stations   " << nMatchedStations << " | "<< ((nMatchedStations/nAllEvents)*100) << "%" << std::endl; 
   std::cout << "Tranverse Impact   " << nDB << " | "<< ((nDB/nAllEvents)*100) << "%" << std::endl; 
   std::cout << "Chi2               " << nChi2 << " | "<< ((nChi2/nAllEvents)*100) << "%" << std::endl << std::endl; 
// std::cout << "Tight Muons        " << nTightMuon << " | " << ((nTightMuon/nAllEvents)*100) << "%" << std::endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Phi_Eta_Analyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Phi_Eta_Analyzer);
