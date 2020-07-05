// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_RapidityGenerator
#define WAC_RapidityGenerator

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// RapidityGenerator                                                   //
//                                                                     //
// Cluster Rapidity Generator                                          //
//
// option:
// 0: uniform distribution
// 1: exponential distribution
// 2: gaussian distribution
//
// pairMode:
// 0: singles only
// 1: pair generation
//
//
/////////////////////////////////////////////////////////////////////////
#include <ostream>
#include "TRandom.h"
using namespace std;

class RapidityGenerator
{
public:
  
  int     option;
  int     pairMode;
  double  maxRapidity;
  double  scale;
  double  minimumSeparation;
  double  store[100];
  int     nStored;
  double  pairRapidity;
  double  saveRapidity;
  int     rank;
  TRandom * r;
  
  
  RapidityGenerator(int opt, int pair, double max, double s=10.0, double minSep=0.1);

  virtual ~RapidityGenerator() {}

  void reset();
  double generate();

  ClassDef(RapidityGenerator,0)

};


#endif /* RapidityGenerator_h */
