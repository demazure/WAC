// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class CellHistos
 \ingroup WAC

 Class defining CellHistos
 */

#include "CellHistos.hpp"
ClassImp(CellHistos);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTOR1
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CellHistos::CellHistos(const TString & name,
                       AnalysisConfiguration * configuration,
                       LogLevel  debugLevel)
:
Histograms(name,configuration,20,debugLevel),
hCellNclus (0),
hCellNpart (0),
hCellPt    (0),
hCellPhi   (0),
hCellY     (0),
hCellMass  (0),
hCellBeta  (0)
{
  if (reportDebug()) cout << "-DEBUG- CellHistos::CTOR() Started" << endl;
  initialize();
  if (reportDebug()) cout << "-DEBUG- CellHistos::CTOR() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTOR2
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CellHistos::CellHistos(TFile * inputFile,
                       const TString & name,
                       AnalysisConfiguration * configuration,
                       LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel),
hCellNclus (0),
hCellNpart (0),
hCellPt    (0),
hCellPhi   (0),
hCellY     (0),
hCellMass  (0),
hCellBeta  (0)
{
  if (reportDebug()) cout << "-DEBUG- CellHistos::CTOR() Started" << endl;
  loadHistograms(inputFile);
  if (reportDebug()) cout << "-DEBUG- CellHistos::CTOR() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellHistos::initialize()
{
  if (reportDebug()) cout << "-DEBUG- CellHistos::initialize() Started" << endl;
  AnalysisConfiguration & ac = *(AnalysisConfiguration*) getConfiguration();
  TString & name = ac.histoBaseName;
  TString yieldLabel = "Counts";
  hCellNclus = createHistogram(name+ "CellNclus",ac.nBins_nCell,        ac.min_nCell,        ac.max_nCell,       "Cell N Cluster",  yieldLabel,scaled, saved, notPlotted, notPrinted);
  hCellNpart = createHistogram(name+ "CellNpart",ac.nBins_nPartPerCell, ac.min_nPartPerCell, ac.max_nPartPerCell,"Cell N Particles",yieldLabel,scaled, saved, notPlotted, notPrinted);
  hCellPt    = createHistogram(name+ "CellPt",   ac.nBins_pt,           ac.min_pt,           ac.max_pt,          "Cell Pt",         yieldLabel,scaled, saved, notPlotted, notPrinted);
  hCellPhi   = createHistogram(name+ "CellPhi",  ac.nBins_phi,          ac.min_phi,          ac.max_phi,         "Cell #phi",       yieldLabel,scaled, saved, notPlotted, notPrinted);
  hCellY     = createHistogram(name+ "CellY",    ac.nBins_eta,          ac.min_eta,          ac.max_eta,         "Cell Y",          yieldLabel,scaled, saved, notPlotted, notPrinted);
  hCellMass  = createHistogram(name+ "CellMass", 100, 0.0, 10.0,   "Cell M",     yieldLabel,scaled, saved, notPlotted, notPrinted);
  hCellBeta  = createHistogram(name+ "CellBeta", 100, 0.0, 1.0,    "Cell #beta", yieldLabel,scaled, saved, notPlotted, notPrinted);
  if (reportDebug()) cout << "-DEBUG- CellHistos::initialize() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load Cell histograms from the given file
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellHistos::loadHistograms(TFile * inputFile)
{
  if (reportDebug()) cout << "CellHistos::loadHistograms() Started" << endl;
  AnalysisConfiguration & ac = *(AnalysisConfiguration*) getConfiguration();
  TString & baseName = ac.histoBaseName;
  hCellNclus = loadH1(inputFile,baseName + "CellNclus");
  hCellNpart = loadH1(inputFile,baseName + "CellNpart");
  hCellPt    = loadH1(inputFile,baseName + "CellPt");
  hCellPhi   = loadH1(inputFile,baseName + "CellPhi");
  hCellY     = loadH1(inputFile,baseName + "CellY");
  hCellMass  = loadH1(inputFile,baseName + "CellMass");
  hCellBeta  = loadH1(inputFile,baseName + "CellBeta");
  if (reportDebug()) cout << "CellHistos::loadHistograms() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fill cell histograms with the given cell information
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellHistos::fill(Cell * cell)
{
  hCellNclus ->Fill(double(cell->nClusters));
  hCellNpart ->Fill(double(cell->nParticles));
  hCellPhi   ->Fill(cell->phi);
  hCellBeta  ->Fill(cell->beta);
}
