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

 Class defining ClusterHistos
 */

#include "ClusterHistos.hpp"
ClassImp(ClusterHistos);

////////////////////////////////////////////////////////////////////////////////////////////////////////
 // CTOR1
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 ClusterHistos::ClusterHistos(const TString & name,
               AnalysisConfiguration * configuration,
               LogLevel  debugLevel)
 :
 Histograms(name,configuration,10,debugLevel),
 h_cluster_Npart(0),
 h_cluster_Pt(0),
 h_cluster_Phi(0),
 h_cluster_Y(0),
 h_cluster_Mass(0)
 {
 if (reportDebug()) cout << "ClusterHistos::CTOR1() Started." << endl;
 initialize();
 if (reportDebug()) cout << "ClusterHistos::CTOR1() Completed." << endl;
 }

 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // CTOR2
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ClusterHistos::ClusterHistos(TFile * inputFile,
               const TString & name,
               AnalysisConfiguration * configuration,
               LogLevel  debugLevel)
 :
 Histograms(name,configuration,100,debugLevel),
 h_cluster_Npart(0),
 h_cluster_Pt(0),
 h_cluster_Phi(0),
 h_cluster_Y(0),
 h_cluster_Mass(0)
 {
 if (reportDebug()) cout << "ClusterHistos::CTOR2() Started" << endl;
 loadHistograms(inputFile);
 if (reportDebug()) cout << "ClusterHistos::CTOR2() Completed" << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // DTOR
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 ClusterHistos::~ClusterHistos()
 {
 if (reportDebug()) cout << "ClusterHistos::DTOR() No ops." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Initialize the cluster histograms
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
  void ClusterHistos::createHistograms()
 {
 if (reportDebug()) cout << "ClusterHistos::initialize() Started." << endl;
 AnalysisConfiguration & ac = *(AnalysisConfiguration*) getConfiguration();
 TString bn = getHistoBaseName();
 TString yieldLabel = "Counts";
 h_cluster_Npart = createHistogram(bn+"ClusterNpart",ac.nBins_nPartTot,ac.min_nPartTot,ac.max_nPartTot,"Cluster N",    yieldLabel, scaled, saved, notPlotted, notPrinted);
 h_cluster_Pt    = createHistogram(bn+"ClusterPt",   ac.nBins_pt,      ac.min_pt,      ac.max_pt,      "Cluster Pt",   yieldLabel, scaled, saved, notPlotted, notPrinted);
 h_cluster_Phi   = createHistogram(bn+"ClusterPhi",  ac.nBins_phi,     ac.min_phi,     ac.max_phi,     "Cluster #phi", yieldLabel, scaled, saved, notPlotted, notPrinted);
 h_cluster_Y     = createHistogram(bn+"ClusterY",    ac.nBins_eta,     ac.min_eta,     ac.max_eta,     "Cluster Y",    yieldLabel, scaled, saved, notPlotted, notPrinted);
 h_cluster_Mass  = createHistogram(bn+"ClusterMass", 200, 0.0, 2.0, "Cluster M", yieldLabel,scaled, saved, notPlotted, notPrinted);

 if (ac.fillQ3D)
   {
   h_cluster_Q3D   = createHistogram(bn+TString("n2_Q3D_Clus"),ac.nBins_DeltaPlong, ac.min_DeltaPlong, ac.max_DeltaPlong,
                                ac.nBins_DeltaPside, ac.min_DeltaPside, ac.max_DeltaPside,ac.nBins_DeltaPout,  ac.min_DeltaPout,  ac.max_DeltaPout,
                                "Q_{long} (GeV/c)", "Q_{side}  (GeV/c)", "Q_{out}  (GeV/c)","Yield", scaled, saved, notPlotted, notPrinted);
   }


 if (reportDebug()) cout << "ClusterHistos::initialize() Completed." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // load the cluster histograms from the given file and base name
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 void ClusterHistos::loadHistograms(TFile * inputFile)
 {
  if (reportDebug()) cout << "ClusterHistos::loadHistograms(...) Started." << endl;
 TString bn = getHistoBaseName();
 h_cluster_Npart = loadH1(inputFile,bn+"ClusterNpart");
 h_cluster_Pt    = loadH1(inputFile,bn+"ClusterPt");
 h_cluster_Phi   = loadH1(inputFile,bn+"ClusterPhi");
 h_cluster_Y     = loadH1(inputFile,bn+"ClusterY");
 h_cluster_Mass  = loadH1(inputFile,bn+"ClusterMass");
  if (reportDebug()) cout << "ClusterHistos::loadHistograms(...) Completed." << endl;
 }

 ////////////////////////////////////////////////////////////////////////////////////////////////////////
 // fill the cluster histograms with the given cluster
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
  void ClusterHistos::fill(Cluster * cluster, double weight)
 {
 h_cluster_Npart ->Fill(double(cluster->nParticles));
 h_cluster_Pt    ->Fill(cluster->p.Pt());
 h_cluster_Phi   ->Fill(cluster->p.Phi());
 h_cluster_Y     ->Fill(cluster->p.Rapidity());
 h_cluster_Mass  ->Fill(cluster->p.M());

 AnalysisConfiguration & ac = *(AnalysisConfiguration*) getConfiguration();
 if (ac.fillQ3D)
   {
   h_cluster_Q3D->Fill(cluster->Qlong, cluster->Qside, cluster->Qout, weight);
   }
 }


  void ClusterHistos::finalize()
 {
  if (reportDebug()) cout << "ClusterHistos::loadHistograms(...) Started." << endl;
 AnalysisConfiguration & ac = *(AnalysisConfiguration*) getConfiguration();
 if (ac.fillQ3D)
   {
   h_cluster_Q3D_xy = (TH2 *) h_cluster_Q3D->Project3D("xy");     addToList(h_cluster_Q3D_xy, notScaled, saved, plotted, printed);
   h_cluster_Q3D_xz = (TH2 *) h_cluster_Q3D->Project3D("xz");     addToList(h_cluster_Q3D_xz, notScaled, saved, plotted, printed);
   h_cluster_Q3D_yz = (TH2 *) h_cluster_Q3D->Project3D("yz");     addToList(h_cluster_Q3D_yz, notScaled, saved, plotted, printed);
   }
  if (reportDebug()) cout << "ClusterHistos::loadHistograms(...) Completed." << endl;
 }
