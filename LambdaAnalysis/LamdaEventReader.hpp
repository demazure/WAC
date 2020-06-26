//
//  LamdaEventReader.hpp
//  CodeV3
//
//  Created by Claude Pruneau on 8/18/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#ifndef LamdaEventReader_hpp
#define LamdaEventReader_hpp
#include "TMath.h"
#include <stdio.h>
#include "TTree.h"
#include "TChain.h"
#include "GeneratorConfiguration.hpp"
#include "EventGenerator.hpp"
class LamdaEventReader : public EventGenerator
{
public:


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  LamdaEventReader(Event * event,
      GeneratorConfiguration * configuration,
      TRandom * randomGenerator,
      bool debugLevel)
:
  EventGenerator  (event, configuration, randomGenerator, debugLevel)
{
    if (debugLevel>0) cout << "-DEBUG- LamdaEventReader::CTOR(...) No ops" << endl;
}

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // DTOR
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual ~LamdaEventReader()
  {
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Initialize generator
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual void initialize()
  {
    if (debugLevel>0) cout << "-DEBUG- LamdaEventReader::initialize() Started" << endl;


    event           ->reset();
    particleFactory ->reset();



    TChain *chain = new TChain("events"," ");
   
    chain->Add("/media/sumit/7392998e-237a-4d75-8cec-93c8749a031d/savefiles/analysis/lamda_analysis/purest_hyperons.root");
    Init(chain);
    jentry = 0;

    nentries = fChain->GetEntries();
    cout << "-INFO- initialize() nentries: " << nentries << endl;
    nbytes = 0;
    nb = 0;



    if (debugLevel>0) cout << "-DEBUG- LamdaEventReader::initialize() Completed" << endl;
  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Reset and Initialize the generator
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  virtual void reset()
  {
    if (debugLevel>0) cout << "-DEBUG- LamdaEventReader::reset() Started" << endl;
    event->reset();
    particleFactory->reset();
    if (debugLevel>0) cout << "-DEBUG- LamdaEventReader::reset() Completed" << endl;

  }


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Generate an event
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Event * generate()
  {
    if (debugLevel>0) cout << "LamdaEventReader::generate() Started" << endl;

    if (!fChain)
    {
      cout << "-FATAL- LamdaEventReader::generate() no TChain available" << endl;
    }

    bool seekingEvent = true;
    while (seekingEvent)
    {
      // load another event from the root file/TTree
      Long64_t ientry = LoadTree(jentry++);

      // returning a null point is an indication that
      // there are no more events in the file or stack of files.
      if (ientry < 0) return 0;

      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      // check if the event is acceptable.
      //  if (Mult==0) continue;
        
     if(cent >= generatorConfiguration->impactParameterMinimum && cent <= generatorConfiguration->impactParameterMaximum) seekingEvent = false;
//cout<<Mult<<"   "<<Cent<<endl;
    }
  //cout<<"   "<<cent<<"  "<<N<<endl;
    //////////////
    int thePid;
    double charge;
    double p_x, p_y, p_z, p_e, p_t, y, y_maxim,m_e;
    double px,py,pz,mt;
    Particle * particle;


    event->reset();
    particleFactory->reset();
      
    int particleAccepted = 0;
    // load particles from TTree and copy those that are selected into
    // event
    for (int iPart1=0; iPart1<N; iPart1++)
    {
      //if(ist[iPart1]!=0) continue;
      //cout << "========== part1: " << iPart1 << endl;
//if(Mult==0) continue;
      thePid = v[iPart1];
      px=pt[iPart1]*(TMath::Cos(phi[iPart1]));
      py=pt[iPart1]*TMath::Sin(phi[iPart1]);
      mt=TMath::Sqrt(pt[iPart1]*pt[iPart1]+m[iPart1]*m[iPart1]);
      pz=mt*TMath::SinH(rap[iPart1]);
       //cout<<thePid<<endl;
   //------------------  Select some pid based BF corr ---------------------Starts
   //     if(thePid==120||thePid==-120||thePid==1120||thePid==-1120) continue;
        
   //------------------Select some pid based BF corr -----------------------Ends
      if ( thePid == 1)
        charge = 1;
      else if (thePid == -1)
        charge = -1;
      else
        charge = 0;
      // only accept charged particles
     
      if (charge==0) continue;
      p_x = px;
      p_y = py;
      p_z = pz;
      m_e = m[iPart1];
      p_t = pt[iPart1];
      p_e =sqrt(px*px + py*py + pz*pz + m[iPart1]*m[iPart1]);
      if (p_t<generatorConfiguration->ptMinimum || p_t>generatorConfiguration->ptMaximum) continue;
      double plus  = p_e + p_z;
      double minus = p_e - p_z;
      if (plus <=0.0)
        y = -1.0E50;
      else if (minus <=0.0)
        y = 1.0E50;
      else
        y = 0.5 * log( plus/minus );
        //y_maxim=rap[iPart1];
       // cout<<"      "<<y_maxim<<"    "<<y<<endl;
       //cout<<"      "<<thePid<<"    "<<y<<endl;
      if (y<generatorConfiguration->yMinimum || y>generatorConfiguration->yMaximum) continue;
        //-------here
      
        
      particle = particleFactory->getObject();
      particle->pid    = thePid;
      particle->charge = charge;
      particle->setPxPyPzE(p_x,p_y,p_z,p_e);
        
      //particle->printProperties(cout);
      particleAccepted++;
    }
    event->nParticles = particleAccepted;
    //cout<<"No of accepted Particles : "<<particleAccepted<<endl;
    //event->printProperties(cout);
    if (debugLevel>0) cout << "-DEBUG- LamdaEventReader::generate() event completed!" << endl;
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

    fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("magfld", &magfld, &b_magfld);
   fChain->SetBranchAddress("cent", &cent, &b_cent);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("q", q, &b_q);
   fChain->SetBranchAddress("tm", &tm, &b_tm);
   fChain->SetBranchAddress("tx", tx, &b_tx);
   fChain->SetBranchAddress("ty", ty, &b_ty);
   fChain->SetBranchAddress("N", &N, &b_N);
   fChain->SetBranchAddress("v", v, &b_v);
   fChain->SetBranchAddress("m", m, &b_m);
   fChain->SetBranchAddress("pt", pt, &b_pt);
   fChain->SetBranchAddress("rap", rap, &b_rap);
   fChain->SetBranchAddress("phi", phi, &b_phi);
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

  static const int arr = 40;

  // Declaration of leaf types
  
   Int_t           run;
   Float_t         magfld;
   Float_t         cent;
   Float_t         vz;
   Float_t         q[4];
   Float_t         tm;
   Float_t         tx[3];
   Float_t         ty[3];
   Int_t           N;
   Int_t           v[arr];   //[N]
   Float_t         m[arr];   //[N]
   Float_t         pt[arr];   //[N]
   Float_t         rap[arr];   //[N]
   Float_t         phi[arr];   //[N]  
  
  
  
  // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_magfld;   //!
   TBranch        *b_cent;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_q;   //!
   TBranch        *b_tm;   //!
   TBranch        *b_tx;   //!
   TBranch        *b_ty;   //!
   TBranch        *b_N;   //!
   TBranch        *b_v;   //!
   TBranch        *b_m;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_rap;   //!
   TBranch        *b_phi;   //!


};



#endif /* LamdaEventReader_hpp */
