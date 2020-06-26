// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class OverlapFunction
 \ingroup WAC

 Class defining OverlapFunction
 */

#include "OverlapFunction.hpp"

ClassImp(OverlapFunction);

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////
OverlapFunction::OverlapFunction(const TString & _systemName,
                                 int _nB, double _maxB,
                                 int _nX, double _maxX,
                                 double _radius,
                                 EccentricityCalc * _eccentricityCalc,
                                 EccentricityVsCent * _eccentricityVsCent,
                                 bool _doThicknessMaps=false)
:
systemName(_systemName),
nB(_nB),
maxB(_maxB),
dB( _maxB/double(_nB) ),
nX(_nX),
maxX(_maxX),
radius(_radius),
h_overlap(0),
h_probability(0),
//h_thick(0),
eccentricityCalc(_eccentricityCalc),
eccentricityVsCent(_eccentricityVsCent),
doThicknessMaps(_doThicknessMaps)
{
  cout << "-INFO- OverlapFunction::OverlapFunction() ctor Started" << endl;
  TString baseName = systemName;
  baseName += "_";
  h_overlap     = new TH1D(baseName+"overlap",baseName+"overlap",  nB,0.0,maxB);
  h_probability = new TH1D(baseName+"probability",baseName+"probability",nB,0.0,maxB);
  h_overlap->Sumw2();
  h_probability->Sumw2();
  //h_thick = new TH2D(name,name,nX,-maxX,maxX, nX, -maxX, maxX);

  if (doThicknessMaps)
    {
    h_thicknessMaps = new TH2*[nB];
    for (int iB=0;iB<nB;iB++)
      {
      TString name = baseName;
      name += "_thick_";
      name += iB;
      h_thicknessMaps[iB] = new TH2D(name,name,nX,-maxX,maxX, nX, -maxX, maxX);
      }
    }
  cout << "-INFO- OverlapFunction::OverlapFunction() Completed" << endl;
}


void OverlapFunction::calculateOverlap(TH2* proj1, TH2* proj2)
{
  double dx = 2.0*maxX/double(nX);
  double b, x, y;
  double sum;

  int j1X, j1Y;
  int j2X, j2Y;

  int nProjX = proj1->GetXaxis()->GetNbins();
  int nProjY = proj1->GetYaxis()->GetNbins();
  double v1, v2, v;

  for (int iB=0; iB<nB; iB++)
    {
    //h_thick->Reset();
    eccentricityCalc->reset();
    b = h_overlap->GetXaxis()->GetBinCenter(iB+1) - dB/2.0;
    //b = dB * double(iB);
    x = -maxX - dx/2.0;
    sum = 0.0;
    for (int iX = 0; iX<nX; iX++)
      {
      x += dx;
      j1X = proj1->GetXaxis()->FindFixBin(x+b/2);
      if (j1X<1 || j1X>nProjX) continue;
      j2X = proj2->GetXaxis()->FindFixBin(x-b/2);
      if (j2X<1 || j2X>nProjX) continue;
      y = -maxX - dx/2.0;
      for (int iY = 0; iY<nX; iY++)
        {
        y += dx;
        j1Y = proj1->GetYaxis()->FindFixBin(y);
        if (j1Y<1 || j1Y>nProjY) continue;
        j2Y = proj2->GetYaxis()->FindFixBin(y);
        if (j2Y<1 || j2Y>nProjY) continue;
        v1 = proj1->GetBinContent(j1X,j1Y);
        v2 = proj2->GetBinContent(j2X,j2Y);
        v  = v1*v2;
        sum += v;
        eccentricityCalc->fill(x,y,v);
        if (doThicknessMaps)
          {
          h_thicknessMaps[iB]->Fill(x,y,v);
          }
        }
      }
    h_overlap->SetBinContent(iB+1,sum);
    h_overlap->SetBinError(iB+1,0.0);
    eccentricityCalc->calculateAverages();
    eccentricityVsCent->fill(iB+1,eccentricityCalc);
    } // for (int iB=0; iB<nB; iB++)
}

void OverlapFunction::calculateProbability(double sigma)
{
  // calculate prob
  double thickness;
  double sum = 0.0;
  for (int iB=0; iB<nB; iB++)
     {
     double b = h_overlap->GetXaxis()->GetBinCenter(iB+1) - dB/2.0;
     //double b = dB * double(iB);
     thickness = h_overlap->GetBinContent(iB);
     double v = 2.0*3.1415927*b*(1 - exp(-sigma*thickness));
     h_probability->SetBinContent(iB,v);
     h_probability->SetBinError(iB,0.0);
     sum += v;
     }
  h_probability->Scale(1.0/sum);

}

void OverlapFunction::save()
{
  h_overlap->Write();
  h_probability->Write();

  if (doThicknessMaps)
    {
    for (int iB=0;iB<nB;iB++)
      {
      h_thicknessMaps[iB]->Write();
      }
    }

}
