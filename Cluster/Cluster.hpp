// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_Cluster
#define WAC_Cluster
#include "TLorentzVector.h"
#include "Particle.hpp"
#include "Factory.hpp"

class Cluster
{
public:
  Cluster();
  ~Cluster();
  Particle * getParticleAt(int index);
  void reset();
  void add(Particle * particle);
  void boost(double beta_x, double beta_y, double beta_z);
  void boostZ(double boost);
  void printProperties(ostream & output);

  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  int  nParticles;
  Particle ** particles;
  TLorentzVector p;
  double Qlong, Qout, Qside;

  static int factorySize;
   static Factory<Cluster> * factory;
   static Factory<Cluster> * getFactory();

   ClassDef(Cluster,0)
};

#endif /* WAC_Cluster */
