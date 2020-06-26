// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_NuDynHistos
#define WAC_NuDynHistos
#include "Histograms.hpp"

class NuDynHistos : public Histograms
{
public:

  NuDynHistos(const TString & collectionName,
                      AnalysisConfiguration * analysisConfiguration,
                      LogLevel  debugLevel);
  NuDynHistos(TFile * inputFile,
                      const TString & collectionName,
                      AnalysisConfiguration * analysisConfiguration,
                      LogLevel  debugLevel);
  virtual ~NuDynHistos();
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void fill(double mult, double cent, double n1, double n2,  double weight);


  ////////////////////////////////////////////////////////////////////////////
  // Data Members - Histograms
  ////////////////////////////////////////////////////////////////////////////
  // n1 number of particles satisfying filter 1
  // n2 number of particles satisfying filter 2
  // h_f1_1 = <n1>
  // h_f1_2 = <n2>
  // h_f2_11 = <n1(n1-1)>
  // h_f2_22 = <n2(n2-1)>
  // h_f2_12 = <n2(n2-1)>
  // h_r2_11 = <n1(n1-1)>/<n1><n1>
  // h_r2_22 = <n2(n2-1)>/<n2><n2>
  // h_r2_12 = <n1(n2-1)>/<n1><n2>
  // h_nudyn_12 = h_r2_11 + h_r2_22 -2*h_r2_12


  // Min bias
  TH1 * h_events;
  TProfile * h_f1_1;
  TProfile * h_f1_2;
  TProfile * h_f2_11;
  TProfile * h_f2_22;
  TProfile * h_f2_12;

  // vs Mult
   TH1 * h_events_vsMult;
  TProfile * h_f1_1_vsMult;
  TProfile * h_f1_2_vsMult;
  TProfile * h_f2_11_vsMult;
  TProfile * h_f2_22_vsMult;
  TProfile * h_f2_12_vsMult;

  // vs Mult
  TH1 * h_events_vsCent;
  TProfile * h_f1_1_vsCent;
  TProfile * h_f1_2_vsCent;
  TProfile * h_f2_11_vsCent;
  TProfile * h_f2_22_vsCent;
  TProfile * h_f2_12_vsCent;

  ClassDef(NuDynHistos,0)
};

#endif /* WAC_NuDynHistos  */



