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
ClassImp(PythiaEventGenerator);

PythiaEventGenerator::PythiaEventGenerator(const TString & name,
                                 TaskConfiguration * configuration,
                                 Event * event,
                                 EventFilter * ef,
                                 ParticleFilter * pf)
:
Task(name, configuration, event),
eventFilter(ef),
particleFilter(pf)
{
  if (reportDebug()) cout << "PythiaEventGenerator::PythiaEventGenerator(...) No ops" << endl;
}

PythiaEventGenerator::~PythiaEventGenerator()
{
if (reportDebug()) cout << "PythiaEventGenerator::~PythiaEventGenerator(...) No ops" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::initialize()
{
  if (reportDebug()) cout << "PythiaEventGenerator::initialize() Started" << endl;

  nMax = 10000;
  particles = new TClonesArray("TParticle", nMax);
  pythia8 = new TPythia8();

  pythia8->ReadString("HardQCD:all = on");
  pythia8->ReadString("Random:setSeed = on");
  // use a reproducible seed: always the same results for the tutorial.
  pythia8->ReadString("Random:seed = 42");
  // Initialize
  pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* TeV */);

  if (reportDebug()) cout << "PythiaEventGenerator::initialize() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset and Initialize the generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::reset()
{
  if (reportDebug()) cout << "PythiaEventGenerator::reset() Started" << endl;
  event->reset();
  Particle::getFactory()->reset();
  if (reportDebug()) cout << "PythiaEventGenerator::reset() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read an ampt event from file
// Copy the event into Event for convenience...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PythiaEventGenerator::execute()
{
  if (reportDebug()) cout << "PythiaEventGenerator::execute() Started" << endl;

  Factory<Particle> * particleFactory = Particle::getFactory();
  int nparts;
  bool seekingEvent = true;
  while (seekingEvent)
    {
    pythia8->GenerateEvent();
    if (reportDebug()) pythia8->EventListing();
    if (reportDebug()) cout << "PythiaEventGenerator::execute() Calling pythia8->ImportParticles()" << endl;

    pythia8->ImportParticles(particles,"Final");
    if (reportDebug()) cout << "PythiaEventGenerator::execute() pythia8->ImportParticles() completed" << endl;
    nparts = particles->GetEntriesFast();
    if (reportDebug()) cout << "PythiaEventGenerator::execute() with nparts:" << nparts << endl;
    if (nparts>2) seekingEvent = false;
    }
  if (nparts>nMax)
      {
      if (reportError()) cout << " ARRAY TOO SMALL np>nMax. nparts:" << nparts << " nMax:" << nMax << endl;
      postTaskFatal();
      //exit(0);
      }

  int thePid;
  double charge, mass, p_x, p_y, p_z, p_e;
  Particle * particle;
  int particleAccepted = 0;
  int particleCounted = 0;

  //------------------- Randomizing the particle phi --------------Starts
  double eventAngle= TMath::TwoPi() * gRandom->Rndm();
  double cosPhi = cos(eventAngle);
  double sinPhi = sin(eventAngle);

  // load particles from TClone storage and copy into event.
  Particle aParticle;
  //if (reportDebug()) cout << "PythiaEventGenerator::execute() starting copy loop into event..." << endl;

  for (int iParticle = 0; iParticle < nparts; iParticle++)
    {
    TParticle & part = * (TParticle*) particles->At(iParticle);
    int ist = part.GetStatusCode();
    //if (reportDebug()) cout << "PythiaEventGenerator::execute() ist: " << ist << endl;
    if (ist <= 0) continue;
    int pdg = part.GetPdgCode();
    mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
    if (mass<0.002) continue;  // no photons, electrons...
    charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
    p_x  = cosPhi*part.Px() - sinPhi*part.Py();
    p_y  = sinPhi*part.Px() + cosPhi*part.Py();
    p_z  = part.Pz();
    p_e  = part.Energy();
    aParticle.setPidPxPyPzE(pdg, charge, p_x,p_y,p_z,p_e);
    //aParticle.printProperties(cout);
    //if (reportDebug()) cout << "PythiaEventGenerator::execute() calling filter " << endl;
    particleCounted++;
    if (!particleFilter->accept(aParticle)) continue;
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
  event->multiplicity = particleCounted;
  if (reportDebug()) cout << "PythiaEventGenerator::execute() No of accepted Particles : "<< particleAccepted<<endl;
  if (reportDebug()) cout << "PythiaEventGenerator::execute() No of counted Particles : "<< particleCounted <<endl;
  if (reportDebug()) cout << "PythiaEventGenerator::execute() event completed!" << endl;
}

void PythiaEventGenerator::finalize()
{
  if (reportDebug()) cout << "PythiaEventGenerator::finalize() started" << endl;
  if (reportInfo()) pythia8->PrintStatistics();
  if (reportDebug()) cout << "PythiaEventGenerator::finalize() completed" << endl;
}




