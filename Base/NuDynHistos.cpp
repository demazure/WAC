//
//  NuDynHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//
#include "NuDynHistos.hpp"
ClassImp(NuDynHistos);

NuDynHistos::NuDynHistos(const TString & name,
                         AnalysisConfiguration * configuration,
                         LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel)
{
  initialize();
}

NuDynHistos::NuDynHistos(TFile * inputFile,
                         const TString & name,
                         AnalysisConfiguration * configuration,
                         LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel)
{
  loadHistograms(inputFile);
}

NuDynHistos::~NuDynHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void NuDynHistos::createHistograms()
{
  AnalysisConfiguration & ac = *getConfiguration();
  TString bn = getHistoBaseName();
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
  h_events   = createHistogram(bn+TString("Nevents"),1,ac.min_mult,  ac.max_mult,  "mult","n_{Events}");
  h_f1_1     = createProfile(bn+TString("f1_1"),     1,ac.min_mult,  ac.max_mult,  "mult","<n_{1}>");
  h_f1_2     = createProfile(bn+TString("f1_2"),     1,ac.min_mult,  ac.max_mult,  "mult","<n_{2}>");
  h_f2_11    = createProfile(bn+TString("f2_11"),    1,ac.min_mult,  ac.max_mult,  "mult","<n_{1}(n_{1}-1)>");
  h_f2_22    = createProfile(bn+TString("f2_22"),    1,ac.min_mult,  ac.max_mult,  "mult","<n_{2}(n_{2}-1)>");
  h_f2_12    = createProfile(bn+TString("f2_12"),    1,ac.min_mult,  ac.max_mult,  "mult","<n_{1}n_{2}>");

   if (ac.nuDynVsMult)
   {
   h_events_vsMult   = createHistogram(bn+TString("Nevents_vsMult"),ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","n_{Events}");
   h_f1_1_vsMult     = createProfile(bn+TString("f1_1_vsMult"),   ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{1}>");
   h_f1_2_vsMult     = createProfile(bn+TString("f1_2_vsMult"),   ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{2}>");
   h_f2_11_vsMult    = createProfile(bn+TString("f2_11_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{1}(n_{1}-1)>");
   h_f2_22_vsMult    = createProfile(bn+TString("f2_22_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{2}(n_{2}-1)>");
   h_f2_12_vsMult    = createProfile(bn+TString("f2_12_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{1}n_{2}>");
   }

   if (ac.nuDynVsCent)
   {
   h_events_vsCent   = createHistogram(bn+TString("Nevents_vsCent"),ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","n_{Events}");
   h_f1_1_vsCent     = createProfile(bn+TString("f1_1_vsCent"),   ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{1}>");
   h_f1_2_vsCent     = createProfile(bn+TString("f1_2_vsCent"),   ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{2}>");
   h_f2_11_vsCent    = createProfile(bn+TString("f2_11_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{1}(n_{1}-1)>");
   h_f2_22_vsCent    = createProfile(bn+TString("f2_22_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{2}(n_{2}-1)>");
   h_f2_12_vsCent    = createProfile(bn+TString("f2_12_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{1}n_{2}>");
   }


}

//________________________________________________________________________
void NuDynHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "-Fatal- Attempting to load NuDynHistos from an invalid file pointer" << endl;
    return;
    }
  AnalysisConfiguration & ac = *getConfiguration();
  TString  bn = getHistoBaseName();
  h_events    = loadH1(inputFile,bn+TString("Nevents"));
   h_f1_1     = loadProfile(inputFile,bn+TString("f1_1"));
   h_f1_2     = loadProfile(inputFile,bn+TString("f1_2"));
   h_f2_11    = loadProfile(inputFile,bn+TString("f2_11"));
   h_f2_22    = loadProfile(inputFile,bn+TString("f2_22"));
   h_f2_12    = loadProfile(inputFile,bn+TString("f2_12"));

  if (ac.nuDynVsMult)
    {
    h_events_vsMult   = loadH1(inputFile,bn+TString("Nevents_vsMult"));
    h_f1_1_vsMult     = loadProfile(inputFile,bn+TString("f1_1_vsMult"));
    h_f1_2_vsMult     = loadProfile(inputFile,bn+TString("f1_2_vsMult"));
    h_f2_11_vsMult    = loadProfile(inputFile,bn+TString("f2_11_vsMult"));
    h_f2_22_vsMult    = loadProfile(inputFile,bn+TString("f2_22_vsMult"));
    h_f2_12_vsMult    = loadProfile(inputFile,bn+TString("f2_12_vsMult"));
    }

  if (ac.nuDynVsCent)
    {
    h_events_vsCent   = loadH1(inputFile,bn+TString("Nevents_vsCent"));
    h_f1_1_vsCent     = loadProfile(inputFile,bn+TString("f1_1_vsCent"));
    h_f1_2_vsCent     = loadProfile(inputFile,bn+TString("f1_2_vsCent"));
    h_f2_11_vsCent    = loadProfile(inputFile,bn+TString("f2_11_vsCent"));
    h_f2_22_vsCent    = loadProfile(inputFile,bn+TString("f2_22_vsCent"));
    h_f2_12_vsCent    = loadProfile(inputFile,bn+TString("f2_12_vsCent"));
    }
}


void NuDynHistos::fill(double mult, double cent, double n1, double n2, double weight)
{
  double f2_11 = n1*(n1-1);
  double f2_12 = n1*n2;
  double f2_22 = n2*(n2-1);
  AnalysisConfiguration & ac = *getConfiguration();
  if (ac.nuDynVsMult)
    {
    h_events->Fill(mult, weight);
    h_f1_1->Fill(mult, n1, weight);
    h_f1_2->Fill(mult, n2, weight);
    h_f2_11->Fill(mult, f2_11, weight);
    h_f2_22->Fill(mult, f2_22, weight);
    h_f2_12->Fill(mult, f2_12, weight);

    h_events_vsMult ->Fill(mult, weight);
    h_f1_1_vsMult   ->Fill(mult, n1,    weight);
    h_f1_2_vsMult   ->Fill(mult, n2,    weight);
    h_f2_11_vsMult  ->Fill(mult, f2_11, weight);
    h_f2_22_vsMult  ->Fill(mult, f2_22, weight);
    h_f2_12_vsMult  ->Fill(mult, f2_12, weight);
    }
  if (ac.nuDynVsCent)
    {
    h_events_vsCent->Fill(cent, weight);
    h_f1_1_vsCent->Fill(cent, n1, weight);
    h_f1_2_vsCent->Fill(cent, n1, weight);
    h_f2_11_vsCent->Fill(cent, f2_11, weight);
    h_f2_22_vsCent->Fill(cent, f2_22, weight);
    h_f2_12_vsCent->Fill(cent, f2_12, weight);

    h_events_vsCent ->Fill(cent, weight);
    h_f1_1_vsCent   ->Fill(cent, n1,    weight);
    h_f1_2_vsCent   ->Fill(cent, n2,    weight);
    h_f2_11_vsCent  ->Fill(cent, f2_11, weight);
    h_f2_22_vsCent  ->Fill(cent, f2_22, weight);
    h_f2_12_vsCent  ->Fill(cent, f2_12, weight);
    }
}

