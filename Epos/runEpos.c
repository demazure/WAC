//
//  runEpos.c
//  CodeV3
//
//  Created by Claude Pruneau on 8/22/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#include <stdio.h>

//
//  cm_figure9_6.c
//  MyMC
//
//  Created by Claude Pruneau on 4/10/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "TRandom.h"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "AnalysisConfiguration.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"
#include "GeneratorConfiguration.hpp"
#include "MasterAnalysis.hpp"

////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void setDefaultOptions(int colorPalette=1)
{
  switch (colorPalette)
  {
    default:
    case 0:
      break;
    case 1:
      gStyle->SetPalette(1,0);
    case 2:
      gStyle->SetPalette(2,0);
    case 3:
      gStyle->SetPalette(3,0);  //deep sea
    case 4:
      gStyle->SetPalette(4,0);  //atlantic
    case 5:
      gStyle->SetPalette(5,0);  //cmyk
    case 6:
      gStyle->SetPalette(6,0);  //island
    case 7:
      gStyle->SetPalette(7,0);
  }
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

}


/////////////////////////////////////////////////////////////////////////////////////////
// Run an analysis with the Cluster Model
/////////////////////////////////////////////////////////////////////////////////////////
void runEpos(long nEventsRequested=30000)
{
  cout << "-INFO- runClusterModelAnalysis() Setup configuration" << endl;

  // ---------------------------
  // debugLevel Levels
  // ---------------------------
  bool debugLoop      = true;
  bool debugGenerator = true;
  bool debugAnalyzer  = false;

  // ---------------------------
  // Graphics parameters
  // ---------------------------
  setDefaultOptions(1);

  // ---------------------------
  // Histogram parameters
  // ---------------------------

  AnalysisConfiguration * ac = new AnalysisConfiguration();
  ac->outputPath               = "../EPOSOUTPUT/";
  ac->baseName                 = "EPOS_0010_4";
  ac->configFileName           = "Config_0010_4.txt";
  ac->rootOutputFileName       = "Histograms_0010_4.root";

  ac->nBins_nCell = 100;
  ac->min_nCell   = 0.0;
  ac->max_nCell   = 100.0;
  ac->nBins_nPartPerCell = 100;
  ac->min_nPartPerCell   = 0.0;
  ac->max_nPartPerCell   = 100.0;

  ac->nBins_pt    = 28;
  ac->min_pt      = 0.2;
  ac->max_pt      = 3.0;
  ac->nBins_eta   = 20;
  ac->min_eta     = -0.8;
  ac->max_eta     = 0.8;
  ac->nBins_y     = 20;
  ac->min_y       = -2.0;
  ac->max_y       = 2.0;
  ac->nBins_phi   = 36;
  ac->min_phi     = 0.0;
  ac->max_phi     = 2.0*3.1415927;

  ac->nBins_DeltaPlong = 40;
  ac->min_DeltaPlong   = -2.0;
  ac->max_DeltaPlong   =  2.0;
  ac->nBins_DeltaPside = 40;
  ac->min_DeltaPside   = -2.0;
  ac->max_DeltaPside   =  2.0;
  ac->nBins_DeltaPout  = 40;
  ac->min_DeltaPout    = -2.0;
  ac->max_DeltaPout    =  2.0;

  ac->fillPairs        = true;
  ac->fill3D           = false;
  ac->fill6D           = false;
  ac->fillQ3D          = false;
  ac->fillY            = true;

  ac->scaleHistograms      = true;
  ac->externalScaling      = 1.0;
  ac->saveHistosToRootFile = true;
  ac->forceRewrite         = true;
  ac->saveHistosToTextFile = false;
  ac->calculateDerived     = true;
  ac->plotGraphs           = false;

  // use the following options to select which graphs are plotted on screen.
  ac->plotSingles         = true;     // selects the single part histo to be printed
  ac->plotPairs           = true;    // selects the part pair histo to be printed
  ac->plotDerivedPairs    = true;    // selects the derived part pair histo  to be printed
  ac->plotCombined        = true;
  ac->plotEventHistos     = true;    // global event stuff

  ac->printGraphs          = false;
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
  // EPOS Model Selection Parameters
  // ==================================================================================
 GeneratorConfiguration * mc = new GeneratorConfiguration(3,"EposModel");
  // ---------------------------
  // Cell Parameters
  // ---------------------------
  mc->impactParameterMinimum     = 0.0;
  mc->impactParameterMaximum     = 4.94;
  mc->ptMinimum = 0.2;
  mc->ptMaximum = 4.0;
  mc->yMinimum = -2.5;
  mc->yMaximum =  2.5;

  MasterAnalysis * analysis = new MasterAnalysis(mc,ac, debugLoop, debugGenerator,debugAnalyzer);
  analysis->run(nEventsRequested);
}
