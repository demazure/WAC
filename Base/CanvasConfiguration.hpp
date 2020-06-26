// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                 *
 * All rights reserved.                                                *
 * Based on the ROOT package and environment                           *
 *                                                                     *
 * For the licensing terms see LICENSE.                                *
 **********************************************************************/

#ifndef WAC_CanvasConfiguration
#define WAC_CanvasConfiguration
#include "TCanvas.h"
class CanvasConfiguration
{
public:
  
  ////////////////////////////////////////////////////
  // Data Members
  ////////////////////////////////////////////////////
  int x;
  int y;
  int width;
  int height;

  float leftMargin;
  float topMargin;
  float rightMargin;
  float bottomMargin;

  bool logx;
  bool logy;
  bool logz;

  float theta;
  float phi;

  ////////////////////////////////////////////////////
  // Member functions
  ////////////////////////////////////////////////////
  CanvasConfiguration(bool log=0, int style=0, int dimension=1);
  CanvasConfiguration(const CanvasConfiguration & canvasConfig);
  CanvasConfiguration & operator=(const CanvasConfiguration & canvasConfig);
  virtual ~CanvasConfiguration();

  ClassDef(CanvasConfiguration,0)

};


#endif /* CanvasConfiguration_hpp */
