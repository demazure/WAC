// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining ClusterEventHistos
 */

#include "ClusterModelEventHistos.hpp"
ClassImp(ClusterModelEventHistos);


ClusterModelEventHistos::ClusterModelEventHistos(const TString & name,
                         AnalysisConfiguration * configuration,
                         LogLevel  debugLevel)
 :
 Histograms(name, configuration, 5, debugLevel)
 {
 if (reportDebug()) cout << "ClusterModelEventHistos::CTOR() Started." << endl;
 initialize();
 if (reportDebug()) cout << "ClusterModelEventHistos::CTOR() Completed." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // CTOR2
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 ClusterModelEventHistos::ClusterModelEventHistos(TFile * inputFile,
                         const TString & name,
                         AnalysisConfiguration * configuration,
                         LogLevel  debugLevel)
 :
 Histograms(name,configuration,5,debugLevel)
 {
 if (reportDebug()) cout << "ClusterModelEventHistos::CTOR2() " << endl;
 loadHistograms(inputFile);
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // DTOR
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 ClusterModelEventHistos::~ClusterModelEventHistos()
 {
 if (reportDebug()) cout << "ClusterModelEventHistos::DTOR() No ops." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Initialize the cluster histograms
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
  void ClusterModelEventHistos::initialize()
 {
 if (reportDebug()) cout << "EventHistos::initialize() Started." << endl;
 AnalysisConfiguration & ac = *getConfiguration();
 TString baseName = ac.histoBaseName;
 h_nCell      = createHistogram(baseName + "nCells",    100,   0.0,  100.0,  "n_{cell}",     "Counts",scaled, saved, notPlotted, notPrinted);
 h_nClusters  = createHistogram(baseName + "nClusters", 1000,  0.0,  1000.0, "n_{clusters}", "Counts",scaled, saved, notPlotted, notPrinted);
 h_nParticles = createHistogram(baseName + "nParticles",1000,  0.0,  1000.0, "n_{particles}","Counts",scaled, saved, notPlotted, notPrinted);
 if (reportDebug()) cout << "EventHistos::initialize() Completed." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // load the cluster histograms from the given file and base name
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 void ClusterModelEventHistos::loadHistograms(TFile * inputFile)
 {
 if (reportDebug()) cout << "EventHistos::loadHistograms(...) Started." << endl;
 TString & baseName = getConfiguration()->histoBaseName;
 h_nCell     =  loadH1(inputFile,baseName + "nCells");
 h_nClusters =  loadH1(inputFile,baseName + "nClusters");
 h_nParticles = loadH1(inputFile,baseName + "nParticles");
 if (reportDebug()) cout << "EventHistos::loadHistograms(...) Completed." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // fill the cluster histograms with the given cluster
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 void ClusterModelEventHistos::fill(ClusterModelEvent * event, double weight)
 {
 h_nCell    ->Fill(double(event->nCells),      weight);
 h_nClusters->Fill(double(event->nClusters),   weight);
 h_nParticles->Fill(double(event->nParticles), weight);
 }

