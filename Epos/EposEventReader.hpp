// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_EposEventReader
#define WAC_EposEventReader
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "Task.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"

class EposEventReader : public Task
{
public:

  EposEventReader(const TString & name,
                  TaskConfiguration * configuration,
                  Event * event,
                  EventFilter * ef,
                  ParticleFilter * pf);
  virtual ~EposEventReader();
  virtual void initialize();
  virtual void reset();
  void execute();

  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void Init(TTree *tree);
  Bool_t Notify();
  void Show(Long64_t entry);
  Int_t Cut(Long64_t entry);

  ////////////////////////////////////////////////////////////////////////////////////////
  // Data members
  ////////////////////////////////////////////////////////////////////////////////////////
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  Long64_t nentries;
  Long64_t nbytes;
  Long64_t nb;

  long jentry;

  static const int arraySize = 22000;

   // Declaration of leaf types
   Int_t           events;
   Int_t           mult;
   Float_t         impact;
   Int_t           pid[arraySize];   //[Mult]
   Float_t         px[arraySize];   //[Mult]
   Float_t         py[arraySize];   //[Mult]
   Float_t         pz[arraySize];   //[Mult]
   Float_t         e[arraySize];   //[Mult]

   // List of branches
   TBranch        *b_Events;   //!
   TBranch        *b_Mult;   //!
   TBranch        *b_Impact;   //!
   TBranch        *b_PID;   //!
   TBranch        *b_Px;   //!
   TBranch        *b_Py;   //!
   TBranch        *b_Pz;   //!
   TBranch        *b_E;   //!
  EventFilter * eventFilter;
  ParticleFilter * particleFilter;

  ClassDef(EposEventReader,0)
};

#endif /* WAC_EposEventReader */
