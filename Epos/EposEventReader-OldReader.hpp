//
//  EposEventReader.hpp
//  CodeV3
//
//  Created by Claude Pruneau on 8/18/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#ifndef EposEventReader_hpp
#define EposEventReader_hpp

#include <stdio.h>
#include "TTree.h"
#include "TChain.h"
#include "GeneratorConfiguration.hpp"
#include "EventGenerator.hpp"
class EposEventReader : public EventGenerator
{
public:


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  EposEventReader(Event * event,
      GeneratorConfiguration * configuration,
      TRandom * randomGenerator,
      bool debugLevel)
:
  EventGenerator  (event, configuration, randomGenerator, debugLevel)
{
    if (debugLevel>0) cout << "-DEBUG- EposEventReader::CTOR(...) No ops" << endl;
}

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // DTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual ~EposEventReader()
  {
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialize generator
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual void initialize()
  {
    if (debugLevel>0) cout << "-DEBUG- EposEventReader::initialize() Started" << endl;


    event           ->reset();
    particleFactory ->reset();



    TChain *chain = new TChain("tree"," ");
    for(int ifl=0; ifl<2; ifl++)
    {
      chain->Add(Form("/media/victor/victor/epos_rootfile/epos_pbpb_urqmd_on_%i.root",ifl+1));
    }
    Init(chain);
    jentry = 0;

    nentries = fChain->GetEntries();
    cout << "-INFO- initialize() nentries: " << nentries << endl;
    nbytes = 0;
    nb = 0;



    if (debugLevel>0) cout << "-DEBUG- EposEventReader::initialize() Completed" << endl;
  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Reset and Initialize the generator
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual void reset()
  {
    if (debugLevel>0) cout << "-DEBUG- EposEventReader::reset() Started" << endl;
    event->reset();
    particleFactory->reset();
    if (debugLevel>0) cout << "-DEBUG- EposEventReader::reset() Completed" << endl;

  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Generate an event
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Event * generate()
  {
    if (debugLevel>0) cout << "EposEventReader::generate() Started" << endl;

    if (!fChain)
    {
      cout << "-FATAL- EposEventReader::generate() no TChain available" << endl;
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

      if(Impact >  generatorConfiguration->impactParameterMinimum &&
          Impact <= generatorConfiguration->impactParameterMaximum) seekingEvent = false;
    }

    if (debugLevel>0) cout << "-DEBUG- EposEventReader::generate() new event b: " << Impact << ",   ntracks: " << Mult << endl;

    //////////////
    int thePid;
    double charge;

    event->reset();
    particleFactory->reset();
    int particleAccepted = 0;

    // load particles from TTree and copy those that are selected into event
    Particle aParticle; /* we use a particle object to extract angular information for indexes extraction */
    Particle *particle; /* this is the actual particle which will be stored */
    for (int iPart1=0; iPart1<Mult; iPart1++)
    {
      //if(ist[iPart1]!=0) continue;
      //cout << "========== part1: " << iPart1 << endl;

      thePid = PID[iPart1];
      if ( thePid == 120 || thePid == 130  || thePid ==1120 )
        charge = 1;
      else if (thePid ==-120 || thePid ==-130 || thePid==-1120)
        charge = -1;
      else
        charge = 0;
      // only accept charged particles
      if (charge==0) continue;

      aParticle.pid = thePid;
      aParticle.charge = charge;
      aParticle.setPxPyPzE(Px[iPart1],Py[iPart1],Pz[iPart1],E[iPart1]);
      aParticle.ixEtaPhi = -1;
      aParticle.ixYPhi = -1;

      if (aParticle.pt<generatorConfiguration->ptMinimum || aParticle.pt>generatorConfiguration->ptMaximum) continue;
      if (aParticle.y<generatorConfiguration->yMinimum || aParticle.y>generatorConfiguration->yMaximum) continue;

      particle = particleFactory->getObject();
      particle->Copy(aParticle);

      //particle->printProperties(cout);
      particleAccepted++;
    }
    event->nParticles = particleAccepted;

    if (debugLevel>0) cout << "-DEBUG- EposEventReader::generate() No of accepted Particles: "<<particleAccepted<<endl;
    //event->printProperties(cout);
    if (debugLevel>0) cout << "-DEBUG- EposEventReader::generate() event completed!" << endl;
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
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("Events", &Events, &b_Events);
    fChain->SetBranchAddress("Mult", &Mult, &b_Mult);
    fChain->SetBranchAddress("Impact", &Impact, &b_Impact);
    fChain->SetBranchAddress("PID", PID, &b_PID);
    fChain->SetBranchAddress("Px", Px, &b_Px);
    fChain->SetBranchAddress("Py", Py, &b_Py);
    fChain->SetBranchAddress("Pz", Pz, &b_Pz);
    fChain->SetBranchAddress("E", E, &b_E);
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

  static const int arr = 22000;

  // Declaration of leaf types
  Int_t           Events;
  Int_t           Mult;
  Float_t         Impact;
  Int_t           PID[arr];   //[Mult]
  Float_t         Px[arr];   //[Mult]
  Float_t         Py[arr];   //[Mult]
  Float_t         Pz[arr];   //[Mult]
  Float_t         E[arr];   //[Mult]

  // List of branches
  TBranch        *b_Events;   //!
  TBranch        *b_Mult;   //!
  TBranch        *b_Impact;   //!
  TBranch        *b_PID;   //!
  TBranch        *b_Px;   //!
  TBranch        *b_Py;   //!
  TBranch        *b_Pz;   //!
  TBranch        *b_E;   //!



};



#endif /* EposEventReader_hpp */
