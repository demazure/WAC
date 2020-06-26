// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class CanvasCollection
 \ingroup WAC

 Canvas container class
 */

#include "CanvasCollection.hpp"

ClassImp(CanvasCollection);

////////////////////////////////////////////////////
// CTOR
////////////////////////////////////////////////////
CanvasCollection::CanvasCollection(int capacity)
: nCanvasCapacity(capacity), nCanvas(0)
{
  collection = new TCanvas*[capacity];
}

////////////////////////////////////////////////////
// DTOR
////////////////////////////////////////////////////
CanvasCollection::~CanvasCollection()
{

}

////////////////////////////////////////////////////
// Create a directory or folder in the file system
// If the directory exists, do nothing.
////////////////////////////////////////////////////
void CanvasCollection::createDirectory(const TString & dirName)
{
  gSystem->mkdir(dirName,1);
}

////////////////////////////////////////////////////
// Create a canvas
////////////////////////////////////////////////////
TCanvas * CanvasCollection::createCanvas(const TString & canvasName, const CanvasConfiguration & canvasConfig, int inc)
{
  TCanvas * canvas = new TCanvas(canvasName,canvasName,canvasConfig.x+nCanvas*inc,canvasConfig.y,canvasConfig.width,canvasConfig.height);
  canvas->SetLogx(canvasConfig.logx);
  canvas->SetLogy(canvasConfig.logy);
  canvas->SetLogz(canvasConfig.logz);
  canvas->SetRightMargin(canvasConfig.rightMargin);
  canvas->SetLeftMargin(canvasConfig.leftMargin);
  canvas->SetBottomMargin(canvasConfig.bottomMargin);
  canvas->SetTopMargin(canvasConfig.topMargin);
  canvas->SetTheta(canvasConfig.theta);
  canvas->SetPhi(canvasConfig.phi);

  canvas->SetFillColor(kWhite);
  canvas->SetFillStyle(1001);
  canvas->SetBorderSize(0);
  canvas->SetBorderMode(0);

  collection[nCanvas] = canvas;
  nCanvas++;

  return canvas;
}

////////////////////////////////////////////////////
// Create a canvas
////////////////////////////////////////////////////
TCanvas * CanvasCollection::createCanvasXX(int nx, int ny, const TString & canvasName, const CanvasConfiguration & canvasConfig, int inc)
{
  TCanvas * canvas = new TCanvas(canvasName,canvasName,canvasConfig.x+nCanvas*inc,canvasConfig.y,canvasConfig.width,canvasConfig.height);
  canvas->Divide(nx,ny,0,0);
  canvas->SetLogx(canvasConfig.logx);
  canvas->SetLogy(canvasConfig.logy);
  canvas->SetLogz(canvasConfig.logz);
  canvas->SetRightMargin(canvasConfig.rightMargin);
  canvas->SetLeftMargin(canvasConfig.leftMargin);
  canvas->SetBottomMargin(canvasConfig.bottomMargin);
  canvas->SetTopMargin(canvasConfig.topMargin);
  canvas->SetTheta(canvasConfig.theta);
  canvas->SetPhi(canvasConfig.phi);

  canvas->SetFillColor(kWhite);
  canvas->SetFillStyle(1001);
  canvas->SetBorderSize(0);
  canvas->SetBorderMode(0);

  collection[nCanvas] = canvas;
  nCanvas++;

  return canvas;
}


////////////////////////////////////////////////////
// Print Canvas
////////////////////////////////////////////////////
void CanvasCollection::printCanvas(TCanvas * canvas, const TString & directoryName, bool printGif, bool printPdf, bool printSvg, bool printC)
{
  createDirectory(directoryName);
  TString fileName = directoryName;
  fileName += "/";
  fileName += canvas->GetName();
  if (printGif) canvas->Print(fileName+".gif");
  if (printPdf) canvas->Print(fileName+".pdf");
  if (printSvg) canvas->Print(fileName+".svg");
  if (printC)   canvas->Print(fileName+".C");
}

////////////////////////////////////////////////////
// Print Canvases
////////////////////////////////////////////////////
void CanvasCollection::printAllCanvas(const TString & outputPath, bool printGif, bool printPdf, bool printSvg, bool printC)
{
  for (int k=0; k<nCanvas; k++)
    {
    printCanvas(collection[k],outputPath,printGif,printPdf,printSvg,printC);
    }
}

////////////////////////////////////////////////////
// Create a label within an existing pad
////////////////////////////////////////////////////
TLatex * CanvasCollection::createLabel(double x, double y, int color, int fontType, double fontSize, const TString & text, bool doDraw)
{
  TLatex * label;
  label = new TLatex(x,y,text);
  label->SetTextColor(color);
  //label->SetTextFont(fontType);
  label->SetTextSize(fontSize);
  if (doDraw) label->Draw();
  return label;
}

////////////////////////////////////////////////////
// Create standard legend
////////////////////////////////////////////////////
TLegend * CanvasCollection::createLegend(float x1, float y1, float x2, float y2, int fontType, float fontSize)
{
  TLegend *legend = new TLegend(x1,y1,x2,y2);
  //legend->SetTextFont(fontType);
  legend->SetTextSize(fontSize);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  return legend;
}

////////////////////////////////////////////////////
// Create simple line
////////////////////////////////////////////////////
TLine * CanvasCollection::createLine(float x1, float y1, float x2, float y2, int style, int color, int width, bool doDraw)
{
  TLine *line = new TLine(x1,y1,x2,y2);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->Draw();
  return line;
}

////////////////////////////////////////////////////
// Create Arrow Line
////////////////////////////////////////////////////
TArrow * CanvasCollection::createArrow(float x1, float y1, float x2, float y2, float arrowSize, Option_t* option, int style, int color, int width, bool doDraw)
{
  TArrow *line = new TArrow(x1,y1,x2,y2,arrowSize,option);
  line->SetLineStyle(style);
  line->SetLineColor(color);
  line->SetLineWidth(width);
  line->Draw();
  return line;
}
