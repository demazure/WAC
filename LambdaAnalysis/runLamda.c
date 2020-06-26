//
//  runLamda.c
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
      gStyle->SetPalette(55,0);
    case 2:
      gStyle->SetPalette(56,0);
    case 3:
      gStyle->SetPalette(51,0);  //deep sea
    case 4:
      gStyle->SetPalette(52,0);  //atlantic
    case 5:
      gStyle->SetPalette(53,0);  //cmyk
    case 6:
      gStyle->SetPalette(54,0);  //island
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
void runLamda(long nEventsRequested=500000000)
{
  cout << "-INFO- runClusterModelAnalysis() Setup configuration" << endl;

  // ---------------------------
  // debugLevel Levels
  // ---------------------------
  bool debugLoop      = false;
  bool debugGenerator = false;
  bool debugAnalyzer  = false;

  // ---------------------------
  // Graphics parameters
  // ---------------------------
  setDefaultOptions(1);

  // ---------------------------
  // Histogram parameters
  // ---------------------------

  AnalysisConfiguration * ac = new AnalysisConfiguration();
  ac->outputPath               = "../Lamda_paper/0p2_2p0_ncls120/";
  ac->baseName                 = "Lamda_0550";
  ac->configFileName           = "Config_0550.txt";
  ac->rootOutputFileName       = "Lamda_Histograms_0550.root";
    cout<<"================================================================"<<endl;
    cout<<""<<endl;
    cout<<"         running for = "<<ac->baseName<<endl;
    cout<<""<<endl;
    cout<<"================================================================"<<endl;
  ac->nBins_nCell = 100;
  ac->min_nCell   = 0.0;
  ac->max_nCell   = 100.0;
  ac->nBins_nPartPerCell = 100;
  ac->min_nPartPerCell   = 0.0;
  ac->max_nPartPerCell   = 100.0;

  ac->nBins_pt    = 14;
  ac->min_pt      = 0.2;
  ac->max_pt      = 3.0;
  ac->nBins_eta   = 10;
  ac->min_eta     = -0.5;
  ac->max_eta     = 0.5;
  ac->nBins_y     = 10;
  ac->min_y       = -0.5;
  ac->max_y       = 0.5;
  ac->nBins_phi   = 36;
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
  ac->plotEventHistos     = true;     // global event stuff

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
 GeneratorConfiguration * mc = new GeneratorConfiguration(6,"LamdaModel");
  // ---------------------------
  // Cell Parameters
  // ---------------------------
  mc->impactParameterMinimum     = 5.0;
  mc->impactParameterMaximum     =50.0;
  mc->ptMinimum = 0.15;
  mc->ptMaximum = 3.5;
  mc->yMinimum = -0.6;
  mc->yMaximum =  0.6;

  MasterAnalysis * analysis = new MasterAnalysis(mc,ac, debugLoop, debugGenerator,debugAnalyzer);
  analysis->run(nEventsRequested);
}
