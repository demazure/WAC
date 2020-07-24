
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

    //cout << ratio->GetName() << " iBin:" << iBin << "  r:" << r << "   er:" << er << endl;
    ratio->SetBinContent(iBin, r);
    ratio->SetBinError  (iBin, er);
    }
}

void calculateMeanAndStd(TH1 * h, int nBins, double &mean, double &typicalError, double &eMean, double &std)
{
  double m1  = 0;
  double e   = 0;
  double em1 = 0;
  double v, ev;
  // calculate mean value of the nBins
  for (int iBin=1; iBin<=nBins; iBin++)
    {
    v  = h->GetBinContent(iBin);
    ev = h->GetBinError(iBin);
    m1 += v;
    e  += ev;
    em1 += ev*ev;
    }
  mean = m1/double(nBins);
  typicalError = e/double(nBins);
  eMean = sqrt(em1)/double(nBins);

  double m2  = 0;
  for (int iBin=1; iBin<=nBins; iBin++)
    {
    v  = h->GetBinContent(iBin)-mean;
    m2 += v*v;
    }
  m2 = m2/double(nBins-1);
  std = sqrt(m2);
}


int binomialLoss(int nIn, double efficiency)
{
  int nOut = 0;
  for (int n=0; n<nIn; n++)
    {
    if (gRandom->Rndm()<efficiency) nOut++;
    }
  return nOut;
}

void fillMeanErrStd(TH1 * h, int n)
{
  double mean, typicalError, errorOnMean, std;
  calculateMeanAndStd(h, n, mean,  typicalError,  errorOnMean, std);
  cout << "         histo:" << h->GetName() << endl;
  cout << "          mean:" << mean << endl;
  cout << "  typicalError:" << typicalError << endl;
  cout << "   errorOnMean:" << errorOnMean << endl;
  cout << "           std:" << std << endl;
  h->SetBinContent(n-2,mean); h->SetBinError(n-2,typicalError);
  h->SetBinContent(n-1,mean); h->SetBinError(n-1,errorOnMean);
  h->SetBinContent(n,  mean); h->SetBinError(n,  std);

}



int StatCDStudy()
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


  long nEvents = 10000;
  int nSubSamples = 4000;

  // Set 1
//  double nPlusAvg = 100;
//   double nMinusAvg = 100;
//   double nPlusPlusAvg = 20;
//   double nMinusMinusAvg = 20;
//   double nPlusMinusAvg = 60;

  // Set 2
  double nPlusAvg = 10;
   double nMinusAvg = 10;
   double nPlusPlusAvg = 2;
   double nMinusMinusAvg = 2;
   double nPlusMinusAvg = 6;

  double plusEfficiency  = 0.87;
  double minusEfficiency = 0.85;

  double nPlus;
  double nMinus;
  double nPlusPlus;
  double nPlusMinus;
  double nMinusMinus;
  double nPlusEff;
  double nMinusEff;

  double f1_1_Mean  = nPlusAvg + nPlusPlusAvg + nPlusMinusAvg;
  double f1_2_Mean  = nMinusAvg + nMinusMinusAvg + nPlusMinusAvg;
  double f2_11_Mean = nPlusAvg*nPlusAvg   + 2.0*nPlusPlusAvg;
  double f2_12_Mean = nPlusAvg*nMinusAvg  + nPlusMinusAvg;
  double f2_22_Mean = nMinusAvg*nMinusAvg + 2.0*nMinusMinusAvg;

  double f1_min = 50;
  double f1_max = 300;
  int nBins = f1_max - f1_min;
  double f2_min = f1_min*f1_min;
  double f2_max = f1_max*f1_max;
  int    nBins2 = (f2_max - f2_min)/4.0;

  TString inputPath = "/Users/claudeapruneau/Documents/GitHub/run/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/run/";
  TString baseName  = "output";
  TString outputFileName = "StatStudyNew.root";

  HistogramCollection * hc = new HistogramCollection("Collection",100);

  TH1* f1_1     = hc->createHistogram("f1_1",    nBins, f1_min, f1_max, "<n_{1}^{(1)}>",  "Counts");
  TH1* f1_2     = hc->createHistogram("f1_2",    nBins, f1_min, f1_max, "<n_{1}^{(2)}>",  "Counts");
  nBins = 50;
  TH1* f2_11    = hc->createHistogram("f2_11",   nBins2, f2_min, f2_max, "<n_{2}^{(11)}>", "Counts");
  TH1* f2_12    = hc->createHistogram("f2_12",   nBins2, f2_min, f2_max, "<n_{2}^{(12)}>", "Counts");
  TH1* f2_22    = hc->createHistogram("f2_22",   nBins2, f2_min, f2_max, "<n_{2}^{(22)}>", "Counts");
  nBins = 1000;
  TH1* r2_11    = hc->createHistogram("r2_11",   nBins, 0.99,  1.01,    "<r_{2}^{(11)}>", "Counts");
  TH1* r2_12    = hc->createHistogram("r2_12",   nBins, 0.99,  1.01,    "<r_{2}^{(12)}>", "Counts");
  TH1* r2_22    = hc->createHistogram("r2_22",   nBins, 0.99,  1.01,    "<r_{2}^{(22)}>", "Counts");
  TH1* nudyn_12 = hc->createHistogram("nudyn_12",nBins, -0.02,   0.02,    "<#nu_{dyn}^{(12)}>", "Counts");

  nBins = 150;
  TH1* f1_1Eff     = hc->createHistogram("f1_1Eff",    nBins, f1_min, f1_max, "<n_{1}^{(1)}>",  "Counts");
  TH1* f1_2Eff     = hc->createHistogram("f1_2Eff",    nBins, f1_min, f1_max, "<n_{1}^{(2)}>",  "Counts");
  TH1* f2_11Eff    = hc->createHistogram("f2_11Eff",   nBins2, f2_min, f2_max, "<n_{2}^{(11)}>", "Counts");
  TH1* f2_12Eff    = hc->createHistogram("f2_12Eff",   nBins2, f2_min, f2_max, "<n_{2}^{(12)}>", "Counts");
  TH1* f2_22Eff    = hc->createHistogram("f2_22Eff",   nBins2, f2_min, f2_max, "<n_{2}^{(22)}>", "Counts");
  nBins = 1000;
  TH1* r2_11Eff    = hc->createHistogram("r2_11Eff",   nBins, 0.99,  1.01,    "<r_{2}^{(11)}>", "Counts");
  TH1* r2_12Eff    = hc->createHistogram("r2_12Eff",   nBins, 0.99,  1.01,    "<r_{2}^{(12)}>", "Counts");
  TH1* r2_22Eff    = hc->createHistogram("r2_22Eff",   nBins, 0.99,  1.01,    "<r_{2}^{(22)}>", "Counts");
  TH1* nudyn_12Eff = hc->createHistogram("nudyn_12Eff",nBins, -0.02,   0.02,  "<#nu_{dyn}^{(12)}>", "Counts");

  nBins = 1000;
  TH1* f1_1Ratio     = hc->createHistogram("f1_1Ratio",    nBins,0.8,   0.90, "<n_{1}^{(1,eff)}>/<n_{1}^{(1)}>",   "Counts");
  TH1* f1_2Ratio     = hc->createHistogram("f1_2Ratio",    nBins,0.8,   0.90, "<n_{1}^{(2,eff)}>/<n_{1}^{(2)}>",   "Counts");
  TH1* f2_11Ratio    = hc->createHistogram("f2_11Ratio",   nBins,0.5,   1.00, "<n_{2}^{(11,eff)}>/<n_{2}^{(11)}>", "Counts");
  TH1* f2_12Ratio    = hc->createHistogram("f2_12Ratio",   nBins,0.5,   1.00, "<n_{2}^{(12,eff)}>/<n_{2}^{(12)}>", "Counts");
  TH1* f2_22Ratio    = hc->createHistogram("f2_22Ratio",   nBins,0.5,   1.00, "<n_{2}^{(22,eff)}>/<n_{2}^{(22)}>", "Counts");
  nBins = 1000;
  TH1* r2_11Ratio    = hc->createHistogram("r2_11Ratio",   nBins,0.99,  1.01, "<r_{2}^{(11,eff)}>/<r_{2}^{(11)}>", "Counts");
  TH1* r2_12Ratio    = hc->createHistogram("r2_12Ratio",   nBins,0.99,  1.01, "<r_{2}^{(12,eff)}>/<r_{2}^{(12)}>", "Counts");
  TH1* r2_22Ratio    = hc->createHistogram("r2_22Ratio",   nBins,0.99,  1.01, "<r_{2}^{(22,eff)}>/<r_{2}^{(22)}>", "Counts");
  TH1* nudyn_12Ratio = hc->createHistogram("nudyn_12Ratio",nBins,0.99,  1.01, "<#nu_{dyn}^{(12,eff)}>/<#nu_{dyn}^{(12)}>", "Counts");

  ///
  nBins = nSubSamples+3;

  TH1* f1_1_vsSub     = hc->createProfile(  "f1_1_vsSub",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(1)}>");
  TH1* f1_2_vsSub     = hc->createProfile(  "f1_2_vsSub",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(2)}>");
  TH1* f2_11_vsSub    = hc->createProfile(  "f2_11_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(11)}>");
  TH1* f2_12_vsSub    = hc->createProfile(  "f2_12_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(12)}>");
  TH1* f2_22_vsSub    = hc->createProfile(  "f2_22_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(22)}>");
  TH1* r2_11_vsSub    = hc->createHistogram("r2_11_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(11)}>");
  TH1* r2_12_vsSub    = hc->createHistogram("r2_12_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(12)}>");
  TH1* r2_22_vsSub    = hc->createHistogram("r2_22_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(22)}>");
  TH1* nudyn_12_vsSub = hc->createHistogram("nudyn_12_vsSub",nBins,0.0,  double(nBins),  "sub-sample","<#nu_{dyn}^{(12)}>");

  TH1* f1_1Eff_vsSub     = hc->createProfile(  "f1_1Eff_vsSub",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(1,eff)}>");
  TH1* f1_2Eff_vsSub     = hc->createProfile(  "f1_2Eff_vsSub",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(2,eff)}>");
  TH1* f2_11Eff_vsSub    = hc->createProfile(  "f2_11Eff_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(11,eff)}>");
  TH1* f2_12Eff_vsSub    = hc->createProfile(  "f2_12Eff_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(12,eff)}>");
  TH1* f2_22Eff_vsSub    = hc->createProfile(  "f2_22Eff_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(22,eff)}>");
  TH1* r2_11Eff_vsSub    = hc->createHistogram("r2_11Eff_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(11,eff)}>");
  TH1* r2_12Eff_vsSub    = hc->createHistogram("r2_12Eff_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(12,eff)}>");
  TH1* r2_22Eff_vsSub    = hc->createHistogram("r2_22Eff_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(22,eff)}>");
  TH1* nudyn_12Eff_vsSub = hc->createHistogram("nudyn_12Eff_vsSub",nBins,0.0,  double(nBins),  "sub-sample","<#nu_{dyn}^{(12,eff)}>");

  TH1* f1_1Ratio_vsSub     = hc->createHistogram("f1_1Ratio_vsSub",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(1,eff)}>/<n_{1}^{(1)}>");
  TH1* f1_2Ratio_vsSub     = hc->createHistogram("f1_2Ratio_vsSub",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(2,eff)}>/<n_{1}^{(2)}>");
  TH1* f2_11Ratio_vsSub    = hc->createHistogram("f2_11Ratio_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(11,eff)}>/<n_{2}^{(11)}>");
  TH1* f2_12Ratio_vsSub    = hc->createHistogram("f2_12Ratio_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(12,eff)}>/<n_{2}^{(12)}>");
  TH1* f2_22Ratio_vsSub    = hc->createHistogram("f2_22Ratio_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(22,eff)}>/<n_{2}^{(22)}>");
  TH1* r2_11Ratio_vsSub    = hc->createHistogram("r2_11Ratio_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(11,eff)}>/<r_{2}^{(11)}>");
  TH1* r2_12Ratio_vsSub    = hc->createHistogram("r2_12Ratio_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(12,eff)}>/<r_{2}^{(12)}>");
  TH1* r2_22Ratio_vsSub    = hc->createHistogram("r2_22Ratio_vsSub",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(22,eff)}>/<r_{2}^{(22)}>");
  TH1* nudyn_12Ratio_vsSub = hc->createHistogram("nudyn_12Ratio_vsSub",nBins,0.0,  double(nBins),  "sub-sample","<#nu_{dyn}^{(12,eff)}>/<#nu_{dyn}^{(12)}>");



  double sub = 0.5;
  for (double iSub=0; iSub<nSubSamples; iSub++)
    {
    for (long iEvent = 0; iEvent<nEvents; iEvent++)
      {
      nPlus       = int( gRandom->Gaus(nPlusAvg,sqrt(nPlusAvg))            );
      nMinus      = int( gRandom->Gaus(nMinusAvg,sqrt(nMinusAvg))          );
      nPlusPlus   = int( gRandom->Gaus(nPlusPlusAvg,sqrt(nPlusPlusAvg))    );
      nPlusMinus  = int( gRandom->Gaus(nPlusMinusAvg,sqrt(nPlusMinusAvg))  );
      nMinusMinus = int( gRandom->Gaus(nMinusMinusAvg,sqrt(nMinusMinusAvg)));
      //    cout << " nPlus:" << nPlus << endl;

      nPlus += 2.0*nPlusPlus;
      nPlus += nPlusMinus;
      nMinus += 2.0*nMinusMinus;
      nMinus += nPlusMinus;

      nPlusEff   = binomialLoss(nPlus,plusEfficiency);
      nMinusEff  = binomialLoss(nMinus,minusEfficiency);

      f1_1->Fill(nPlus);
      f1_2->Fill(nMinus);
      f2_11->Fill(nPlus*(nPlus-1));
      f2_12->Fill(nPlus*nMinus);
      f2_22->Fill(nMinus*(nMinus-1));

      f1_1_vsSub->Fill(sub, nPlus);
      f1_2_vsSub->Fill(sub, nMinus);
      f2_11_vsSub->Fill(sub, nPlus*(nPlus-1));
      f2_12_vsSub->Fill(sub, nPlus*nMinus);
      f2_22_vsSub->Fill(sub, nMinus*(nMinus-1));

      f1_1Eff->Fill(nPlusEff);
      f1_2Eff->Fill(nMinusEff);
      f2_11Eff->Fill(nPlusEff*(nPlusEff-1));
      f2_12Eff->Fill(nPlusEff*nMinusEff);
      f2_22Eff->Fill(nMinusEff*(nMinusEff-1));

      f1_1Eff_vsSub->Fill(sub, nPlusEff);
      f1_2Eff_vsSub->Fill(sub, nMinusEff);
      f2_11Eff_vsSub->Fill(sub, nPlusEff*(nPlusEff-1));
      f2_12Eff_vsSub->Fill(sub, nPlusEff*nMinusEff);
      f2_22Eff_vsSub->Fill(sub, nMinusEff*(nMinusEff-1));


      }

    sub += 1.0;
    }

  calculateR2(f1_1_vsSub,     f2_11_vsSub,  r2_11_vsSub);
  calculateR2(f1_1_vsSub,     f1_2_vsSub,   f2_12_vsSub, r2_12_vsSub);
  calculateR2(f1_2_vsSub,     f2_22_vsSub,  r2_22_vsSub);
  calculateNuDyn(r2_11_vsSub, r2_22_vsSub,  r2_12_vsSub, nudyn_12_vsSub);

  calculateR2(f1_1Eff_vsSub,     f2_11Eff_vsSub,  r2_11Eff_vsSub);
  calculateR2(f1_1Eff_vsSub,     f1_2Eff_vsSub,   f2_12Eff_vsSub, r2_12Eff_vsSub);
  calculateR2(f1_2Eff_vsSub,     f2_22Eff_vsSub,  r2_22Eff_vsSub);
  calculateNuDyn(r2_11Eff_vsSub, r2_22Eff_vsSub,  r2_12Eff_vsSub, nudyn_12Eff_vsSub);

  calculateRatio(f1_1Eff_vsSub, f1_1_vsSub,  f1_1Ratio_vsSub);
  calculateRatio(f1_2Eff_vsSub, f1_2_vsSub,  f1_2Ratio_vsSub);
  calculateRatio(f2_11Eff_vsSub,f2_11_vsSub, f2_11Ratio_vsSub);
  calculateRatio(f2_12Eff_vsSub,f2_12_vsSub, f2_12Ratio_vsSub);
  calculateRatio(f2_22Eff_vsSub,f2_22_vsSub, f2_22Ratio_vsSub);
  calculateRatio(r2_11Eff_vsSub,r2_11_vsSub, r2_11Ratio_vsSub);
  calculateRatio(r2_12Eff_vsSub,r2_12_vsSub, r2_12Ratio_vsSub);
  calculateRatio(r2_22Eff_vsSub,r2_22_vsSub, r2_22Ratio_vsSub);
  calculateRatio(nudyn_12Eff_vsSub, nudyn_12_vsSub, nudyn_12Ratio_vsSub);

  for (int iSub=1; iSub<=nSubSamples; iSub++)
    {
    double f1_1_value  = f1_1_vsSub->GetBinContent(iSub);
    double f1_2_value  = f1_2_vsSub->GetBinContent(iSub);
    double f2_11_value = f2_11_vsSub->GetBinContent(iSub);
    double f2_12_value = f2_12_vsSub->GetBinContent(iSub);
    double f2_22_value = f2_22_vsSub->GetBinContent(iSub);

    if (f1_1_value <= 0.0 || f1_2_value <= 0.0) continue;

    double r2_11_value = f2_11_value/(f1_1_value*f1_1_value);
    double r2_12_value = f2_12_value/(f1_1_value*f1_2_value);
    double r2_22_value = f2_22_value/(f1_2_value*f1_2_value);
    double nudyn_value = r2_11_value  + r2_22_value - 2.0*r2_12_value;

    r2_11->Fill(r2_11_value);
    r2_12->Fill(r2_12_value);
    r2_22->Fill(r2_22_value);
    nudyn_12->Fill(nudyn_value);

    double f1_1Eff_value  = f1_1Eff_vsSub->GetBinContent(iSub);
    double f1_2Eff_value  = f1_2Eff_vsSub->GetBinContent(iSub);
    double f2_11Eff_value = f2_11Eff_vsSub->GetBinContent(iSub);
    double f2_12Eff_value = f2_12Eff_vsSub->GetBinContent(iSub);
    double f2_22Eff_value = f2_22Eff_vsSub->GetBinContent(iSub);

    if (f1_1Eff_value <= 0.0 || f1_2Eff_value <= 0.0) continue;

    double r2_11Eff_value = f2_11Eff_value/(f1_1Eff_value*f1_1Eff_value);
    double r2_12Eff_value = f2_12Eff_value/(f1_1Eff_value*f1_2Eff_value);
    double r2_22Eff_value = f2_22Eff_value/(f1_2Eff_value*f1_2Eff_value);
    double nudynEff_value = r2_11Eff_value  + r2_22Eff_value - 2.0*r2_12Eff_value;

    r2_11Eff->Fill(r2_11Eff_value);
    r2_12Eff->Fill(r2_12Eff_value);
    r2_22Eff->Fill(r2_22Eff_value);
    nudyn_12Eff->Fill(nudynEff_value);

    double f1_1Ratio_value  = f1_1_value!=0 ? f1_1Eff_value/f1_1_value : 0.0;
    double f1_2Ratio_value  = f1_2_value!=0 ? f1_2Eff_value/f1_2_value : 0.0;
    double f2_11Ratio_value = f2_11_value!=0 ? f2_11Eff_value/f2_11_value : 0.0;
    double f2_12Ratio_value = f2_12_value!=0 ? f2_12Eff_value/f2_12_value : 0.0;
    double f2_22Ratio_value = f2_22_value!=0 ? f2_22Eff_value/f2_22_value : 0.0;
    double r2_11Ratio_value = r2_11_value!=0 ? r2_11Eff_value/r2_11_value : 0.0;
    double r2_12Ratio_value = r2_12_value!=0 ? r2_12Eff_value/r2_12_value : 0.0;
    double r2_22Ratio_value = r2_22_value!=0 ? r2_22Eff_value/r2_22_value : 0.0;
    double nudyn_12Ratio_value = nudyn_value!=0 ? nudynEff_value/nudyn_value : 0.0;

    f1_1Ratio->Fill(f1_1Ratio_value);
    f1_2Ratio->Fill(f1_2Ratio_value);
    f2_11Ratio->Fill(f2_11Ratio_value);
    f2_12Ratio->Fill(f2_12Ratio_value);
    f2_22Ratio->Fill(f2_22Ratio_value);
    r2_11Ratio->Fill(r2_11Ratio_value);
    r2_12Ratio->Fill(r2_12Ratio_value);
    r2_22Ratio->Fill(r2_22Ratio_value);
    nudyn_12Ratio->Fill(r2_22Ratio_value);
    }


  fillMeanErrStd(f1_1_vsSub,nSubSamples);
  fillMeanErrStd(f1_2_vsSub,nSubSamples);
  fillMeanErrStd(f2_11_vsSub,nSubSamples);
  fillMeanErrStd(f2_22_vsSub,nSubSamples);
  fillMeanErrStd(f2_12_vsSub,nSubSamples);
  fillMeanErrStd(r2_11_vsSub,nSubSamples);
  fillMeanErrStd(r2_22_vsSub,nSubSamples);
  fillMeanErrStd(r2_12_vsSub,nSubSamples);
  fillMeanErrStd(nudyn_12_vsSub,nSubSamples);

  fillMeanErrStd(f1_1Eff_vsSub,nSubSamples);
  fillMeanErrStd(f1_2Eff_vsSub,nSubSamples);
  fillMeanErrStd(f2_11Eff_vsSub,nSubSamples);
  fillMeanErrStd(f2_22Eff_vsSub,nSubSamples);
  fillMeanErrStd(f2_12Eff_vsSub,nSubSamples);
  fillMeanErrStd(r2_11Eff_vsSub,nSubSamples);
  fillMeanErrStd(r2_22Eff_vsSub,nSubSamples);
  fillMeanErrStd(r2_12Eff_vsSub,nSubSamples);
  fillMeanErrStd(nudyn_12Eff_vsSub,nSubSamples);

  fillMeanErrStd(f1_1Ratio_vsSub,nSubSamples);
  fillMeanErrStd(f1_2Ratio_vsSub,nSubSamples);
  fillMeanErrStd(f2_11Ratio_vsSub,nSubSamples);
  fillMeanErrStd(f2_22Ratio_vsSub,nSubSamples);
  fillMeanErrStd(f2_12Ratio_vsSub,nSubSamples);
  fillMeanErrStd(r2_11Ratio_vsSub,nSubSamples);
  fillMeanErrStd(r2_22Ratio_vsSub,nSubSamples);
  fillMeanErrStd(r2_12Ratio_vsSub,nSubSamples);
  fillMeanErrStd(nudyn_12Ratio_vsSub,nSubSamples);

  TFile * outputFile = new TFile(outputFileName,"RECREATE");

  f1_1->Write();
  f1_2->Write();
  f2_11->Write();
  f2_12->Write();
  f2_22->Write();
  r2_11->Write();
  r2_12->Write();
  r2_22->Write();
  nudyn_12->Write();

  f1_1Eff->Write();
  f1_2Eff->Write();
  f2_11Eff->Write();
  f2_12Eff->Write();
  f2_22Eff->Write();
  r2_11Eff->Write();
  r2_12Eff->Write();
  r2_22Eff->Write();
  nudyn_12Eff->Write();

  f1_1Ratio->Write();
  f1_2Ratio->Write();
  f2_11Ratio->Write();
  f2_12Ratio->Write();
  f2_22Ratio->Write();
  r2_11Ratio->Write();
  r2_12Ratio->Write();
  r2_22Ratio->Write();
  nudyn_12Ratio->Write();

  f1_1_vsSub->Write();
  f1_2_vsSub->Write();
  f2_11_vsSub->Write();
  f2_12_vsSub->Write();
  f2_22_vsSub->Write();
  r2_11_vsSub->Write();
  r2_12_vsSub->Write();
  r2_22_vsSub->Write();
  nudyn_12_vsSub->Write();

  f1_1Eff_vsSub->Write();
  f1_2Eff_vsSub->Write();
  f2_11Eff_vsSub->Write();
  f2_12Eff_vsSub->Write();
  f2_22Eff_vsSub->Write();
  r2_11Eff_vsSub->Write();
  r2_12Eff_vsSub->Write();
  r2_22Eff_vsSub->Write();
  nudyn_12Eff_vsSub->Write();

  f1_1Ratio_vsSub->Write();
  f1_2Ratio_vsSub->Write();
  f2_11Ratio_vsSub->Write();
  f2_12Ratio_vsSub->Write();
  f2_22Ratio_vsSub->Write();
  r2_11Ratio_vsSub->Write();
  r2_12Ratio_vsSub->Write();
  r2_22Ratio_vsSub->Write();
  nudyn_12Ratio_vsSub->Write();

  outputFile->Close();
  //  canvas = canvasCollection.createCanvas(canvasNameBase+"nudyn_vsMult",cc1d);
  //    for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDynDerived[iFile]->h_nudyn_12_vsMult;
  //    plot(nFiles, hh, graphConfigs1D, &histogramCollection,rebin);
  //
  //  canvasCollection.printAllCanvas(outputPath);
  return 0;

}

//void plot(int n, TH1** h, GraphConfiguration** g, HistogramCollection * hc, int rebin)
//{
//  for (int i=0; i<n; i++)
//    {
//    hc->setHistoProperties(h[i], *g[i]);
//    if (rebin>1)
//      {
//      h[i]->Rebin(rebin);
//      h[i]->Scale(1.0/double(rebin));
//      }
//
//    if (i==0)
//      h[i]->Draw();
//    else
//      h[i]->Draw("SAME");
//    }
//}
