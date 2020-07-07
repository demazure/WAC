
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


int binomialLoss(int nIn, double efficiency)
{
  int nOut = 0;
  for (int n=0; n<nIn; n++)
    {
    if (gRandom->Rndm()<efficiency) nOut++;
    }
  return nOut;
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


  long nEvents = 1000000;
  int nSubSamples = 200;

  double nPlusAvg = 100;
  double nMinusAvg = 100;
  double nPlusPlusAvg = 10;
  double nMinusMinusAvg = 10;
  double nPlusMinusAvg = 13;

  double plusEfficiency = 0.9;
  double minusEfficiency = 0.8;

  double nPlus;
  double nMinus;
  double nPlusPlus;
  double nPlusMinus;
  double nMinusMinus;
  double nPlusEff;
  double nMinusEff;

  double f1_1Mean,     f1_1Err,     f1_1Std;
  double f1_2Mean,     f1_2Err,     f1_2Std;
  double f2_11Mean,    f2_11Err,    f2_11Std;
  double f2_12Mean,    f2_12Err,    f2_12Std;
  double f2_22Mean,    f2_22Err,    f2_22Std;
  double r2_11Mean,    r2_11Err,    r2_11Std;
  double r2_12Mean,    r2_12Err,    r2_12Std;
  double r2_22Mean,    r2_22Err,    r2_22Std;
  double nudyn_12Mean, nudyn_12Err, nudyn_12Std;

  TString inputPath = "/Users/claudeapruneau/Documents/GitHub/run/";
  TString outputPath = "/Users/claudeapruneau/Documents/GitHub/run/";
  TString baseName  = "output";
  TString outputFileName = "StatStudy.root";

  HistogramCollection * hc = new HistogramCollection("Collection",100);
  int nBins = nSubSamples+2;
  TH1* f1_1     = hc->createProfile(  "f1_1",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(1)}>");
  TH1* f1_2     = hc->createProfile(  "f1_2",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(2)}>");
  TH1* f2_11    = hc->createProfile(  "f2_11",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(11)}>");
  TH1* f2_12    = hc->createProfile(  "f2_12",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(12)}>");
  TH1* f2_22    = hc->createProfile(  "f2_22",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(22)}>");
  TH1* r2_11    = hc->createHistogram("r2_11",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(11)}>");
  TH1* r2_12    = hc->createHistogram("r2_12",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(12)}>");
  TH1* r2_22    = hc->createHistogram("r2_22",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(22)}>");
  TH1* nudyn_12 = hc->createHistogram("nudyn_12",nBins,0.0,  double(nBins),  "sub-sample","<#nu_{dyn}^{(12)}>");

  TH1* f1_1Eff     = hc->createProfile(  "f1_1Eff",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(1,eff)}>");
  TH1* f1_2Eff     = hc->createProfile(  "f1_2Eff",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(2,eff)}>");
  TH1* f2_11Eff    = hc->createProfile(  "f2_11Eff",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(11,eff)}>");
  TH1* f2_12Eff    = hc->createProfile(  "f2_12Eff",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(12,eff)}>");
  TH1* f2_22Eff    = hc->createProfile(  "f2_22Eff",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(22,eff)}>");
  TH1* r2_11Eff    = hc->createHistogram("r2_11Eff",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(11,eff)}>");
  TH1* r2_12Eff    = hc->createHistogram("r2_12Eff",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(12,eff)}>");
  TH1* r2_22Eff    = hc->createHistogram("r2_22Eff",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(22,eff)}>");
  TH1* nudyn_12Eff = hc->createHistogram("nudyn_12Eff",nBins,0.0,  double(nBins),  "sub-sample","<#nu_{dyn}^{(12,eff)}>");

  TH1* f1_1Ratio     = hc->createHistogram("f1_1Ratio",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(1,eff)}>/<n_{1}^{(1)}>");
  TH1* f1_2Ratio     = hc->createHistogram("f1_2Ratio",    nBins,0.0,  double(nBins),  "sub-sample","<n_{1}^{(2,eff)}>/<n_{1}^{(2)}>");
  TH1* f2_11Ratio    = hc->createHistogram("f2_11Ratio",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(11,eff)}>/<n_{2}^{(11)}>");
  TH1* f2_12Ratio    = hc->createHistogram("f2_12Ratio",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(12,eff)}>/<n_{2}^{(12)}>");
  TH1* f2_22Ratio    = hc->createHistogram("f2_22Ratio",   nBins,0.0,  double(nBins),  "sub-sample","<n_{2}^{(22,eff)}>/<n_{2}^{(22)}>");
  TH1* r2_11Ratio    = hc->createHistogram("r2_11Ratio",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(11,eff)}>/<r_{2}^{(11)}>");
  TH1* r2_12Ratio    = hc->createHistogram("r2_12Ratio",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(12,eff)}>/<r_{2}^{(12)}>");
  TH1* r2_22Ratio    = hc->createHistogram("r2_22Ratio",   nBins,0.0,  double(nBins),  "sub-sample","<r_{2}^{(22,eff)}>/<r_{2}^{(22)}>");
  TH1* nudyn_12Ratio = hc->createHistogram("nudyn_12Ratio",nBins,0.0,  double(nBins),  "sub-sample","<#nu_{dyn}^{(12,eff)}>/<#nu_{dyn}^{(12)}>");


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

        f1_1->Fill(sub, nPlus);
        f1_2->Fill(sub, nMinus);
        f2_11->Fill(sub, nPlus*(nPlus-1));
        f2_12->Fill(sub, nPlus*nMinus);
        f2_22->Fill(sub, nMinus*(nMinus-1));

        nPlusEff   = binomialLoss(nPlus,plusEfficiency);
        nMinusEff  = binomialLoss(nMinus,minusEfficiency);

        f1_1Eff->Fill(sub, nPlusEff);
        f1_2Eff->Fill(sub, nMinusEff);
        f2_11Eff->Fill(sub, nPlusEff*(nPlusEff-1));
        f2_12Eff->Fill(sub, nPlusEff*nMinusEff);
        f2_22Eff->Fill(sub, nMinusEff*(nMinusEff-1));
        }
    sub += 1.0;
    }

  calculateR2(f1_1,     f2_11,  r2_11);
  calculateR2(f1_1,     f1_2,   f2_12, r2_12);
  calculateR2(f1_2,     f2_22,  r2_22);
  calculateNuDyn(r2_11, r2_22,  r2_12, nudyn_12);

  calculateR2(f1_1Eff,     f2_11Eff,  r2_11Eff);
  calculateR2(f1_1Eff,     f1_2Eff,   f2_12Eff, r2_12Eff);
  calculateR2(f1_2Eff,     f2_22Eff,  r2_22Eff);
  calculateNuDyn(r2_11Eff, r2_22Eff,  r2_12Eff, nudyn_12Eff);

  calculateRatio(f1_1Eff, f1_1,  f1_1Ratio);
  calculateRatio(f1_2Eff, f1_2,  f1_2Ratio);
  calculateRatio(f2_11Eff,f2_11, f2_11Ratio);
  calculateRatio(f2_12Eff,f2_12, f2_12Ratio);
  calculateRatio(f2_22Eff,f2_22, f2_22Ratio);
  calculateRatio(r2_11Eff,r2_11, r2_11Ratio);
  calculateRatio(r2_12Eff,r2_12, r2_12Ratio);
  calculateRatio(r2_22Eff,r2_22, r2_22Ratio);
  calculateRatio(nudyn_12Eff,nudyn_12, nudyn_12Ratio);

  calculateMeanAndStd(f1_1Ratio, nSubSamples,f1_1Mean,  f1_1Err,  f1_1Std);
  calculateMeanAndStd(f1_2Ratio, nSubSamples,f1_2Mean,  f1_2Err,  f1_2Std);
  calculateMeanAndStd(f2_11Ratio,nSubSamples,f2_11Mean, f2_11Err,  f2_11Std);
  calculateMeanAndStd(f2_12Ratio,nSubSamples,f2_12Mean, f2_12Err,  f2_12Std);
  calculateMeanAndStd(f2_22Ratio,nSubSamples,f2_22Mean, f2_22Err,  f2_22Std);
  calculateMeanAndStd(r2_11Ratio,nSubSamples,r2_11Mean, r2_11Err,  r2_11Std);
  calculateMeanAndStd(r2_12Ratio,nSubSamples,r2_12Mean, r2_12Err,  r2_12Std);
  calculateMeanAndStd(r2_22Ratio,nSubSamples,r2_22Mean, r2_22Err,  r2_22Std);
  calculateMeanAndStd(nudyn_12Ratio,nSubSamples,nudyn_12Mean, nudyn_12Err, nudyn_12Std);

  cout << "     f1_1Mean: " <<  f1_1Mean     << " f1_1Std : "    <<   f1_1Std << endl;
  cout << "     f1_2Mean: " <<  f1_2Mean     << " f1_2Std : "    <<   f1_2Std << endl;
  cout << "    f2_11Mean: " <<  f2_11Mean    << " f2_11Std : "   <<   f2_11Std << endl;
  cout << "    f2_12Mean: " <<  f2_12Mean    << " f2_12Std : "   <<   f2_12Std << endl;
  cout << "    f2_22Mean: " <<  f2_22Mean    << " f2_22Std: "    <<   f2_22Std << endl;
  cout << "    r2_11Mean: " <<  r2_11Mean    << " r2_11Std: "    <<   r2_11Std << endl;
  cout << "    r2_12Mean: " <<  r2_12Mean    << " r2_12Std: "    <<   r2_12Std << endl;
  cout << "    r2_22Mean: " <<  r2_22Mean    << " r2_22Std: "    <<   r2_22Std << endl;
  cout << " nudyn_12Mean: " <<  nudyn_12Mean << " nudyn_12Std: " <<   nudyn_12Std << endl;

  f1_1Ratio->SetBinContent(nBins-1,f1_1Mean); f1_1Ratio->SetBinError(nBins-1,f1_1Err);
   f1_2Ratio->SetBinContent(nBins-1,f1_2Mean); f1_2Ratio->SetBinError(nBins-1,f1_2Err);
   f2_11Ratio->SetBinContent(nBins-1,f2_11Mean); f2_11Ratio->SetBinError(nBins-1,f2_11Err);
   f2_12Ratio->SetBinContent(nBins-1,f2_12Mean); f2_12Ratio->SetBinError(nBins-1,f2_12Err);
   f2_22Ratio->SetBinContent(nBins-1,f2_22Mean); f2_22Ratio->SetBinError(nBins-1,f2_22Err);
   r2_11Ratio->SetBinContent(nBins-1,r2_11Mean); r2_11Ratio->SetBinError(nBins-1,r2_11Err);
   r2_12Ratio->SetBinContent(nBins-1,r2_12Mean); r2_12Ratio->SetBinError(nBins-1,r2_12Err);
   r2_22Ratio->SetBinContent(nBins-1,r2_22Mean); r2_22Ratio->SetBinError(nBins-1,r2_22Err);
   nudyn_12Ratio->SetBinContent(nBins-1,nudyn_12Mean); nudyn_12Ratio->SetBinError(nBins-1,nudyn_12Err);


  f1_1Ratio->SetBinContent(nBins,f1_1Mean); f1_1Ratio->SetBinError(nBins,f1_1Std);
  f1_2Ratio->SetBinContent(nBins,f1_2Mean); f1_2Ratio->SetBinError(nBins,f1_2Std);
  f2_11Ratio->SetBinContent(nBins,f2_11Mean); f2_11Ratio->SetBinError(nBins,f2_11Std);
  f2_12Ratio->SetBinContent(nBins,f2_12Mean); f2_12Ratio->SetBinError(nBins,f2_12Std);
  f2_22Ratio->SetBinContent(nBins,f2_22Mean); f2_22Ratio->SetBinError(nBins,f2_22Std);
  r2_11Ratio->SetBinContent(nBins,r2_11Mean); r2_11Ratio->SetBinError(nBins,r2_11Std);
  r2_12Ratio->SetBinContent(nBins,r2_12Mean); r2_12Ratio->SetBinError(nBins,r2_12Std);
  r2_22Ratio->SetBinContent(nBins,r2_22Mean); r2_22Ratio->SetBinError(nBins,r2_22Std);
  nudyn_12Ratio->SetBinContent(nBins,nudyn_12Mean); nudyn_12Ratio->SetBinError(nBins,nudyn_12Std);


 TFile * outputFile = new TFile(outputFileName,"RECREATE");

  f1_1 ->Write();
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

  outputFile->Close();
//  canvas = canvasCollection.createCanvas(canvasNameBase+"nudyn_vsMult",cc1d);
//    for (int iFile=0; iFile<nFiles; iFile++) hh[iFile] = nuDynDerived[iFile]->h_nudyn_12_vsMult;
//    plot(nFiles, hh, graphConfigs1D, &histogramCollection,rebin);
//
//  canvasCollection.printAllCanvas(outputPath);
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
