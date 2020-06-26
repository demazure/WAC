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

#include "Cluster.hpp"


ClassImp(Cluster);

////////////////////////////////////////////////////
// CTOR
////////////////////////////////////////////////////
Cluster::Cluster()
:
nParticles(0),
particles (0),
p         (0,0,0,0),
Qlong     (0),
Qout      (0),
Qside     (0)
{
  particles = new Particle*[5];
}
////////////////////////////////////////////////////
// DTOR
////////////////////////////////////////////////////
Cluster::~Cluster()
{
  delete[] particles;
}

////////////////////////////////////////////////////
// Get the particle at the given index
////////////////////////////////////////////////////
Particle * Cluster::getParticleAt(int index)
{
  //cout << "---DEBUG--- getParticle(int index) index:" << index << endl;
  if (index>=0 && index<nParticles)
    {
    return particles[index];
    }
  else
    return 0;
}

////////////////////////////////////////////////////
// Reset this cluster
////////////////////////////////////////////////////
void Cluster::reset()
{
  nParticles = 0;
}

////////////////////////////////////////////////////
// Add the given particle to this cluster
////////////////////////////////////////////////////
void Cluster::add(Particle * particle)
{
  if (!particle)
    {
    cout << "-ERROR- Cluster::add(Particle*) Invalid particle pointer." << endl;
    return;
    }
//  cout << "    Cluster::add nParticles = " << nParticles << endl;
  particles[nParticles++] = particle;
//  cout << "    Cluster::add nParticles = " << nParticles << endl;
}

void Cluster::boost(double beta_x, double beta_y, double beta_z)
{
  // boost the momentum of the cluster
  p.Boost(beta_x, beta_y, beta_z);
  // boost all particles within the cluster

  //  cout << " boost beta(x,y,z):" << beta_x << "," << beta_y << "," << beta_z << endl;

  for (int iPart=0; iPart<nParticles; iPart++)
    {
    //cout <<  "boost() before particles[k]->boost(-beta_x, -beta_y, -beta_z) k:" << k << " E:" << particles[k]->e << " px:" << particles[k]->px << " py:" << particles[k]->py << " pz:" << particles[k]->pz << endl;
    particles[iPart]->boost(-beta_x, -beta_y, -beta_z);
    //cout <<  "boost()  after particles[k]->boost(-beta_x, -beta_y, -beta_z) k:" << k << " E:" << particles[k]->e << " px:" << particles[k]->px << " py:" << particles[k]->py << " pz:" << particles[k]->pz << endl;
    }
}

void Cluster::boostZ(double boost)
{
  double rapidity, mt, px, py, pz, e;
  // boost the momentum of the cluster
  rapidity = p.Rapidity() + boost;
  mt       = p.Mt();
  px       = p.Px();
  py       = p.Py();
  pz       = mt * sinh(rapidity);
  e        = mt * cosh(rapidity);
  p.SetPxPyPzE(px,py,pz,e);
   for (int iPart=0; iPart<nParticles; iPart++)
    {
    particles[iPart]->boostRapidity(boost);
    }
}

////////////////////////////////////////////////////
// Print properties of this cluster
////////////////////////////////////////////////////
void Cluster::printProperties(ostream & output)
{

  output << "      Cluster mass: " << p.M() << endl;
  output << "        Cluster pt: " << p.Pt() << endl;
  output << "       Cluster phi: " << p.Phi() << endl;
  output << "  Cluster rapidity: " << p.Rapidity() << endl;
  output << "       Cluster eta: " << p.Eta() << endl;
  output << "Cluster nParticles: " << nParticles << endl;
  for (int iPart=0; iPart<nParticles; iPart++)
    {
    particles[iPart]->printProperties(output);
    }
}

int Cluster::factorySize = 500;
Factory<Cluster> * Cluster::factory = 0;
Factory<Cluster> * Cluster::getFactory()
{
  if (!factory)
    {
    factory = new Factory<Cluster>();
    factory->initialize(factorySize);
    }
   return factory;
}



