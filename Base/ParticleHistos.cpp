//
//  ParticleHistos.cpp
//  MyMC
//
//  Created by Claude Pruneau on 9/23/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "TLorentzVector.h"
#include "ParticleHistos.hpp"
ClassImp(ParticleHistos);

ParticleHistos::ParticleHistos(const TString & name,
                               AnalysisConfiguration * configuration,
                               LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel)
{
  initialize();
}

ParticleHistos::ParticleHistos(TFile * inputFile,
                               const TString & name,
                               AnalysisConfiguration * configuration,
                               LogLevel  debugLevel)
:
Histograms(name,configuration,100,debugLevel)
{
  loadHistograms(inputFile);
}

ParticleHistos::~ParticleHistos()
{
  //deleteHistograms();
}

// for now use the same boundaries for eta and y histogram
void ParticleHistos::createHistograms()
{
  AnalysisConfiguration & ac = *getConfiguration();
  TString bn = getHistoBaseName();
  h_n1         = createHistogram(bn+TString("n1"),             1000,  0.0,  1000.0,  "n_1","N", scaled, saved, plotted, notPrinted);
  h_n1_pt      = createHistogram(bn+TString("n1_pt"),          ac.nBins_pt,  ac.min_pt,  ac.max_pt,  "p_{T}","N", scaled, saved, plotted, notPrinted);
  h_n1_ptXS    = createHistogram(bn+TString("n1_ptXS"),        ac.nBins_pt,  ac.min_pt,  ac.max_pt,  "p_{T}","1/p_{T} dN/p_{T}", scaled, saved, plotted, notPrinted);
  h_n1_eta     = createHistogram(bn+TString("n1_eta"),         ac.nBins_eta, ac.min_eta, ac.max_eta, "#eta","N", scaled, saved, plotted, notPrinted);
  h_n1_phi     = createHistogram(bn+TString("n1_phi"),         ac.nBins_phi, ac.min_phi, ac.max_phi, "#varphi","N", scaled, saved, plotted, notPrinted);
  h_n1_ptEta   = createHistogram(bn+TString("n1_ptEta"),       ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_pt, ac.min_pt, ac.max_pt,"#eta","p_{T}","N", scaled, saved, plotted, notPrinted);
  h_n1_phiEta  = createHistogram(bn+TString("n1_phiEta"),      ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_phi, ac.min_phi, ac.max_phi,"#eta","#varphi","N", scaled, saved, plotted, notPrinted);
  h_spt_phi    = createHistogram(bn+TString("sumpt1_phi"),    ac.nBins_phi, ac.min_phi, ac.max_phi, "#varphi","#sum p_{T}", scaled, saved, plotted, notPrinted);
  h_spt_eta    = createHistogram(bn+TString("sumpt1_eta"),    ac.nBins_eta, ac.min_eta, ac.max_eta, "#eta", "#sum p_{T}", scaled, saved, plotted, notPrinted);
  h_spt_phiEta = createHistogram(bn+TString("sumpt1_phiEta"), ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_phi, ac.min_phi, ac.max_phi,"#eta","#varphi","#sum p_{T}", scaled, saved, notPlotted, notPrinted);
  h_pt_phi     = createHistogram(bn+TString("pt1_phi"),       ac.nBins_phi, ac.min_phi, ac.max_phi, "#varphi","#LTp_{T}#GT", notScaled, saved, plotted, notPrinted);
  h_pt_eta     = createHistogram(bn+TString("pt1_eta"),       ac.nBins_eta, ac.min_eta, ac.max_eta, "#eta", "#LTp_{T}#GT", notScaled, saved, plotted, notPrinted);
  h_pt_phiEta  = createHistogram(bn+TString("pt1_phiEta"),    ac.nBins_eta, ac.min_eta, ac.max_eta, ac.nBins_phi, ac.min_phi, ac.max_phi,"#eta","#varphi","#LTp_{T}#GT", notScaled, saved, notPlotted, notPrinted);

  if (ac.fillY)
    {
    h_n1_y     = createHistogram(bn+TString("n1_y"),         ac.nBins_y, ac.min_y, ac.max_y, "y","N", scaled, saved, plotted, notPrinted);
    h_n1_ptY   = createHistogram(bn+TString("n1_ptY"),       ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_pt, ac.min_pt, ac.max_pt,"#eta","p_{T}","N", scaled, saved, notPlotted, notPrinted);
    h_n1_phiY  = createHistogram(bn+TString("n1_phiY"),      ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_phi, ac.min_phi, ac.max_phi,"y","#varphi","N", scaled, saved, notPlotted, notPrinted);
    h_spt_y    = createHistogram(bn+TString("sumpt1_y"),    ac.nBins_y, ac.min_y, ac.max_y, "y", "#sum p_{T}", scaled, saved, notPlotted, notPrinted);
    h_spt_phiY = createHistogram(bn+TString("sumpt1_phiY"), ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_phi, ac.min_phi, ac.max_phi,"y","#varphi","#sum p_{T}", scaled, saved, notPlotted, notPrinted);
    h_pt_y     = createHistogram(bn+TString("pt1_y"),       ac.nBins_y, ac.min_y, ac.max_y, "y", "#LTp_{T}#GT", notScaled, saved, notPlotted, notPrinted);
    h_pt_phiY  = createHistogram(bn+TString("pt1_phiY"),    ac.nBins_y, ac.min_y, ac.max_y, ac.nBins_phi, ac.min_phi, ac.max_phi,"y","#varphi","#LTp_{T}#GT", notScaled, saved, notPlotted, notPrinted);
    }

  if (ac.fill3D)
    {
    h_n1_ptPhiEta = createHistogram(bn+TString("n1_ptPhiEta"),
                                    ac.nBins_eta, ac.min_eta, ac.max_eta,
                                    ac.nBins_phi, ac.min_phi, ac.max_phi,
                                    ac.nBins_pt, ac.min_pt, ac.max_pt,
                                    "#eta","#varphi","p_{T}","N", scaled, saved, notPlotted, notPrinted);
    if (ac.fillY)
      {
      h_n1_ptPhiY = createHistogram(bn+TString("n1_ptPhiY"),
                                    100, ac.min_y, ac.max_y,
                                    100, ac.min_phi, ac.max_phi,
                                    100, ac.min_pt,  ac.max_pt,
                                    "y","#varphi","p_{T}","N", scaled, saved, notPlotted, notPrinted);
      }
    }


}

//________________________________________________________________________
void ParticleHistos::loadHistograms(TFile * inputFile)
{
  if (!inputFile)
    {
    if (reportFatal()) cout << "-Fatal- Attempting to load ParticleHistos from an invalid file pointer" << endl;
    return;
    }
  AnalysisConfiguration & ac = *getConfiguration();
  TString bn = getHistoBaseName();
  h_n1         = loadH1(inputFile,bn+TString("n1")       ,true);
  h_n1_pt      = loadH1(inputFile,bn+TString("n1_pt")    ,true);
  h_n1_ptXS    = loadH1(inputFile,bn+TString("n1_ptXS")  ,true);
  h_n1_eta     = loadH1(inputFile,bn+TString("n1_eta")   ,true);
  h_n1_phi     = loadH1(inputFile,bn+TString("n1_phi")   ,true);
  h_n1_ptEta   = loadH2(inputFile,bn+TString("n1_ptEta") ,true);
  h_n1_phiEta  = loadH2(inputFile,bn+TString("n1_phiEta"),true);
  h_spt_phiEta = loadH2(inputFile,bn+TString("sumpt1_phiEta"),true);
  h_spt_phi    = loadH1(inputFile,bn+TString("sumpt1_phi")   ,true);
  h_spt_eta    = loadH1(inputFile,bn+TString("sumpt1_eta")  ,true);
  h_pt_phiEta  = loadH2(inputFile,bn+TString("pt1_phiEta"),false,true);
  h_pt_phi     = loadH1(inputFile,bn+TString("pt1_phi")   ,false,true);
  h_pt_eta     = loadH1(inputFile,bn+TString("pt1_eta")  ,false,true);
  if (ac.fill3D)
    {
    h_n1_ptPhiEta = loadH3(inputFile,bn+TString("n1_ptPhiEta"),true);
    }
  if (ac.fillY)
    {
    h_n1_y    = loadH1(inputFile,bn+TString("n1_Y"),true);
    h_n1_ptY  = loadH2(inputFile,bn+TString("n1_ptY"),true);
    h_n1_phiY = loadH2(inputFile,bn+TString("n1_phiY"),true);
    h_spt_phiY = loadH2(inputFile,bn+TString("sumpt1_phiY"),true);
    h_spt_y    = loadH1(inputFile,bn+TString("sumpt1_y"),true);
    h_pt_phiY = loadH2(inputFile,bn+TString("pt1_phiY"),false,true);
    h_pt_y    = loadH1(inputFile,bn+TString("pt1_y"),false,true);
    }
  if (ac.fill3D && ac.fillY)
    {
    h_n1_ptPhiY = loadH3(inputFile,bn+TString("n1_ptPhiY"),true);
    }
  return;
}


void ParticleHistos::fill(Particle & particle, double weight)
{
  double pt   = particle.pt;
  double eta  = particle.eta;
  double phi  = particle.phi; if (phi<0) phi += TMath::TwoPi();
  AnalysisConfiguration & ac = *getConfiguration();
  h_n1_pt     ->Fill(pt, weight);
  h_n1_ptXS   ->Fill(pt, weight/pt);
  // delayed fill h_n1_eta    ->Fill(eta, weight);
  // delayed fill h_n1_phi    ->Fill(phi, weight);
  h_n1_ptEta  ->Fill(eta, pt,  weight);
  h_n1_phiEta ->Fill(eta, phi, weight);
  // delayed fill h_spt_phi    ->Fill(phi, pt*weight);
  // delayed fill h_spt_eta    ->Fill(eta, pt*weight);
  h_spt_phiEta ->Fill(eta, phi, pt*weight);

  if (ac.fillY)
    {
    double y    = particle.y;
    h_n1_y     ->Fill(y, weight);
    h_n1_ptY   ->Fill(y, pt,  weight);
    h_n1_phiY  ->Fill(y, phi, weight);
    h_spt_y     ->Fill(y, pt*weight);
    h_spt_phiY  ->Fill(y, phi, pt*weight);
    }

  //    if (analysisConfiguration->fill3D) h_n1_ptPhiEta->Fill(eta, phi, pt, weight);
  //    if (analysisConfiguration->fill3D && analysisConfiguration->fillY) h_n1_ptPhiY->Fill(y, phi, pt, weight);
}


void ParticleHistos::fill(TLorentzVector & p, double weight)
{
  double pt    = p.Pt();
  double eta   = p.Eta();
  double phi   = p.Phi(); if (phi<0) phi += TMath::TwoPi();
  double y     = p.Rapidity();
  AnalysisConfiguration & ac = *getConfiguration();
  h_n1_pt      ->Fill(pt, weight);
  h_n1_ptXS    ->Fill(pt, weight/pt);
  // delayed fill h_n1_eta     ->Fill(eta, weight);
  // delayed fill h_n1_phi     ->Fill(phi, weight);
  h_n1_ptEta   ->Fill(eta, pt,  weight);
  h_n1_phiEta  ->Fill(eta, phi, weight);
  // delayed fill h_spt_phi    ->Fill(phi, pt*weight);
  // delayed fill h_spt_eta    ->Fill(eta, pt*weight);
  h_spt_phiEta ->Fill(eta, phi, pt*weight);

  if (ac.fillY)
    {
    // delayed fill h_n1_y      ->Fill(y, weight);
    h_n1_ptY    ->Fill(y, pt,  weight);
    h_n1_phiY   ->Fill(y, phi, weight);
    // delayed fill h_spt_y     ->Fill(y, pt*weight);
    h_spt_phiY  ->Fill(y, phi, pt*weight);
    }

  if (ac.fill3D) h_n1_ptPhiEta->Fill(eta, phi, pt, weight);
  if (ac.fill3D && ac.fillY) h_n1_ptPhiY->Fill(y, phi, pt, weight);
}

void ParticleHistos::fillMultiplicity(double nAccepted, double weight)
{
  h_n1->Fill(nAccepted, weight);
}

// complete filling the addicional histograms by projecting the
// higher dimensional ones
void ParticleHistos::completeFill()
{
  AnalysisConfiguration & ac = *getConfiguration();

  TH1* h_eta = h_n1_phiEta->ProjectionX();
  h_n1_eta->Reset();
  h_n1_eta->Add(h_eta);
  TH1* h_phi = h_n1_phiEta->ProjectionY();
  h_n1_phi->Reset();
  h_n1_phi->Add(h_phi);
  TH1 *h_pteta = h_spt_phiEta->ProjectionX();
  h_spt_eta->Reset();
  h_spt_eta->Add(h_pteta);
  TH1 *h_ptphi = h_spt_phiEta->ProjectionY();
  h_spt_phi->Reset();
  h_spt_phi->Add(h_ptphi);
  delete h_ptphi;
  delete h_pteta;
  delete h_phi;
  delete h_eta;

  if (ac.fillY)
    {
    TH1 * h_y = h_n1_phiY->ProjectionX();
    h_n1_y->Reset();
    h_n1_y->Add(h_y);
    TH1 * h_pty = h_spt_phiY->ProjectionX();
    h_spt_y->Reset();
    h_spt_y->Add(h_pty);
    delete h_y;
    delete h_pty;
    }

}



void ParticleHistos::calculateAverages()
{
  AnalysisConfiguration & ac = *getConfiguration();
  calculateAveragePt(h_spt_phi,h_n1_phi,h_pt_phi);
  calculateAveragePt(h_spt_eta,h_n1_eta,h_pt_eta);
  calculateAveragePtH2(h_spt_phiEta,h_n1_phiEta,h_pt_phiEta);
  if (ac.fillY)
    {
    calculateAveragePt(h_spt_y,h_n1_y,h_pt_y);
    calculateAveragePtH2(h_spt_phiY,h_n1_phiY,h_pt_phiY);
    }
}

