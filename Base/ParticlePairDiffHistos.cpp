// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Task
 */

#include "ParticlePairDiffHistos.hpp"

ParticlePairDiffHistos::ParticlePairDiffHistos(const TString& name,
                                               AnalysisConfiguration* configuration,
                                               LogLevel debugLevel)
  : Histograms(name, configuration, 150, debugLevel)
{
  if (reportDebug())
    cout << "ParticlePairDiffHistos::CTOR() Started." << endl;
  initialize();
  if (reportDebug())
    cout << "ParticlePairDiffHistos::CTOR() Completed." << endl;
}

ParticlePairDiffHistos::ParticlePairDiffHistos(TFile* inputFile,
                                               const TString& name,
                                               AnalysisConfiguration* configuration,
                                               LogLevel debugLevel)
  : Histograms(name, configuration, 150, debugLevel)
{
  if (reportDebug())
    cout << "ParticlePairDiffHistos::CTOR() Started." << endl;
  loadHistograms(inputFile);
  if (reportDebug())
    cout << "ParticlePairDiffHistos::CTOR() Completed." << endl;
}

ParticlePairDiffHistos::~ParticlePairDiffHistos()
{
  // deleteHistograms();
}

void ParticlePairDiffHistos::initialize()
{
  if (reportDebug())
    cout << "ParticlePairDiffHistos::initialize() Started." << endl;
  AnalysisConfiguration& ac = *(AnalysisConfiguration*)getConfiguration();
  TString bn = getHistoBaseName();
  ac.range_pt = ac.max_pt - ac.min_pt;
  ac.range_phi = ac.max_phi - ac.min_phi;
  ac.range_eta = ac.max_eta - ac.min_eta;
  ac.range_y = ac.max_y - ac.min_y;
  ac.nBins_phiEta = ac.nBins_eta * ac.nBins_phi;
  ac.nBins_phiEtaPt = ac.nBins_eta * ac.nBins_phi * ac.nBins_pt;
  ac.nBins_phiY = ac.nBins_y * ac.nBins_phi;
  ac.nBins_phiYPt = ac.nBins_y * ac.nBins_phi * ac.nBins_pt;

  ac.nBins_Deta = ac.nBins_eta * 2 - 1;
  ac.min_Deta = ac.min_eta - ac.max_eta;
  ac.max_Deta = ac.max_eta - ac.min_eta;
  ac.width_Deta = (ac.max_eta - ac.min_eta) / ac.nBins_Deta;
  ac.nBins_Dphi = ac.nBins_phi;
  ac.nBins_Dphi_shft = ac.nBins_phi / 4;
  ac.width_Dphi = kTWOPI / ac.nBins_Dphi;
  ac.min_Dphi = -ac.width_Dphi / 2.;
  ac.max_Dphi = kTWOPI - ac.width_Dphi / 2.;
  ac.min_Dphi_shft = ac.min_Dphi - ac.width_Dphi * ac.nBins_Dphi_shft;
  ac.max_Dphi_shft = ac.max_Dphi - ac.width_Dphi * ac.nBins_Dphi_shft;
  ac.nBins_Dy = 2 * ac.nBins_y - 1;
  ac.width_Dy = (ac.max_y - ac.min_y) / ac.nBins_Dy;
  ac.min_Dy = ac.min_y - ac.max_y;
  ac.max_Dy = ac.max_y - ac.min_y;

  /* big histograms are forced to be created without sumw2 structure for it will not be used */
  bool defsumw2 = TH1::GetDefaultSumw2();
  TH1::SetDefaultSumw2(false);

  h_n2_ptPt = createHistogram(bn + TString("n2_ptPt"), ac.nBins_pt, ac.min_pt, ac.max_pt, ac.nBins_pt, ac.min_pt, ac.max_pt, "p_{T,1}", "p_{T,2}", "N_{2}", scaled, saved, plotted, notPrinted);
  h_n2_DetaDphi = createHistogram(bn + TString("n2_DetaDphi"), ac.nBins_Deta, ac.min_Deta, ac.max_Deta, ac.nBins_Dphi, ac.min_Dphi, ac.max_Dphi, "#Delta#eta", "#Delta#varphi", "n_{2}", scaled, saved, notPlotted, notPrinted, false);
  h_ptpt_DetaDphi = createHistogram(bn + TString("ptpt_DetaDphi"), ac.nBins_Deta, ac.min_Deta, ac.max_Deta, ac.nBins_Dphi, ac.min_Dphi, ac.max_Dphi, "#Delta#eta", "#Delta#varphi", "p_{T}xp_{T}", scaled, saved, notPlotted, notPrinted, false);
  h_dptdpt_DetaDphi = createHistogram(bn + TString("dptdpt_DetaDphi"), ac.nBins_Deta, ac.min_Deta, ac.max_Deta, ac.nBins_Dphi, ac.min_Dphi, ac.max_Dphi, "#Delta#eta", "#Delta#varphi", "(p_{T}-<p_{T}>)x(p_{T}-<p_{T}>)", scaled, saved, notPlotted, notPrinted, false);
  if (ac.fillY) {
    h_n2_DyDphi = createHistogram(bn + TString("n2_DyDphi"), ac.nBins_Dy, ac.min_Dy, ac.max_Dy, ac.nBins_Dphi, ac.min_Dphi, ac.max_Dphi, "#Delta y", "#Delta#varphi", "<n_{2}>", scaled, saved, notPlotted, notPrinted, false);
    h_ptpt_DyDphi = createHistogram(bn + TString("dptdpt_DyDphi"), ac.nBins_Dy, ac.min_Dy, ac.max_Dy, ac.nBins_Dphi, ac.min_Dphi, ac.max_Dphi, "#Delta y", "#Delta#varphi", "p_{T}xp_{T}", scaled, saved, notPlotted, notPrinted, false);
    h_dptdpt_DyDphi = createHistogram(bn + TString("dptdpt_DyDphi"), ac.nBins_Dy, ac.min_Dy, ac.max_Dy, ac.nBins_Dphi, ac.min_Dphi, ac.max_Dphi, "#Delta y", "#Delta#varphi", "p_{T}xp_{T}", scaled, saved, notPlotted, notPrinted, false);
  }

  /* big histograms are forced to be created without sumw2 structure for it will not be used */
  h_n2_DetaDphi->SetBit(TH1::kIsNotW);
  h_n2_DetaDphi->Sumw2(false);
  h_ptpt_DetaDphi->SetBit(TH1::kIsNotW);
  h_ptpt_DetaDphi->Sumw2(false);
  h_dptdpt_DetaDphi->SetBit(TH1::kIsNotW);
  h_dptdpt_DetaDphi->Sumw2(false);
  if (ac.fillY) {
    h_n2_DyDphi->SetBit(TH1::kIsNotW);
    h_n2_DyDphi->Sumw2(false);
    h_ptpt_DyDphi->SetBit(TH1::kIsNotW);
    h_ptpt_DyDphi->Sumw2(false);
    h_dptdpt_DyDphi->SetBit(TH1::kIsNotW);
    h_dptdpt_DyDphi->Sumw2(false);
  }

  /* back to default behavior */
  TH1::SetDefaultSumw2(defsumw2);
  if (reportDebug())
    cout << "ParticlePairDiffHistos::initialize() Completed." << endl;
}

void ParticlePairDiffHistos::fill(Particle& particle1, Particle& particle2, double weight1, double weight2, double pTavg1, double pTavg2)
{
  int globaletabinno = getGlobalDeltaEtaDeltaPhiIndex(particle1, particle2);
  h_n2_ptPt->Fill(particle1.pt, particle2.pt, weight1 * weight2);
  h_n2_DetaDphi->AddBinContent(globaletabinno, weight1 * weight2);
  h_ptpt_DetaDphi->AddBinContent(globaletabinno, weight1 * particle1.pt * weight2 * particle2.pt);
  h_dptdpt_DetaDphi->AddBinContent(globaletabinno, (weight1 * particle1.pt - pTavg1) * (weight2 * particle2.pt - pTavg2));
  h_n2_DetaDphi->SetEntries(h_n2_ptPt->GetEntries());
  h_ptpt_DetaDphi->SetEntries(h_n2_ptPt->GetEntries());
  h_dptdpt_DetaDphi->SetEntries(h_n2_ptPt->GetEntries());

  if (configuration->fillY) {
    int globalybinno = getGlobalDeltaRapidityDeltaPhiIndex(particle1, particle2);
    h_n2_DyDphi->AddBinContent(globalybinno, weight1 * weight2);
    h_ptpt_DyDphi->AddBinContent(globalybinno, weight1 * particle1.pt * weight2 * particle2.pt);
    h_dptdpt_DyDphi->AddBinContent(globalybinno, (weight1 * particle1.pt - pTavg1) * (weight2 * particle2.pt - pTavg2));
    h_n2_DyDphi->SetEntries(h_n2_ptPt->GetEntries());
    h_ptpt_DyDphi->SetEntries(h_n2_ptPt->GetEntries());
    h_dptdpt_DyDphi->SetEntries(h_n2_ptPt->GetEntries());
  }
}


//________________________________________________________________________
void ParticlePairDiffHistos::loadHistograms(TFile* inputFile)
{
  if (!inputFile) {
    cout << "-Fatal- Attempting to load ParticleHistos from an invalid file pointer" << endl;
    return;
  }
  AnalysisConfiguration& ac = *(AnalysisConfiguration*)getConfiguration();
  TString bn = getHistoBaseName();
  ac.range_pt = ac.max_pt - ac.min_pt;
  ac.range_phi = ac.max_phi - ac.min_phi;
  ac.range_eta = ac.max_eta - ac.min_eta;
  ac.range_y = ac.max_y - ac.min_y;
  ac.nBins_phiEta = ac.nBins_eta * ac.nBins_phi;
  ac.nBins_phiEtaPt = ac.nBins_eta * ac.nBins_phi * ac.nBins_pt;
  ac.nBins_phiY = ac.nBins_y * ac.nBins_phi;
  ac.nBins_phiYPt = ac.nBins_y * ac.nBins_phi * ac.nBins_pt;

  ac.nBins_Deta = ac.nBins_eta * 2 - 1;
  ac.width_Deta = (ac.max_eta - ac.min_eta) / ac.nBins_Deta;
  ac.min_Deta = ac.min_eta - ac.max_eta;
  ac.max_Deta = ac.max_eta - ac.min_eta;
  ac.nBins_Dphi = ac.nBins_phi;
  ac.nBins_Dphi_shft = ac.nBins_phi / 4;
  ac.width_Dphi = kTWOPI / ac.nBins_Dphi;
  ac.min_Dphi = -ac.width_Dphi / 2.;
  ac.max_Dphi = kTWOPI - ac.width_Dphi / 2.;
  ac.min_Dphi_shft = ac.min_Dphi - ac.width_Dphi * double(ac.nBins_Dphi / 4);
  ac.max_Dphi_shft = ac.max_Dphi - ac.width_Dphi * double(ac.nBins_Dphi / 4);
  ac.nBins_Dy = 2 * ac.nBins_y - 1;
  ac.width_Dy = (ac.max_y - ac.min_y) / ac.nBins_Dy;
  ac.min_Dy = ac.min_y - ac.max_y;
  ac.max_Dy = ac.max_y - ac.min_y;

  h_n2_DetaDphi = loadH2(inputFile, bn + TString("n2_DetaDphi"), true);
  h_ptpt_DetaDphi = loadH2(inputFile, bn + TString("ptpt_DetaDphi"), true);
  h_dptdpt_DetaDphi = loadH2(inputFile, bn + TString("dptdpt_DetaDphi"), true);

  if (ac.fillY) {
    h_n2_DyDphi = loadH2(inputFile, bn + TString("n2_DyDphi"), true);
    h_ptpt_DyDphi = loadH2(inputFile, bn + TString("ptpt_DyDphi"), true);
    h_dptdpt_DyDphi = loadH2(inputFile, bn + TString("dptdpt_DyDphi"), true);
  }
  /* the histograms are not owned */
  bOwnTheHistograms = false;
  return;
}

ClassImp(ParticlePairDiffHistos)
