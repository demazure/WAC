
//#include "TString.h"
//#include "TFile.h"
//#include "AnalysisConfiguration.hpp"
//#include "NuDynHistos.hpp"
//#include "NuDynDerivedHistos.hpp"
//#include "CanvasConfiguration.hpp"
//#include "HistogramCollection.hpp"
//#include "GraphConfiguration.hpp"
//#include "CanvasConfiguration.hpp"
//#include "TRint.h"


//R__LOAD_LIBRARY(/Users/claudeapruneau/opt/WAC/lib/libBase.dylib)

void plot(int n, TH1** h, GraphConfiguration** g, HistogramCollection * hc, int rebin=1);


int PlotNuDynHistos()
//int  main()
{
  //gSystem->Load("/Users/claudeapruneau/opt/WAC/lib/libBase.dylib");

  TString includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/Base/";

  gSystem->Load(includesPath+"CanvasCollection.hpp");
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"TaskConfiguration.hpp");
  gSystem->Load(includesPath+"EnhancedGraph.hpp");
  gSystem->Load(includesPath+"Factory.hpp");
  gSystem->Load(includesPath+"FunctionCollection.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"HistogramCollection.hpp");
  gSystem->Load(includesPath+"Histograms.hpp");
  gSystem->Load(includesPath+"RandomGenerators.hpp");
  gSystem->Load(includesPath+"RapidityGenerator.hpp");
  gSystem->Load(includesPath+"Task.hpp");
  gSystem->Load(includesPath+"TaskCollection.hpp");
  gSystem->Load(includesPath+"Property.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
  gSystem->Load(includesPath+"AnalysisConfiguration.hpp");
  gSystem->Load(includesPath+"Event.hpp");
  gSystem->Load(includesPath+"EventFilter.hpp");
  gSystem->Load(includesPath+"EventHistos.hpp");
  gSystem->Load(includesPath+"EventLoop.hpp");
  gSystem->Load(includesPath+"GeneralizedGaussianConfiguration.hpp");
  gSystem->Load(includesPath+"GeneralizedGaussianProfile.hpp");
  gSystem->Load(includesPath+"GeneratorConfiguration.hpp");
  gSystem->Load(includesPath+"TwoPartCorrelationAnalyzer.hpp");
  gSystem->Load(includesPath+"Particle.hpp");
  gSystem->Load(includesPath+"ParticleFilter.hpp");
  gSystem->Load(includesPath+"ParticleHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairCombinedHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairDerivedHistos.hpp");
  gSystem->Load(includesPath+"ParticlePairFilter.hpp");
  gSystem->Load(includesPath+"ParticlePairHistos.hpp");
  gSystem->Load(includesPath+"TrackAndPairConfiguration.hpp");
  gSystem->Load(includesPath+"NuDynTask.hpp");
  gSystem->Load(includesPath+"NuDynHistos.hpp");
  gSystem->Load(includesPath+"NuDynDerivedHistos.hpp");

  gSystem->Load("libBase.dylib");


  int nFiles = 4;
  TString modelName = "PYTHIA";
//  TString modelName = "Ampt";
  TString inputPath = "/Users/claudeapruneau/Documents/GitHub/run/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/run/";
  TString baseName  = "output";
  TString dataType  = "NuDyn";
  TString ** inputFileNames            = new TString*[nFiles];
  TString ** pairLabels                = new TString*[nFiles];
  TString ** pairNames                 = new TString*[nFiles];
  TString ** centralityNames           = new TString*[nFiles];

  TFile **   inputFiles                = new TFile*[nFiles];
  NuDynHistos ** nuDyn                 = new NuDynHistos*[nFiles];
  NuDynDerivedHistos ** nuDynDerived   = new NuDynDerivedHistos*[nFiles];
  GraphConfiguration ** graphConfigs1D = new GraphConfiguration *[nFiles];
  GraphConfiguration ** graphConfigs2D = new GraphConfiguration *[nFiles];

  HistogramCollection histogramCollection("MyCollection");
  histogramCollection.setDefaultOptions(1);
  TString canvasName;
  TString histoName;

  CanvasCollection canvasCollection;
  CanvasConfiguration cc1d(0,0,1);
  CanvasConfiguration cc2d(1,1,2);
  cc2d.width = 1500;
  cc2d.height = 600;

  TCanvas * canvas;
  TH1 ** hh = new TH1*[nFiles];




  AnalysisConfiguration * ac = new AnalysisConfiguration(modelName,modelName,"1.0");
  ac->loadHistograms  = false;
  ac->createHistograms  = false;
  ac->scaleHistograms  = false;
  ac->calculateDerivedHistograms  = false;
  ac->saveHistograms  = false;
  ac->resetHistograms  = false;
  ac->clearHistograms  = false;
  ac->forceHistogramsRewrite  = false;
  ac->nuDynVsMult     = true;
  ac->nuDynVsCent     = false;

  centralityNames[0] = new TString("MB");
  centralityNames[1] = new TString("MB");
  centralityNames[2] = new TString("MB");
  centralityNames[3] = new TString("MB");

  pairLabels[0] = new TString("ALL");
  pairLabels[1] = new TString("PiPi");
  pairLabels[2] = new TString("KK");
  pairLabels[3] = new TString("PP");

  pairNames[0] = new TString("HPHM");
  pairNames[1] = new TString("PiPPiM");
  pairNames[2] = new TString("KPKM");
  pairNames[3] = new TString("PPPM");


  TString fileNameBase  = baseName + modelName + "_";
   fileNameBase += dataType;
   fileNameBase += "_";
  TString canvasNameBase  = modelName + "_";
   canvasNameBase += dataType;
   canvasNameBase += "_";

  int rebin = 10;

  for (int iFile=0; iFile<nFiles; iFile++)
    {
    TString fileName = inputPath;
    fileName +=  fileNameBase;
    fileName +=  *pairLabels[iFile];
    fileName +=  "_";
    fileName +=  *centralityNames[iFile];
    fileName += ".root";
    inputFiles[iFile]  = new TFile( fileName);
    if (!inputFiles[iFile])
      {
      cout << "<ERROR> PlotNuDynHistos() Unable to open root file " << fileName << endl;
      return 1;
      }
    nuDyn[iFile] = new NuDynHistos(inputFiles[iFile],*pairNames[iFile], ac, MessageLogger::Debug);
    nuDynDerived[iFile] = new NuDynDerivedHistos(inputFiles[iFile],*pairNames[iFile], ac, MessageLogger::Debug);
    graphConfigs1D[iFile]  = new GraphConfiguration(1,iFile);
    graphConfigs2D[iFile]  = new GraphConfiguration(2);
    }

  canvas = canvasCollection.createCanvas(canvasNameBase+"NeventVsMult",cc1d);
  for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDyn[iFile]->h_events_vsMult;
  plot(nFiles, hh, graphConfigs1D, &histogramCollection);

  canvas = canvasCollection.createCanvas(canvasNameBase+"r211_vsMult",cc1d);
    for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDynDerived[iFile]->h_r2_11_vsMult;
  plot(nFiles, hh, graphConfigs1D, &histogramCollection,rebin);

   canvas = canvasCollection.createCanvas(canvasNameBase+"r212_vsMult",cc1d);
    for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDynDerived[iFile]->h_r2_12_vsMult;
    plot(nFiles, hh, graphConfigs1D, &histogramCollection,rebin);

   canvas = canvasCollection.createCanvas(canvasNameBase+"r222_vsMult",cc1d);
    for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDynDerived[iFile]->h_r2_22_vsMult;
    plot(nFiles, hh, graphConfigs1D, &histogramCollection,rebin);

   canvas = canvasCollection.createCanvas(canvasNameBase+"nudyn_vsMult",cc1d);
    for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDynDerived[iFile]->h_nudyn_12_vsMult;
    plot(nFiles, hh, graphConfigs1D, &histogramCollection,rebin);

  canvasCollection.printAllCanvas(outputPath);
  return 0;

}

void plot(int n, TH1** h, GraphConfiguration** g, HistogramCollection * hc, int rebin)
 {
 for (int i=0; i<n; i++)
   {
   hc->setHistoProperties(h[i], *g[i]);
   if (rebin>1)
     {
     h[i]->Rebin(rebin);
     h[i]->Scale(1.0/double(rebin));
     }

   if (i==0)
     h[i]->Draw();
   else
     h[i]->Draw("SAME");
   }
 }
