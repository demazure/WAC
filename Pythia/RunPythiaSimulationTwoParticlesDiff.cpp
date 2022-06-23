//  Created by Claude Pruneau on 6/19/2020.
//  Copyright © 2020 Claude Pruneau. All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include "Event.hpp"
#include "AnalysisConfiguration.hpp"
#include "TwoPartDiffCorrelationAnalyzer.hpp"
#include "ParticleAnalyzer.hpp"
#include "EventLoop.hpp"
#include "EventFilter.hpp"
#include "ParticleFilter.hpp"
#include "PythiaConfiguration.hpp"
#include "PythiaEventGenerator.hpp"

int main(int argc, char* argv[])
{
  if (argc > 3 or argc < 2) {
    Fatal("main", "Wrong number of arguments. Use RunPythiaSimulationTwoParticleDiff jobix seed");
  }
  int jobix = stoi(argv[1]);
  long seed = stol(argv[2]);

  time_t begin, end; // time_t is a datatype to store time values.
  time(&begin);      // note time before execution

  // =========================
  // Short configuration
  // =========================
  float min_eta = -13.;
  float max_eta = 13.;
  float min_pt = 0.0;
  float max_pt = 10.;
  int nBins_eta = int((max_eta - min_eta) / 1.);
  int nBins_pt = int((max_pt - min_pt) / 0.2);

  long nEventsRequested = 100000;
  int nEventsReport = 10000;
  MessageLogger::LogLevel logLevel = MessageLogger::Info;

  // ==========================
  // Event Section
  // ==========================
  Event* event = Event::getEvent();

  // ==========================
  // Generator Section
  // ==========================
  int nOptions = 0;
  TString** pythiaOptions = new TString*[50];
  /* no decay of strange barions
  pythiaOptions[nOptions++] = new TString("130:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("310:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("311:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3112:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3222:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3212:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3322:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3312:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3334:mayDecay = off");
  pythiaOptions[nOptions++] = new TString("3122:mayDecay = off"); */

  pythiaOptions[nOptions++] = new TString("Init:showChangedSettings = on");      // list changed settings
  pythiaOptions[nOptions++] = new TString("Init:showChangedParticleData = on");  // list changed particle data
  pythiaOptions[nOptions++] = new TString("Next:numberCount = 10000");           // print message every n events
  pythiaOptions[nOptions++] = new TString("Next:numberShowInfo = 1");            // print event information n times
  pythiaOptions[nOptions++] = new TString("Next:numberShowProcess = 0");         // print process record n times
  pythiaOptions[nOptions++] = new TString("Next:numberShowEvent = 0");
  pythiaOptions[nOptions++] = new TString("Random:setSeed = on");
  pythiaOptions[nOptions++] = new TString(TString::Format("Random:seed = %ld", seed));
  pythiaOptions[nOptions++] = new TString("SoftQCD:all = on"); // Allow total sigma = elastic/SD/DD/ND
  pythiaOptions[nOptions++] = new TString("Tune:pp = 14");     // Monash 2013
  pythiaOptions[nOptions++] = new TString("Fragmentation:SetVertices = on"); // else Lednicky will return a lot of NANs
  // pythiaOptions[nOptions++] = new TString("HardQCD:all = on");
  PythiaConfiguration* pc = new PythiaConfiguration(2212 /* p */,
                                                    2212 /* p */,
                                                    7000.0, /* energy in GeV */
                                                    nOptions,
                                                    pythiaOptions);
  EventFilter* eventFilterGen = new EventFilter(EventFilter::MinBias, 0.0, 0.0);
  ParticleFilter* particleFilterGen = new ParticleFilter(ParticleFilter::Strings,
                                                         ParticleFilter::AllCharges,
                                                         min_pt, max_pt,
                                                         min_eta, max_eta,
                                                         min_eta, max_eta);
  PythiaEventGenerator* generator = new PythiaEventGenerator("PYTHIA", pc, event, eventFilterGen, particleFilterGen);
  generator->reportLevel = logLevel;

  // ==========================
  // Analysis Section
  // ==========================
  AnalysisConfiguration* ac = new AnalysisConfiguration("PYTHIA", "PYTHIA", "1.0");
  ac->loadHistograms = false;
  ac->createHistograms = true;
  ac->scaleHistograms = false;
  ac->calculateDerivedHistograms = true;
  ac->essentialHistos = true;
  ac->withWeight = false;
  ac->saveHistograms = true;
  ac->resetHistograms = false;
  ac->clearHistograms = false;
  ac->forceHistogramsRewrite = true;
  ac->inputPath = "Input/";
  ac->rootInputFileName = "";
  ac->outputPath = "Output/";
  ac->rootOuputFileName = TString::Format("PYTHIA8_Pairs_%03d_", jobix).Data();
  ac->histoBaseName = "TEST";

  ac->nBins_pt = nBins_pt;
  ac->min_pt = min_pt;
  ac->max_pt = max_pt;
  ac->nBins_eta = nBins_eta;
  ac->min_eta = min_eta;
  ac->max_eta = max_eta;
  ac->nBins_y = nBins_eta;
  ac->min_y = min_eta;
  ac->max_y = max_eta;
  ac->nBins_phi = 72;
  ac->min_phi = 0.0;
  ac->max_phi = kTWOPI;

  ac->fillPairs = true;
  ac->fill3D = false;
  ac->fill6D = false;
  ac->fillQ3D = false;
  ac->fillY = false;

  TString taskName;
  int nAnalysisTasks = 20;
  Task** analysisTasks = new Task*[nAnalysisTasks];

  /* for having the balance function correctly extracted the particle filters have to follow certain order */
  /* - charged particle should come always first                                                           */
  /* - particles of the same species, the positive has to come first and the negative immediately after    */
  /* - after the charged particles the might come any number of neutral                                    */
  /* - the balance function produced for neutrals will not have any sense                                  */
  EventFilter* eventFilter = new EventFilter(EventFilter::MinBias, 0.0, 0.0);
  std::vector<ParticleFilter*> particleFilters;
  //particleFilters.push_back(new ParticleFilter(ParticleFilter::Pion, ParticleFilter::Positive, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  //particleFilters.push_back(new ParticleFilter(ParticleFilter::Pion, ParticleFilter::Negative, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  //particleFilters.push_back(new ParticleFilter(ParticleFilter::Kaon, ParticleFilter::Positive, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  //particleFilters.push_back(new ParticleFilter(ParticleFilter::Kaon, ParticleFilter::Negative, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  //particleFilters.push_back(new ParticleFilter(ParticleFilter::Proton, ParticleFilter::Positive, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  //particleFilters.push_back(new ParticleFilter(ParticleFilter::Proton, ParticleFilter::Negative, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  particleFilters.push_back(new ParticleFilter(ParticleFilter::MString, ParticleFilter::Charged, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));
  particleFilters.push_back(new ParticleFilter(ParticleFilter::BString, ParticleFilter::Neutral, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y));

  int iTask = 0;
  /* the two-particle analyzer */
  analysisTasks[iTask++] = new TwoPartDiffCorrelationAnalyzer("NarrowPiKaPr", ac, event, eventFilter, particleFilters);
  analysisTasks[iTask - 1]->reportLevel = logLevel;

  /* single particle analysis filters */
  /* int nParticleFilters = 1;
  ParticleFilter** singleParticleFilters = new ParticleFilter*[nParticleFilters];
  singleParticleFilters[0] = new ParticleFilter(ParticleFilter::AllSpecies, ParticleFilter::AllCharges, ac->min_pt, ac->max_pt, ac->min_eta, ac->max_eta, ac->min_y, ac->max_y);
  analysisTasks[iTask++] = new ParticleAnalyzer("SinglesPythia", ac, event, eventFilter, nParticleFilters, singleParticleFilters);
  analysisTasks[iTask - 1]->reportLevel = logLevel; */

  nAnalysisTasks = iTask;

  // ==========================
  // Event Loop
  // ==========================

  EventLoop* eventLoop = new EventLoop();
  eventLoop->addTask((Task*)generator);
  for (int iAnalysisTask = 0; iAnalysisTask < nAnalysisTasks; iAnalysisTask++) {
    eventLoop->addTask(analysisTasks[iAnalysisTask]);
  }
  eventLoop->run(nEventsRequested, nEventsReport);

  cout << "<INFO> PYTHIA Model Analysis - Pair Differential Correlations Histograms  - Completed" << endl;
  time(&end); // note time after execution
  double difference = difftime(end, begin);
  cout << "<INFO> in " << difference << " seconds" << endl;
}
