// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_GraphConfiguration
#define WAC_GraphConfiguration

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// GraphConfiguration                                                  //
//                                                                     //
// Class used to describe a graph configuration                        //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TString.h"
#include "TAttMarker.h"
#include "TAttLine.h"

using namespace std;

class GraphConfiguration
{
 public:
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  int npx;
  int npy;

  int lineColor;
  int lineStyle;
  int lineWidth;

  int markerColor;
  int markerStyle;
  float markerSize;

  int nXDivisions;
  float xTitleSize;
  float xTitleOffset;
  float xLabelSize;
  float xLabelOffset;
  TString xTitle;

  int nYDivisions;
  float yTitleSize;
  float yTitleOffset;
  float yLabelSize;
  float yLabelOffset;
  TString yTitle;

  int nZDivisions;
  float zTitleSize;
  float zTitleOffset;
  float zLabelSize;
  float zLabelOffset;
  TString zTitle;

  TString plotOption;

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Member functrions
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  GraphConfiguration(int d = 1, int type = 1);
  GraphConfiguration(const GraphConfiguration& source);

  virtual ~GraphConfiguration() {}

  ClassDef(GraphConfiguration, 0)
};

#endif
