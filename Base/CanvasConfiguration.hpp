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

class CanvasConfiguration
{
public:

  enum CanvasFormat   { PortraitTight, Portrait, PortraitWide, SquareTight, Square, SquareWide, LandscapeTight, Landscape, LandscapeWide };
  enum CanvasAxes     { Linear, LogX, LogY, LogZ, LogXY, LogXZ, LogYZ, LogXYZ  };

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
  CanvasConfiguration(CanvasFormat format, CanvasAxes axes);
  CanvasConfiguration(bool log=0, int style=0, int dimension=1);
  CanvasConfiguration(const CanvasConfiguration & canvasConfig);
  CanvasConfiguration & operator=(const CanvasConfiguration & canvasConfig);
  virtual ~CanvasConfiguration();

  void setFormat(CanvasFormat format=Square);
  void setAxes(CanvasAxes format=Linear);
  void setMargins(float leftMargin, float topMargin, float rightMargin, float bottomMargin);
  void setSize(int width, int height);

  ClassDef(CanvasConfiguration,0)

};


#endif /* CanvasConfiguration_hpp */
