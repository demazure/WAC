//
//  NuDynDerivedHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "NuDynDerivedHistos.hpp"
ClassImp(NuDynDerivedHistos);

NuDynDerivedHistos::NuDynDerivedHistos(const TString & name,
                                         AnalysisConfiguration * configuration,
                                         LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel)
{
  initialize();
}

NuDynDerivedHistos::NuDynDerivedHistos(TFile * inputFile,
                               const TString & name,
                               AnalysisConfiguration * configuration,
                               LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel)
{
  loadHistograms(inputFile);
}

NuDynDerivedHistos::~NuDynDerivedHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void NuDynDerivedHistos::createHistograms()
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
  h_r2_11    = createHistogram(bn+TString("r2_11"),  1,ac.min_mult,  ac.max_mult,  "mult","<r_{2}^{11}>");
  h_r2_22    = createHistogram(bn+TString("r2_22"),  1,ac.min_mult,  ac.max_mult,  "mult","<r_{2}^{22}>");
  h_r2_12    = createHistogram(bn+TString("r2_12"),  1,ac.min_mult,  ac.max_mult,  "mult","<r_{2}^{12}>");
  h_nudyn_12 = createHistogram(bn+TString("nudyn_12"),  1,ac.min_mult,  ac.max_mult,  "mult","#nu_{dyn}^{12}>");

   if (ac.nuDynVsMult)
     {
     h_r2_11_vsMult    = createHistogram(bn+TString("r2_11_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<r_{2}^{11}>");
     h_r2_22_vsMult    = createHistogram(bn+TString("r2_22_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<r_{2}^{22}>");
     h_r2_12_vsMult    = createHistogram(bn+TString("r2_12_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<r_{2}^{12}>");
     h_nudyn_12_vsMult = createHistogram(bn+TString("nudyn_12_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","#nu_{dyn}^{12}>");
     }

  if (ac.nuDynVsCent)
    {
    h_r2_11_vsCent    = createHistogram(bn+TString("r2_11_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "Cent","<r_{2}^{11}>");
    h_r2_22_vsCent    = createHistogram(bn+TString("r2_22_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "Cent","<r_{2}^{22}>");
    h_r2_12_vsCent    = createHistogram(bn+TString("r2_12_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "Cent","<r_{2}^{12}>");
    h_nudyn_12_vsCent = createHistogram(bn+TString("nudyn_12_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "Cent","#nu_{dyn}^{12}>");
    }

}

//________________________________________________________________________
void NuDynDerivedHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "-Fatal- Attempting to load NuDynDerivedHistos from an invalid file pointer" << endl;
    return;
    }
  AnalysisConfiguration & ac = *getConfiguration();
  TString  bn = getHistoBaseName();
   h_r2_11    = loadH1(inputFile,bn+TString("r2_11"));
   h_r2_22    = loadH1(inputFile,bn+TString("r2_22"));
   h_r2_12    = loadH1(inputFile,bn+TString("r2_12"));
   h_nudyn_12 = loadH1(inputFile,bn+TString("nudyn_12"));

  if (ac.nuDynVsMult)
    {
    h_r2_11_vsMult    = loadH1(inputFile,bn+TString("r2_11_vsMult"));
    h_r2_22_vsMult    = loadH1(inputFile,bn+TString("r2_22_vsMult"));
    h_r2_12_vsMult    = loadH1(inputFile,bn+TString("r2_12_vsMult"));
    h_nudyn_12_vsMult = loadH1(inputFile,bn+TString("nudyn_12_vsMult"));
    }

  if (ac.nuDynVsCent)
    {
    h_r2_11_vsCent    = loadH1(inputFile,bn+TString("r2_11_vsCent"));
    h_r2_22_vsCent    = loadH1(inputFile,bn+TString("r2_22_vsCent"));
    h_r2_12_vsCent    = loadH1(inputFile,bn+TString("r2_12_vsCent"));
    h_nudyn_12_vsCent = loadH1(inputFile,bn+TString("nudyn_12_vsCent"));
    }

}

void NuDynDerivedHistos::calculateDerivedHistograms(NuDynHistos * source)
{
  // h_r2_11 = <n1(n1-1)>/<n1><n1>
  // h_r2_22 = <n2(n2-1)>/<n2><n2>
  // h_r2_12 = <n1(n2-1)>/<n1><n2>
  // h_nudyn_12 = h_r2_11 + h_r2_22 -2*h_r2_12

  AnalysisConfiguration & ac = *getConfiguration();

  double f1_1, f1_2, f2_11, f2_12, f2_22;
  double ef1_1, ef1_2, ef2_11, ef2_12, ef2_22;
  double r2_11, r2_12, r2_22, nudyn;
  double er2_11, er2_12, er2_22, enudyn;

  // min bias
  f1_1   = source->h_f1_1->GetBinContent(1);
  ef1_1  = source->h_f1_1->GetBinError(1);
  f1_2   = source->h_f1_2->GetBinContent(1);
  ef1_2  = source->h_f1_2->GetBinError(1);
  f2_11  = source->h_f2_11->GetBinContent(1);
  ef2_11 = source->h_f2_11->GetBinError(1);
  f2_12  = source->h_f2_12->GetBinContent(1);
  ef2_12 = source->h_f2_12->GetBinError(1);
  f2_22  = source->h_f2_22->GetBinContent(1);
  ef2_22 = source->h_f2_22->GetBinError(1);
  calculateR2(f1_1,ef1_1,f2_11,ef2_11,r2_11,er2_11);
  calculateR2(f1_1,ef1_1,f1_2,ef1_2,f2_12,ef2_12,r2_12,er2_12);
  calculateR2(f1_2,ef1_2,f2_22,ef2_22,r2_22,er2_22);
  calculateNudyn(r2_11,er2_11,r2_12,er2_12,r2_22,er2_22,nudyn,enudyn);
  h_r2_11->SetBinContent(1,r2_11);     h_r2_11->SetBinError(1,er2_11);
  h_r2_12->SetBinContent(1,r2_12);     h_r2_11->SetBinError(1,er2_12);
  h_r2_22->SetBinContent(1,r2_22);     h_r2_11->SetBinError(1,er2_22);
  h_nudyn_12->SetBinContent(1,nudyn);  h_nudyn_12->SetBinError(1,enudyn);

  if (ac.nuDynVsMult)
    {
    int nBins = source->h_f1_1_vsMult->GetNbinsX();
    for (int i=1; i<=nBins; i++)
      {
      f1_1   = source->h_f1_1_vsMult->GetBinContent(i);
      ef1_1  = source->h_f1_1_vsMult->GetBinError(i);
      f1_2   = source->h_f1_2_vsMult->GetBinContent(i);
      ef1_2  = source->h_f1_2_vsMult->GetBinError(i);
      f2_11  = source->h_f2_11_vsMult->GetBinContent(i);
      ef2_11 = source->h_f2_11_vsMult->GetBinError(i);
      f2_12  = source->h_f2_12_vsMult->GetBinContent(i);
      ef2_12 = source->h_f2_12_vsMult->GetBinError(i);
      f2_22  = source->h_f2_22_vsMult->GetBinContent(i);
      ef2_22 = source->h_f2_22_vsMult->GetBinError(i);
      calculateR2(f1_1,ef1_1,f2_11,ef2_11,r2_11,er2_11);
      calculateR2(f1_1,ef1_1,f1_2,ef1_2,f2_12,ef2_12,r2_12,er2_12);
      calculateR2(f1_2,ef1_2,f2_22,ef2_22,r2_22,er2_22);
      calculateNudyn(r2_11,er2_11,r2_12,er2_12,r2_22,er2_22,nudyn,enudyn);
      h_r2_11_vsMult->SetBinContent(i,r2_11);     h_r2_11_vsMult->SetBinError(i,er2_11);
      h_r2_12_vsMult->SetBinContent(i,r2_12);     h_r2_11_vsMult->SetBinError(i,er2_12);
      h_r2_22_vsMult->SetBinContent(i,r2_22);     h_r2_11_vsMult->SetBinError(i,er2_22);
      h_nudyn_12_vsMult->SetBinContent(i,nudyn);  h_nudyn_12_vsMult->SetBinError(i,enudyn);
      }
    }

  if (ac.nuDynVsCent)
    {
    int nBins = source->h_f1_1_vsCent->GetNbinsX();
    for (int i=1; i<=nBins; i++)
      {
      f1_1   = source->h_f1_1_vsCent->GetBinContent(i);
      ef1_1  = source->h_f1_1_vsCent->GetBinError(i);
      f1_2   = source->h_f1_2_vsCent->GetBinContent(i);
      ef1_2  = source->h_f1_2_vsCent->GetBinError(i);
      f2_11  = source->h_f2_11_vsCent->GetBinContent(i);
      ef2_11 = source->h_f2_11_vsCent->GetBinError(i);
      f2_12  = source->h_f2_12_vsCent->GetBinContent(i);
      ef2_12 = source->h_f2_12_vsCent->GetBinError(i);
      f2_22  = source->h_f2_22_vsCent->GetBinContent(i);
      ef2_22 = source->h_f2_22_vsCent->GetBinError(i);
      calculateR2(f1_1,ef1_1,f2_11,ef2_11,r2_11,er2_11);
      calculateR2(f1_1,ef1_1,f1_2,ef1_2,f2_12,ef2_12,r2_12,er2_12);
      calculateR2(f1_2,ef1_2,f2_22,ef2_22,r2_22,er2_22);
      calculateNudyn(r2_11,er2_11,r2_12,er2_12,r2_22,er2_22,nudyn,enudyn);
      h_r2_11_vsCent->SetBinContent(i,r2_11);     h_r2_11_vsCent->SetBinError(i,er2_11);
      h_r2_12_vsCent->SetBinContent(i,r2_12);     h_r2_11_vsCent->SetBinError(i,er2_11);
      h_r2_22_vsCent->SetBinContent(i,r2_22);     h_r2_11_vsCent->SetBinError(i,er2_11);
      h_nudyn_12_vsCent->SetBinContent(i,nudyn);  h_nudyn_12_vsCent->SetBinError(i,er2_11);
      }
    }
}

// calculation carried out assuming no correlation between the different factors...
void NuDynDerivedHistos::calculateR2(double f1,double ef1,double f2,double ef2,double & r2,double & er2)
{
  // calculate ratio and error for r2 = f2/f1/f1
  double ref1, ref2, rer2;
  if (f1>0)
    {
    ref1 = ef1/f1;
    ref2   = ef2/f2;
    rer2   = 4*ref1*ref1 + ref2*ref2;
    r2 = f2/(f1*f1) - 1.0;
    er2 = r2*sqrt(rer2);
    }
  else
    {
    r2 = 0.0;
    er2 = 0.0;
    }
}

 // calculation carried out assuming no correlation between the different factors...
 void NuDynDerivedHistos::calculateR2(double f1_1,double ef1_1,double f1_2,double ef1_2,double f2,double ef2,double & r2,double & er2)
 {
 // calculate ratio and error for r2 = f2/f1_1/f1_2

   double ref1_1, ref1_2, ref2, rer2;
   if (f1_1>0 & f1_2>0)
     {
     ref1_1 = ef1_1/f1_1;
     ref1_2 = ef1_2/f1_2;
     ref2   = ef2/f2;
     rer2   = ref1_1*ref1_1 + ref1_2*ref1_2 + ref2*ref2;
     r2 = f2/(f1_1*f1_2) - 1.0;
     er2 = r2*sqrt(rer2);
     }
   else
     {
     r2 = 0.0;
     er2 = 0.0;
     }
 }

void NuDynDerivedHistos::calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn)
{
  nudyn = r2_11 + r2_22 - 2.0*r2_12;
  enudyn = sqrt(er2_11*er2_11 + er2_22*er2_22 + 4.0*er2_12*er2_12);
}

