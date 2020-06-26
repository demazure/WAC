// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/

#ifndef WAC_ClusterHistos
#define WAC_ClusterHistos
#include "Histograms.hpp"
#include "Cluster.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cluster Histograms
////////////////////////////////////////////////////////////////////////////////////////////////////////
class ClusterHistos : public Histograms
{
public:
  
  ClusterHistos(const TString & name,
                AnalysisConfiguration * configuration,
                LogLevel  debugLevel);
  ClusterHistos(TFile * inputFile,
                const TString & collectionName,
                AnalysisConfiguration * analysisConfiguration,
                LogLevel  debugLevel);
  virtual ~ClusterHistos();
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  virtual void fill(Cluster * cluster, double weight=1);
  virtual void finalize();

  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members -- Computed
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  TH1 * h_cluster_Npart;
  TH1 * h_cluster_Pt;
  TH1 * h_cluster_Phi;
  TH1 * h_cluster_Y;
  TH1 * h_cluster_Mass;
  TH3 * h_cluster_Q3D;
  TH2 * h_cluster_Q3D_xy;
  TH2 * h_cluster_Q3D_xz;
  TH2 * h_cluster_Q3D_yz;

    ClassDef(ClusterHistos,0)
};

#endif /* WAC_ClusterHistos */
