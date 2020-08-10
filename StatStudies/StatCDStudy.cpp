#include "TString.h"
#include "TRandom.h"
#include "TSystem.h"
#include "../Base/StatStudyHistograms.hpp"
#include "../Base/StatStudyModel.hpp"

//R__LOAD_LIBRARY(/Users/claudeapruneau/opt/WAC/lib/libBase.dylib)


int StatCDStudy()
{
  TString includesPath = "/Users/claudeapruneau/Documents/GitHub/WAC/Base/";
  gSystem->Load(includesPath+"CanvasConfiguration.hpp");
  gSystem->Load(includesPath+"GraphConfiguration.hpp");
  gSystem->Load(includesPath+"MessageLogger.hpp");
//  gSystem->Load(includesPath+"StatStudyHistograms.hpp");
//  gSystem->Load(includesPath+"StatStudyModel.hpp");
  //gSystem->Load("libBase.dylib");
  gSystem->Load("/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBase.dylib");

  TString inputPath      = "/Users/claudeapruneau/Documents/GitHub/run/StatStudy/";
  TString outputPath     = "/Users/claudeapruneau/Documents/GitHub/run/StatStudy/";
  TString studyName      = "StatStudySet";
  int nSubSamples        = 1000;
  int nModels            = 5;

  StatStudyHistograms ** statStudy = new StatStudyHistograms*[nModels];
  StatStudyModel      ** model     = new StatStudyModel*[nModels];

  for (int iModel=0;iModel<nModels;iModel++)
    {
    statStudy[iModel] = new StatStudyHistograms(nSubSamples, studyName+iModel,MessageLogger::Info);
    }

  model[0] = new StatStudyModel(100, 100, 20, 60, 20, 0.87, 0.85, 1000);
  model[1] = new StatStudyModel(100, 100, 20, 60, 20, 0.87, 0.85, 10000);
  model[2] = new StatStudyModel(100, 100, 20, 60, 20, 0.87, 0.85, 100000);
  model[3] = new StatStudyModel(50,  50,  10, 30, 10, 0.87, 0.85, 100000);
  model[4] = new StatStudyModel(10,  10,  2,  6,  2, 0.87, 0.85,  100000);

  double nPlus;
  double nMinus;
  double nPlusEff;
  double nMinusEff;

  double sub = 0.5;
  for (double iSub=0; iSub<nSubSamples; iSub++)
    {
    for (int iModel=0; iModel<nModels; iModel++)
      {
       for (long iEvent = 0; iEvent<model[iModel]->nEvents; iEvent++)
            {
            model[iModel]->generate(nPlus,nMinus,nPlusEff,nMinusEff);
            statStudy[iModel]->fill(sub, nPlus, nMinus, nPlusEff, nMinusEff);
            }
      }
    sub += 1.0;
    }

  TFile * outputFile;
  TString outputFileNameBase = outputPath+studyName;
  TString outputFileName;
  for (int iModel=0;iModel<nModels;iModel++)
     {
     double scalingFactor1 = 1.0/double(model[iModel]->nEvents*nSubSamples);
     double scalingFactor2 = 1.0/double(nSubSamples);

     statStudy[iModel]->computeDerivedHistograms();
     statStudy[iModel]->scaleHistograms(scalingFactor1,scalingFactor2);
     outputFileName = outputFileNameBase;
     outputFileName += iModel;
     outputFileName += ".root";
     outputFile = new TFile(outputFileName,"RECREATE");
     statStudy[iModel]->saveHistograms(outputFile);
     outputFile->Close();
     }
  return 0;
}
