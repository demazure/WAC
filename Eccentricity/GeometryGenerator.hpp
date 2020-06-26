// Author: Claude Pruneau   05/08/2020

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/
#ifndef WAC_GeometryGenerator
#define WAC_GeometryGenerator
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TLine.h"
#include "TArrow.h"
#include "TAxis.h"
#include "TRandom.h"
#include "TMath.h"
#include "GraphConfiguration.hpp"
#include "GeometryPoint.hpp"
#include "GeometryMoments.hpp"

////////////////////////////////////////////////////
// Correlation Dist Width Calculator
////////////////////////////////////////////////////
class GeometryGenerator
{
public:

  TString generatorBaseName;
  TString generatorName;

  int    nR;
  double maxR;
  int    nX;
  double maxX;
  int    type;
  double parA;
  double parB;
  double parC;
  long   nEvents;
  double crossSection;
  double rmsRadius;

  double nucleusSize;
  double nucleonnucleonCrossSection; // in fm^2
  double nucleonRadius;
  double nucleusRadius;
  double maxDistanceSq;
  int    generationMode;
  int    nParticles;

  double rho0;
  double volume;
  double avgR;
  double rmsR;

  TH1 * rDensity;
  TH1 * rProfile;
  TH1 * rProfileGen;
  TH1 * thicknessVsR;
  TH2 * xyThickness;

  GeometryPoint ** points;
  GeometryMoments geometryMoments;

//  double * xPos;
//  double * yPos;
//  double * zPos;

  GeometryGenerator(const TString & generatorBaseName,
                    int nR, double maxR,
                    int nX, double maxX,
                    int type, double parA, double parB, double parC,
                    double _nucleusSize,
                    double _nucleusRadius,
                    double _nnCrossSection,
                    double _nucleonRadius,
                    long nEvents, double crossSection);

  GeometryGenerator(const TString & generatorBaseName,
                    int _nR, double _maxR,
                    int _nX, double _maxX,
                    int _type, double _parA, double _parB, double _parC,
                    double _nucleusSize,
                    double _nucleusRadius,
                    double _nnCrossSection,
                    double _nucleonRadius,
                    int _generationMode);

  ~GeometryGenerator()
  {
  // no ops.
  }

  void createHistograms();
  void calculateMoments();
  void generatePoint(GeometryPoint & point);
  void generate();
  void generateNucleus();
  void generateThicknessFunction();
  void save();

  ClassDef(GeometryGenerator,0)
  
};

#endif /* GeometryGenerator_hpp */
