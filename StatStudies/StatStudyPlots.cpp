
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

void plot(int n, TH1** h, GraphConfiguration** g, HistogramCollection * hc, int rebin=1);


// r = f2/f1
void calculateRatio(double f1,double ef1,double f2,double ef2,double & r,double & er)
{
   double ref1, ref2, rer;
  if (f2!=0)
    {
    ref1   = ef1/f1;
    ref2   = ef2/f2;
    rer    = ref1*ref1 + ref2*ref2;
    r = f1/f2;
    er = r*sqrt(rer);
    }
  else
    {
    r = 0.0;
    er = 0.0;
    }
}


// calculation carried out assuming no correlation between the different factors...
// r = f2/f1/f1
void calculateR2(double f1,double ef1,double f2,double ef2,double & r2,double & er2)
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
 void calculateR2(double f1_1,double ef1_1,double f1_2,double ef1_2,double f2,double ef2,double & r2,double & er2)
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

void calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn)
{
  nudyn = r2_11 + r2_22 - 2.0*r2_12;
  enudyn = sqrt(er2_11*er2_11 + er2_22*er2_22 + 4.0*er2_12*er2_12);
}


void calculateR2(TH1* h1, TH1* h2, TH1* hr)
{
  double v1, ev1, v2, ev2, r, er;
  int nBins = h1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    v1   = h1->GetBinContent(iBin);
    ev1  = h1->GetBinError(iBin);
    v2   = h2->GetBinContent(iBin);
    ev2  = h2->GetBinError(iBin);
    calculateR2(v1,ev1,v2,ev2,r,er);
    hr->SetBinContent(iBin, r);
    hr->SetBinError(iBin,   er);
    }
}

void calculateR2(TH1* h1_1, TH1* h1_2, TH1* h2_12, TH1* hr)
{
  double v1_1, ev1_1, v1_2, ev1_2, v2, ev2, r, er;
  int nBins = h1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    v1_1   = h1_1->GetBinContent(iBin);
    ev1_1  = h1_1->GetBinError(iBin);
    v1_2   = h1_2->GetBinContent(iBin);
    ev1_2  = h1_2->GetBinError(iBin);
    v2     = h2_12->GetBinContent(iBin);
    ev2    = h2_12->GetBinError(iBin);
    calculateR2(v1_1,ev1_1,v1_2,ev1_2,v2,ev2,r,er);
    hr->SetBinContent(iBin, r);
    hr->SetBinError(iBin,   er);
    }
}

void calculateNuDyn(TH1* r2_11, TH1* r2_12, TH1* r2_22, TH1* nudyn)
{
  double v2_11, ev2_11, v2_12, ev2_12, v2_22, ev2_22, nu, enu;
  int nBins = r2_11->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    v2_11   = r2_11->GetBinContent(iBin);
    ev2_11  = r2_11->GetBinError(iBin);
    v2_12   = r2_12->GetBinContent(iBin);
    ev2_12  = r2_12->GetBinError(iBin);
    v2_22   = r2_22->GetBinContent(iBin);
    ev2_22  = r2_22->GetBinError(iBin);
    calculateNudyn(v2_11,ev2_11,v2_12,ev2_12,v2_22,ev2_22,nu,enu);
    nudyn->SetBinContent(iBin, nu);
    nudyn->SetBinError(iBin,   enu);
    }
}
// Calculate r = h2/h1
void calculateRatio(TH1 * h1, TH1 * h2, TH1 * ratio)
{
  double v1, ev1, v2, ev2, r, er;
  int nBins = h1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    v1   = h1->GetBinContent(iBin);
    ev1  = h1->GetBinError(iBin);
    v2   = h2->GetBinContent(iBin);
    ev2  = h2->GetBinError(iBin);
    calculateRatio(v1,ev1,v2,ev2,r,er);

    cout << ratio->GetName() << " iBin:" << iBin << "  r:" << r << "   er:" << er << endl;
    ratio->SetBinContent(iBin, r);
    ratio->SetBinError  (iBin, er);
    }
}

void calculateMeanAndStd(TH1 * h, int nBins, double &mean, double &eMean, double &std)
{
  double m1  = 0;
  double em1 = 0;
  double m2  = 0;
  double v, ev;
  // calculate mean value of the nBins
  for (int iBin=1; iBin<=nBins; iBin++)
    {
    v  = h->GetBinContent(iBin);
    ev = h->GetBinError(iBin);
    m1 += v;
    em1 += ev*ev;
    }
  mean = m1/double(nBins);
  eMean = sqrt(em1)/double(nBins);

  for (int iBin=1; iBin<=nBins; iBin++)
    {
    v  = h->GetBinContent(iBin)-mean;
    m2 += v*v;
    }
  m2 = m2/double(nBins-1);
  std = sqrt(m2);
}

void extractGauss(TH1* h, double & mean, double & sigma)
 {
 TF1* h_fct = h->GetFunction("gaus");
 mean  = h_fct->GetParameter(1);
 sigma = h_fct->GetParameter(2);
 cout << h->GetName() << " mean:" << mean << "  sigma:" << sigma << endl;
 }

void compareRatio(TString name,
                  double num,  double eNum,
                  double dem,  double eDem,
                  double & predictedRatio, double & predictedError,
                  double ratioMean,  double  ratioSigma)
{
  double predictedToActualMean, predictedToActualSigma;

  predictedRatio  = num/dem;
  predictedToActualMean = predictedRatio/ratioMean;

  // assume no correlations are present...
  double re1, re2;
  re1 = eNum/num;
  re2 = eDem/dem;
  predictedError  = predictedRatio*sqrt(re1*re1 + re2*re2);
  predictedToActualSigma = predictedError/ratioSigma;

  cout << "Compare ratio for " << name << endl;
  cout << "Uncorrelated errors" << endl;
  cout << "Predicted Ratio Mean  :" << predictedRatio  << " Actual Ratio Mean:" << ratioMean << " Ratio of Predicted to actual Ratio Mean: " << predictedToActualMean << endl;
  cout << "Predicted Ratio Sigma :" << predictedError << " Actual Ratio Sigma:" << ratioSigma << " Ratio of Predicted to actual Ratio Sigma: " << predictedToActualSigma << endl;

  predictedError  = predictedRatio*sqrt( fabs((1.0- 2.0*predictedRatio)*re1*re1 + re2*re2));
  predictedToActualSigma = predictedError/ratioSigma;

  cout << "Binomial  errors" << endl;
  cout << "Predicted Ratio Mean  :" << predictedRatio  << " Actual Ratio Mean:" << ratioMean << " Ratio of Predicted to actual Ratio Mean: " << predictedToActualMean << endl;
  cout << "Predicted Ratio Sigma :" << predictedError << " Actual Ratio Sigma:" << ratioSigma << " Ratio of Predicted to actual Ratio Sigma: " << predictedToActualSigma << endl;



}


void compareR2(TString name,
               double f2_mean, double f2_sigma,
               double f1_mean, double f1_sigma,
               double & predictedR2Mean, double & predictedR2Sigma,
               double r2_mean, double r2_sigma)
{
  double predictedToActualMean, predictedToActualSigma;

  predictedR2Mean = f2_mean/(f1_mean*f1_mean);
  predictedToActualMean = predictedR2Mean/r2_mean;
  double ref2 = f2_sigma/f2_mean;
  double ref1 = f1_sigma/f1_mean;
  predictedR2Sigma = predictedR2Mean * sqrt(ref2*ref2 + 4.0*ref1*ref1);
  predictedToActualSigma = predictedR2Sigma/r2_sigma;
  cout << "Compare r2 for " <<  name << endl;
   cout << "Predicted r2 Mean  :" << predictedR2Mean  << "  Actual r2 Mean:" << r2_mean << " Ratio of Predicted to actual r2 Mean: " << predictedToActualMean << endl;
   cout << "Predicted r2 Sigma :" << predictedR2Sigma << " Actual r2 Sigma:" << r2_sigma << " Ratio of Predicted to actual r2 Sigma: " << predictedToActualSigma << endl;
}

void compareR2(TString name,
               double f2_mean, double f2_sigma,
               double f1_1_mean, double f1_1_sigma,
               double f1_2_mean, double f1_2_sigma,
               double & predictedR2Mean, double & predictedR2Sigma,
               double r2_mean, double r2_sigma)
{
  double predictedToActualMean, predictedToActualSigma;
  predictedR2Mean = f2_mean/(f1_1_mean*f1_2_mean);
  predictedToActualMean = predictedR2Mean/r2_mean;
  double ref2 = f2_sigma/f2_mean;
  double ref11 = f1_1_sigma/f1_1_mean;
  double ref12 = f1_2_sigma/f1_2_mean;
  predictedR2Sigma = predictedR2Mean * sqrt(ref2*ref2 + ref11*ref11 + ref12*ref12);
  predictedToActualSigma = predictedR2Sigma/r2_sigma;

  cout << "Compare r2 for " <<  name << endl;
  cout << "Predicted r2 Mean  :" << predictedR2Mean  << "  Actual r2 Mean:" << r2_mean << " Ratio of Predicted to actual r2 Mean: " << predictedToActualMean << endl;
  cout << "Predicted r2 Sigma :" << predictedR2Sigma << " Actual r2 Sigma:" << r2_sigma << " Ratio of Predicted to actual r2 Sigma: " << predictedToActualSigma << endl;
}


//void compareF2Ratio(TString name, double f1EffMean,  double f1EffSigma,  double f1Mean,  double f1Sigma,  double f1RatioMean,  double  f1RatioSigma)
//{
//  double predictedF1RatioMean,  predictedF1RatioSigma;
//  double predictedToActualMean, predictedToActualSigma;
//
//  predictedF1RatioMean  = f1EffMean/f1Mean;
//  predictedToActualMean = predictedF1RatioMean/f1RatioMean;
//
//  // assume no correlations are present...
//  double re1, re2;
//  re1 = f1EffSigma/f1EffMean;
//  re2 = f1Sigma/f1Mean;
//  predictedF1RatioSigma  = predictedF1RatioMean*sqrt(re1*re1 + re2*re2);
//  predictedToActualSigma = predictedF1RatioSigma/f1RatioSigma;
//
//  cout << name << endl;
//  cout << "Predicted Ratio Mean  :" << predictedF1RatioMean  << " Actual Ratio Mean:" << f1RatioMean << " Ratio of Predicted to actual Ratio Mean: " << predictedToActualMean << endl;
//  cout << "Predicted Ratio Sigma :" << predictedF1RatioSigma << " Actual Ratio Sigma:" << f1RatioSigma << " Ratio of Predicted to actual Ratio Sigma: " << predictedToActualSigma << endl;
//
//}


//int binomialLoss(int nIn, double efficiency)
//{
//  int nOut = 0;
//  for (int n=0; n<nIn; n++)
//    {
//    if (gRandom->Rndm()<efficiency) nOut++;
//    }
//  return nOut;
//}

int StatStudyPlots()
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
  gSystem->Load("libBase.dylib");


  int nSubSamples = 4000;
  TString inputPath = "/Users/claudeapruneau/Documents/GitHub/run/StatStudy/";
  TString outputPath = "./"; // /Users/claudeapruneau/Documents/GitHub/run/";
  TString baseName  = "output";
  TString inputFileName = "StatStudySet4.root";
  TString canvasNameBase = "Set4_";

  HistogramCollection * hc = new HistogramCollection("Collection",100);
  hc->setDefaultOptions(1);
  CanvasCollection    * canvasCollection = new CanvasCollection();
  CanvasConfiguration * cc1d  = new CanvasConfiguration(0,1,1);
  cc1d->width = 1000;
  cc1d->height = 500;
  cc1d->topMargin = 0.06;
  cc1d->logy = 0;
  GraphConfiguration ** gc1d  = new GraphConfiguration *[6];

  for (int i=0;i<6;i++)
    {
    gc1d[i] = new GraphConfiguration(1,i);
    }

  TFile * inputFile = new TFile(inputPath+inputFileName,"OLD");
  if (!inputFile)
    {
    cout << "File " << inputPath+inputFileName << " not found. Abort." << endl;
    return 1;
    }

  TH1* f1_1     = hc->loadH1( inputFile,   "f1_1");
  TH1* f1_2     = hc->loadH1( inputFile,   "f1_2");
  TH1* f2_11    = hc->loadH1( inputFile,   "f2_11");
  TH1* f2_12    = hc->loadH1( inputFile,   "f2_12");
  TH1* f2_22    = hc->loadH1( inputFile,   "f2_22");
  TH1* r2_11    = hc->loadH1( inputFile, "r2_11");
  TH1* r2_12    = hc->loadH1( inputFile, "r2_12");
  TH1* r2_22    = hc->loadH1( inputFile, "r2_22");
  TH1* nudyn_12 = hc->loadH1( inputFile, "nudyn_12");

  TH1* f1_1Eff     = hc->loadH1( inputFile,   "f1_1Eff");
  TH1* f1_2Eff     = hc->loadH1( inputFile,   "f1_2Eff");
  TH1* f2_11Eff    = hc->loadH1( inputFile,   "f2_11Eff");
  TH1* f2_12Eff    = hc->loadH1( inputFile,   "f2_12Eff");
  TH1* f2_22Eff    = hc->loadH1( inputFile,   "f2_22Eff");
  TH1* r2_11Eff    = hc->loadH1( inputFile, "r2_11Eff");
  TH1* r2_12Eff    = hc->loadH1( inputFile, "r2_12Eff");
  TH1* r2_22Eff    = hc->loadH1( inputFile, "r2_22Eff");
  TH1* nudyn_12Eff = hc->loadH1( inputFile, "nudyn_12Eff");

  TH1* f1_1Ratio     = hc->loadH1( inputFile, "f1_1Ratio");
  TH1* f1_2Ratio     = hc->loadH1( inputFile, "f1_2Ratio");
  TH1* f2_11Ratio    = hc->loadH1( inputFile, "f2_11Ratio");
  TH1* f2_12Ratio    = hc->loadH1( inputFile, "f2_12Ratio");
  TH1* f2_22Ratio    = hc->loadH1( inputFile, "f2_22Ratio");
  TH1* r2_11Ratio    = hc->loadH1( inputFile, "r2_11Ratio");
  TH1* r2_12Ratio    = hc->loadH1( inputFile, "r2_12Ratio");
  TH1* r2_22Ratio    = hc->loadH1( inputFile, "r2_22Ratio");
  TH1* nudyn_12Ratio = hc->loadH1( inputFile, "nudyn_12Ratio");

  TH1* r2_11_vsSub    = hc->loadH1( inputFile, "r2_11_vsSub");
   TH1* r2_12_vsSub    = hc->loadH1( inputFile, "r2_12_vsSub");
   TH1* r2_22_vsSub    = hc->loadH1( inputFile, "r2_22_vsSub");
   TH1* nudyn_12_vsSub = hc->loadH1( inputFile, "nudyn_12_vsSub");


  TH1* f1_1Ratio_vsSub     = hc->loadH1( inputFile, "f1_1Ratio_vsSub");
   TH1* f1_2Ratio_vsSub     = hc->loadH1( inputFile, "f1_2Ratio_vsSub");
   TH1* f2_11Ratio_vsSub    = hc->loadH1( inputFile, "f2_11Ratio_vsSub");
   TH1* f2_12Ratio_vsSub    = hc->loadH1( inputFile, "f2_12Ratio_vsSub");
   TH1* f2_22Ratio_vsSub    = hc->loadH1( inputFile, "f2_22Ratio_vsSub");
   TH1* r2_11Ratio_vsSub    = hc->loadH1( inputFile, "r2_11Ratio_vsSub");
   TH1* r2_12Ratio_vsSub    = hc->loadH1( inputFile, "r2_12Ratio_vsSub");
   TH1* r2_22Ratio_vsSub    = hc->loadH1( inputFile, "r2_22Ratio_vsSub");
   TH1* nudyn_12Ratio_vsSub = hc->loadH1( inputFile, "nudyn_12Ratio_vsSub");


//  cout << "     f1_1Mean: " <<  f1_1Mean     << " f1_1Std : "    <<   f1_1Std << endl;
//  cout << "     f1_2Mean: " <<  f1_2Mean     << " f1_2Std : "    <<   f1_2Std << endl;
//  cout << "    f2_11Mean: " <<  f2_11Mean    << " f2_11Std : "   <<   f2_11Std << endl;
//  cout << "    f2_12Mean: " <<  f2_12Mean    << " f2_12Std : "   <<   f2_12Std << endl;
//  cout << "    f2_22Mean: " <<  f2_22Mean    << " f2_22Std: "    <<   f2_22Std << endl;
//  cout << "    r2_11Mean: " <<  r2_11Mean    << " r2_11Std: "    <<   r2_11Std << endl;
//  cout << "    r2_12Mean: " <<  r2_12Mean    << " r2_12Std: "    <<   r2_12Std << endl;
//  cout << "    r2_22Mean: " <<  r2_22Mean    << " r2_22Std: "    <<   r2_22Std << endl;
//  cout << " nudyn_12Mean: " <<  nudyn_12Mean << " nudyn_12Std: " <<   nudyn_12Std << endl;

   TCanvas * canvas;

  canvas = canvasCollection->createCanvas(canvasNameBase+"f1",*cc1d);
  TLegend * legend1 = canvasCollection->createLegend(0.20, 0.4, 0.35, 0.8, 0, 0.055);
  hc->setHistoProperties(f1_1,    *gc1d[0]);
  hc->setHistoProperties(f1_2,    *gc1d[1]);
  hc->setHistoProperties(f1_1Eff, *gc1d[2]);
  hc->setHistoProperties(f1_2Eff, *gc1d[3]);
  f1_1->GetXaxis()->SetTitle("< n_{1} >");
  f1_1->GetYaxis()->SetTitle("Events");
  //f1_1->GetXaxis()->SetRangeUser(0.8,0.9);
  f1_1->SetMinimum(0.0);
  f1_1->SetMaximum(1000000.0);

  f1_1->Fit("gaus");
  f1_2->Fit("gaus");
  f1_1Eff->Fit("gaus");
  f1_2Eff->Fit("gaus");
  f1_1->Draw();         legend1->AddEntry(f1_1,"n_{1}^{(1)}");
  f1_2->Draw("SAME");   legend1->AddEntry(f1_2,"n_{1}^{(2)}");
  f1_1Eff->Draw("SAME");legend1->AddEntry(f1_1Eff,"n_{1}^{(1),Eff}");
  f1_2Eff->Draw("SAME");legend1->AddEntry(f1_2Eff,"n_{1}^{(2),Eff}");
  legend1->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"f1Ratio",*cc1d);
  TLegend * legend1a = canvasCollection->createLegend(0.16, 0.4, 0.32, 0.8, 0, 0.055);
  hc->setHistoProperties(f1_1Ratio,    *gc1d[0]);
  hc->setHistoProperties(f1_2Ratio,    *gc1d[1]);
  f1_1Ratio->Fit("gaus");
  f1_2Ratio->Fit("gaus");
  f1_1Ratio->GetXaxis()->SetTitle("f_{1}^{Eff}/f_{1}");
  f1_1Ratio->GetYaxis()->SetTitle("Events");
  f1_1Ratio->GetXaxis()->SetRangeUser(0.84,0.88);
  f1_1Ratio->SetMinimum(0.0);
  f1_1Ratio->SetMaximum(200.0);
  f1_1Ratio->Draw();         legend1a->AddEntry(f1_1Ratio,"f_{1}^{(1,Eff)}/f_{1}^{(1)}");
  f1_2Ratio->Draw("SAME");   legend1a->AddEntry(f1_2,"f_{1}^{(2,Eff)}/f_{1}^{(2)}");
  legend1a->Draw();


  canvas = canvasCollection->createCanvas(canvasNameBase+"f2",*cc1d);
  TLegend * legend2 = canvasCollection->createLegend(0.70, 0.4, 0.85, 0.9, 0, 0.055);
  hc->setHistoProperties(f2_11,    *gc1d[0]);
  hc->setHistoProperties(f2_12,    *gc1d[1]);
  hc->setHistoProperties(f2_12,    *gc1d[2]);
  hc->setHistoProperties(f2_11Eff, *gc1d[3]);
  hc->setHistoProperties(f2_12Eff, *gc1d[4]);
  hc->setHistoProperties(f2_12Eff, *gc1d[5]);
  f2_11->Fit("gaus");
  f2_12->Fit("gaus");
  f2_22->Fit("gaus");
  f2_11Eff->Fit("gaus");
  f2_12Eff->Fit("gaus");
  f2_22Eff->Fit("gaus");
  f2_11->Rebin(128);
  f2_12->Rebin(128);
  f2_22->Rebin(128);
  f2_11Eff->Rebin(128);
  f2_12Eff->Rebin(128);
  f2_22Eff->Rebin(128);

  f2_11->GetXaxis()->SetTitle("< n_{2} >");
  f2_11->GetYaxis()->SetTitle("Events");
  f2_11->SetMinimum(0.0);
  f2_11->SetMaximum(1000000.0);
  f2_11->Draw();       legend2->AddEntry(f2_11,"f_{2}^{(11)}");
  f2_12->Draw("SAME"); legend2->AddEntry(f2_12,"f_{2}^{(12)}");
  f2_22->Draw("SAME"); legend2->AddEntry(f2_22,"f_{2}^{(22)}");
  f2_11Eff->Draw("SAME");legend2->AddEntry(f2_11Eff,"f_{2}^{(11),Eff}");
  f2_12Eff->Draw("SAME");legend2->AddEntry(f2_12Eff,"f_{2}^{(12),Eff}");
  f2_22Eff->Draw("SAME");legend2->AddEntry(f2_22Eff,"f_{2}^{(22),Eff}");
  legend2->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"f2Ratio",*cc1d);
   TLegend * legend2a = canvasCollection->createLegend(0.70, 0.4, 0.85, 0.9, 0, 0.055);
   hc->setHistoProperties(f2_11Ratio,    *gc1d[0]);
   hc->setHistoProperties(f2_12Ratio,    *gc1d[1]);
   hc->setHistoProperties(f2_12Ratio,    *gc1d[2]);
   f2_11Ratio->Fit("gaus");
   f2_12Ratio->Fit("gaus");
   f2_22Ratio->Fit("gaus");
   f2_11Ratio->GetXaxis()->SetTitle("< n_{2}^{Eff} >/< n_{2} >");
   f2_11Ratio->GetYaxis()->SetTitle("Events");
   f2_11Ratio->SetMinimum(0.0);
   f2_11Ratio->SetMaximum(600.0);
   f2_11Ratio->GetXaxis()->SetRangeUser(0.7,0.8);
   f2_11Ratio->Draw();       legend2a->AddEntry(f2_11Ratio,"f_{2}^{(11,Eff)}/f_{2}^{(11)}");
   f2_12Ratio->Draw("SAME"); legend2a->AddEntry(f2_12Ratio,"f_{2}^{(12,Eff)}/f_{2}^{(12)}");
   f2_22Ratio->Draw("SAME"); legend2a->AddEntry(f2_22Ratio,"f_{2}^{(22,Eff)}/f_{2}^{(22)}");
   legend2a->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"R2",*cc1d);
  TLegend * legend3 = canvasCollection->createLegend(0.20, 0.35, 0.35, 0.9, 0, 0.055);
  hc->setHistoProperties(r2_11,    *gc1d[0]);
  hc->setHistoProperties(r2_12,    *gc1d[1]);
  hc->setHistoProperties(r2_12,    *gc1d[2]);
  hc->setHistoProperties(r2_11Eff, *gc1d[3]);
  hc->setHistoProperties(r2_12Eff, *gc1d[4]);
  hc->setHistoProperties(r2_12Eff, *gc1d[5]);
//  r2_11->Fit("gaus");
//  r2_12->Fit("gaus");
//  r2_22->Fit("gaus");
//  r2_11Eff->Fit("gaus");
//  r2_12Eff->Fit("gaus");
//  r2_22Eff->Fit("gaus");
  r2_11->GetXaxis()->SetTitle("r_{2}");
  r2_11->GetYaxis()->SetTitle("Events");
  r2_11->SetMinimum(0.0);
  r2_11->SetMaximum(300.0);
  r2_11->GetXaxis()->SetRangeUser(0.0, 0.003);

  r2_11->Draw();       legend3->AddEntry(r2_11,"r_{2}^{(11)}");
  r2_12->Draw("SAME"); legend3->AddEntry(r2_12,"r_{2}^{(12)}");
  r2_22->Draw("SAME"); legend3->AddEntry(r2_22,"r_{2}^{(22)}");
  r2_11Eff->Draw("SAME");legend3->AddEntry(r2_11Eff,"r_{2}^{(11),Eff}");
  r2_12Eff->Draw("SAME");legend3->AddEntry(r2_12Eff,"r_{2}^{(12),Eff}");
  r2_22Eff->Draw("SAME");legend3->AddEntry(r2_22Eff,"r_{2}^{(22),Eff}");
  legend3->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"R2Ratio",*cc1d);
  TLegend * legend3a = canvasCollection->createLegend(0.20, 0.4, 0.35, 0.9, 0, 0.055);
  hc->setHistoProperties(r2_11Ratio,    *gc1d[0]);
  hc->setHistoProperties(r2_12Ratio,    *gc1d[1]);
  hc->setHistoProperties(r2_22Ratio,    *gc1d[2]);
  r2_11Ratio->GetXaxis()->SetTitle("r_{2}^{Eff}/r_{2}");
  r2_11Ratio->GetYaxis()->SetTitle("Events");
  r2_11Ratio->Fit("gaus");
  r2_12Ratio->Fit("gaus");
  r2_22Ratio->Fit("gaus");
  r2_11Ratio->SetMinimum(0.0);
  r2_11Ratio->SetMaximum(60.0);
  r2_11Ratio->GetXaxis()->SetRangeUser(0.8,1.2);
  r2_11Ratio->Draw();       legend3a->AddEntry(r2_11Ratio,"r_{2}^{(11,Eff)}/r_{2}^{(11)}");
  r2_12Ratio->Draw("SAME"); legend3a->AddEntry(r2_12Ratio,"r_{2}^{(12,Eff)}/r_{2}^{(12)}");
  r2_22Ratio->Draw("SAME"); legend3a->AddEntry(r2_22Ratio,"r_{2}^{(22),Eff}/r_{2}^{(22)}");
  legend3a->Draw();


  canvas = canvasCollection->createCanvas(canvasNameBase+"NuDyn",*cc1d);
  TLegend * legend5 = canvasCollection->createLegend(0.20, 0.4, 0.45, 0.9, 0, 0.055);
  hc->setHistoProperties(nudyn_12,    *gc1d[0]);
  hc->setHistoProperties(nudyn_12Eff,    *gc1d[1]);

  nudyn_12->Fit("gaus");
  nudyn_12Eff->Fit("gaus");
  nudyn_12->GetXaxis()->SetTitle("#nu_{dyn}");
  nudyn_12->GetYaxis()->SetTitle("Events");
  nudyn_12->SetMinimum(0.0);
  nudyn_12->SetMaximum(140.0);
  nudyn_12->GetXaxis()->SetRangeUser(-0.01,0.01);
  nudyn_12->Draw();          legend5->AddEntry(nudyn_12,"#nu_{dyn}");
  nudyn_12Eff->Draw("SAME"); legend5->AddEntry(nudyn_12Eff,"#nu_{dyn}^{Eff}");
  legend5->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"NuDynRatio",*cc1d);
  TLegend * legend6 = canvasCollection->createLegend(0.20, 0.4, 0.45, 0.9, 0, 0.055);
  hc->setHistoProperties(nudyn_12Ratio,    *gc1d[0]);
  nudyn_12Ratio->Fit("gaus");
  nudyn_12Ratio->GetXaxis()->SetTitle("#nu_{dyn}^{Eff}/#nu_{dyn}");
  nudyn_12Ratio->GetYaxis()->SetTitle("Events");
  nudyn_12Ratio->SetMinimum(0.0);
  nudyn_12Ratio->SetMaximum(20.0);
  nudyn_12Ratio->GetXaxis()->SetRangeUser(0.5,1.5);
  nudyn_12Ratio->Draw(); legend6->AddEntry(nudyn_12Ratio,"#nu_{dyn}^{Eff}/#nu_{dyn}");
  legend6->Draw();

//===
//  TH1* f1_1Ratio_vsSub     = hc->loadH1( inputFile, "f1_1Ratio_vsSub");
//    TH1* f1_2Ratio_vsSub     = hc->loadH1( inputFile, "f1_2Ratio_vsSub");
//    TH1* f2_11Ratio_vsSub    = hc->loadH1( inputFile, "f2_11Ratio_vsSub");
//    TH1* f2_12Ratio_vsSub    = hc->loadH1( inputFile, "f2_12Ratio_vsSub");
//    TH1* f2_22Ratio_vsSub    = hc->loadH1( inputFile, "f2_22Ratio_vsSub");
//    TH1* r2_11Ratio_vsSub    = hc->loadH1( inputFile, "r2_11Ratio_vsSub");
//    TH1* r2_12Ratio_vsSub    = hc->loadH1( inputFile, "r2_12Ratio_vsSub");
//    TH1* r2_22Ratio_vsSub    = hc->loadH1( inputFile, "r2_22Ratio_vsSub");
//    TH1* nudyn_12Ratio_vsSub = hc-


  canvas = canvasCollection->createCanvas(canvasNameBase+"f1_1Ratio_vsSub_Zoom",*cc1d);
   TLegend * legend7 = canvasCollection->createLegend(0.20, 0.7, 0.45, 0.9, 0, 0.055);
   hc->setHistoProperties(f1_1Ratio_vsSub,    *gc1d[0]);
   f1_1Ratio_vsSub->SetMinimum(0.86);
   f1_1Ratio_vsSub->SetMaximum(0.88);
  f1_1Ratio_vsSub->GetXaxis()->SetRangeUser(980.0,1010.0);
   f1_1Ratio_vsSub->Draw(); legend7->AddEntry(f1_1Ratio_vsSub,"f_{1}^{Eff}/f_{1}");
   legend7->Draw();
//  canvas = canvasCollection->createCanvas(canvasNameBase+"f1_1Ratio_vsSub_zoom",*cc1d);
//   TLegend * legend7a = canvasCollection->createLegend(0.20, 0.7, 0.45, 0.9, 0, 0.055);
//   hc->setHistoProperties(f1_1Ratio_vsSub,    *gc1d[0]);
//   f1_1Ratio_vsSub->SetMinimum(0.86);
//   f1_1Ratio_vsSub->SetMaximum(0.88);
//  f1_1Ratio_vsSub->GetXaxis()->SetRangeUser(3980.0, 4010.0);
//   f1_1Ratio_vsSub->Draw(); legend7a->AddEntry(f1_1Ratio_vsSub,"f_{1}^{Eff}/f_{1}");
//   legend7a->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"r2_11_vsSub",*cc1d);
   TLegend * legend7a = canvasCollection->createLegend(0.20, 0.7, 0.45, 0.9, 0, 0.055);
   hc->setHistoProperties(r2_11_vsSub,    *gc1d[0]);
//   r2_11_vsSub->SetMinimum(0.8);
//   r2_11_vsSub->SetMaximum(1.2);
   //r2_11Ratio_vsSub->GetXaxis()->SetRangeUser(3980.0, 4010.0);
   r2_11_vsSub->Draw(); legend7a->AddEntry(r2_11_vsSub,"r_{2}^{11}");
   legend7a->Draw();


  canvas = canvasCollection->createCanvas(canvasNameBase+"r2_11Ratio_vsSub_zoom",*cc1d);
   //canvas = canvasCollection->createCanvas(canvasNameBase+"r2_11Ratio_vsSub",*cc1d);
   TLegend * legend8 = canvasCollection->createLegend(0.20, 0.7, 0.45, 0.9, 0, 0.055);
   hc->setHistoProperties(r2_11Ratio_vsSub,    *gc1d[0]);
   r2_11Ratio_vsSub->SetMinimum(0.8);
   r2_11Ratio_vsSub->SetMaximum(1.2);
   r2_11Ratio_vsSub->GetXaxis()->SetRangeUser(980.0, 1010.0);
   r2_11Ratio_vsSub->Draw(); legend8->AddEntry(r2_11Ratio_vsSub,"r_{2}^{11,Eff}/r_{2}^{11}");
   legend8->Draw();

  canvas = canvasCollection->createCanvas(canvasNameBase+"nudyn_12Ratio_vsSub_zoom",*cc1d);
   TLegend * legend9 = canvasCollection->createLegend(0.20, 0.7, 0.45, 0.9, 0, 0.055);
   hc->setHistoProperties(nudyn_12Ratio_vsSub,    *gc1d[0]);
   nudyn_12Ratio_vsSub->SetMinimum(0.5);
   nudyn_12Ratio_vsSub->SetMaximum(1.5);
   nudyn_12Ratio_vsSub->GetXaxis()->SetRangeUser(980.0, 1010.0);
   nudyn_12Ratio_vsSub->Draw(); legend9->AddEntry(nudyn_12Ratio_vsSub,"#nu_{\rm Dyn}^{12,Eff}/#nu_{\rm Dyn}^{12}");
   legend9->Draw();

  //return 0;

//  double f1_1_mean,  f1_2_mean;
//  double f1_1_sigma, f1_2_sigma;
//  double f2_11_mean,  f2_12_mean,  f2_22_mean;
//  double f2_11_sigma, f2_12_sigma, f2_22_sigma;
//  double r2_11_mean,  r2_12_mean,  r2_22_mean;
//  double r2_11_sigma, r2_12_sigma, r2_22_sigma;
//  double nudyn_12_mean, nudyn_12_sigma;
//
//  double f1_1Eff_mean,  f1_2Eff_mean;
//  double f1_1Eff_sigma, f1_2Eff_sigma;
//  double f2_11Eff_mean,  f2_12Eff_mean,  f2_22Eff_mean;
//  double f2_11Eff_sigma, f2_12Eff_sigma, f2_22Eff_sigma;
//  double r2_11Eff_mean,  r2_12Eff_mean,  r2_22Eff_mean;
//  double r2_11Eff_sigma, r2_12Eff_sigma, r2_22Eff_sigma;
//  double nudyn_12Eff_mean, nudyn_12Eff_sigma;
//
//  double f1_1Ratio_mean,  f1_2Ratio_mean;
//  double f1_1Ratio_sigma, f1_2Ratio_sigma;
//  double f2_11Ratio_mean,  f2_12Ratio_mean,  f2_22Ratio_mean;
//  double f2_11Ratio_sigma, f2_12Ratio_sigma, f2_22Ratio_sigma;
//  double r2_11Ratio_mean,  r2_12Ratio_mean,  r2_22Ratio_mean;
//  double r2_11Ratio_sigma, r2_12Ratio_sigma, r2_22Ratio_sigma;
//  double nudyn_12Ratio_mean, nudyn_12Ratio_sigma;
//
//  double nEvents = 4000;
//  double scaling = sqrt(nEvents);
//
//  extractGauss(f1_1,  f1_1_mean,  f1_1_sigma);  f1_1_sigma = f1_1_sigma/scaling;
//  extractGauss(f1_2,  f1_2_mean,  f1_2_sigma);  f1_2_sigma = f1_2_sigma/scaling;
//  extractGauss(f2_11, f2_11_mean, f2_11_sigma);  f2_11_sigma = f2_11_sigma/scaling;
//  extractGauss(f2_12, f2_12_mean, f2_12_sigma);  f2_12_sigma = f2_12_sigma/scaling;
//  extractGauss(f2_22, f2_22_mean, f2_22_sigma);  f2_22_sigma = f2_22_sigma/scaling;
//  extractGauss(r2_11, r2_11_mean, r2_11_sigma);  //r2_11_sigma = r2_11_sigma/scaling;
//  extractGauss(r2_12, r2_12_mean, r2_12_sigma);  //r2_12_sigma = r2_12_sigma/scaling;
//  extractGauss(r2_22, r2_22_mean, r2_22_sigma);  //r2_22_sigma = r2_22_sigma/scaling;
//  extractGauss(nudyn_12, nudyn_12_mean, nudyn_12_sigma);  //nudyn_12_sigma = nudyn_12_sigma/scaling;
//
//  extractGauss(f1_1Eff,  f1_1Eff_mean,  f1_1Eff_sigma);  f1_1Eff_sigma = f1_1Eff_sigma/scaling;
//  extractGauss(f1_2Eff,  f1_2Eff_mean,  f1_2Eff_sigma);  f1_2Eff_sigma = f1_2Eff_sigma/scaling;
//  extractGauss(f2_11Eff, f2_11Eff_mean, f2_11Eff_sigma);  f2_11Eff_sigma = f2_11Eff_sigma/scaling;
//  extractGauss(f2_12Eff, f2_12Eff_mean, f2_12Eff_sigma);  f2_12Eff_sigma = f2_12Eff_sigma/scaling;
//  extractGauss(f2_22Eff, f2_22Eff_mean, f2_22Eff_sigma);  f2_22Eff_sigma = f2_22Eff_sigma/scaling;
//  extractGauss(r2_11Eff, r2_11Eff_mean, r2_11Eff_sigma);  //r2_11Eff_sigma = r2_11Eff_sigma/scaling;
//  extractGauss(r2_12Eff, r2_12Eff_mean, r2_12Eff_sigma);  //r2_12Eff_sigma = r2_12Eff_sigma/scaling;
//  extractGauss(r2_22Eff, r2_22Eff_mean, r2_22Eff_sigma);  //r2_22Eff_sigma = r2_22Eff_sigma/scaling;
//  extractGauss(nudyn_12Eff, nudyn_12Eff_mean, nudyn_12Eff_sigma);  //nudyn_12Eff_sigma = nudyn_12Eff_sigma/scaling;
//
// extractGauss(f1_1Ratio,  f1_1Ratio_mean,  f1_1Ratio_sigma);
//  extractGauss(f1_2Ratio,  f1_2Ratio_mean,  f1_2Ratio_sigma);
//  extractGauss(f2_11Ratio, f2_11Ratio_mean, f2_11Ratio_sigma);
//  extractGauss(f2_12Ratio, f2_12Ratio_mean, f2_12Ratio_sigma);
//  extractGauss(f2_22Ratio, f2_22Ratio_mean, f2_22Ratio_sigma);
//  extractGauss(r2_11Ratio, r2_11Ratio_mean, r2_11Ratio_sigma);
//  extractGauss(r2_12Ratio, r2_12Ratio_mean, r2_12Ratio_sigma);
//  extractGauss(r2_22Ratio, r2_22Ratio_mean, r2_22Ratio_sigma);
//  extractGauss(nudyn_12Ratio, nudyn_12Ratio_mean, nudyn_12Ratio_sigma);
//
//  double predictedR2_11_mean, predictedR2_11_sigma;
//  double predictedR2_12_mean, predictedR2_12_sigma;
//  double predictedR2_22_mean, predictedR2_22_sigma;
//
//  double predictedF1_1Ratio_mean, predictedF1_1Ratio_sigma;
//  double predictedF1_2Ratio_mean, predictedF1_2Ratio_sigma;
//  double predictedF2_11Ratio_mean, predictedF2_11Ratio_sigma;
//  double predictedF2_12Ratio_mean, predictedF2_12Ratio_sigma;
//  double predictedF2_22Ratio_mean, predictedF2_22Ratio_sigma;
//  double predictedR2_11Ratio_mean, predictedR2_11Ratio_sigma;
//  double predictedR2_12Ratio_mean, predictedR2_12Ratio_sigma;
//  double predictedR2_22Ratio_mean, predictedR2_22Ratio_sigma;
//  double predictedNudyn_12Ratio_mean, predictedNudyn_12Ratio_sigma;
//
//  compareR2("r2_11", f2_11_mean, f2_11_sigma, f1_1_mean, f1_1_sigma, predictedR2_11_mean, predictedR2_11_sigma, r2_11_mean, r2_11_sigma);
//  compareR2("r2_22", f2_22_mean, f2_22_sigma, f1_2_mean, f1_2_sigma, predictedR2_12_mean, predictedR2_12_sigma, r2_22_mean, r2_22_sigma);
//  compareR2("r2_12", f2_12_mean, f2_12_sigma, f1_1_mean, f1_1_sigma, f1_2_mean, f1_2_sigma, predictedR2_22_mean, predictedR2_22_sigma, r2_12_mean, r2_12_sigma);
//
// // compareNudyn("nudyn_12", r2_11_mean, r2_11_sigma, r2_11_mean, r2_11_sigma, r2_11_mean, r2_11_sigma, nudyn_12_mean, nudyn_12_sigma);
//
//
//
//  compareRatio("f1_1", f1_1Eff_mean, f1_1Eff_sigma, f1_1_mean, f1_1_sigma, predictedF1_1Ratio_mean, predictedF1_1Ratio_sigma, f1_1Ratio_mean, f1_1Ratio_sigma);
//  compareRatio("f1_2", f1_2Eff_mean, f1_2Eff_sigma, f1_2_mean, f1_2_sigma, predictedF1_2Ratio_mean, predictedF1_2Ratio_sigma, f1_2Ratio_mean, f1_2Ratio_sigma);
//
//  compareRatio("f2_11", f2_11Eff_mean, f2_11Eff_sigma, f2_11_mean, f2_11_sigma, predictedF2_11Ratio_mean, predictedF2_11Ratio_sigma, f2_11Ratio_mean, f2_11Ratio_sigma);
//  compareRatio("f2_12", f2_12Eff_mean, f2_12Eff_sigma, f2_12_mean, f2_12_sigma, predictedF2_12Ratio_mean, predictedF2_12Ratio_sigma, f2_12Ratio_mean, f2_12Ratio_sigma);
//  compareRatio("f2_22", f2_22Eff_mean, f2_22Eff_sigma, f2_22_mean, f2_22_sigma, predictedF2_22Ratio_mean, predictedF2_22Ratio_sigma, f2_22Ratio_mean, f2_22Ratio_sigma);
//
//  compareRatio("r2_11", r2_11Eff_mean, r2_11Eff_sigma, r2_11_mean, r2_11_sigma, predictedR2_11Ratio_mean, predictedR2_11Ratio_sigma, r2_11Ratio_mean, r2_11Ratio_sigma);
//  compareRatio("r2_12", r2_12Eff_mean, r2_12Eff_sigma, r2_12_mean, r2_12_sigma, predictedR2_12Ratio_mean, predictedR2_11Ratio_sigma, r2_12Ratio_mean, r2_12Ratio_sigma);
//  compareRatio("r2_22", r2_22Eff_mean, r2_22Eff_sigma, r2_22_mean, r2_22_sigma, predictedR2_22Ratio_mean, predictedR2_11Ratio_sigma, r2_22Ratio_mean, r2_22Ratio_sigma);
//  compareRatio("nudyn_12", nudyn_12Eff_mean, nudyn_12Eff_sigma, nudyn_12_mean, nudyn_12_sigma, predictedNudyn_12Ratio_mean, predictedNudyn_12Ratio_sigma, nudyn_12Ratio_mean, nudyn_12Ratio_sigma);
//


  canvasCollection->printAllCanvas(outputPath);
  return 0;

}

//void plot(int n, TH1** h, GraphConfiguration** g, HistogramCollection * hc, int rebin)
// {
// for (int i=0; i<n; i++)
//   {
//   hc->setHistoProperties(h[i], *g[i]);
//   if (rebin>1)
//     {
//     h[i]->Rebin(rebin);
//     h[i]->Scale(1.0/double(rebin));
//     }
//
//   if (i==0)
//     h[i]->Draw();
//   else
//     h[i]->Draw("SAME");
//   }
// }
