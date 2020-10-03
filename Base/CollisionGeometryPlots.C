
#include "TString.h"
#include "TRandom.h"
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


int CollisionGeometryPlots()
{
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
  gSystem->Load(includesPath+"ParticleHistos.hpp");
  gSystem->Load(includesPath+"Plotter.hpp");
  gSystem->Load(includesPath+"CollisionGeometryHistograms.hpp");
  gSystem->Load("libBase.dylib");

  HistogramCollection * histogramCollection = new HistogramCollection("Collection",200);
  histogramCollection->setDefaultOptions(1);
  CanvasCollection    * canvasCollection    = new CanvasCollection();
  CanvasConfiguration * canvasConfiguration = new CanvasConfiguration(CanvasConfiguration::Landscape,CanvasConfiguration::Linear);
  GraphConfiguration  ** graphConfigurations = new GraphConfiguration*[40];
  for (int iGraph=0;iGraph<40;iGraph++)
  {
  graphConfigurations[iGraph] = new GraphConfiguration(1,iGraph%10);
  graphConfigurations[iGraph]->markerSize = 0.5;
  graphConfigurations[iGraph]->plotOption = "E0";
  }

  TString inputPath  = "/Users/claudeapruneau/Documents/GitHub/run/GeometryStudies/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/run/GeometryStudies/";


  int nFiles            = 3;
  TString ** fileNames = new TString*[nFiles];
  TString fileName;
  fileNames[0] = new TString("CollisionGeometryPbPbWS-Nominal.root");
  fileNames[1] = new TString("CollisionGeometryPbPbWS-ConfigB.root");
  fileNames[2] = new TString("CollisionGeometryPbPbWS-ConfigC.root");
  //fileNames[2] = new TString("hardOffPYTHIA_Singles_MB.root");

  //  fileNames[1] = new TString("softOffPYTHIA_Singles_MB.root");
  //  fileNames[2] = new TString("softOffPYTHIA_Singles_MB.root");
  TFile ** inputFiles  = new TFile* [nFiles];
  for (int iFile=0;iFile<nFiles;iFile++)
  {
  fileName = inputPath+*fileNames[iFile];
  inputFiles[iFile] = new TFile(fileName,"OLD");
  if (inputFiles[iFile])
    {
    cout << "<I> CollisionGeometryPlots() Successfully opened: " << fileName << endl;
    }
  else
    {
    cout << "<E> CollisionGeometryPlots() Unable to open: " << fileName << endl;
    return 1;
    }
  }

  int nModels = nFiles;
  TString ** modelNames = new TString*[nModels];
  modelNames[0] = new TString("CollisionGeometryPbPbWSNominal");
  modelNames[1] = new TString("CollisionGeometryPbPbWSConfigB");
  modelNames[2] = new TString("CollisionGeometryPbPbWSConfigC");
  TString ** modelTitles = new TString*[nModels];
  modelTitles[0] = new TString("Nominal");
  modelTitles[1] = new TString("Config B");
  modelTitles[2] = new TString("Config C");

  CollisionGeometryConfiguration * ac = new CollisionGeometryConfiguration("CollisionGeometry","CollisionGeometry","1.0");
  ac->histoBaseName =  "geom";

  CollisionGeometryHistograms  ** geomHistos = new CollisionGeometryHistograms *[nModels];
  cout << "<I> CollisionGeometryPlots()  Loading histograms" << endl;
  for (int iModel=0;iModel<nModels;iModel++)
  {
  cout << "iModel:" << iModel << " Name:" <<  *modelTitles[iModel]  << endl;
  geomHistos[iModel] = new CollisionGeometryHistograms(inputFiles[iModel],"geom",ac,MessageLogger::Debug);
  }
  TString canvasNameBase = "GCS_";
  canvasNameBase += "_";


  Plotter ** compPlotters = new Plotter*[20];
  TString canvasName;
  TH1 ** histograms = new TH1*[20];
  TString ** titles = new TString*[20];


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPart";
  histograms[0] = geomHistos[0]->h_nPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPart; titles[2] = modelTitles[2];
  compPlotters[0] = new Plotter();
  compPlotters[0]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}/2", 0.0, 240.0,
                        "Counts", 1.0, -1.0,
                        histograms,titles,0.55, 0.75, 0.8, 0.9, 0.04);
  compPlotters[0]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinary";
  histograms[0] = geomHistos[0]->h_nBinary; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinary; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinary; titles[2] = modelTitles[2];
  compPlotters[1] = new Plotter();
  compPlotters[1]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{bin}", 0.0, 240.0,
                        "Counts", 1.0, -1.0,
                        histograms,titles,0.55, 0.75, 0.8, 0.9, 0.04);
  compPlotters[0]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartVsB_Prof";
  histograms[0] = geomHistos[0]->h_nPartVsB_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartVsB_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartVsB_Prof; titles[2] = modelTitles[2];
  compPlotters[1] = new Plotter();
  compPlotters[1]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "<N_{part}/2>", 0.0, 240,
                        histograms,titles,0.55, 0.75, 0.8, 0.9, 0.04);
  compPlotters[1]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryVsB_Prof";
  histograms[0] = geomHistos[0]->h_nBinaryVsB_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryVsB_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryVsB_Prof; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "<N_{bin}>", 0.0, 1500.0,
                        histograms,titles,0.55, 0.75, 0.8, 0.9, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryVsNPart_Prof";
  histograms[0] = geomHistos[0]->h_nBinaryVsNPart_Prof; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryVsNPart_Prof; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryVsNPart_Prof; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}/2", 0.0, 240.0,
                        "<N_{bin}>", 0.0, 1500.0,
                        histograms,titles,0.65, 0.35, 0.8, 0.5, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartRmsVsB";
  histograms[0] = geomHistos[0]->h_nPartRmsVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartRmsVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartRmsVsB; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "RMS[N_{part}/2]", 0.0, 10.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsB";
  histograms[0] = geomHistos[0]->h_nBinaryRmsVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryRmsVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryRmsVsB; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "RMS[N_{bin}]", 0.0, 100.0,
                        histograms,titles,0.65, 0.65, 0.8, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryRmsVsNPart";
  histograms[0] = geomHistos[0]->h_nBinaryRmsVsNPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryRmsVsNPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryRmsVsNPart; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}/2", 0.0, 240.0,
                        "RMS[N_{bin}]", 0.0, 100.0,
                        histograms,titles,0.35, 0.65, 0.5, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartOmegaVsB";
  histograms[0] = geomHistos[0]->h_nPartOmegaVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartOmegaVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartOmegaVsB; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "#omega[N_{Part}/2]", 0.0, 2.0,
                        histograms,titles,0.35, 0.65, 0.5, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinarytOmegaVsB";
  histograms[0] = geomHistos[0]->h_nBinaryOmegaVsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryOmegaVsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryOmegaVsB; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "#omega[N_{bin}]", 0.0, 5.0,
                        histograms,titles,0.35, 0.65, 0.5, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryOmegaVsNPart";
  histograms[0] = geomHistos[0]->h_nBinaryOmegaVsNPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryOmegaVsNPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryOmegaVsNPart; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}/2", 0.0, 240.0,
                        "#omega[N_{bin}]", 0.0, 5.0,
                        histograms,titles,0.35, 0.35, 0.5, 0.55, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nPartR2VsB";
  histograms[0] = geomHistos[0]->h_nPartR2VsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nPartR2VsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nPartR2VsB; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "R_{2}[N_{part}/2]", -1.0, 1.0,
                        histograms,titles,0.35, 0.65, 0.5, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);


  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinarytR2VsB";
  histograms[0] = geomHistos[0]->h_nBinaryR2VsB; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryR2VsB; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryR2VsB; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "b (fm)", 0.0, 18.0,
                        "R_{2}[N_{bin}]",  -1.0, 1.0,
                        histograms,titles,0.35, 0.65, 0.5, 0.85, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);

  canvasConfiguration->setAxes(CanvasConfiguration::Linear);
  canvasName = "ModelComp_nBinaryR2VsNPart";
  histograms[0] = geomHistos[0]->h_nBinaryR2VsNPart; titles[0] = modelTitles[0];
  histograms[1] = geomHistos[1]->h_nBinaryR2VsNPart; titles[1] = modelTitles[1];
  histograms[2] = geomHistos[2]->h_nBinaryR2VsNPart; titles[2] = modelTitles[2];
  compPlotters[2] = new Plotter();
  compPlotters[2]->plot(nModels, canvasName,canvasConfiguration,graphConfigurations,
                        "N_{part}/2", 0.0, 240.0,
                        "R_{2}[N_{bin}]",  -0.10, 0.10,
                        histograms,titles,0.35, 0.35, 0.5, 0.55, 0.04);
  compPlotters[2]->printAllCanvas(outputPath);


  return 0;
}

