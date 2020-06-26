//
//  UrqmdEventReader.hpp
//  CodeV3
//
//  Created by Claude Pruneau on 8/18/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#ifndef UrqmdEventReader_hpp
#define UrqmdEventReader_hpp

#include <stdio.h>
#include "TTree.h"
#include "TChain.h"
#include "UrQMDEvent.hpp"
#include "GeneratorConfiguration.hpp"
#include "EventGenerator.hpp"

using std::cout;
using std::endl;

class UrqmdEventReader : public EventGenerator
{
public:


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  UrqmdEventReader(Event * event,
      GeneratorConfiguration * configuration,
      TRandom * randomGenerator,
      bool debugLevel)
:
  EventGenerator  (event, configuration, randomGenerator, debugLevel),
  fChain(NULL),
  fCurrent(-1),
  nentries(0),
  nbytes(0),
  nb(0),
  jentry(0),
  anEvent(NULL),
  nleptons(0L)
{
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::CTOR(...) No ops" << endl;
}

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // DTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual ~UrqmdEventReader()
  {
    delete anEvent;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialize generator
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual void initialize()
  {
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::initialize() Started" << endl;


    event           ->reset();
    particleFactory ->reset();
    anEvent = new UrQMDEvent("event");



    TChain *chain = new TChain("UrQMD_event_tree"," ");
    chain->Add(generatorConfiguration->eventsSource.Data());
//    chain->Add("/local/victor/PROJECTSTWO/TWOPARTICLECORRELATIONS/URQMDGEN/DATA/QMD201712261756/UrQMD_generated.root");
//    chain->Add("/local/victor/PROJECTSTWO/TWOPARTICLECORRELATIONS/URQMDGEN/DATA/QMD201712261920/UrQMD_generated.root");

    Init(chain);
    jentry = 0;
    fCurrent = -1;

    nentries = fChain->GetEntries();
    cout << "-INFO- initialize() nentries: " << nentries << " from " << generatorConfiguration->eventsSource << endl;
    nbytes = 0;
    nb = 0;



    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::initialize() Completed" << endl;
  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Reset and Initialize the generator
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual void reset()
  {
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::reset() Started" << endl;
    event->reset();
    particleFactory->reset();
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::reset() Completed" << endl;

  }

  bool acceptPID(Particle p)
  {
    bool wrongPID = false;

    /* leptons check */
    wrongPID = (1010 < p.pid) && (p.pid < 1019);
    if (wrongPID) nleptons++;

    return !wrongPID;
  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Generate an event
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Event * generate()
  {
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::generate() Started" << endl;

    if (!fChain)
    {
      cout << "-FATAL- UrqmdEventReader::generate() no TChain available" << endl;
    }

    bool seekingEvent = true;
    while (seekingEvent)
    {
      // load another event from the root file/TTree
      Long64_t ientry = LoadTree(jentry);

      // returning a null point is an indication that
      // there are no more events in the file or stack of files.
      if (ientry < 0) return 0;

      nb = fChain->GetEntry(jentry++);   nbytes += nb;
      if (nb == 0) return 0;

      if(anEvent->GetImpactParameter() >  generatorConfiguration->impactParameterMinimum &&
          anEvent->GetImpactParameter() <= generatorConfiguration->impactParameterMaximum) seekingEvent = false;
    }

    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::generate() new event b: " << anEvent->GetImpactParameter() << ",   ntracks: " << anEvent->GetNtrack() << endl;

    //////////////
    double charge;

    event->reset();
    particleFactory->reset();
    int particleAccepted = 0;
    nleptons = 0L;
    
    //------------------- Randomising the phi --------------Starts
    float eventAngle=gRandom->Uniform(0,3.14);
    //cout<< "------------------> "<<eventAngle <<endl;
    //------------------- Randomising the phi --------------Ends
      
  
    // load particles from TTree and copy those that are selected into event
    Particle aParticle; /* we use a particle object to extract angular information for indexes extraction */
    Particle *particle; /* this is the actual particle which will be stored */
    for (int iPart1=0; iPart1 < anEvent->GetNtrack(); iPart1++)
    {
      UrQMDParticle *aUrQMDParticle = (UrQMDParticle *) anEvent->GetTracks()->At(iPart1);

      charge = aUrQMDParticle->GetCharge();
      if (charge==0) continue;

      double p_x = aUrQMDParticle->GetPx();
      double p_y = aUrQMDParticle->GetPy();
      double p_z = aUrQMDParticle->GetPz();
      double p_e = aUrQMDParticle->GetE();

      aParticle.pid = aUrQMDParticle->GetItyp();
      aParticle.charge = charge;
      aParticle.setPxPyPzE(p_x,p_y,p_z,p_e);
      aParticle.ixEtaPhi = -1;
      aParticle.ixYPhi = -1;

      if (aParticle.pt<generatorConfiguration->ptMinimum || aParticle.pt>generatorConfiguration->ptMaximum) continue;
      if (aParticle.y<generatorConfiguration->yMinimum || aParticle.y>generatorConfiguration->yMaximum) continue;
      if (aParticle.eta<generatorConfiguration->etaMinimum || aParticle.eta>generatorConfiguration->etaMaximum) continue;
      if (!acceptPID(aParticle)) continue;

      /* rotate according to the event plane angle */
      double phi_new = (aParticle.phi-eventAngle);
      double p_x_new= aParticle.pt*TMath::Cos(phi_new);
      double p_y_new= aParticle.pt*TMath::Sin(phi_new);
      aParticle.setPxPyPzE(p_x_new,p_y_new,p_z,p_e);

      particle = particleFactory->getObject();
      particle->Copy(aParticle);

      //particle->printProperties(cout);
      particleAccepted++;
    }
    event->nParticles = particleAccepted;

    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::generate() No of accepted Particles: "<<particleAccepted<<endl;
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::generate() No of rejected leptons: " << nleptons << endl;
    //event->printProperties(cout);
    if (debugLevel>0) cout << "-DEBUG- UrqmdEventReader::generate() event completed!" << endl;
    return event;
  }


  Int_t GetEntry(Long64_t entry)
  {
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
  }

  Long64_t LoadTree(Long64_t entry)
  {
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
    }
    return centry;
  }

  void Init(TTree *tree)
  {
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;

    fChain->SetBranchAddress("UrQMDEventBranch",&anEvent);

    Notify();


    nentries = fChain->GetEntriesFast();
    nbytes = 0;
    nb = 0;

  }

  Bool_t Notify()
  {


    return kTRUE;
  }

  void Show(Long64_t entry)
  {

    if (!fChain) return;
    fChain->Show(entry);
  }
  Int_t Cut(Long64_t entry)
  {
    return 1;
  }




  ////////////////////////////////////////////////////////////////////////////////////////
  // Data members
  ////////////////////////////////////////////////////////////////////////////////////////
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  Long64_t nentries;
  Long64_t nbytes;
  Long64_t nb;

  long jentry;

  /* the UrQMD event */
  UrQMDEvent *anEvent;
  /* the number of rejected leptons */
  long nleptons;
};



#endif /* UrqmdEventReader_hpp */
