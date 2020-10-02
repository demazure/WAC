//
//  CollisionGeometryHistograms.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
// ===========================================================
#include "CollisionGeometryHistograms.hpp"
ClassImp(CollisionGeometryHistograms);

CollisionGeometryHistograms::CollisionGeometryHistograms(const TString & collectionName,
                                                         CollisionGeometryConfiguration * _configuration,
                                                         LogLevel  debugLevel)
:
Histograms(collectionName,nullptr,100,debugLevel),
configuration(_configuration),
h_b(0),
h_nPart(0),
h_nBinary(0),
h_nPartVsB_Prof(0),
h_nBinaryVsB_Prof(0),
h_nPartVsB(0),
h_nBinaryVsB(0),
h_xyNNIntVsB(0),
h_varXVsB_Prof(0),
h_varYVsB_Prof(0),
h_covXYVsB_Prof(0),
h_epsilonXVsB_Prof(0),
h_epsilonYVsB_Prof(0),
h_epsilonXYVsB_Prof(0),
h_psi2VsB_Prof(0),
h_psi2VsB(0)
{
  initialize();
}

CollisionGeometryHistograms::CollisionGeometryHistograms(TFile * inputFile,
                                                         const TString & collectionName,
                                                         CollisionGeometryConfiguration * _configuration,
                                                         LogLevel  debugLevel)
:
Histograms(collectionName,nullptr,100,debugLevel),
configuration(_configuration),
h_b(0),
h_nPart(0),
h_nBinary(0),
h_nPartVsB_Prof(0),
h_nBinaryVsB_Prof(0),
h_nPartVsB(0),
h_nBinaryVsB(0),
h_xyNNIntVsB(0),
h_varXVsB_Prof(0),
h_varYVsB_Prof(0),
h_covXYVsB_Prof(0),
h_epsilonXVsB_Prof(0),
h_epsilonYVsB_Prof(0),
h_epsilonXYVsB_Prof(0),
h_psi2VsB_Prof(0),
h_psi2VsB(0)
{
  loadHistograms(inputFile);
}

// for now use the same boundaries for eta and y histogram
void CollisionGeometryHistograms::createHistograms()
{
  CollisionGeometryConfiguration & ac = *configuration;
  TString bn = ac.histoBaseName; bn += "_";

  h_b                  = createHistogram(bn+TString("b"),                     ac.nBins_b,      ac.min_b,       ac.max_b,       "b",        "Counts", 0, 1);
  h_nPart              = createHistogram(bn+TString("nPart"),                 ac.nBins_nPart,  ac.min_nPart,   ac.max_nPart,   "n_{Part}", "Counts", 0, 1);
  h_nBinary            = createHistogram(bn+TString("nBinary"),               ac.nBins_nBinary,ac.min_nBinary, ac.max_nBinary, "n_{Bin}",  "Counts", 0, 1);
  h_nPartVsB_Prof      = createProfile(bn+TString("nPartVsB_Prof"),           ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "n_{Part}",1);
  h_nBinaryVsB_Prof    = createProfile(bn+TString("nBinaryVsB_Prof"),         ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "n_{Bin}", 1);
  h_nPartSqVsB_Prof    = createProfile(bn+TString("nPartSqVsB_Prof"),         ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "n_{Part}^{2}",1);
  h_nBinarySqVsB_Prof  = createProfile(bn+TString("nBinarySqVsB_Prof"),       ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "n_{Bin}^{2}", 1);
  h_nBinaryVsNPart_Prof   = createProfile(bn+TString("nBinaryVsNPart_Prof"),  ac.nBins_nPart, ac.min_nPart,   ac.max_nPart,  "n_{Part}", "n_{Bin}", 1);
  h_nBinarySqVsNPart_Prof = createProfile(bn+TString("nBinarySqVsNPart_Prof"),ac.nBins_nPart, ac.min_nPart,   ac.max_nPart,"n_{Part}", "n_{Bin}^{2}", 1);
  h_nPartVsB          = createHistogram(bn+TString("nPartVs"),                ac.nBins_b,     ac.min_b,  ac.max_b,  ac.nBins_nPart,   ac.min_nPart,   ac.max_nPart,   "b", "n_{Part}", "Counts",0,1);
  h_nBinaryVsB        = createHistogram(bn+TString("nBinaryVsB"),             ac.nBins_b,     ac.min_b,  ac.max_b,  ac.nBins_nBinary, ac.min_nBinary, ac.max_nBinary, "b", "n_{Bin}",  "Counts",0,1);
  h_nPartRmsVsB       = createHistogram(bn+TString("nPartRmsVsB"),            ac.nBins_b,     ac.min_b,  ac.max_b,  "b", "<n_{Part}^{2}>^{1/2}",0,1);
  h_nBinaryRmsVsB     = createHistogram(bn+TString("nBinaryRmsVsB"),          ac.nBins_b,     ac.min_b,  ac.max_b,  "b", "<n_{Bin}^{2}>^{1/2}",0,1);
  h_nBinaryRmsVsNPart = createHistogram(bn+TString("nBinaryRmsVsNPart"),      ac.nBins_nPart, ac.min_nPart,   ac.max_nPart, "n_{Part}", "<n_{Bin}^{2}>^{1/2}",0,1);

  h_nPartOmegaVsB        = createHistogram(bn+TString("nPartOmegaVsB"),       ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "#omega_{N_{part}}");
  h_nBinaryOmegaVsB      = createHistogram(bn+TString("nBinaryOmegaVsB"),     ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "#omega_{N_{Bin}}");
  h_nBinaryOmegaVsNPart  = createHistogram(bn+TString("nBinaryOmegaVsNPart"), ac.nBins_nPart, ac.min_nPart,   ac.max_nPart, "n_{Part}",   "#omega_{N_{Bin}}");
  h_nPartR2VsB           = createHistogram(bn+TString("nPartR2VsB"),          ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "R_{2}^{N_{part}}");
  h_nBinaryR2VsB         = createHistogram(bn+TString("nBinaryR2VsB"),        ac.nBins_b,     ac.min_b,  ac.max_b,  "b",  "R_{2}^{N_{bin}}");
  h_nBinaryR2VsNPart     = createHistogram(bn+TString("nBinaryR2VsNPart"),    ac.nBins_nPart, ac.min_nPart,   ac.max_nPart,"n_{Part}", "R_{2}^{N_{bin}}");

  h_xyNNIntVsB       = createHistogram(bn+TString("xyNNIntVsB"),     ac.nBins_b,  ac.min_b,  ac.max_b,  40, -10.0, 10.0, 40, -10.0, 10.0, "b", "x", "y", "Counts",0,1);

  h_varXVsB_Prof     = createProfile(bn+"varXVsB_Prof",     ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "Var[x]", 1);
  h_varYVsB_Prof     = createProfile(bn+"varYVsB_Prof",     ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "Var[y]", 1);
  h_covXYVsB_Prof    = createProfile(bn+"covXYVsB_Prof",    ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "Cov[x,y]", 1);
  h_epsilonXVsB_Prof = createProfile(bn+"epsilonXVsB_Prof", ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "#epsilon_{x}", 1);
  h_epsilonYVsB_Prof = createProfile(bn+"epsilonYVsB_Prof", ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "#epsilon_{y}", 1);
  h_epsilonXYVsB_Prof  = createProfile(bn+"epsilonXYVsB_Prof",  ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "|#epsilon|", 1);
  h_epsilonXYVsB       = createHistogram(bn+TString("epsilonXYVsB"), ac.nBins_b,  ac.min_b,  ac.max_b,  40, -1.0, 1.0,  40, -1.0, 1.0, "b", "#epsilon_{x}", "#epsilon_{y}", "Counts",0,1);;

  h_psi2VsB_Prof  = createProfile(bn+"psi2VsB_Prof",         ac.nBins_b,  ac.min_b,  ac.max_b,  "b",  "#psi_{2}", 1);
  h_psi2VsB       = createHistogram(bn+TString("psi2VsB"),   ac.nBins_b,  ac.min_b,  ac.max_b,  40, -TMath::Pi(), TMath::Pi(), "b", "#psi_2", "Counts",0,1);
}

//________________________________________________________________________
void CollisionGeometryHistograms::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "-Fatal- Attempting to load CollisionGeometryHistograms from an invalid file pointer" << endl;
    return;
    }
  CollisionGeometryConfiguration & ac = *configuration;
  TString bn = ac.histoBaseName; bn += "_";
  h_b                 = loadH1(inputFile,      bn+TString("b") );
  h_nPart             = loadH1(inputFile,      bn+TString("nPart") );
  h_nBinary           = loadH1(inputFile,      bn+TString("nBinary") );
  h_nPartVsB_Prof     = loadProfile(inputFile, bn+TString("nPartVsB_Prof"));
  h_nPartSqVsB_Prof   = loadProfile(inputFile, bn+TString("nPartSqVsB_Prof"));
  h_nBinaryVsB_Prof   = loadProfile(inputFile, bn+TString("nBinaryVsB_Prof"));
  h_nBinarySqVsB_Prof = loadProfile(inputFile, bn+TString("nBinarySqVsB_Prof"));
  h_nPartVsB          = loadH2(inputFile,      bn+TString("nPartVsB"));
  h_nBinaryVsB        = loadH2(inputFile,      bn+TString("nBinaryVsB"));

  h_nBinaryVsNPart_Prof   = loadProfile(inputFile, bn+TString("nBinaryVsNPart_Prof"));
  h_nBinarySqVsNPart_Prof = loadProfile(inputFile, bn+TString("nBinarySqVsNPart_Prof"));
  h_nPartRmsVsB           = loadH1(inputFile,      bn+TString("nPartRmsVsB"));
  h_nBinaryRmsVsB         = loadH1(inputFile,      bn+TString("nBinaryRmsVsB"));
  h_nBinaryRmsVsNPart     = loadH1(inputFile,      bn+TString("nBinaryRmsVsNPart"));

  h_nPartOmegaVsB         = loadH1(inputFile,      bn+TString("nPartOmegaVsB"));
  h_nBinaryOmegaVsB       = loadH1(inputFile,      bn+TString("nPartOmegaVsB"));
  h_nBinaryOmegaVsNPart   = loadH1(inputFile,      bn+TString("nBinaryOmegaVsNPart"));
  h_nPartR2VsB            = loadH1(inputFile,      bn+TString("nPartR2VsB"));
  h_nBinaryR2VsB          = loadH1(inputFile,      bn+TString("nBinaryR2VsB"));
  h_nBinaryR2VsNPart      = loadH1(inputFile,      bn+TString("nBinaryR2VsNPart"));

  h_xyNNIntVsB         = loadH3(inputFile,     bn+TString("xyNNIntVsB"));

  h_varXVsB_Prof       = loadProfile(inputFile,bn+TString("varXVsB_Prof"));
  h_varYVsB_Prof       = loadProfile(inputFile,bn+TString("varYVsB_Prof"));
  h_covXYVsB_Prof      = loadProfile(inputFile,bn+TString("covXYVsB_Prof"));
  h_epsilonXVsB_Prof   = loadProfile(inputFile,bn+TString("epsilonXVsB_Prof"));
  h_epsilonYVsB_Prof   = loadProfile(inputFile,bn+TString("epsilonModVsB_Prof"));
  h_epsilonXYVsB_Prof  = loadProfile(inputFile,bn+TString("epsilonXYVsB_Prof"));
  h_epsilonXYVsB       = loadH3(inputFile,     bn+TString("epsilonXYVsB"));
  h_psi2VsB_Prof       = loadProfile(inputFile,bn+TString("psi2VsB_Prof"));
  h_psi2VsB            = loadH2(inputFile,     bn+TString("psi2VsB"));
}


void CollisionGeometryHistograms::fill(CollisionGeometry * collisionGeometry, double weight)
{
  double impactPar = collisionGeometry->b;
  double nPart     = collisionGeometry->nParticipant;
  double nBinary   = collisionGeometry->nBinary;
  if (nBinary<2) return;

  h_b        ->Fill(impactPar,   weight);
  h_nPart    ->Fill(nPart,       weight);
  h_nBinary  ->Fill(nBinary,     weight);

  h_nPartVsB_Prof    ->Fill(impactPar, nPart,   weight);
  h_nPartSqVsB_Prof  ->Fill(impactPar, nPart*nPart,   weight);
  h_nBinaryVsB_Prof  ->Fill(impactPar, nBinary, weight);
  h_nBinarySqVsB_Prof->Fill(impactPar, nBinary*nBinary, weight);
  h_nPartVsB        ->Fill(impactPar, nPart,   weight);
  h_nBinaryVsB      ->Fill(impactPar, nBinary, weight);
  h_nBinaryVsNPart_Prof   ->Fill(nPart, nBinary,   weight);
  h_nBinarySqVsNPart_Prof ->Fill(nPart, nBinary*nBinary,   weight);

  double x,x2; //x3,x4,x6,x8;
  double y,y2; //y3,y4,y6,y8;
  double xy;
  double counts = 0.0;
  double momX = 0.0; double momX2 = 0.0; //double momX3 = 0.0; double momX4 = 0.0; double momX6 = 0.0; double momX8 = 0.0;
  double momY = 0.0; double momY2 = 0.0; //double momY3 = 0.0; double momY4 = 0.0; double momY6 = 0.0; double momY8 = 0.0;
  double momXY = 0.0;
  for (int iBinary=0; iBinary<nBinary; iBinary++)
    {
    x = collisionGeometry->x[iBinary];  x2 = x*x; //x3 = x2*x; x4 = x3*x; x6 = x3*x3; x8 = x4*x4;
    y = collisionGeometry->y[iBinary];  y2 = y*y; //y3 = y2*y; y4 = y3*y; y6 = y3*y3; y8 = y4*y4;
    xy = x*y;
    h_xyNNIntVsB->Fill(impactPar, x, y, weight);
    counts += weight;
     momX  += weight*x; momX2 += weight*x2; //momX3 += weight*x3; momX4 += weight*x4; momX6 += weight*x6; momX8 += weight*x8;
     momY  += weight*y; momY2 += weight*y2; //momY3 += weight*y3; momY4 += weight*y4; momY6 += weight*y6; momY8 += weight*y8;
     momXY += weight*xy;
    }

  momX /= counts; momX2 /= counts; //momX3 /= counts; momX4 /= counts; momX6 /= counts; momX8 /= counts;
  momY /= counts; momY2 /= counts; //momY3 /= counts; momY4 /= counts; momY6 /= counts; momY8 /= counts;
  momXY /= counts;

  double varX  = momX2 - momX*momX;
  double varY  = momY2 - momY*momY;
  double varXY = momXY - momX*momY;

  double epsDenom = varX + varY;      //cout << " eps_denom:" << eps_denom << endl;

  if (epsDenom<1E-4) return;

  double epsX     = (varY - varX)/epsDenom;      //cout << " eps_x:" << eps_x << endl;
  double epsY     = 2*varXY/epsDenom;             //cout << " eps_y:" << eps_y << endl;
  double epsMod   = sqrt(epsX*epsX + epsY*epsY); //cout << " eps_mod:" << eps_mod << endl;
  double psi2     = atan2(epsY,epsX);

  h_varXVsB_Prof       ->Fill(impactPar, varX,   weight);
  h_varYVsB_Prof       ->Fill(impactPar, varY,   weight);
  h_covXYVsB_Prof      ->Fill(impactPar, varXY,  weight);
  h_epsilonXVsB_Prof   ->Fill(impactPar, epsX,   weight);
  h_epsilonYVsB_Prof   ->Fill(impactPar, epsY,   weight);
  h_epsilonXYVsB_Prof  ->Fill(impactPar, epsMod, weight);
  h_epsilonXYVsB       ->Fill(impactPar, epsX, epsY, weight);
  h_psi2VsB_Prof       ->Fill(impactPar, psi2,   weight);
  h_psi2VsB            ->Fill(impactPar, psi2,   weight);

}

void CollisionGeometryHistograms::calculateDerivedHistograms()
{
  calculateRms(h_nPartVsB_Prof,       h_nPartSqVsB_Prof,       h_nPartRmsVsB,       h_nPartOmegaVsB,       h_nPartR2VsB);
  calculateRms(h_nBinaryVsB_Prof,     h_nBinarySqVsB_Prof,     h_nBinaryRmsVsB,     h_nBinaryOmegaVsB,     h_nBinaryR2VsB);
  calculateRms(h_nBinaryVsNPart_Prof, h_nBinarySqVsNPart_Prof, h_nBinaryRmsVsNPart, h_nBinaryOmegaVsNPart, h_nBinaryR2VsNPart);
}

void CollisionGeometryHistograms::calculateRms(TProfile * h1, TProfile * h1Sq,   TH1* h1Rms, TH1* h1Omega, TH1* h1R2)
{
  double v, ev, v2, ev2, rms, erms, omega, eomega, R2, eR2;
  int nBins = h1->GetNbinsX();
  for (int iBin=1; iBin<nBins; iBin++)
  {
  v = h1->GetBinContent(iBin);
  ev = 0.0;
  v2 = h1Sq->GetBinContent(iBin);
  ev2 = 0.0;
  rms = v2 - v*v;
  if (rms>0) rms = sqrt(rms);
  erms = 0.0;


  if (v>0)
    {
    omega  = rms*rms/v;
    eomega = 0.0;
    R2 = (v2-v)/v/v - 1.0;
    eR2 = 0;
    }
  else
    {
    omega  = 0.0;
    eomega = 0.0;
    R2     = 0.0;
    eR2    = 0;
    }

  cout << " iBin: " << iBin << " v:" << v << " v2:" << v2 << " rms:" << rms << " omega:" << omega << " R2:" << R2 << endl;

  h1Rms->SetBinContent(iBin, rms);
  h1Rms->SetBinError  (iBin, erms);

  h1Omega->SetBinContent(iBin, omega);
  h1Omega->SetBinError(iBin, eomega);

  h1R2->SetBinContent(iBin, R2);
  h1R2->SetBinError(iBin, eR2);
  }
}
