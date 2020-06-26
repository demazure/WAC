// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ClusterModelEventHistos
#define WAC_ClusterModelEventHistos
#include "Histograms.hpp"
#include "ClusterModelEvent.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cluster Histograms
////////////////////////////////////////////////////////////////////////////////////////////////////////
class ClusterModelEventHistos : public Histograms
{
public:
  
  ClusterModelEventHistos(const TString & name,
                          AnalysisConfiguration * configuration,
                          LogLevel  debugLevel);
  ClusterModelEventHistos(TFile * inputFile,
                          const TString & name,
                          AnalysisConfiguration * configuration,
                          LogLevel  debugLevel);
  ~ClusterModelEventHistos();
  void initialize();
  void loadHistograms(TFile * inputFile);
  void fill(ClusterModelEvent * event, double weight);

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members -- Inputs
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  int nBins_nParts; double min_nParts; double max_nParts;
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members -- Computed
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  TH1 * h_nCell;
  TH1 * h_nClusters;
  TH1 * h_nParticles;
  ClassDef(ClusterModelEventHistos,0)
};


#endif /* WAC_ClusterModelEventHistos */
