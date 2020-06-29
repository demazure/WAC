
#include "TString.h"
#include "TFile.h"
#include "AnalysisConfiguration.hpp"
#include "NuDynHistos.hpp"
#include "NuDynDerivedHistos.hpp"

void PlotNuDynHistos()
{

  int nFiles = 4;

  TString ** inputFileNames          = new TString*[nFiles];
  TFile **   inputFiles              = new TFile*[nFiles];
  NuDynHistos ** nuDyn               = new NuDynHistos*[nFiles];
  NuDynDerivedHistos ** nuDynDerived = new NuDynDerivedHistos*[nFiles];


  AnalysisConfiguration * ac = new AnalysisConfiguration("Ampt","Ampt","1.0");
  ac->loadHistograms  = false;
  ac->createHistograms  = false;
  ac->scaleHistograms  = false;
  ac->calculateDerivedHistograms  = false;
  ac->saveHistograms  = false;
  ac->resetHistograms  = false;
  ac->clearHistograms  = false;
  ac->forceHistogramsRewrite  = false;

  ac->configurationFileName = "configuration";
  ac->rootInputFileName = "";
  ac->outputPath = "./";
  ac->rootOuputFileName =  "output";
  ac->histoBaseName =  "ampt";

  ac->fillPairs        = true;
  ac->fill3D           = false;
  ac->fill6D           = false;
  ac->fillQ3D          = false;
  ac->fillY            = true;

  ac->nuDynVsMult     = true;
   ac->nuDynVsCent     = false;

  TString inputPath = "./";
  TString baseName  = "output";
  TString model     = "Ampt";

  TString fileName  = baseName+model+"_";
  inputFileNames[0] = new TString(fileName+"NuDyn_ALL_MB.root");
  inputFileNames[1] = new TString(fileName+"NuDyn_PiPi_MB.root");
  inputFileNames[2] = new TString(fileName+"NuDyn_KK_MB.root");
  inputFileNames[3] = new TString(fileName+"NuDyn_PP_MB.root");

  for (int iFile=0; iFile<nFiles; iFile++)
    {
    inputFiles[iFile]  = new TFile( *inputFileNames[iFile] );
    if (!inputFiles[iFile])
      {
      cout << "<ERROR> PlotNuDynHistos() Unable to open root file " << *inputFileNames[iFile] << endl;
      return;
      }
    nuDyn[iFile] = NuDynHistos(inputFiles[iFile],"NuDyn", ac, MessageLogger::Info);


    }


  // Min bias
//  h_events   = createHistogram(bn+TString("Nevents"),1,ac.min_mult,  ac.max_mult,  "mult","n_{Events}");
//  h_f1_1     = createProfile(bn+TString("f1_1"),     1,ac.min_mult,  ac.max_mult,  "mult","<n_{1}>");
//  h_f1_2     = createProfile(bn+TString("f1_2"),     1,ac.min_mult,  ac.max_mult,  "mult","<n_{2}>");
//  h_f2_11    = createProfile(bn+TString("f2_11"),    1,ac.min_mult,  ac.max_mult,  "mult","<n_{1}(n_{1}-1)>");
//  h_f2_22    = createProfile(bn+TString("f2_22"),    1,ac.min_mult,  ac.max_mult,  "mult","<n_{2}(n_{2}-1)>");
//  h_f2_12    = createProfile(bn+TString("f2_12"),    1,ac.min_mult,  ac.max_mult,  "mult","<n_{1}n_{2}>");
//
//   if (ac.nuDynVsMult)
//   {
//   h_events_vsMult   = createHistogram(bn+TString("Nevents_vsMult"),ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","n_{Events}");
//   h_f1_1_vsMult     = createProfile(bn+TString("f1_1_vsMult"),   ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{1}>");
//   h_f1_2_vsMult     = createProfile(bn+TString("f1_2_vsMult"),   ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{2}>");
//   h_f2_11_vsMult    = createProfile(bn+TString("f2_11_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{1}(n_{1}-1)>");
//   h_f2_22_vsMult    = createProfile(bn+TString("f2_22_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{2}(n_{2}-1)>");
//   h_f2_12_vsMult    = createProfile(bn+TString("f2_12_vsMult"),  ac.nBins_mult,ac.min_mult,  ac.max_mult,  "mult","<n_{1}n_{2}>");
//   }
//
//   if (ac.nuDynVsCent)
//   {
//   h_events_vsCent   = createHistogram(bn+TString("Nevents_vsCent"),ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","n_{Events}");
//   h_f1_1_vsCent     = createProfile(bn+TString("f1_1_vsCent"),   ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{1}>");
//   h_f1_2_vsCent     = createProfile(bn+TString("f1_2_vsCent"),   ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{2}>");
//   h_f2_11_vsCent    = createProfile(bn+TString("f2_11_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{1}(n_{1}-1)>");
//   h_f2_22_vsCent    = createProfile(bn+TString("f2_22_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{2}(n_{2}-1)>");
//   h_f2_12_vsCent    = createProfile(bn+TString("f2_12_vsCent"),  ac.nBins_cent,ac.min_cent,  ac.max_cent,  "cent","<n_{1}n_{2}>");
//   }


}

