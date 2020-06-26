// Author: Claude Pruneau   12/12/16

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class CanvasConfiguration
 \ingroup WAC

 Utility class used to define the parameters of a root canvas
 */


#include "Event.hpp"

ClassImp(Factory<Particle>);

//Factory<Particle> * particleFactory = 0;
//
//extern Factory<Particle> * getParticleFactory(int nParticleMax=5000)
//{
//  if (!particleFactory)
//    {
//    particleFactory = new Factory<Particle>();
//    particleFactory->initialize(nParticleMax);
//    }
//  return particleFactory;
//}


ClassImp(Event);

Event::Event()
 :
 index(0),
 nParticles(0),
 centrality(0),
 multiplicity(0),
 impactParameter(0),
 other(0)
 {
 }

 //////////////////////////
 // DTOR
 //////////////////////////
 Event::~Event()
 {
 }

 ////////////////////////////////////////////////////
 // Call before restarting simulation
 ////////////////////////////////////////////////////
void Event::clear()
 {
 index      = 0;
 nParticles = 0;
 //getParticleFactory()->reset();
 Particle::getFactory()->reset();
 }

 ////////////////////////////////////////////////////
 // Call before generating new event
 ////////////////////////////////////////////////////
  void Event::reset()
 {
 index++;
 nParticles = 0;
 //getParticleFactory()->reset();
 Particle::getFactory()->reset();
 }


 //
 // Get the particle at the given index
 //
 Particle * Event::getParticleAt(int index)
 {
 if (index>=0 && index<nParticles)
   {
   //return getParticleFactory()->getObjectAt(index);
   return Particle::getFactory()->getObjectAt(index);
   }
 else
   return 0;
 }


 ///////////////////////////////////////////////////////
 // Print properties of this event at the given output
 ///////////////////////////////////////////////////////
void Event::printProperties(ostream & output)
 {
 output << "      Event# " << index << endl;
 output << " nParticles: " << nParticles << endl;

 for (int iParticle=0; iParticle<nParticles; iParticle++)
   {
     //getParticleFactory()->getObjectAt(iParticle)->printProperties(output);
   Particle::getFactory()->getObjectAt(iParticle)->printProperties(output);
   }
 }

Event * Event::event = 0;
Event * Event::getEvent()
{
  if (!event)
    {
    event = new Event();
    }
  return event;
}

ClassImp(Factory<Particle>);

