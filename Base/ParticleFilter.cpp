//
//  ParticleFilter.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/12/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include <TMath.h>
#include "ParticleFilter.hpp"

ClassImp(ParticleFilter);

//////////////////////////////////////////////////////////////////////////////////////////
// CTOR
//////////////////////////////////////////////////////////////////////////////////////////
ParticleFilter::ParticleFilter(ParticleFilter::SpeciesSelection pid,
                               ParticleFilter::ChargeSelection  charge,
                               double minPt,
                               double maxPt,
                               double minEta,
                               double maxEta,
                               double minY,
                               double maxY)
:
pidRequested     (pid),
chargeRequested  (charge),
min_pt           (minPt),
max_pt           (maxPt),
min_eta          (minEta),
max_eta          (maxEta),
min_y            (minY),
max_y            (maxY)
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

//////////////////////////////////////////////////////////////////////////////////////////
// accept/reject the given particle based on filter parameter
//////////////////////////////////////////////////////////////////////////////////////////
bool ParticleFilter::accept(Particle & particle)
{
//  enum ChargeSelection   { AllCharges, Negative, Positive, Charged, Neutral };
//  enum SpeciesSelection  { AllSpecies, Photon, Lepton, Electron, Muon, Hadron, Pion, Kaon, Baryon, Proton, Lambda };


  bool accepting = true;
  double charge = particle.charge;
  switch (chargeRequested)
    {
      case AllCharges:   accepting = true; break;
      case Negative: accepting = (charge < 0); break;
      case Positive: accepting = (charge > 0); break;
      case Charged:  accepting = (charge != 0); break;
      case Neutral:  accepting = (charge == 0); break;
    }
  if (!accepting) return false;
  double pid = TMath::Abs(particle.pid);
  switch (pidRequested)
    {
      case AllSpecies:   accepting = true; break;
      case Photon:   accepting = (pid == 22); break;
      case Lepton:   accepting = (pid == 11) || (pid == 12)|| (pid == 13)|| (pid == 14)|| (pid == 15)|| (pid == 16)  ; break;
      case Electron: accepting = (pid == 11); break;
      case Muon:     accepting = (pid == 13); break;
      case Hadron:  accepting = (pid == 211) || (pid == 321) || (pid == 2212); break;
      case Pion:    accepting = (pid == 111) || (pid == 211); break;
      case Kaon:    accepting = (pid == 321) || (pid == 311) || (pid == 310) || (pid == 130); break;
      case Baryon:  accepting = (pid == 2212) || (pid == 2112) || (pid == 3122); break;
      case Proton:  accepting = (pid == 2212); break;
      case Lambda:  accepting = (pid == 3122); break;
    }

  if (!accepting) return false;

  double pt  = particle.pt;
  double y   = particle.y;
  double eta = particle.eta;
  if (pt  >  min_pt &&
      pt  <= max_pt &&
      eta >= min_eta &&
      eta <= max_eta)
    return true;
  else
    return false;
}


TString ParticleFilter::getName()
{
  TString name;
  switch (pidRequested)
    {
      default: name = "WakyWaky"; break;
      case AllSpecies: name = "All"; break;
      case Photon:     name = "Gam"; break;
      case Lepton:     name = "Lep"  ; break;
      case Electron:   name = "El"; break;
      case Muon:       name = "Mu"; break;
      case Hadron:     name = "H"; break;
      case Pion:       name = "Pi"; break;
      case Kaon:       name = "K"; break;
      case Baryon:     name = "B"; break;
      case Proton:     name = "P"; break;
      case Lambda:     name = "Lam"; break;
    }

  if (pidRequested == Photon) return name;

  switch (chargeRequested)
     {
       default: name = "WakyWakyWakyWaky"; break;
       case AllCharges:   name += "All"; break; // all charges
       case Negative: name += "M"; break; // minus
       case Positive: name += "P"; break;
       case Charged:  name += "C"; break;
       case Neutral:  name += "0"; break;
     }
  return name;
}

TString ParticleFilter::getTitle()
{
  TString name;
  switch (pidRequested)
    {
      case AllSpecies:name = "All Specie"; break;
      case Photon:   name = "Gamma"; break;
      case Lepton:   name = "Lepton"  ; break;
      case Electron: name = "Electron"; break;
      case Muon:     name = "Muon"; break;
      case Hadron:   name = "Hadron"; break;
      case Pion:     name = "Pion"; break;
      case Kaon:     name = "Kaon"; break;
      case Baryon:   name = "Baryon"; break;
      case Proton:   name = "Proton"; break;
      case Lambda:   name = "Lambda"; break;
    }

  if (pidRequested == Photon) return name;


  switch (chargeRequested)
    {
      case AllCharges:name += " All Charge"; break; // all charges
      case Negative: name += " Minus"; break; // minus
      case Positive: name += " Plus"; break;
      case Charged:  name += " Charged"; break;
      case Neutral:  name += " Neutral"; break;
    }
  return name;
}
