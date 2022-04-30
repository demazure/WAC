//
//  ParticleFilter.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include <TMath.h>
#include "ParticleFilter.hpp"

ClassImp(ParticleFilter)

  //////////////////////////////////////////////////////////////////////////////////////////
  // CTOR
  //////////////////////////////////////////////////////////////////////////////////////////
  ParticleFilter::ParticleFilter(ParticleFilter::SpeciesSelection pid,
                                 ParticleFilter::ChargeSelection charge,
                                 double minPt, double maxPt, double minEta,
                                 double maxEta, double minY, double maxY)
  : pidRequested(pid), chargeRequested(charge), min_pt(minPt), max_pt(maxPt), min_eta(minEta), max_eta(maxEta), min_y(minY), max_y(maxY)
{
  // no ops
}

//////////////////////////////////////////////////////////////////////////////////////////
// DTOR
//////////////////////////////////////////////////////////////////////////////////////////
ParticleFilter::~ParticleFilter()
{
  // no ops
}

TString ParticleFilter::getName()
{
  TString name;
  switch (pidRequested) {
    default:
      name = "WakyWaky";
      break;
    case AllSpecies:
      name = "All";
      break;
    case Photon:
      name = "Gam";
      break;
    case Lepton:
      name = "Lep";
      break;
    case Electron:
      name = "El";
      break;
    case Muon:
      name = "Mu";
      break;
    case Hadron:
      name = "H";
      break;
    case Pion:
      name = "Pi";
      break;
    case Kaon:
      name = "K";
      break;
    case Baryon:
      name = "B";
      break;
    case Proton:
      name = "P";
      break;
    case Lambda:
      name = "Lam";
      break;
  }

  if (pidRequested == Photon)
    return name;

  switch (chargeRequested) {
    default:
      name = "WakyWakyWakyWaky";
      break;
    case AllCharges:
      name += "All";
      break; // all charges
    case Negative:
      name += "M";
      break; // minus
    case Positive:
      name += "P";
      break;
    case Charged:
      name += "C";
      break;
    case Neutral:
      name += "0";
      break;
  }
  return name;
}

TString ParticleFilter::getTitle()
{
  TString name;
  switch (pidRequested) {
    case AllSpecies:
      name = "All Specie";
      break;
    case Photon:
      name = "Gamma";
      break;
    case Lepton:
      name = "Lepton";
      break;
    case Electron:
      name = "Electron";
      break;
    case Muon:
      name = "Muon";
      break;
    case Hadron:
      name = "Hadron";
      break;
    case Pion:
      name = "Pion";
      break;
    case Kaon:
      name = "Kaon";
      break;
    case Baryon:
      name = "Baryon";
      break;
    case Proton:
      name = "Proton";
      break;
    case Lambda:
      name = "Lambda";
      break;
  }

  if (pidRequested == Photon)
    return name;

  switch (chargeRequested) {
    case AllCharges:
      name += " All Charge";
      break; // all charges
    case Negative:
      name += " Minus";
      break; // minus
    case Positive:
      name += " Plus";
      break;
    case Charged:
      name += " Charged";
      break;
    case Neutral:
      name += " Neutral";
      break;
  }
  return name;
}
