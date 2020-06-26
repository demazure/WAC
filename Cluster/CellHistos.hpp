// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
#ifndef WAC_CellHistos
#define WAC_CellHistos
#include "TString.h"
#include "Histograms.hpp"
#include "Cell.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cell Histograms
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CellHistos : public Histograms
{

public:

  CellHistos(const TString & collectionName,
             AnalysisConfiguration * configuration,
             LogLevel  debugLevel);
  CellHistos(TFile * inputFile,
             const TString & collectionName,
             AnalysisConfiguration * configuration,
             LogLevel  debugLevel);
  virtual void initialize();
  void loadHistograms(TFile * inputFile);
  void fill(Cell * cell);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Data Members -- Computed
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  TH1 * hCellNclus;
  TH1 * hCellNpart;
  TH1 * hCellPt;
  TH1 * hCellPhi;
  TH1 * hCellY;
  TH1 * hCellMass;
  TH1 * hCellBeta;
  ClassDef(CellHistos,0)
};


#endif /* WAC_CellHistos  */
