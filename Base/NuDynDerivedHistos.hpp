// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_NuDynDerivedHistos
#define WAC_NuDynDerivedHistos
#include "Histograms.hpp"
#include "NuDynHistos.hpp"


class NuDynDerivedHistos : public Histograms
{
public:

  NuDynDerivedHistos(const TString & collectionName,
                      AnalysisConfiguration * analysisConfiguration,
                      LogLevel  debugLevel);
  NuDynDerivedHistos(TFile * inputFile,
                      const TString & collectionName,
                      AnalysisConfiguration * analysisConfiguration,
                      LogLevel  debugLevel);
  virtual ~NuDynDerivedHistos();
  void createHistograms();
  void loadHistograms(TFile * inputFile);
  void calculateDerivedHistograms(NuDynHistos * source);

  void calculateR2(double f1,double ef1,double f2,double ef2,double & r2,double & er2);
  void calculateR2(double f1_1,double ef1_1,double f1_2,double ef1_2,double f2,double ef2,double & r2,double & er2);
  void calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn);
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
  TH1 * h_r2_11;
  TH1 * h_r2_22;
  TH1 * h_r2_12;
  TH1 * h_nudyn_12;

  // vs Mult
  TH1 * h_r2_11_vsMult;
  TH1 * h_r2_22_vsMult;
  TH1 * h_r2_12_vsMult;
  TH1 * h_nudyn_12_vsMult;

  // vs Cent
  TH1 * h_r2_11_vsCent;
  TH1 * h_r2_22_vsCent;
  TH1 * h_r2_12_vsCent;
  TH1 * h_nudyn_12_vsCent;

  ClassDef(NuDynDerivedHistos,0)
};

#endif /* WAC_NuDynDerivedHistos  */



