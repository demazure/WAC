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
#include "TDatabasePDG.h"
#include "PythiaEventGenerator.hpp"
#include "PythiaConfiguration.hpp"

ClassImp(PythiaEventGenerator);

PythiaEventGenerator::PythiaEventGenerator(const TString& name,
                                           TaskConfiguration* configuration,
                                           Event* event,
                                           EventFilter* ef,
                                           ParticleFilter* pf)
  : Task(name, configuration, event),
    eventFilter(ef),
    particleFilter(pf)
{
  if (reportDebug())
    cout << "PythiaEventGenerator::PythiaEventGenerator(...) No ops" << endl;
}

PythiaEventGenerator::~PythiaEventGenerator()
{
  if (reportDebug())
    cout << "PythiaEventGenerator::~PythiaEventGenerator(...) No ops" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::initialize()
{
  if (reportDebug())
    cout << "PythiaEventGenerator::initialize() Started" << endl;

  nMax = 10000;
  particles = new TClonesArray("TParticle", nMax);
  pythia8 = new TPythia8();

  PythiaConfiguration* pc = (PythiaConfiguration*)getTaskConfiguration();
  for (int iOption = 0; iOption < pc->nOptions; iOption++) {
    pythia8->ReadString(pc->options[iOption]->Data());
  }
  pythia8->Initialize(pc->beam, pc->target, pc->energy);
  // pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* GeV */);
  if (reportDebug())
    cout << "PythiaEventGenerator::initialize() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset and Initialize the generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::reset()
{
  if (reportDebug())
    cout << "PythiaEventGenerator::reset() Started" << endl;
  event->reset();
  Particle::getFactory()->reset();
  if (reportDebug())
    cout << "PythiaEventGenerator::reset() Completed" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read an ampt event from file
// Copy the event into Event for convenience...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::execute()
{
  if (reportDebug())
    cout << "PythiaEventGenerator::execute() Started" << endl;

  Factory<Particle>* particleFactory = Particle::getFactory();
  int nparts;
  pythia8->GenerateEvent();
  if (reportDebug())
    pythia8->EventListing();
  if (reportDebug())
    cout << "PythiaEventGenerator::execute() Calling pythia8->ImportParticles()" << endl;

  nparts = pythia8->ImportParticles(particles, "Final");
  if (reportDebug()) {
    cout << "PythiaEventGenerator::execute() pythia8->ImportParticles() completed" << endl;
    cout << "PythiaEventGenerator::execute() with nparts:" << nparts << endl;
  }
  if (nparts > nMax) {
    if (reportError())
      cout << " ARRAY TOO SMALL np>nMax. nparts: " << nparts << " nMax: " << nMax << endl;
    postTaskFatal();
    // exit(0);
  }

  double charge, p_x, p_y, p_z, p_e, Pos_x, Pos_y, Pos_z, Pos_t;
  Particle* particle;
  int particleAccepted = 0;
  int particleCounted = 0;

  //------------------- Randomizing the particle phi --------------Starts
  double eventAngle = TMath::TwoPi() * gRandom->Rndm();
  double cosPhi = cos(eventAngle);
  double sinPhi = sin(eventAngle);

  // load particles from TClone storage and copy into event.
  Particle aParticle;
  // if (reportDebug()) cout << "PythiaEventGenerator::execute() starting copy loop into event..." << endl;

  for (int iParticle = 0; iParticle < nparts; iParticle++) {
    TParticle& part = *(TParticle*)particles->At(iParticle);
    int ist = part.GetStatusCode();
    // if (reportDebug()) cout << "PythiaEventGenerator::execute() ist: " << ist << endl;
    if (ist <= 0)
      continue;
    int pdg = part.GetPdgCode();
    charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
    p_x = cosPhi * part.Px() - sinPhi * part.Py();
    p_y = sinPhi * part.Px() + cosPhi * part.Py();
    p_z = part.Pz();
    p_e = part.Energy();
    Pos_x = part.Vx()*1e12;
    Pos_y = part.Vy()*1e12;
    Pos_z = part.Vz()*1e12;
    Pos_t = part.T()*1e12;
    aParticle.setPidPxPyPzE(pdg, charge, p_x, p_y, p_z, p_e, Pos_x, Pos_y, Pos_z, Pos_t);
    //aParticle.printProperties(cout);
    // if (reportDebug()) cout << "PythiaEventGenerator::execute() calling filter " << endl;
    particleCounted++;
    if (!particleFilter->accept(aParticle))
      continue;
    particle = particleFactory->getNextObject();
    *particle = aParticle;
    particleAccepted++;
    //    if (true)
    //      {
    //      cout << "PythiaEventGenerator::execute() particle: " << iParticle << " / " << particleAccepted << endl;
    //      particle->printProperties(cout);
    //      }
  }

  event->nParticles = particleAccepted;
  event->multiplicity = particleAccepted;
  if (reportDebug())
    cout << "PythiaEventGenerator::execute() No of accepted Particles : " << particleAccepted << endl;
  if (reportDebug())
    cout << "PythiaEventGenerator::execute() No of counted Particles : " << particleCounted << endl;
  if (reportDebug())
    cout << "PythiaEventGenerator::execute() event completed!" << endl;
}

void PythiaEventGenerator::finalize()
{
  if (reportDebug())
    cout << "PythiaEventGenerator::finalize() started" << endl;
  if (reportInfo())
    pythia8->PrintStatistics();
  if (reportDebug())
    cout << "PythiaEventGenerator::finalize() completed" << endl;
}

// pythia8->ReadString("Init:showChangedSettings = on");      // list changed settings
// pythia8->ReadString("Init:showChangedParticleData = off"); // list changed particle data
// pythia8->ReadString("Next:numberCount = 100000");            // print message every n events
// pythia8->ReadString("Next:numberShowInfo = 1");            // print event information n times
// pythia8->ReadString("Next:numberShowProcess = 0");         // print process record n times
// pythia8->ReadString("Next:numberShowEvent = 0");
//
// pythia8->ReadString("SoftQCD:all = off");                   // Allow total sigma = elastic/SD/DD/ND
//                                                            // Optionally only study one or a few processes at a time.
//                                                            //  pythia8->ReadString("SoftQCD:elastic = on");               // Elastic
//                                                            //  pythia8->ReadString("SoftQCD:singleDiffractive = on");     // Single diffractive
//                                                            //  pythia8->ReadString("SoftQCD:doubleDiffractive = on");     // Double diffractive
//                                                            //  pythia8->ReadString("SoftQCD:centralDiffractive = on");    // Central diffractive
//                                                            //  pythia8->ReadString("SoftQCD:nonDiffractive = on");        // Nondiffractive (inelastic)
//                                                            //  pythia8->ReadString("SoftQCD:inelastic = on");             // All inelastic
//                                                            // Optionally switch on hand-set cross section and Coulomb term.
//                                                            // Note: these values are illustrative only, not to be taken seriously.
//                                                            //  pythia8->ReadString("SigmaTotal:setOwn = on");              // set your own total/elastic/diffr. sigma
//                                                            //  pythia8->ReadString("SigmaTotal:sigmaTot = 106.0");         // total cross section (excluding Coulomb)
//                                                            //  pythia8->ReadString("SigmaTotal:sigmaXX = 5.0");        // reduce double diffractive cross section
//                                                            //  pythia8->ReadString("SigmaElastic:setOwn = on");        // suboption for Coulomb term in elastic
//                                                            //  pythia8->ReadString("SigmaElastic:tAbsMin = 4e-5");        // divergence requires lower cutoff
//
// pythia8->ReadString("HardQCD:all = on");
//
//
////  Optionally select diffractive model.
////  pythia8->ReadString("SigmaDiffractive:mode = 2");           // the MBR model for diffraction
//
//// Multiparton interactions and impact parameter picture.
//// Note: these values are illustrative only, not to be taken seriously.
////  pythia8->ReadString("MultipartonInteractions:pT0Ref = 2.");
////  pythia8->ReadString("MultipartonInteractions:ecmRef = 1960.");
////  pythia8->ReadString("MultipartonInteractions:ecmPow = 0.16");
////  pythia8->ReadString("MultipartonInteractions:pTmin = 0.2");
////  pythia8->ReadString("MultipartonInteractions:bProfile = 2");
////  pythia8->ReadString("MultipartonInteractions:coreRadius = 0.4");
////  pythia8->ReadString("MultipartonInteractions:coreFraction = 0.5");
////  pythia8->ReadString("ultipartonInteractions:expPow = 1.");
//
//// use a reproducible seed: always the same results for the tutorial.
////pythia8->ReadString("Random:setSeed = on");
////pythia8->ReadString("Random:seed = 42");
