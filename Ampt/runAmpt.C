// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#include <iostream>
#include <fstream>
#include "TRandom.h"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "AnalysisConfiguration.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"
#include "GeneratorConfiguration.hpp"
#include "AmptEventReader.hpp"
#include "GenericEventAnalyzer.hpp"


// =================================================
// Run a correlation analysis with AMPT events
// =================================================
void runAmpt(long nEventsRequested=5000000)
{
  cout << "<INFO> runClusterModelAnalysis() Setup configuration" << endl;

  AnalysisConfiguration * ac = new AnalysisConfiguration();
  ac->outputPath               = "../Ampt_paper/0p2_p20_set4/";
  ac->baseName                 = "Ampt_6099";
  ac->configFileName           = "Config_6099.txt";
  ac->rootOutputFileName       = "Ampt_Histograms_6099.root";

    cout<<"================================================================"<<endl;
    cout<<""<<endl;
    cout<<"         running for = "<<ac->baseName<<endl;
    cout<<""<<endl;
    cout<<"================================================================"<<endl;


  ac->nBins_pt    = 18;
  ac->min_pt      = 0.2;
  ac->max_pt      = 2.0;
  ac->nBins_eta   = 20;
  ac->min_eta     = -2.0;
  ac->max_eta     = 2.0;
  ac->nBins_y     = 20;
  ac->min_y       = -2.0;
  ac->max_y       = 2.0;
  ac->nBins_phi   = 18;
  ac->min_phi     = 0.0;
  ac->max_phi     = 2.0*3.1415927;

  ac->nBins_DeltaPlong = 10;
  ac->min_DeltaPlong   = -1.0;
  ac->max_DeltaPlong   =  1.0;
  ac->nBins_DeltaPside = 10;
  ac->min_DeltaPside   = -1.0;
  ac->max_DeltaPside   =  1.0;
  ac->nBins_DeltaPout  = 10;
  ac->min_DeltaPout    = -1.0;
  ac->max_DeltaPout    =  1.0;

  ac->fillPairs        = true;
  ac->fill3D           = false;
  ac->fill6D           = false;
  ac->fillQ3D          = true;
  ac->fillY            = true;

  ac->scaleHistograms      = true;
  ac->externalScaling      = 1.0;
  ac->saveHistosToRootFile = true;
  ac->forceRewrite         = true;
  ac->saveHistosToTextFile = false;
  ac->calculateDerived     = true;
  ac->plotGraphs           = true; //on off plots for root display


  // use the following options to select which graphs are plotted on screen.
  ac->plotSingles         = true;     // selects the single part histo to be printed
  ac->plotPairs           = true;    // selects the part pair histo to be printed
  ac->plotDerivedPairs    = true;    // selects the derived part pair histo  to be printed
  ac->plotCombined        = true;
  ac->plotEventHistos     = true;    // global event stuff

  ac->printGraphs          = false; //on off plots for root display
  ac->plotAll              = false;
  ac->printAll             = false;

  ac->particleFilter1      = new ParticleFilter( 1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta,ac->min_y,ac->max_y); // +ve only
  ac->particleFilter2      = new ParticleFilter(-1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta,ac->min_y,ac->max_y); // -ve only
  ac->particlePairFilter12 = new ParticlePairFilter(0.0, 100.0,
                                                    0.0, 2.0*3.1415927,
                                                    0.0, 6.0);

  ac->canvasConfiguration1D  = new CanvasConfiguration(1);
  ac->graphConfiguration1D   = new GraphConfiguration(1,1);
  ac->canvasConfiguration2D  = new CanvasConfiguration(0);
  ac->graphConfiguration2D   = new GraphConfiguration(2);

  // ==================================================================================
  // Ampt Model Selection Parameters
  // ==================================================================================
  GeneratorConfiguration * mc = new GeneratorConfiguration("AmptModel",4);
  mc->impactParameterMinimum     = 12.47;
  mc->impactParameterMaximum     = 20.01;
  mc->ptMinimum = 0.1;
  mc->ptMaximum = 2.5;
  mc->yMinimum = -2.5;
  mc->yMaximum =  2.5;

  AmptEventReader * eventGenerator = new AmptEventReader(event,modelConfig,randomGenerator,debugGenerator);
  GenericEventAnalyzer * eventAnalyzer  = new GenericEventAnalyzer(analysisConfig,randomGenerator,debugAnalyzer);

  EventLoop eventLoop = new EventLoop();
  eventLoop->addTask(eventGenerator);
  eventLoop->addTask(eventAnalyzer);
  eventLoop->run(nEventsRequested);
}
