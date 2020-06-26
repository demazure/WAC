//
//
//  Created by Victor Gonzalez on 20190206 based in the work of Claude Pruneau and the ALICE AliPhysics SW framework
//  Copyright © 2017-2019 Claude Pruneau. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TROOT.h>
#include <TMath.h>
#include "TRandom.h"
#include "ParticleFilter.hpp"
#include "ParticlePairFilter.hpp"
#include "BWModelConfiguration.hpp"
#include "BWModelEventGenerator.hpp"
#include "GeneralizedGaussianConfiguration.hpp"
#include "AnalysisConfiguration.hpp"
#include "GenericEventAnalyzer.hpp"
#include "EventLoop.hpp"
#include "CanvasConfiguration.hpp"
#include "GraphConfiguration.hpp"
#include "MasterAnalysis.hpp"

////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void setDefaultOptions(int colorPalette=0)
{
  switch (colorPalette)
  {
    default:
    case 0:
      break;
    case 1:
      gStyle->SetPalette(1,0);
      break;
    case 7:
      gStyle->SetPalette(7,0);
      break;
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
void runBWModel(long nEventsRequested=50000)
{
  cout << "-INFO- runbwModelAnalysis() Setup configuration" << endl;

  // ---------------------------
  // debugLevel Levels
  // ---------------------------
  bool debugLoop      = true;
  bool debugGenerator = false;
  bool debugTrackEffects = false;
  bool debugAnalyzer  = false;

  // ---------------------------
  // Graphics parameters
  // ---------------------------
  setDefaultOptions(0);

  // ---------------------------
  // Histogram parameters
  // ---------------------------

  AnalysisConfiguration * ac = new AnalysisConfiguration();
  ac->outputPath               = "./BW_RESULTS/";
  ac->baseName                 = "BW_";
  ac->configFileName           = "Config.txt";
  ac->rootOutputFileName       = "Histograms.root";

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
  ac->max_phi     = TMath::TwoPi();

  ac->nBins_DeltaPlong = 40;
  ac->min_DeltaPlong   = -1.0;
  ac->max_DeltaPlong   =  1.0;
  ac->nBins_DeltaPside = 40;
  ac->min_DeltaPside   = -1.0;
  ac->max_DeltaPside   =  1.0;
  ac->nBins_DeltaPout  = 40;
  ac->min_DeltaPout    = -1.0;
  ac->max_DeltaPout    =  1.0;

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

  ac->particleFilter1      = new ParticleFilter( 1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y); // +ve only
  ac->particleFilter2      = new ParticleFilter(-1,ac->min_pt+0.001,ac->max_pt,ac->min_eta,ac->max_eta, ac->min_y,ac->max_y); // -ve only
  ac->particlePairFilter12 = new ParticlePairFilter(0.0, 100.0,0.0, TMath::TwoPi(),-6.0, 6.0);

  ac->canvasConfiguration1D  = new CanvasConfiguration(1);
  ac->graphConfiguration1D   = new GraphConfiguration(1,1);
  ac->canvasConfiguration2D  = new CanvasConfiguration(0);
  ac->graphConfiguration2D   = new GraphConfiguration(2);

  // ==================================================================================
  // Blast Wave Model Generator Parameters
  // ==================================================================================
  BWModelConfiguration *bwc = new BWModelConfiguration(7,"BWModel");

  bwc->totalMultiplicityMean = 426.0;  /* ALICE 30-40% central events */
  bwc->totalMultiplicitySigma = 15.0;  /* ALICE 30-40% central events */
  bwc->pT_Min = 0.0;
  bwc->pT_Max = 3.0;

  bwc->useAllKinds = true;
  bwc->useBGBlastWave = true;
  bwc->temperatureAllKinds = 0.106;    /* ALICE 30-40% central events */
  bwc->betaAvgBGBW = 0.604;            /* ALICE 30-40% central events */
  bwc->betaProfileBGBW = 0.841;        /* ALICE 30-40% central events */
  bwc->longitudinalFlow = 0.0;         /* a guess */

  /* azimuthal flow */
  bwc->useFlow = true;
  bwc->ellipticFlowAllKinds = 0.09;
  bwc->triangularFlowAllKinds = 0.03;

  // ==================================================================================
  // Track and pair effects processing
  // ==================================================================================
  GeneralizedGaussianConfiguration *tpc = new GeneralizedGaussianConfiguration(1,"2DGeneralizedGaussian");
  tpc->amplitude = 0.0144;
  tpc->gammaeta = 1.8529;
  tpc->gammaphi = 2.0560;
  tpc->omegaeta = 1.2385;
  tpc->omegaphi = 0.6946;

//  AMPT ON OFF 30-40%
//  [35.0,[[0.913777,0.006357],[0.484324,0.000643],[0.014447,0.000158],[1.238510,0.006263],[0.694564,0.000709],\
//  [1.852866,0.006611],[2.056014,0.003298],[0.014666,0.000013],[0.005334,0.000009],[0.000290,0.000008],\
//  [0.000000,0.000001],[0.000000,0.000000],[0.000000,0.000000]]],\


  MasterAnalysis * analysis = new MasterAnalysis(bwc,tpc,ac, debugLoop, debugGenerator,debugTrackEffects,debugAnalyzer);
  analysis->run(nEventsRequested);
}
