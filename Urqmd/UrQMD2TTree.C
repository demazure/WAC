//---------------------------------------
// Code for Urqmd 3.03
// Sumit Basu (sumits.basu@gmail.com)
// for Urqmd 3.04 add 3 more parameters
//---------------------------------------

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TROOT.h"

#include "UrQMDEvent.h"

using std::cout;
using std::endl;
using std::ifstream;

void UrQMD2TTree(const char* filename, Int_t arrno)
{

  // event variable
  Int_t Events;

  printf("trying to open file: %s \n", filename);
  ifstream fInputFile;
  fInputFile.open(filename);
  if (!fInputFile.is_open()) {
    cout << "-ERROR - Cannot open input file. ABORTING" << endl;
    return;
  }

  Events = 0;

  UrQMDEvent* anEvent = new UrQMDEvent();

  // ROOT FILE DEFINE
  TFile* f = new TFile(Form("UrQMD_output_%d.root", arrno), "recreate");

  // TREE DEFINE
  TTree* UrQMDtree = new TTree("UrQMD_event_tree", "UrQMD Event data tree");
  UrQMDtree->Branch("UrQMDEventBranch", "UrQMDEvent", &anEvent, 32000, 99);

  /* for error conditions */
  Bool_t eventlineread = kFALSE;
  while (fInputFile.peek() != EOF) { // event loop starts

    TString currLine;

    // ---> Read and check first event header line from input file
    if (!eventlineread) {
      currLine.ReadLine(fInputFile);
    } else {
      eventlineread = kFALSE;
    }

    if (fInputFile.peek() == EOF) {
      cout << "-I- End of input file reached." << endl;
      fInputFile.close();
      break;
    }

    if (!currLine.BeginsWith("UQMD")) {
      cout << "-E- Wrong event header" << endl;
      break;
    }

    // ---> Read rest of event header
    Int_t aProj, zProj, aTarg, zTarg;
    currLine.ReadLine(fInputFile);
    sscanf(currLine.Data(), "projectile:  (mass, char) %d %d   target:  (mass, char) %d%d", &aProj, &zProj, &aTarg, &zTarg);
    currLine.ReadLine(fInputFile); // sscanf(currLine.Data(),"transformation betas (NN,lab,pro)     0.0000000  0.9561610 -0.9561610");

    Float_t breal, bmin, bmax, xsection;
    currLine.ReadLine(fInputFile);
    sscanf(currLine.Data(), "impact_parameter_real/min/max(fm):   %f %f %f  total_cross_section(mbarn): %f", &breal, &bmin, &bmax, &xsection);

    Int_t eos;
    Float_t e_lab, sqrt_s, p_lab;
    currLine.ReadLine(fInputFile);
    sscanf(currLine.Data(), "equation_of_state: %d  E_lab(GeV/u): %e  sqrt(s)(GeV): %e  p_lab(GeV/u): %e", &eos, &e_lab, &sqrt_s, &p_lab);

    Int_t evnr;
    currLine.ReadLine(fInputFile);
    sscanf(currLine.Data(), "event#  %d random seed", &evnr);

    /* for the time being we ignore the options */
    for (Int_t iline = 0; iline < 11; iline++) {
      currLine.ReadLine(fInputFile);
    }

    Int_t ntracks, outtime;
    currLine.ReadLine(fInputFile);
    sscanf(currLine.Data(), "%d %d", &ntracks, &outtime);

    Int_t ncoll, elcoll, inelcoll, blkcoll, decays, hbaryres, sbaryres, baryresofres;
    currLine.ReadLine(fInputFile);
    sscanf(currLine.Data(), "%d %d %d %d %d %d %d %d", &ncoll, &elcoll, &inelcoll, &blkcoll, &decays, &hbaryres, &sbaryres, &baryresofres);

    cout << "aTarg " << aTarg << " aProj " << aProj << " evnr " << evnr << " breal " << breal << " ntracks " << ntracks << endl;

    Events++;

    /* let's build the event */
    anEvent->Build(evnr, breal);

    // ---> Loop over tracks in the current event
    // TODO: to protect from bad generated events
    for (Int_t itrack = 0; itrack < ntracks; itrack++) {
      currLine.ReadLine(fInputFile);

      if (currLine.BeginsWith("UQMD")) {
        /* early event termination */
        eventlineread = kTRUE;
        break;
      }

      TObjArray* values = currLine.Tokenize(" ");

      anEvent->AddParticle(values);
      delete values;
    } // particle loop
    UrQMDtree->Fill();
    cout << "\tActually filled " << anEvent->GetNtrack() << " particles" << endl;
    anEvent->Clear();
  } // event loop

  f->Write();
  cout << "Ur Tree made successfully! Stored " << Events << " events!" << endl;

  f->Close();
  fInputFile.close();
} // end
