//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include "Event.hpp"
#include "AnalysisConfiguration.hpp"
#include "TwoPartCorrelationAnalyzer.hpp"
#include "EventLoop.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "PythiaConfiguration.hpp"
#include "PythiaEventGenerator.hpp"
#include "NuDynTask.hpp"

int main()
{
  time_t begin,end; // time_t is a datatype to store time values.
  time (&begin); // note time before execution
  cout << "<INFO> PYTHIA Model Analysis - Starting" << endl;

//  long nEventsRequested = 100;
  long nEventsRequested = 1000;
  int  nEventsReport    = 100000;

  // ==========================
  // Event Section
  // ==========================
  Event * event = Event::getEvent();

  // ==========================
  // Generator Section
  // ==========================
  int nOptions = 0;
  TString ** pythiaOptions = new TString* [50];
  pythiaOptions[nOptions++] = new TString("Init:showChangedSettings = on");      // list changed settings
  pythiaOptions[nOptions++] = new TString("Init:showChangedParticleData = off"); // list changed particle data
  pythiaOptions[nOptions++] = new TString("Next:numberCount = 10000");            // print message every n events
  pythiaOptions[nOptions++] = new TString("Next:numberShowInfo = 1");            // print event information n times
  pythiaOptions[nOptions++] = new TString("Next:numberShowProcess = 0");         // print process record n times
  pythiaOptions[nOptions++] = new TString("Next:numberShowEvent = 0");
  pythiaOptions[nOptions++] = new TString("SoftQCD:all = on");                   // Allow total sigma = elastic/SD/DD/ND
                                                                                 //pythiaOptions[nOptions++] = new TString("HardQCD:all = on");
  PythiaConfiguration * pc = new PythiaConfiguration(2212 /* p */,
                                                     2212 /* p */,
                                                     14000.0, /* energy in GeV */
                                                     nOptions,
                                                     pythiaOptions);
  EventFilter     * eventFilterGen    = new EventFilter(EventFilter::MinBias,0.0,0.0);
  ParticleFilter  * particleFilterGen = new ParticleFilter(ParticleFilter::Hadron,
                                                           ParticleFilter::Charged,
                                                           0.2,100.0,
                                                           -1.0,1.0,
                                                           -5.0,5.0);
  Task * generator = new PythiaEventGenerator("PYTHIA",pc, event,eventFilterGen,particleFilterGen);

  // ==========================
  // Analysis Section
  // ==========================
  AnalysisConfiguration * ac = new AnalysisConfiguration("PYTHIA","PYTHIA","1.0");
  ac->loadHistograms         = false;
  ac->createHistograms       = true;
  ac->scaleHistograms        = true;
  ac->calculateDerivedHistograms  = true;
  ac->saveHistograms         = true;
  ac->resetHistograms        = false;
  ac->clearHistograms        = false;
  ac->forceHistogramsRewrite = true;
  ac->inputPath              = "Input/";
  ac->rootInputFileName      = "";
  ac->outputPath             = "Output/";
  ac->rootOuputFileName      =  "PYTHIA_softOnHardOff_NuDyn_";

  ac->nBins_pt    = 40;
  ac->min_pt      = 0.2;
  ac->max_pt      = 2.0;
  ac->nBins_eta   = 20;
  ac->min_eta     = -2;
  ac->max_eta     = 2;
  ac->nBins_y     = 20;
  ac->min_y       = -2;
  ac->max_y       = 2;
  ac->nBins_phi   = 36;
  ac->min_phi     = 0.0;
  ac->max_phi     = 2.0*3.1415927;

  ac->nuDynVsMult  = true;
  ac->nuDynVsCent  = false;
  ac->nBins_mult   = 100;
  ac->min_mult     = 0.0;
  ac->max_mult     = 200.0;
  ac->nBins_cent   = 20;
  ac->min_cent     = 0.0;
  ac->max_cent     = 100.0;

  EventFilter     * eventFilter        = new EventFilter(EventFilter::MinBias,0.0,0.0);
  ParticleFilter  * particleFilter_HP  = new ParticleFilter(ParticleFilter::Hadron, ParticleFilter::Positive,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_HM  = new ParticleFilter(ParticleFilter::Hadron, ParticleFilter::Negative,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_PiP = new ParticleFilter(ParticleFilter::Pion,   ParticleFilter::Positive,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_PiM = new ParticleFilter(ParticleFilter::Pion,   ParticleFilter::Negative,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_KP  = new ParticleFilter(ParticleFilter::Kaon,   ParticleFilter::Positive,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_KM  = new ParticleFilter(ParticleFilter::Kaon,   ParticleFilter::Negative,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_PP  = new ParticleFilter(ParticleFilter::Proton, ParticleFilter::Positive,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);
  ParticleFilter  * particleFilter_PM  = new ParticleFilter(ParticleFilter::Proton, ParticleFilter::Negative,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y);

  int nAnalysisTasks = 4;
  Task ** analysisTasks = new Task*[nAnalysisTasks];

  analysisTasks[0] = new NuDynTask(ac->rootOuputFileName+"HPHPHPHP", ac, event, eventFilter,particleFilter_HP,particleFilter_HP,particleFilter_HP,particleFilter_HP);
  analysisTasks[1] = new NuDynTask(ac->rootOuputFileName+"HPHPHPHM", ac, event, eventFilter,particleFilter_HP,particleFilter_HP,particleFilter_HP,particleFilter_HM);
  analysisTasks[2] = new NuDynTask(ac->rootOuputFileName+"HPHPHMHM", ac, event, eventFilter,particleFilter_HP,particleFilter_HP,particleFilter_HM,particleFilter_HM);
  analysisTasks[3] = new NuDynTask(ac->rootOuputFileName+"HPHMHMHM", ac, event, eventFilter,particleFilter_HP,particleFilter_HM,particleFilter_HM,particleFilter_HM);

  // ==========================
  // Event Loop
  // ==========================
  EventLoop * eventLoop = new EventLoop();
  eventLoop->addTask( generator );
  for (int iAnalysisTask=0;iAnalysisTask<nAnalysisTasks;iAnalysisTask++)
  {
    eventLoop->addTask( analysisTasks[iAnalysisTask] );
  }
  eventLoop->run(nEventsRequested,nEventsReport);

  cout << "<INFO> PYTHIA Analysis - Completed" << endl;
  time (&end); // note time after execution
  double difference = difftime (end,begin);
  cout << "<INFO> in " <<  difference << " seconds";
}

//  TwoPartCorrelationAnalyzer  * ana1 = new TwoPartCorrelationAnalyzer ("PYTHIA_TPCA_ALL",  ac,  event, eventFilter,particleFilter_HP,particleFilter_HM);
//  TwoPartCorrelationAnalyzer  * ana2 = new TwoPartCorrelationAnalyzer ("PYTHIA_TPCA_PiPi", ac,  event, eventFilter,particleFilter_PiP,particleFilter_PiM);
//  TwoPartCorrelationAnalyzer  * ana3 = new TwoPartCorrelationAnalyzer ("PYTHIA_TPCA_KK",   ac,  event, eventFilter,particleFilter_KP,particleFilter_KM);
//  TwoPartCorrelationAnalyzer  * ana4 = new TwoPartCorrelationAnalyzer ("PYTHIA_TPCA_PP",   ac,  event, eventFilter,particleFilter_PP,particleFilter_PM);


