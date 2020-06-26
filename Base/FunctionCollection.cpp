// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class FunctionCollection
 \ingroup WAC

 Container class for root functions TF1
 */

#include "FunctionCollection.hpp"

ClassImp(FunctionCollection);

FunctionCollection::FunctionCollection(int functionCapacity)
: nFunctionCapacity(functionCapacity),nFunctions(0)
{
  cout << "FunctionCollection() called" << endl;
  functions = new TF1*[nFunctionCapacity];
}

FunctionCollection::FunctionCollection(const FunctionCollection & a)
: nFunctionCapacity(a.nFunctionCapacity),nFunctions(0)
{
  functions = new TF1*[nFunctionCapacity];
}

FunctionCollection::~FunctionCollection()
{
  //deletefunctions();
}

void FunctionCollection::addToList(TF1 * f)
{
  functions[nFunctions++] = f;
}

TF1 * FunctionCollection::getFunction(int i)
{
  return functions[i];
}


////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void FunctionCollection::setDefaultOptions(bool color)
{
  if (color)
    gStyle->SetPalette(1,0);
  else
    gStyle->SetPalette(7,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);

}

////////////////////////////////////////////////////////////////////////
// Setting Function Properties
////////////////////////////////////////////////////////////////////////


void FunctionCollection::setFunctionProperties(TF1 * f, const GraphConfiguration & graphConfiguration)
{
  cout << "Setting properties of histo: " << f->GetTitle() << endl;
  f->SetLineColor(graphConfiguration.lineColor);
  f->SetLineStyle(graphConfiguration.lineStyle);
  f->SetLineWidth(graphConfiguration.lineWidth);
  TAxis * xAxis = (TAxis *) f->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  //xAxis->SetTitle(graphConfiguration.xTitle);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) f->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
  //yAxis->SetTitle(graphConfiguration.yTitle);
  if (f->IsA() == TF2::Class() )
    {
    cout << "Setting properties as 2D function: " << f->GetTitle() << endl;
    TAxis * zAxis = (TAxis *) f->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.nZDivisions);
    zAxis->SetTitleSize(graphConfiguration.zTitleSize);
    zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
    zAxis->SetLabelSize(graphConfiguration.zLabelSize);
    zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
    }
}


void FunctionCollection::plotAllFunctions(const TString & outputPath, bool doPrint)
{
  //cout << "-INFO- plotAllHistos(...) Ploting all histos of the collection." << endl;
  cout << "-INFO- plotAllHistos(...) nFunctions:" << nFunctions << " with capacity:" << nFunctionCapacity << endl;

  GraphConfiguration * gc1D = new GraphConfiguration(1,0);
  GraphConfiguration * gc2D = new GraphConfiguration(2,0);
  CanvasConfiguration * cc1D = new CanvasConfiguration(1);
  CanvasConfiguration * cc2D = new CanvasConfiguration(0);
  CanvasCollection * canvasCollection = new CanvasCollection();
  //cout << "-INFO- plotAllHistos(...) Creating directory (unless it already exists):" << outputPath << endl;
  canvasCollection->createDirectory(outputPath);
  TString name;
  //cout << "-INFO- plotAllHistos(...) nFunctions:" << nFunctions << " with capacity:" << nFunctionCapacity << endl;

  for (int iFunc=0; iFunc<nFunctions; iFunc++)
    {
    TF1* f = functions[iFunc];
    name = f->GetName();
    if (f->IsA() == TF1::Class())
      {
      cout << "-INFO- plotAllHistos(...) Plotting 1D histo #" << iFunc << " named " << f->GetTitle() << endl;
      TCanvas * c = canvasCollection->createCanvas(name, *cc1D, 30);
      setFunctionProperties(f, *gc1D);
      f->Draw();
      }
    else if (f->IsA() == TF2::Class())
      {
      cout << "-INFO- plotAllHistos(...) Plotting 2D histo #" << iFunc << " named " << f->GetTitle() << endl;
      TCanvas * c = canvasCollection->createCanvas(name, *cc2D, 30);
      setFunctionProperties(f, *gc2D);
      f->Draw("SURF3");
      }
    }
  if (doPrint) canvasCollection->printAllCanvas(outputPath);
  }
