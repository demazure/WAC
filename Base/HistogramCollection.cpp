//
//  HistogramCollection.cpp
//  MyMC
//
//  Created by Claude Pruneau on 12/11/16.
//  Copyright © 2016 Claude Pruneau. All rights reserved.
//

#include "HistogramCollection.hpp"
#include "TLorentzVector.h"

ClassImp(HistogramCollection);

double const  kPI        = TMath::Pi();
double const  kTWOPI     = TMath::TwoPi();

////////////////////////////////////////////////////////////////////////////
// CTOR1
////////////////////////////////////////////////////////////////////////////
HistogramCollection::HistogramCollection(const TString & name,
                                         int  histoCapacity,
                                         LogLevel debugLevel)
:
MessageLogger(debugLevel),
collectionName(name),
nHistoCapacity(histoCapacity),
nHistograms   (0),
histograms    (0),
isSaved       (0),
isPlotted     (0),
isPrinted     (0),
isScaled      (0),
bOwnTheHistograms(true),
scaled    (1),
saved     (1),
plotted   (1),
printed   (1),
notScaled (0),
notSaved  (0),
notPlotted (0),
notPrinted (0),
randomGenerator(new TRandom())
{
  if (reportDebug()) cout << "HistogramCollection() started" << endl;
  histograms   = new TH1*[nHistoCapacity];
  isSaved      = new bool[nHistoCapacity];
  isPlotted    = new bool[nHistoCapacity];
  isPrinted    = new bool[nHistoCapacity];
  isScaled     = new bool[nHistoCapacity];
  if (reportDebug()) cout << "HistogramCollection() completed" << endl;
}

////////////////////////////////////////////////////////////////////////////
// DTOR
////////////////////////////////////////////////////////////////////////////
HistogramCollection::~HistogramCollection()
{
  if (reportDebug()) cout << "~HistogramCollection() started" << endl;
  if (bOwnTheHistograms) {
    /* the instance own its histograms so, they have to be deleted */
    for (int i = 0; i < nHistograms; i++) {
      delete histograms[i];
    }
    delete randomGenerator;
    delete [] isScaled;
    delete [] isPrinted;
    delete [] isPlotted;
    delete [] isSaved;
    delete [] histograms;
  }
  if (reportDebug()) cout << "~HistogramCollection() completed" << endl;
}

////////////////////////////////////////////////////////////////////////////
// Add the given histogram to the list
////////////////////////////////////////////////////////////////////////////
void HistogramCollection::addToList(TH1 * h,
                                    bool doScale,
                                    bool doSave,
                                    bool doPlot,
                                    bool doPrint,
                                    bool doSumw2)
{
  if (reportInfo()) cout << "HistogramCollection::addToList(...) Adding histo " << h->GetName() << " w/ index:" << nHistograms << " max:" << nHistoCapacity << " w/ doSave: " << doSave << " doPlot:" << doPlot << " doPrint:" << doPrint << " doScale:" << doScale  << endl;
  //if (doSumw2)  h->Sumw2();
  histograms  [nHistograms] = h;
  isSaved     [nHistograms] = doSave;
  isPlotted   [nHistograms] = doPlot;
  isPrinted   [nHistograms] = doPrint;
  isScaled    [nHistograms] = doScale;
  nHistograms++;
}

TH1 * HistogramCollection::getHisto(int i)
{
  return histograms[i];
}


////////////////////////////////////////////////////
// Set Default Style for Plots
////////////////////////////////////////////////////
void HistogramCollection::setDefaultOptions(bool color)
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

/*
 void deleteHistograms();
 TH1 * getHisto(int i);
 int  openHistogramFile(const TString & fileName, const TString & openFileOption);
 void saveHistogramFile();
 void closeHistogramFile();
 */

////////////////////////////////////////////////////////////////////////
// Histogram Creation
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
TH1 * HistogramCollection::createHistogram(const TString &  name,
                                           int n, double min_x, double max_x,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           bool  scale,
                                           bool  save,
                                           bool  plot,
                                           bool  print,
                                           bool  sumw2 )
{
  //createHistogram new 1D historgram
  if (reportDebug()) cout << "Creating  1D histo " << name << " nBins:" << n << " min_x:" << min_x << " max_x:" << max_x << endl;
  TH1 * h;
  h = new TH1F(name,name,n,min_x,max_x);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}


////////////////////////////////////////////////////////////////////////
TH1 * HistogramCollection::createHistogram(const TString &  name,
                                           int n, double * bins,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           bool  scale,
                                           bool  save,
                                           bool  plot,
                                           bool  print,
                                           bool  sumw2 )
{
  //createHistogram new 1D historgram
  if (reportDebug()) cout << "Creating  1D histo " << name << " with " << n << " non uniform nBins:" << endl;
  TH1 * h;
  h = new TH1F(name,name,n,bins);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}


////////////////////////////////////////////////////////////////////////
TH2 * HistogramCollection::createHistogram(const TString &  name,
                                           int n_x, double min_x, double max_x,
                                           int n_y, double min_y, double max_y,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           const TString &  title_z,
                                           bool  scale,
                                           bool  save,
                                           bool  plot,
                                           bool  print,
                                           bool  sumw2 )
{
  //createHistogram new 2D historgram
  if (reportDebug()) cout << "Creating  2D histo " << name << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x << " n_y:" << n_y << " min_y:" << min_y << " max_y:" << max_y<< endl;
  TH2 * h;
  h = new TH2F(name,name,n_x,min_x,max_x,n_y,min_y,max_y);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  h->GetZaxis()->SetTitle(title_z);
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}

////////////////////////////////////////////////////////////////////////
TH2 * HistogramCollection::createHistogram(const TString &  name,
                                           int n_x, double* xbins, int n_y, double min_y, double max_y,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           const TString &  title_z,
                                           bool  scale,
                                           bool  save,
                                           bool  plot,
                                           bool  print,
                                           bool  sumw2 )

{
  if (reportDebug()) cout << "Creating  2D histo " << name << " with " << n_x << " vs " << n_y << " non uniform nBins:" << endl;
  TH2 * h;
  h = new TH2F(name,name,n_x,xbins,n_y,min_y,max_y);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  h->GetZaxis()->SetTitle(title_z);
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}


////////////////////////////////////////////////////////////////////////
TH3 * HistogramCollection::createHistogram(const TString &  name,
                                           int n_x, double min_x, double max_x,
                                           int n_y, double min_y, double max_y,
                                           int n_z, double min_z, double max_z,
                                           const TString &  title_x,
                                           const TString &  title_y,
                                           const TString &  title_z,
                                           const TString &  title_w = "w",
                                           bool  scale,
                                           bool  save,
                                           bool  plot,
                                           bool  print,
                                           bool  sumw2 )

{
  if (reportDebug()) cout
    << "Creating  3D histo " << name
    << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x << " X:" << title_x
    << " n_y:" << n_y << " min_y:" << min_y << " max_y:" << max_y << " Y:" << title_y
    << " n_z:" << n_z << " min_z:" << min_z << " max_z:" << max_z << " Z:" << title_z
    << " W:" << title_w  << endl;
  TH3 * h;
  h = new TH3F(name,name,n_x,min_x,max_x,n_y,min_y,max_y,n_z,min_z,max_z);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  h->GetZaxis()->SetTitle(title_z);
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}


////////////////////////////////////////////////////////////////////////
TProfile * HistogramCollection::createProfile(const TString & name,
                                              int n_x,double min_x,double max_x,
                                              const TString &  title_x,
                                              const TString &  title_y,
                                              bool  save,
                                              bool  plot,
                                              bool  print )

{
  if (reportDebug()) cout << "Creating  1D profile " << name << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x << endl;
  TProfile * h = new TProfile(name,name,n_x,min_x,max_x);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  addToList(h,false,save,plot,print,false);
  return h;
}

////////////////////////////////////////////////////////////////////////
TProfile * HistogramCollection::createProfile(const TString &  name,
                                              int n_x,  double* bins,
                                              const TString &  title_x,
                                              const TString &  title_y,
                                              bool  scale,
                                              bool  save,
                                              bool  plot,
                                              bool  print )
{
  if (reportDebug()) cout << "Creating  1D profile " << name << " n_x:" << n_x << " non-uniform bins" << endl;
  TProfile * h = new TProfile(name,name,n_x,bins);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  addToList(h,false,save,plot,print,false);
  return h;
}




////////////////////////////////////////////////////////////////////////
TProfile2D * HistogramCollection::createProfile(const TString &  name,
                                                int n_x, double min_x, double max_x,
                                                int n_y, double min_y, double max_y,
                                                const TString &  title_x,
                                                const TString &  title_y,
                                                const TString &  title_z,
                                                bool  scale,
                                                bool  save,
                                                bool  plot,
                                                bool  print,
                                                bool  sumw2 )

{
  if (reportDebug()) cout << "Creating  2D profile " << name
    << " n_x:" << n_x << " min_x:" << min_x << " max_x:" << max_x
    << " n_y:" << n_y << " min_y:" << min_y << " max_y:" << max_y
    << endl;
  TProfile2D * h = new TProfile2D(name,name,n_x,min_x,max_x,n_y,min_y,max_y);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  h->GetZaxis()->SetTitle(title_z);
  addToList(h,false,save,plot,print,false);
  return h;
}

////////////////////////////////////////////////////////////////////////
// Add Histograms to an external list
////////////////////////////////////////////////////////////////////////
void HistogramCollection::addHistogramsToExtList(TList *list, bool all)
{
  if (reportDebug()) cout << "HistogramCollection::addHistogramsToExtList(TList *list) started."  << endl;

  for (int k=0; k<nHistograms; k++)
    {
    if (isSaved[k] || all) list->Add(histograms[k]);
    }
  /* the instance stops the histograms ownership */
  bOwnTheHistograms = false;

  if (reportDebug()) cout << "HistogramCollection::addHistogramsToExtList(TList *list) completed."  << endl;
}

////////////////////////////////////////////////////////////////////////
// Save Histograms to file
////////////////////////////////////////////////////////////////////////
void HistogramCollection::saveHistograms(TFile * outputFile, bool saveAll)
{
  if (reportDebug()) cout << "HistogramCollection::saveHistograms(TFile * outputFile) started."  << endl;
  outputFile->cd();
  for (int k=0; k<nHistograms; k++)
    {
    if (isSaved[k] || saveAll) histograms[k]->Write();
    }
  if (reportDebug()) cout << "HistogramCollection::saveHistograms(TFile * outputFile) completed."  << endl;
}

////////////////////////////////////////////////////////////////////////
// Scale Histograms
//
// Set scaleAll to true to scale all histogram of this container.
////////////////////////////////////////////////////////////////////////
void HistogramCollection::scale(double factor, bool scaleAll)
{
  if (reportDebug()) cout << "HistogramCollection::scale(double factor) started."  << endl;
  for (int k=0; k<nHistograms; k++)
    {
    if (isScaled[k] || scaleAll)
      {
      if (false)
        scaleByBinWidth(histograms[k], factor);
      else
        histograms[k]->Scale(factor);
      }
    }
  if (reportDebug()) cout << "HistogramCollection::scale(double factor) completed." << endl;
}

////////////////////////////////////////////////////////////////////////
// Plot Selected Histograms
////////////////////////////////////////////////////////////////////////
void HistogramCollection::plotHistograms(const TString             & outputPath,
                                         CanvasCollection          & canvasCollection,
                                         CanvasConfiguration & cc1d,
                                         CanvasConfiguration & cc2d,
                                         GraphConfiguration  & gc1D,
                                         GraphConfiguration  & gc2D)
{
  if (reportDebug()) cout << "HistogramCollection::plotHistograms(...) collection: " << collectionName << " started  w/ nHistograms =" << nHistograms << endl;

  canvasCollection.createDirectory(outputPath);
  TString   canvasName;
  TCanvas * canvas = 0;
  for (int iHisto=0; iHisto<nHistograms; iHisto++)
    {
    TH1* h = histograms[iHisto];
    canvasName = h->GetName();
    if (isPlotted[iHisto])
      {
      if (h->IsA() == TH1::Class()  ||
          h->IsA() == TH1F::Class() ||
          h->IsA() == TH1D::Class())
        {
        if (reportDebug()) cout << "-INFO- HistogramCollection::plotHistograms(...) Plotting 1D histo #" << iHisto << " named " << h->GetTitle() << endl;
        canvas = canvasCollection.createCanvas(canvasName,cc1d, 30);
        setHistoProperties(h,gc1D);
        h->Draw(gc1D.plotOption);
        }
      else if (h->IsA() == TH2::Class()  ||
               h->IsA() == TH2F::Class() ||
               h->IsA() == TH2D::Class())
        {
        int nx = h->GetNbinsX();
        int ny = h->GetNbinsY();
        if (nx*ny<=10000)
          {
          if (reportDebug()) cout << "-INFO- HistogramCollection::plotHistograms(...) Plotting 2D histo #" << iHisto << " named " << h->GetTitle() << endl;
          canvas = canvasCollection.createCanvas(canvasName,cc2d, 30);
          setHistoProperties(h,gc2D);
          h->Draw(gc2D.plotOption);
          }
        else
          {
          if (reportDebug()) cout << "-INFO- HistogramCollection::plotHistograms(...) Skipping 2D histo #" << iHisto << " named " << h->GetTitle() << endl;
          }
        }
      else if (h->IsA() == TH3::Class())
        {
        if (reportDebug()) cout << "-INFO- plotHistograms(...) Skipping 3D histo called " << h->GetTitle() << endl;
        }
      else
        {
        if (reportDebug()) cout << "-INFO- plotHistograms(...) Skipping histo #" << iHisto << " of unfamiliar type named " << h->GetTitle() << endl;
        }

      if (isPrinted[iHisto] )
        canvasCollection.printCanvas(canvas, outputPath, 0, 1, 0, 0);

      }
    else
      {
      if (reportDebug()) cout << "-INFO- plotHistograms(...) Skipping histo #" << iHisto << " named " << h->GetTitle() << endl;
      }
    }
  if (reportDebug()) cout << "HistogramCollection::plotHistograms(...) completed."<< endl;
}

////////////////////////////////////////////////////////////////////////
// Setting Histogram Properties
////////////////////////////////////////////////////////////////////////
void HistogramCollection::setHistoProperties(TH1 * h, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug()) cout << "Setting properties of histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfiguration.lineColor);
  h->SetLineStyle(graphConfiguration.lineStyle);
  h->SetLineWidth(graphConfiguration.lineWidth);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  //xAxis->SetTitle(graphConfiguration.xTitle);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
  //yAxis->SetTitle(graphConfiguration.yTitle);
  if (h->IsA() == TH2::Class()  || h->IsA() == TH2F::Class() || h->IsA() == TH2F::Class() )
    {
    if (reportDebug()) cout << "Setting properties as 2D histo: " << h->GetTitle() << endl;
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.nZDivisions);
    zAxis->SetTitleSize(graphConfiguration.zTitleSize);
    zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
    zAxis->SetLabelSize(graphConfiguration.zLabelSize);
    zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
    }
}

void HistogramCollection::setHistoProperties(TH2 * h, const GraphConfiguration & graphConfiguration)
{
  if (reportDebug()) cout << "----------------------------------------------------" << endl;
  if (reportDebug()) cout << "Setting properties of 2D histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfiguration.lineColor);
  h->SetLineStyle(graphConfiguration.lineStyle);
  h->SetLineWidth(graphConfiguration.lineWidth);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);

  if (h->IsA() == TH2::Class() || h->IsA() == TH2F::Class() || h->IsA() == TH2D::Class())
    {
    TAxis * zAxis = (TAxis *) h->GetZaxis();
    zAxis->SetNdivisions(graphConfiguration.nZDivisions);
    zAxis->SetTitleSize(graphConfiguration.zTitleSize);
    zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
    zAxis->SetLabelSize(graphConfiguration.zLabelSize);
    zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
    }
}

void HistogramCollection::setHistoProperties(TH1 * h, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle)
{
  if (reportDebug()) cout << "Setting properties of histo: " << h->GetTitle() << endl;
  h->SetLineColor(graphConfiguration.lineColor);
  h->SetLineStyle(graphConfiguration.lineStyle);
  h->SetLineWidth(graphConfiguration.lineWidth);
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
  yAxis->SetTitle(yTitle);
}

void HistogramCollection::setHistoProperties(TH2 * h, const GraphConfiguration & graphConfiguration, const TString & xTitle, const TString & yTitle, const TString & zTitle)
{
  if (reportDebug()) cout << "Setting properties of histo: " << h->GetTitle() << endl;
  TAxis * xAxis = (TAxis *) h->GetXaxis();
  xAxis->SetNdivisions(graphConfiguration.nXDivisions);
  xAxis->SetTitleSize(graphConfiguration.xTitleSize);
  xAxis->SetTitleOffset(graphConfiguration.xTitleOffset);
  xAxis->SetTitle(xTitle);
  xAxis->SetLabelSize(graphConfiguration.xLabelSize);
  xAxis->SetLabelOffset(graphConfiguration.xLabelOffset);
  TAxis * yAxis = (TAxis *) h->GetYaxis();
  yAxis->SetNdivisions(graphConfiguration.nYDivisions);
  yAxis->SetTitleSize(graphConfiguration.yTitleSize);
  yAxis->SetTitleOffset(graphConfiguration.yTitleOffset);
  yAxis->SetLabelSize(graphConfiguration.yLabelSize);
  yAxis->SetLabelOffset(graphConfiguration.yLabelOffset);
  yAxis->SetTitle(yTitle);
  TAxis * zAxis = (TAxis *) h->GetZaxis();
  zAxis->SetNdivisions(graphConfiguration.nZDivisions);
  zAxis->SetTitleSize(graphConfiguration.zTitleSize);
  zAxis->SetTitleOffset(graphConfiguration.zTitleOffset);
  zAxis->SetLabelSize(graphConfiguration.zLabelSize);
  zAxis->SetLabelOffset(graphConfiguration.zLabelOffset);
  zAxis->SetTitle(zTitle);
}

////////////////////////////////////////////////////////////////////////
// Histogram Operation on Collections
////////////////////////////////////////////////////////////////////////

void HistogramCollection::addHistos(HistogramCollection * c1, double a1)
{
  for (int i=0; i<nHistograms; ++i)
    {
    TH1 * h1 = getHisto(i);
    TH1 * h2 = c1->getHisto(i);
    h1->Add(h2,a1);
    }
}

void HistogramCollection::addHistos(HistogramCollection * c1, HistogramCollection * c2, double a1, double a2)
{
  for (int i=0; i<nHistograms; ++i)
    {
    TH1 * h  = getHisto(i);
    TH1 * h1 = c1->getHisto(i);
    TH1 * h2 = c2->getHisto(i);
    h->Add(h1,h2,a1,a2);
    }
}

void HistogramCollection::addHistos(HistogramCollection * c1, HistogramCollection * c2, HistogramCollection * c3, double a1, double a2, double a3)
{
  for (int i=0; i<nHistograms; ++i)
    {
    TH1 * h  = getHisto(i);
    TH1 * h1 = c1->getHisto(i);
    TH1 * h2 = c2->getHisto(i);
    TH1 * h3 = c3->getHisto(i);
    h->Add(h1,h2,a1,a2);
    h->Add(h3,a3);
    }
}

void HistogramCollection::addHistos(HistogramCollection * c1, HistogramCollection * c2, HistogramCollection * c3, HistogramCollection * c4,
                                    double a1, double a2, double a3, double a4)
{
  for (int i=0; i<nHistograms; ++i)
    {
    TH1 * h  = getHisto(i);
    TH1 * h1 = c1->getHisto(i);
    TH1 * h2 = c2->getHisto(i);
    TH1 * h3 = c3->getHisto(i);
    TH1 * h4 = c4->getHisto(i);
    h->Add(h1,h2,a1,a2);
    h->Add(h3,a3);
    h->Add(h4,a4);
    }
}


////////////////////////////////////////////////////////////////////////
// Histogram Operations
////////////////////////////////////////////////////////////////////////


//Calculate External Product h_1 x h_2 and store into n1n1_12
//Verify if dimensions are compatible, aborts if they are not.
//The output is a 1D histogram packed according to the size of h_1 and h_2.
double HistogramCollection::calculateN1N1(const TH1 * h_1, const TH1 * h_2, TH1 * h_12, double a1, double a2)
{
  if (!h_1  || !h_2 || !h_12)
    {
    if (reportDebug()) cout << "-E- calculateN1N1(...) Null pointers as arguments" << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return -1.;
    }

  int n1  = h_1->GetNbinsX();
  int n2  = h_2->GetNbinsX();
  int n3  = h_12->GetNbinsX();
  if (n3 != n1*n2 )
    {
    if (reportDebug()) cout << "-E- calculateN1N1_H1H1(...) Incompatible histo dimensions" << endl;
    if (reportDebug()) cout << "H1: " << h_1->GetName()  << " nBins:" << n1 << endl;
    if (reportDebug()) cout << "H2: " << h_2->GetName()  << " nBins:" << n2 << endl;
    if (reportDebug()) cout << "H3: " << h_12->GetName() << " nBins:" << n3  << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return -1.;
    }
  double v1,ev1,v2,ev2,v,ev, r1,r2;
  double sum = 0.;
  double norm = 0.;
  int i3 = 1;
  for (int i1=1; i1<=n1;++i1)
    {
    v1  = a1*h_1->GetBinContent(i1);
    ev1 = a1*h_1->GetBinError(i1);
    for (int i2=1;i2<=n2;++i2)
      {
      v2  = a2*h_2->GetBinContent(i2);
      ev2 = a2*h_2->GetBinError(i2);
      v = v1*v2;
      if (v>0)
        {
        r1 = ev1/v1; r2 = ev2/v2;
        ev = v*sqrt(r1*r1+r2*r2);
        }
      else
        ev = 0.;
      h_12->SetBinContent(i3,v);
      h_12->SetBinError(i3,ev);
      sum  += v;
      norm += 1.;
      ++i3;
      }
    }
  //return average across bins
  return sum/norm;
}



//Calculate External Product n1_1 x n1_2 and store into n1n1_12
double HistogramCollection::calculateN1N1_H1H1H2(const TH1 * h_1, const TH1 * h_2, TH2 * h_12, double a1, double a2)
{
  if (!h_1  || !h_2 || !h_12)
    {
    if (reportDebug()) cout << "-E- calculateN1N1_H1H1H2(...) Null pointers as arguments" << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return -1.;
    }

  int n1   = h_1->GetNbinsX();
  int n2   = h_2->GetNbinsX();
  int n3x  = h_12->GetNbinsX();
  int n3y  = h_12->GetNbinsY();

  if (n3x!=n1 || n3y!=n2)
    {
    if (reportDebug()) cout << "-E- calculateN1N1_H1H1H2(...) Incompatible histo dimensions" << endl;
    if (reportDebug()) cout << "H1: " << h_1->GetName()    << " nBins:" << n1 << endl;
    if (reportDebug()) cout << "H2: " << h_2->GetName()    << " nBins:" << n2 << endl;
    if (reportDebug()) cout << "H3: " << h_12->GetName()   << " nBins_x:" << n3x << " nBins_y:" << n3y << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return -1.;
    }
  double v1,ev1,v2,ev2,v,ev, r1,r2;
  double sum  = 0.;
  double norm = 0.;
  for (int i1=1; i1<=n1; ++i1)
    {
    v1  = a1*h_1->GetBinContent(i1);
    ev1 = a1*h_1->GetBinError(i1);
    for (int i2=1; i2<=n2; ++i2)
      {
      v2  = a2*h_2->GetBinContent(i2);
      ev2 = a2*h_2->GetBinError(i2);
      v = v1*v2;
      if (v>0)
        {
        r1 = ev1/v1;
        r2 = ev2/v2;
        ev = v*sqrt(r1*r1+r2*r2);
        }
      else
        ev = 0.;
      h_12->SetBinContent(i1,i2,v);
      h_12->SetBinError(i1,i2,ev);
      sum  += v;
      norm += 1.;
      }
    }
  //return average across bins
  return sum/norm;
}

//Calculate External Product n1_1 x n1_2 and store into n1n1_12
double HistogramCollection::calculateN1N1_H2H2H2(const TH2 *h_1, const TH2 * h_2, TH2 * h_12, double a1, double a2)
{
  if (!h_1  || !h_2 || !h_12)
    {
    if (reportDebug()) cout << "-E- calculateN1N1_H2H2H2(...) Null pointers as arguments" << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return -1.;
    }
  int n1x = h_1->GetNbinsX();
  int n1y = h_1->GetNbinsY();
  int n2x = h_2->GetNbinsX();
  int n2y = h_2->GetNbinsY();
  int n3x = h_12->GetNbinsX();
  int n3y = h_12->GetNbinsY();
  if (n3x!=(n1x*n1y) || n3y!=(n2x*n2y) )
    {
    if (reportDebug()) cout << "-E- calculateN1N1_H2H2H2(...) Incompatible histo dimensions" << endl;
    if (reportDebug()) cout << "H1: " << h_1->GetName()  << " nBins_x:" << n1x << " nBins_y:" << n1y << endl;
    if (reportDebug()) cout << "H2: " << h_2->GetName()  << " nBins_x:" << n2x << " nBins_y:" << n2y << endl;
    if (reportDebug()) cout << "H3: " << h_12->GetName() << " nBins_x:" << n3x << " nBins_y:" << n3y << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return -1.;
    }
  double v1,ev1,v2,ev2,v,ev, r1,r2;
  double sum  = 0.;
  double norm = 0.;
  for (int i1x=0;i1x<n1x;++i1x)
    {
    for (int i1y=0;i1y<n1y;++i1y)
      {
      v1  = a1*h_1->GetBinContent(i1x+1,i1y+1);
      ev1 = a1*h_1->GetBinError(i1x+1,i1y+1);
      for (int i2x=0;i2x<n2x;++i2x)
        {
        for (int i2y=0;i2y<n2y;++i2y)
          {
          v2  = a2*h_2->GetBinContent(i2x+1,i2y+1);
          ev2 = a2*h_2->GetBinError(i2x+1,i2y+1);
          v = v1*v2;
          if (v>0)
            {
            r1 = ev1/v1;
            r2 = ev2/v2;
            ev = v*sqrt(r1*r1+r2*r2);
            }
          else
            ev = 0.;
          int i3x = i1x*n1y+i1y+1;
          int i3y = i2x*n2y+i2y+1;
          h_12->SetBinContent(i3x,i3y,v);
          h_12->SetBinError(i3x,i3y,ev);
          sum += v;
          norm = 1.;
          }
        }
      }
    }
  //return average across bins
  return sum/norm;
}


//External Product N1N1N1
double HistogramCollection::calculateN1N1N1(const TH1 * h_1, const TH1 * h_2, const TH1 * h_3, TH1 * h_123)
{
  int n = h_1->GetNbinsX();
  double v1,ev1,v2,ev2,v3,ev3,v,ev,r1,r2,r3;
  double sum = 0.;
  int i4 = 1;
  for (int i1=1;i1<=n;++i1)
    {
    v1  = h_1->GetBinContent(i1);
    ev1 = h_1->GetBinError(i1);
    for (int i2=1;i2<=n;++i2)
      {
      v2  = h_2->GetBinContent(i2);
      ev2 = h_2->GetBinError(i2);
      for (int i3=1;i3<=n;++i3)
        {
        v3  = h_3->GetBinContent(i3);
        ev3 = h_3->GetBinError(i3);
        v = v1*v2*v3;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2;
          r3 = ev3/v3;
          ev = v*sqrt(r1*r1+r2*r2+r3*r3);
          }
        else
          ev = 0.;
        h_123->SetBinContent(i4,v);
        h_123->SetBinError(i4,ev);
        sum += v;
        ++i4;
        }
      }
    }
  //return average across bins
  return sum/n/n/n;
}


//External Product N1N1N1
double HistogramCollection::calculateN1N1N1(const TH1 * h_1, const TH1 * h_2, const TH1 * h_3, TH3 * h_123)
{
  int n = h_1->GetNbinsX();
  double v1,ev1,v2,ev2,v3,ev3,v,ev,r1,r2,r3;
  double sum = 0.;
  for (int i1=1;i1<=n;++i1)
    {
    v1  = h_1->GetBinContent(i1);
    ev1 = h_1->GetBinError(i1);
    for (int i2=1;i2<=n;++i2)
      {
      v2  = h_2->GetBinContent(i2);
      ev2 = h_2->GetBinError(i2);
      for (int i3=1;i3<=n;++i3)
        {
        v3  = h_3->GetBinContent(i3);
        ev3 = h_3->GetBinError(i3);
        v = v1*v2*v3;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2; r3 = ev3/v3;
          ev = v*sqrt(r1*r1+r2*r2+r3*r3);
          }
        else
          ev = 0.;
        h_123->SetBinContent(i1,i2,i3,v);
        h_123->SetBinError(i1,i2,i3,ev);
        sum += v;
        }
      }
    }
  //return average across bins
  return sum/n/n/n;
}


void HistogramCollection::calculateN2N1(const TH2 * s2, const TH1* s1, TH2 * target, int single)
{
  int n = s2->GetNbinsX();
  double v1,v2,v3,ev1,ev2,ev3,v,ev,r1,r2;
  int dPhi_xy;
  int dPhi_xz;
  int dPhi_yz;
  int dPhi12;
  int dPhi13;
  for (int i_x=0;i_x<n; ++i_x)
    {
    int i_x1 = i_x+1;
    for (int i_y=0;i_y<n; ++i_y)
      {
      int i_y1 = i_y+1;
      dPhi_xy = i_x-i_y; if (dPhi_xy<0) dPhi_xy += n; dPhi_xy+=1;
      for (int iz=0;iz<n; ++iz)
        {
        int iz1 = iz+1;
        dPhi_xz = i_x-iz; if (dPhi_xz<0) dPhi_xz += n; dPhi_xz+=1;
        dPhi_yz = i_y-iz; if (dPhi_yz<0) dPhi_yz += n; dPhi_yz+=1;
        v1   = s2->GetBinContent(i_x1,i_y1);
        ev1  = s2->GetBinError(i_x1,i_y1);
        v2   = s1->GetBinContent(iz1);
        ev2  = s1->GetBinError(iz1);
        switch (single)
          {
            case 3: dPhi12 = dPhi_xy; dPhi13 = dPhi_xz; break;
            case 2: dPhi13 = dPhi_xy; dPhi12 = dPhi_xz; break;
            case 1: dPhi12 = dPhi_xz; dPhi13 = dPhi_yz; break;
            default: return;//error("calculateN2N1()") << "invalid argument"<< endl; return;
          }
        v3   = target->GetBinContent(dPhi12,dPhi13);
        ev3  = target->GetBinError(dPhi12,dPhi13);
        v = v1*v2;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2;
          ev = v*v*(r1*r1+r2*r2);
          }
        else
          ev = 0.;
        v = v+v3;
        ev = sqrt(ev3*ev3+ev);
        target->SetBinContent(dPhi12,dPhi13,v);
        target->SetBinError(dPhi12,dPhi13,ev);
        }
      }
    }
}

//Fold n_xn by n

void HistogramCollection::calculateN2N1x(const TH2 * s2, const TH1* s1, TH3 * target, int single)
{
  int n = s2->GetNbinsX();
  double v1,v2,v3,ev1,ev2,ev3,v,ev,r1,r2;
  for (int i_x=0;i_x<n; ++i_x)
    {
    int i_x1 = i_x+1;
    for (int i_y=0;i_y<n; ++i_y)
      {
      int i_y1 = i_y+1;
      for (int iz=0;iz<n; ++iz)
        {
        int iz1 = iz+1;
        v1   = s2->GetBinContent(i_x1,i_y1);
        ev1  = s2->GetBinError(i_x1,i_y1);
        v2   = s1->GetBinContent(iz1);
        ev2  = s1->GetBinError(iz1);

        v = v1*v2;
        if (v>0)
          {
          r1 = ev1/v1;
          r2 = ev2/v2;
          ev = v*v*(r1*r1+r2*r2);
          }
        else
          ev = 0.;

        switch (single)
          {
            case 3:
            v3   = target->GetBinContent(i_x1,i_y1,iz1);
            ev3  = target->GetBinError(i_x1,i_y1,iz1);
            break;
            case 2:
            v3   = target->GetBinContent(i_x1,iz1,i_y1);
            ev3  = target->GetBinError(i_x1,iz1,i_y1);
            break;
            case 1:
            v3   = target->GetBinContent(iz1,i_x1,i_y1);
            ev3  = target->GetBinError(iz1,i_x1,i_y1);
            break;
            default:  //error("calculateN2N1") << "-E- calculateN2N1() invalid argument"<< endl;
            return;
          }

        v = v+v3;
        ev = sqrt(ev3*ev3+ev);
        switch (single)
          {
            case 3:
            target->SetBinContent(i_x1,i_y1,iz1,v);
            target->SetBinError(i_x1,i_y1,iz1,ev);
            break;
            case 2:
            target->SetBinContent(i_x1,iz1,i_y1,v);
            target->SetBinError(i_x1,iz1,i_y1,ev);
            break;
            case 1:
            target->SetBinContent(iz1,i_x1,i_y1,v);
            target->SetBinError(iz1,i_x1,i_y1,ev);
            break;
            default: if (reportDebug()) cout << "invalid argument"<< endl; return;
          }
        }
      }
    }
}

bool HistogramCollection::sameDimensions1D(const TH1* h1, const TH1* h2)
{
  int n1x = h1->GetNbinsX();
  int n2x = h2->GetNbinsX();
  if (n1x==n2x)
    {
    return true;
    }
  else
    {
    if (reportDebug()) cout << "-ERROR- Histograms titled " << h1->GetTitle() << " and " << h2->GetTitle() << " have incompatible dimensions" << endl;
    return false;
    }

}

bool HistogramCollection::sameDimensions2D(const TH2* h1, const TH2* h2)
{
  int n1x = h1->GetNbinsX();
  int n1y = h1->GetNbinsY();
  int n2x = h2->GetNbinsX();
  int n2y = h2->GetNbinsY();
  if (n1x==n2x && n1y==n2y)
    {
    return true;
    }
  else
    {
    if (reportDebug()) cout << "-ERROR- Histograms titled " << h1->GetTitle() << " and " << h2->GetTitle() << " have incompatible dimensions" << endl;
    return false;
    }

}

bool HistogramCollection::sameDimensions3D(const TH3* h1, const TH3* h2)
{
  int n1x = h1->GetNbinsX();
  int n1y = h1->GetNbinsY();
  int n1z = h1->GetNbinsY();
  int n2x = h2->GetNbinsX();
  int n2y = h2->GetNbinsY();
  int n2z = h2->GetNbinsY();
  if (n1x==n2x && n1y==n2y && n1z==n2z)
    {
    return true;
    }
  else
    {
    if (reportDebug()) cout << "-ERROR- Histograms titled " << h1->GetTitle() << " and " << h2->GetTitle() << " have incompatible dimensions" << endl;
    return false;
    }

}

void HistogramCollection::calculateDptDpt(const TH2 * spp, const TH2 * spn, const TH2 * snp, const TH2 * snn,
                                          const TH2 * avgp1, const TH2 * avgp2,  TH2 * s2dptdpt,  TH2 * dptdpt,
                                          bool ijNormalization, int nEta, int nPhi)
{
  if (!sameDimensions2D(spp,spn)) return;
  if (!sameDimensions2D(spp,snp)) return;
  if (!sameDimensions2D(spp,snn)) return;
  //int nx = spp->GetNbinsX();
  //int ny = spp->GetNbinsY();

  double v1,ev1,v2,ev2,v3,ev3,v4,ev4,v5,ev5,v6,ev6,p1,p2;
  int k, k1, k2;
  int nEtaPhi = nEta*nPhi;

  double sumPt1 = 0;
  double sumPt2 = 0;
  for (int iEta1=0;iEta1<nEta;++iEta1)
    {
    for (int iPhi1=0;iPhi1<nPhi;++iPhi1)
      {
      //k = iEta1*nPhi + iPhi1 + 1;
      sumPt1 += avgp1->GetBinContent(iEta1+1, iPhi1+1);
      sumPt2 += avgp2->GetBinContent(iEta1+1, iPhi1+1);
      }
    }
  p1 = sumPt1/double(nEtaPhi);
  p2 = sumPt2/double(nEtaPhi);


  for (int iEta1=0;iEta1<nEta;++iEta1)
    {
    //p1  = ptAvg1[iEta1];
    for (int iPhi1=0;iPhi1<nPhi;++iPhi1)
      {
      k1 = 1+ iEta1*nPhi + iPhi1;
      //c1 = avgp1->GetBinContent(k1+1)/p1;
      for (int iEta2=0;iEta2<nEta;++iEta2)
        {
        //p2  = ptAvg2[iEta2];
        for (int iPhi2=0;iPhi2<nPhi;++iPhi2)
          {
          k2 = 1+ iEta2*nPhi + iPhi2;
          v1  = spp->GetBinContent(k1,k2); ev1 = spp->GetBinError(k1,k2);
          v2  = spn->GetBinContent(k1,k2); ev2 = spn->GetBinError(k1,k2);
          v3  = snp->GetBinContent(k1,k2); ev3 = snp->GetBinError(k1,k2);
          v4  = snn->GetBinContent(k1,k2); ev4 = snn->GetBinError(k1,k2);
          //c2 = avgp2->GetBinContent(k2+1)/p2;

          if (v4!=0 ) // && ev4/v4<0.5)
            {
            if (ijNormalization)
              {
              v5  = 2*(v1 - v2*p2 - p1*v3 + p1*p2*v4);
              ev5 = v5*ev4/v4;
              v6  = v5/(2*v4);
              ev6 = v6*ev4/v4;
              }
            else
              {
              v5  = v1 - v2*p2 - p1*v3 + p1*p2*v4;
              ev5 = v5*ev4/v4;
              v6  = v5/v4;
              ev6 = v6*ev4/v4;
              }
            }
          else
            {
            v5 = v6 = ev5 = ev6 = 0;
            }
          //s2dptdpt->SetBinContent(k,v5); s2dptdpt->SetBinError(k,ev5);
          //dptdpt->SetBinContent(k,  v6); dptdpt->SetBinError(k,  ev6);
          //s2dptdpt->SetBinContent(k1, k2, v5); s2dptdpt->SetBinError(k1, k2, ev5);
          dptdpt->SetBinContent(  k1, k2, v6); dptdpt->SetBinError(  k1, k2, ev6);
          }
        }
      }
    }

}

/*  hc->calculateDptDpt(histosPair->h_ptpt_etaEta, histosPair->h_ptn_etaEta, histosPair->h_npt_etaEta,  histosPair->h_n2_etaEta,
 histosPart1->h_pt_eta,
 histosPart2->h_pt_eta,
 h_DptDpt_etaEta,
 ijNormalization, nBins_eta);
 */

// this shoudl work for eta vs eta.
void HistogramCollection::calculateDptDpt(const TH2 * spp, const TH2 * spn, const TH2 * snp, const TH2 * snn,
                                          const TH1 * avgp1, const TH1 * avgp2,
                                          TH2 * dptdpt,
                                          bool ijNormalization, int nBins)
{
  if (!sameDimensions2D(spp,spn) || !sameDimensions2D(spp,snp) || !sameDimensions2D(spp,snn))
    {
    cout << "-ERROR- HistogramCollection::calculateDptDpt() Arguments have incompatible dimensions. Skip." << endl;
    return;
    }
  double v1,ev1,v2,ev2,v3,ev3,v4,ev4,v5,ev5,v6,ev6,p1,p2;
  for (int i1=1;i1<=nBins;++i1)
    {
    p1 = avgp1->GetBinContent(i1);
    for (int i2=1;i2<=nBins;++i2)
      {
      p2 = avgp2->GetBinContent(i2);
      v1  = spp->GetBinContent(i1,i2); ev1 = spp->GetBinError(i1,i2);
      v2  = spn->GetBinContent(i1,i2); ev2 = spn->GetBinError(i1,i2);
      v3  = snp->GetBinContent(i1,i2); ev3 = snp->GetBinError(i1,i2);
      v4  = snn->GetBinContent(i1,i2); ev4 = snn->GetBinError(i1,i2);
      if (v4!=0) // && ev4/v4<0.5)
        {
        if (ijNormalization)
          {
          v5  = 2*(v1 - v2*p2 - p1*v3 + p1*p2*v4);
          ev5 = v5*ev4/v4;
          v6  = v5/(2*v4);
          ev6 = v6*ev4/v4;
          }
        else
          {
          v5  = v1 - v2*p2 - p1*v3 + p1*p2*v4;
          ev5 = v5*ev4/v4;
          v6  = v5/v4;
          ev6 = v6*ev4/v4;
          }
        }
      else
        {
        v5 = v6 = ev5 = ev6 = 0;
        }
      dptdpt->SetBinContent(i1,i2, v6);
      dptdpt->SetBinError(i1,i2, ev6);
      }
    }

}


void HistogramCollection::calculateSc(const TH1 * spp, const TH1 * n1n1, const TH1 * pt1pt1, TH1 * sean, bool ijNormalization)
{
  int n1 = spp->GetNbinsX();
  int n2 = n1n1->GetNbinsX();
  int n3 = pt1pt1->GetNbinsX();
  int n4 = sean->GetNbinsX();
  if (n1!=n2 || n1!=n3 || n1!=n4)
    {
    if (reportDebug()) cout << "-E- calculateSean(...) Arguments have different sizes." << endl;
    return;
    }
  double v1,ev1,v2,ev2,v3,ev3,v4,ev4;
  for (int i1=1;i1<=n1;++i1)
    {
    v1  = spp->GetBinContent(i1);    ev1 = spp->GetBinError(i1);
    v2  = n1n1->GetBinContent(i1);   ev2 = n1n1->GetBinError(i1);
    v3  = pt1pt1->GetBinContent(i1); ev3 = pt1pt1->GetBinError(i1);
    if (v2>0)
      {
      if (ijNormalization)
        v4  = 2*v1/v2 - v3;
      else
        v4  = v1/v2 - v3;
      ev4 = v1>0 ? v4*ev1/v1 : 0;
      }
    else
      {
      v4 = ev4 = 0;
      }
    sean->SetBinContent(i1,v4); sean->SetBinError(i1,ev4);
    }
}

void HistogramCollection::calculateG2_H2H2H2H2(const TH2 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * sean, double pt1avg, double pt2avg, bool ijNormalization, double a1, double a2)
{
  if (reportDebug()) cout << "calculateSean_H1H2H2H2(  ) Started" << endl;
  int n1x  = spp->GetNbinsX();
  int n1y  = spp->GetNbinsY();
  int n2x = n1n1->GetNbinsX();
  int n2y = n1n1->GetNbinsY();
  int n3x = pt1pt1->GetNbinsX();
  int n3y = pt1pt1->GetNbinsY();
  int n4x = sean->GetNbinsX();
  int n4y = sean->GetNbinsY();
  if (n1x!=n2x || n2x!=n3x || n2x!=n4x){
    if (reportError()) {
      cout << "-E- calculateSean(...) Arguments have different sizes." << endl;
      cout << n1x  << endl;
      cout << n1y  << endl;
      cout << n2x << endl;
      cout << n2y << endl;
      cout << n3x << endl;
      cout << n3y << endl;
      cout << n4x << endl;
      cout << n4y << endl;
      return;
    }
  }

  /* we prepare the transposed sean histo for properly handling the unlike tracks pair */
  TH2* sean_D = new TH2F(TString::Format("%s_Direct",sean->GetName()),sean->GetTitle(),
                                     sean->GetNbinsX(),sean->GetXaxis()->GetBinLowEdge(1),sean->GetXaxis()->GetBinUpEdge(sean->GetNbinsX()),
                                     sean->GetNbinsY(),sean->GetYaxis()->GetBinLowEdge(1),sean->GetYaxis()->GetBinUpEdge(sean->GetNbinsY()));
  TH2* sean_T = new TH2F(TString::Format("%s_Transposed",sean->GetName()),sean->GetTitle(),
                                     sean->GetNbinsY(),sean->GetYaxis()->GetBinLowEdge(1),sean->GetYaxis()->GetBinUpEdge(sean->GetNbinsY()),
                                     sean->GetNbinsX(),sean->GetXaxis()->GetBinLowEdge(1),sean->GetXaxis()->GetBinUpEdge(sean->GetNbinsX()));

  double v1,ev1,v2,ev2,v3,ev3,v4,ev4;
  for (int i1=1;i1<=n2x;++i1)
    {
    for (int i2=1;i2<=n2y;++i2)
      {
      v1  = a1*spp->GetBinContent(i1,i2);       ev1 = a1*spp->GetBinError(i1,i2);
      v2  = a2*n1n1->GetBinContent(i1,i2);      ev2 = a2*n1n1->GetBinError(i1,i2);
      v3  = pt1pt1->GetBinContent(i1,i2);       ev3 = pt1pt1->GetBinError(i1,i2);
      //v3  = 0.6*0.6;//pt1pt1->GetBinContent(15,2);    ev3 = pt1pt1->GetBinError(15,2);

      //if (true)
      //  {
      //cout << i1 << " " << i2 << " num:"<< v1 << " denom" << v2 << " ptpt" << v3 << endl;
      //  }
      if (v2>0)
        {
        if (ijNormalization)
          v4  = 2*v1/v2 - v3;
        else
          v4  = v1/v2 - v3;
        ev4 = v1>0 ? v4*ev1/v1 : 0;
        }
      else
        {
        v4 = ev4 = 0;
        }
      sean_D->SetBinContent(i1,i2,v4/pt1avg/pt2avg); sean_D->SetBinError(i1,i2,ev4/pt1avg/pt2avg);
      sean_T->SetBinContent(i2,i1,v4/pt1avg/pt2avg); sean_T->SetBinError(i2,i1,ev4/pt1avg/pt2avg);
      }
    }
  sean->Reset();
  sean->Add(sean_D,sean_T,0.5,0.5);
  delete sean_T;
  delete sean_D;
  if (reportDebug()) cout << "calculateSean_H1H2H2H2(  ) Done" << endl;

}

void checkSymmetry(const TH2 *h1, const TH2 *h2) {
  Warning("HistogramCollection::checkSymmetry","Checking symmetry");
  int nbinsdiff = 0;
  for (int ix = 0; ix < h1->GetNbinsX(); ++ix) {
    for (int iy = 0; iy < h1->GetNbinsY(); ++iy) {
      float diff = h1->GetBinContent(ix+1,iy+1) - h2->GetBinContent(iy+1,ix+1);
      if (diff > 1e-4)
        nbinsdiff++;
    }
  }
  if (nbinsdiff > 0) {
    Warning("HistogramCollection::checkSymmetry","Histograms differ in %d bins",nbinsdiff);
  }
}

void calculateG2_H2H2H2H2H2H2(const TH2 *, const TH2 *, const TH2 *, const TH2 *, const TH2 *, TH2 *, bool, double, double)
{
  Fatal("HistogramCollection::calculateG2_H2H2H2H2H2H2", "This routine should not be used");
  /* Calculate G2 considering 12 vs 21 track combinations */
  /* for same tracks this is not relevant and could be    */
  /* skipped but for unlike tracks, it definitelly is     */

  /* this was the check done 
  TH2 *spp_21 = transpose(spp_12);

  TH2 *h_g2_12 = (TH2*) sean->Clone(TString::Format("%s_12",sean->GetName()));
  TH2 *h_g2_21 = (TH2*) sean->Clone(TString::Format("%s_21",sean->GetName()));

  TH2 *h_n11n12 = (TH2*) spp_12->Clone("n11n12_forG2");
  TH2 *h_n12n11 = (TH2*) spp_12->Clone("n12n11_forG2");
  TH2 *h_pT1pT2 = (TH2*) spp_12->Clone("pT1pT2_forG2");
  TH2 *h_pT2pT1 = (TH2*) spp_12->Clone("pT2pT1_forG2");

  calculateN1N1_H2H2H2( n1_1, n1_2, h_n11n12, 1.0, 1.0);
  calculateN1N1_H2H2H2( n1_2, n1_1, h_n12n11, 1.0, 1.0);
  calculateN1N1_H2H2H2( pt_1, pt_2, h_pT1pT2, 1.0, 1.0);
  calculateN1N1_H2H2H2( pt_2, pt_1, h_pT2pT1, 1.0, 1.0);

  calculateG2_H2H2H2H2(spp_12,h_n11n12,h_pT1pT2,h_g2_12,ijNormalization,a1,a2);
  calculateG2_H2H2H2H2(spp_21,h_n12n11,h_pT2pT1,h_g2_21,ijNormalization,a1,a2);

  checkSymmetry(h_g2_12, h_g2_21);

  sean->Reset();
  sean->Add(h_g2_12,h_g2_21,0.5,0.5);
  delete h_pT2pT1;
  delete h_pT1pT2;
  delete h_n12n11;
  delete h_n11n12;
  delete h_g2_21;
  delete h_g2_12;
  delete spp_21;

  RESULT: the symmetry check revealed that the results g2_12 and g2_21 are symmetric
  so whe use this fact to improve performance directly in the original G2 routine*/
}


/* calculate the balance functions components associated to the current pair */
/* independent of R2, two components are alway computed, for LS they will match but don't for US */
/* now we correctly use the proper convolution n1_1*1 or 1*n1_2 which should give the same results as for R2 */
/* if when with R2 it is used in the same way  */
void HistogramCollection::calculateBf(const TH2 *n2, const TH2 *n1_1, const TH2 *n1_2, TH2 *bf_12, TH2 *bf_21)
{
  int nbinsx = n2->GetNbinsX();
  int nbinsx1 = n1_1->GetNbinsX();
  int nbinsx2 = n1_2->GetNbinsX();
  int nbinsy = n2->GetNbinsY();
  int nbinsy1 = n1_1->GetNbinsY();
  int nbinsy2 = n1_2->GetNbinsY();

  if ((nbinsx != nbinsx1*nbinsy1) or (nbinsy != nbinsx2*nbinsy2)) {
    ::Fatal("HistogramCollection::calculateBf","Incompatible dimensions");
  }

  bf_12->Reset();
  bf_21->Reset();

  for (int ix1 = 0; ix1 < nbinsx1; ++ix1)
  {
    for (int iy1 = 0; iy1 < nbinsy1; ++iy1)
    {
      int ix = ix1*nbinsy1+iy1;
      double n1_1v = n1_1->GetBinContent(ix1+1,iy1+1);
      double n1_1er = n1_1->GetBinError(ix1+1,iy1+1)/n1_1v;
      for (int ix2 = 0; ix2 < nbinsx2; ++ix2)
      {
        for (int iy2 = 0; iy2 < nbinsy2; ++iy2)
        {
          int iy = ix2*nbinsy2+iy2;
          double n1_2v = n1_2->GetBinContent(ix2+1,iy2+1);
          double n1_2er = n1_2->GetBinError(ix2+1,iy2+1)/n1_2v;

          double v = n2->GetBinContent(ix+1,iy+1);
          double ve = n2->GetBinError(ix+1,iy+1);

          double v12 = v/n1_1v;
          double v12e = v12*TMath::Sqrt(ve/v*ve/v+n1_1er*n1_1er);
          double v21 = v/n1_2v;
          double v21e = v21*TMath::Sqrt(ve/v*ve/v+n1_2er*n1_2er);

          bf_12->SetBinContent(ix+1,iy+1,v12);
          bf_12->SetBinError(ix+1,iy+1,v12e);
          bf_21->SetBinContent(iy+1,ix+1,v21);
          bf_21->SetBinError(iy+1,ix+1,v21e);
        }
      }
    }
  }
  bf_12->SetEntries(n2->GetEntries());
  bf_21->SetEntries(n2->GetEntries());
}

/* calculate the balance functions components associated to passed R2 */
/* two components are alway computed, for LS they will match but don't for US */
/* R2 is already reduced, projected and shifted so no symmetriztion is taken */
void HistogramCollection::calculateBfR2(const TH2 *r2, const TH2 *n1_1, const TH2 *n1_2, TH2 *bf_12, TH2 *bf_21)
{
  int nbinsx = r2->GetNbinsX();
  int nbinsy = r2->GetNbinsY();

  bf_12->Reset();
  bf_21->Reset();
  double n11_inte = 0.0;
  double n11_int = n1_1->IntegralAndError(1,n1_1->GetNbinsX(),1,n1_1->GetNbinsY(),n11_inte);
  double n11_inter = n11_inte/n11_int;
  /* we need to convert it to a density */
  n11_int /= kTWOPI*(n1_1->GetXaxis()->GetBinUpEdge(n1_1->GetNbinsX())-n1_1->GetXaxis()->GetBinLowEdge(1));

  double n12_inte = 0.0;
  double n12_int = n1_2->IntegralAndError(1,n1_2->GetNbinsX(),1,n1_2->GetNbinsY(),n12_inte);
  double n12_inter = n12_inte/n12_int;
  /* we need to convert it to a density */
  n12_int /= kTWOPI*(n1_2->GetXaxis()->GetBinUpEdge(n1_2->GetNbinsX())-n1_2->GetXaxis()->GetBinLowEdge(1));

  for (int ix = 0; ix < nbinsx; ++ix)
  {
    for (int iy = 0; iy < nbinsy; ++iy)
    {
      double v = r2->GetBinContent(ix+1,iy+1);
      double ve = r2->GetBinError(ix+1,iy+1);

      double v12 = v*n12_int;
      double v12e = v12*TMath::Sqrt(ve/v*ve/v+n12_inter*n12_inter);
      double v21 = v*n11_int;
      double v21e = v21*TMath::Sqrt(ve/v*ve/v+n11_inter*n11_inter);

      bf_12->SetBinContent(ix+1,iy+1,v12);
      bf_12->SetBinError(ix+1,iy+1,v12e);
      bf_21->SetBinContent(ix+1,iy+1,v21);
      bf_21->SetBinError(ix+1,iy+1,v21e);
    }
  }
  bf_12->SetEntries(r2->GetEntries());
  bf_21->SetEntries(r2->GetEntries());
}

void HistogramCollection::calculateSean_H1H2H2H2(const TH1 * spp, const TH2 * n1n1, const TH2 * pt1pt1, TH2 * sean, bool ijNormalization, double a1, double a2)
{

  if (reportDebug()) cout << "calculateSean_H1H2H2H2(  ) Started" << endl;
  int n1  = spp->GetNbinsX();
  int n2x = n1n1->GetNbinsX();
  int n2y = n1n1->GetNbinsY();
  int n3x = pt1pt1->GetNbinsX();
  int n3y = pt1pt1->GetNbinsY();
  int n4x = sean->GetNbinsX();
  int n4y = sean->GetNbinsY();
  if (n1!=n2x*n2y || n2x!=n3x || n2x!=n4x)
    {
    if (reportDebug()) cout << "-E- calculateSean(...) Arguments have different sizes." << endl;
    cout << n1  << endl;
    cout << n2x << endl;
    cout << n2y << endl;
    cout << n3x << endl;
    cout << n3y << endl;
    cout << n4x << endl;
    cout << n4y << endl;
    return;
    }
  int index;
  double v1,ev1,v2,ev2,v3,ev3,v4,ev4;
  index = 1;
  for (int i1=1;i1<=n2x;++i1)
    {
    for (int i2=1;i2<=n2y;++i2)
      {
      v1  = a1*spp->GetBinContent(index);       ev1 = a1*spp->GetBinError(index);
      v2  = a2*n1n1->GetBinContent(i1,i2);      ev2 = a2*n1n1->GetBinError(i1,i2);
      v3  = pt1pt1->GetBinContent(i1,i2);       ev3 = pt1pt1->GetBinError(i1,i2);
      //v3  = 0.6*0.6;//pt1pt1->GetBinContent(15,2);    ev3 = pt1pt1->GetBinError(15,2);

      //if (true)
      //  {
      //cout << i1 << " " << i2 << " num:"<< v1 << " denom" << v2 << " ptpt" << v3 << endl;
      //  }
      if (v2>0)
        {
        if (ijNormalization)
          v4  = 2*v1/v2 - v3;
        else
          v4  = v1/v2 - v3;
        ev4 = v1>0 ? v4*ev1/v1 : 0;
        }
      else
        {
        v4 = ev4 = 0;
        }
      sean->SetBinContent(i1,i2,v4); sean->SetBinError(i1,i2,ev4);
      index++;
      }
    }
  if (reportDebug()) cout << "calculateSean_H1H2H2H2(  ) Done" << endl;

}


int  HistogramCollection::calculateQ3DwPtPhiEta(double pt1, double phi1, double eta1,
                                                double pt2, double phi2, double eta2,
                                                double & Qlong, double & Qout, double & Qside)
{

  TLorentzVector p1;
  TLorentzVector p2;
  p1.SetPtEtaPhiM(pt1,eta1,phi1,0.13957);
  p2.SetPtEtaPhiM(pt2,eta2,phi2,0.13957);
  if (p1.P()>p1.E()) return 1;
  if (p2.P()>p2.E()) return 2;

  double px1   = p1.Px();
  double py1   = p1.Py();
  double pz1   = p1.Pz();
  double e1    = p1.E();

  double px2   = p2.Px();
  double py2   = p2.Py();
  double pz2   = p2.Pz();
  double e2    = p2.E();

  double pt,s,Mlong,roots;
  double ptot[4],q[4];
  const int g[4]={1,-1,-1,-1};
  int alpha;
  s=0.0;
  ptot[0] = e1  + e2;
  ptot[1] = px1 + px2;
  ptot[2] = py1 + py2;
  ptot[3] = pz1 + pz2;
  q[0] = e1  - e2;
  q[1] = px1 - px2;
  q[2] = py1 - py2;
  q[3] = pz1 - pz2;
  s = ptot[0]*ptot[0] - ptot[1]*ptot[1] - ptot[2]*ptot[2] - ptot[3]*ptot[3];
  pt=sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
  Mlong=sqrt(s+pt*pt);
  roots=sqrt(s);

  if ((p1+p2).P()>(p1+p2).E()) return 3;

  if (pt>0)
    {
    Qside = (ptot[1]*q[2]-ptot[2]*q[1])/pt;
    Qlong = (ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
    Qout  = (roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
    }
  else
    {
    Qlong = q[3];
    Qside = q[2];
    Qout  = q[1];
    }

  return 0;
}

int  HistogramCollection::calculateQ3DwPtPhiY(double pt1, double phi1, double y1,
                                              double pt2, double phi2, double y2,
                                              double & Qlong, double & Qout, double & Qside)
{

  TLorentzVector p1;
  TLorentzVector p2;
  double mPart = 0.13957;
  double mSq   = mPart*mPart;
  double mt1   = sqrt(mSq + pt1*pt1);
  double p1z   = mt1 * sinh(y1);
  double mt2   = sqrt(mSq + pt2*pt2);
  double p2z   = mt2 * sinh(y2);
  p1.SetXYZM(pt1*cos(phi1),pt1*sin(phi1),p1z,mPart);
  p2.SetXYZM(pt2*cos(phi2),pt2*sin(phi2),p2z,mPart);

  if (p1.P()>p1.E()) return 1;
  if (p2.P()>p2.E()) return 2;

  double px1   = p1.Px();
  double py1   = p1.Py();
  double pz1   = p1.Pz();
  double e1    = p1.E();

  double px2   = p2.Px();
  double py2   = p2.Py();
  double pz2   = p2.Pz();
  double e2    = p2.E();

  double pt,s,Mlong,roots;
  double ptot[4],q[4];
  const int g[4]={1,-1,-1,-1};
  int alpha;
  s=0.0;
  ptot[0] = e1  + e2;
  ptot[1] = px1 + px2;
  ptot[2] = py1 + py2;
  ptot[3] = pz1 + pz2;
  q[0] = e1  - e2;
  q[1] = px1 - px2;
  q[2] = py1 - py2;
  q[3] = pz1 - pz2;
  s = ptot[0]*ptot[0] - ptot[1]*ptot[1] - ptot[2]*ptot[2] - ptot[3]*ptot[3];
  pt=sqrt(ptot[1]*ptot[1]+ptot[2]*ptot[2]);
  Mlong=sqrt(s+pt*pt);
  roots=sqrt(s);

  if ((p1+p2).P()>(p1+p2).E()) return 3;

  if (pt>0)
    {
    Qside = (ptot[1]*q[2]-ptot[2]*q[1])/pt;
    Qlong = (ptot[0]*q[3]-ptot[3]*q[0])/Mlong;
    Qout  = (roots/Mlong)*(ptot[1]*q[1]+ptot[2]*q[2])/pt;
    }
  else
    {
    Qlong = q[3];
    Qside = q[2];
    Qout  = q[1];
    }

  return 0;
}


// Calculate n1n1 Q3D based on h2(y,pt) x h2(y,pt)
void HistogramCollection::calculateN1N1H2H2_Q3D_MCY(TH2 * n1_1, TH2 * n1_2, TH3 * n1n1_Q3D, double a1, double a2)
{
  if (!sameDimensions2D(n1_1,n1_2))
    {
    cout << "-ERROR- HistogramCollection::calculateN1N1_Q3D() abort calculation.";
    return;
    }
  double Qlong, Qout, Qside;
  double pt1, phi1, y1;
  double pt2, phi2, y2;
  int nIter = 100000000;

  double ss = 2.0/double(n1n1_Q3D->GetNbinsX());
  double avgN1 = n1_1->Integral();
  double avgN2 = n1_2->Integral();

  cout << "-INFO- calculateN1N1H2H2_Q3D_MC()  avgN1: " << avgN1 << endl;
  cout << "-INFO- calculateN1N1H2H2_Q3D_MC()  avgN2: " << avgN2 << endl;

  double scalingFactor = avgN1*avgN2/double(nIter);

  for (int k=0;k<nIter;k++)
    {
    n1_1->GetRandom2 (y1, pt1);
    n1_2->GetRandom2 (y2, pt2);
    phi1 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    phi2 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    bool status = calculateQ3DwPtPhiY(pt1, phi1, y1, pt2, phi2, y2, Qlong, Qout, Qside);
    if (status==0)
      {
      n1n1_Q3D->Fill(Qlong, Qside, Qout, 1.0);
      }
    }
  n1n1_Q3D->Scale(scalingFactor);
}

// Calculate n1n1 Q3D based on h2(eta,pt) x h2(eta,pt)
void HistogramCollection::calculateN1N1H2H2_Q3D_MCEta(TH2 * n1_1, TH2 * n1_2, TH3 * n1n1_Q3D, double a1, double a2)
{
  if (!sameDimensions2D(n1_1,n1_1))
    {
    cout << "-ERROR- HistogramCollection::calculateN1N1_Q3D() abort calculation.";
    return;
    }
  double Qlong, Qout, Qside;
  double pt1, phi1, eta1;
  double pt2, phi2, eta2;
  int nIter = 10000000;

  double avgN1 = n1_1->Integral();
  double avgN2 = n1_2->Integral();

  cout << "-INFO- calculateN1N1H2H2_Q3D_MC()  avgN1: " << avgN1 << endl;
  cout << "-INFO- calculateN1N1H2H2_Q3D_MC()  avgN2: " << avgN2 << endl;

  double scalingFactor = avgN1*avgN2/double(nIter);

  for (int k=0;k<nIter;k++)
    {
    n1_1->GetRandom2 (eta1, pt1);
    n1_2->GetRandom2 (eta2, pt2);
    phi1 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    phi2 = 2.0*TMath::Pi() * randomGenerator->Rndm();
    bool status = calculateQ3DwPtPhiEta(pt1, phi1, eta1, pt2, phi2, eta2, Qlong, Qout, Qside);
    if (status==0)
      {
      n1n1_Q3D->Fill(Qlong, Qside, Qout, 1.0);
      }
    }
  n1n1_Q3D->Scale(scalingFactor);
}

// Calculate n1n1 Q3D based on h2(eta,pt) x h2(eta,pt)
void HistogramCollection::calculateN1N1H2H2_Q3D(const TH2 * n1_1, const TH2 * n1_2, TH3 * n1n1_Q3D, double a1, double a2)
{
  if (!sameDimensions2D(n1_1,n1_1))
    {
    cout << "-ERROR- HistogramCollection::calculateN1N1_Q3D() abort calculation.";
    return;
    }
  int nx = n1_1->GetNbinsX();
  int ny = n1_1->GetNbinsY();
  int nz = n1_1->GetNbinsX();

  double v1,ev1,er1;
  double v2,ev2,er2;
  double v3,ev3;
  double Qlong, Qout, Qside;
  double pt1, phi1, eta1;
  double pt2, phi2, eta2;

  for (int i1x=1;i1x<=nx;++i1x)
    {
    for (int i1y=1;i1y<=ny;++i1y)
      {
      for (int i2x=1;i2x<=nx;++i2x)
        {
        for (int i2y=1;i2y<=ny;++i2y)
          {
          v1  = a1*n1_1->GetBinContent(i1x,i1y); //ev1 = a1*n1_1->GetBinError(i1x,i1y,i1z);
          v2  = a2*n1_2->GetBinContent(i2x,i2y); //ev2 = a2*n1_2->GetBinError(i2x,i2y,i2z);
          if (v1>0 && v2>0)
            {
            v3  = v1*v2;
            for (int k=0;k<100000;k++)
              {
              phi1 = 2.0*TMath::Pi() * randomGenerator->Rndm();
              phi2 = 2.0*TMath::Pi() * randomGenerator->Rndm();
              eta1 = n1_1->GetXaxis()->GetBinCenter(i1x);
              pt1  = n1_1->GetZaxis()->GetBinCenter(i1y);
              eta2 = n1_2->GetXaxis()->GetBinCenter(i2x);
              pt2  = n1_2->GetZaxis()->GetBinCenter(i2y);
              bool status = calculateQ3DwPtPhiEta(pt1, phi1, eta1, pt2, phi2, eta2, Qlong, Qout, Qside);
              if (status==0)
                {
                //cout << " i1x:" << i1x << " i1y: " << i1y << " i1z: " << i1z << " i2x:" << i1x << " i2y: " << i2y << " i2z: " << i2z << " v1: " << v1 << " v2: " << v2 << endl;
                //cout << " pt1:" << pt1 << " phi1: " << phi1 << " eta1: " << eta1 << " pt2:" << pt2 << " phi2: " << phi2 << " eta2: " << eta2 << " Qlong:" << Qlong << " Qout:" << Qout << " Qside:" << Qside << " v3:" << v3 << endl;
                n1n1_Q3D->Fill(Qlong, Qside, Qout, v3);
                }
              }
            }
          }
        }
      }
    }

}

void HistogramCollection::calculateN1N1H3H3_Q3D(const TH3 * n1_1, const TH3 * n1_2, TH3 * n1n1_Q3D, double a1, double a2)
{
  if (!sameDimensions3D(n1_1,n1_1))
    {
    cout << "-ERROR- HistogramCollection::calculateN1N1_Q3D() abort calculation.";
    return;
    }
  int nx = n1_1->GetNbinsX();
  int ny = n1_1->GetNbinsY();
  int nz = n1_1->GetNbinsX();

  double v1,ev1,er1;
  double v2,ev2,er2;
  double v3,ev3;
  double Qlong, Qout, Qside;
  double pt1, phi1, eta1;
  double pt2, phi2, eta2;

  for (int i1x=1;i1x<=nx;++i1x)
    {
    for (int i1y=1;i1y<=ny;++i1y)
      {
      for (int i1z=1;i1z<=nz;++i1z)
        {
        for (int i2x=1;i2x<=nx;++i2x)
          {
          for (int i2y=1;i2y<=ny;++i2y)
            {
            for (int i2z=1;i2z<=nz;++i2z)
              {
              v1  = a1*n1_1->GetBinContent(i1x,i1y,i1z); //ev1 = a1*n1_1->GetBinError(i1x,i1y,i1z);
              v2  = a2*n1_2->GetBinContent(i2x,i2y,i2z); //ev2 = a2*n1_2->GetBinError(i2x,i2y,i2z);
              if (v1>0 && v2>0)
                {
                //er1 = ev1/v1;
                //er2 = ev2/v2;
                //if (er1< 10.5 && er2<10.5)
                //  {
                v3  = v1*v2;
                //ev3 = v3*sqrt(er1*er1 + er2*er2);

                eta1 = n1_1->GetXaxis()->GetBinCenter(i1x);
                phi1 = n1_1->GetYaxis()->GetBinCenter(i1y);
                pt1  = n1_1->GetZaxis()->GetBinCenter(i1z);
                eta2 = n1_2->GetXaxis()->GetBinCenter(i2x);
                phi2 = n1_2->GetYaxis()->GetBinCenter(i2y);
                pt2  = n1_2->GetZaxis()->GetBinCenter(i2z);
                bool status = calculateQ3DwPtPhiEta(pt1, phi1, eta1, pt2, phi2, eta2, Qlong, Qout, Qside);
                if (status==0)
                  {
                  //cout << " i1x:" << i1x << " i1y: " << i1y << " i1z: " << i1z << " i2x:" << i1x << " i2y: " << i2y << " i2z: " << i2z << " v1: " << v1 << " v2: " << v2 << endl;
                  //cout << " pt1:" << pt1 << " phi1: " << phi1 << " eta1: " << eta1 << " pt2:" << pt2 << " phi2: " << phi2 << " eta2: " << eta2 << " Qlong:" << Qlong << " Qout:" << Qout << " Qside:" << Qside << " v3:" << v3 << endl;
                  n1n1_Q3D->Fill(Qlong, Qside, Qout, v3);
                  }
                // }
                }
              }
            }
          }
        }}}

}

void HistogramCollection::calculateR2_Q3D(const TH3 * n2_Q3D, const TH3 * n1n1_Q3D, TH3 * R2_Q3D, double a1, double a2)
{
  if (!sameDimensions3D(n2_Q3D,n1n1_Q3D) || !sameDimensions3D(n2_Q3D,R2_Q3D))
    {
    cout << "-ERROR- HistogramCollection::calculateR2_Q3D() abort calculation.";
    return;
    }
  int nx = n2_Q3D->GetNbinsX();
  int ny = n2_Q3D->GetNbinsY();
  int nz = n2_Q3D->GetNbinsX();
  double v1,ev1,er1;
  double v2,ev2,er2;
  double v3,ev3;
  for (int i1=1;i1<=nx;++i1)
    {
    for (int i2=1;i2<=ny;++i2)
      {
      for (int i3=1;i3<=nz;++i3)
        {
        v1  = a1*n2_Q3D->GetBinContent(i1,i2,i3); ev1 = a1*n2_Q3D->GetBinError(i1,i2,i3);
        v2  = a2*n1n1_Q3D->GetBinContent(i1,i2,i3); ev2 = a2*n1n1_Q3D->GetBinError(i1,i2,i3);
        if (v1>0 && v2>0)
          {
          er1 = 0;//ev1/v1;
          er2 = 0;//ev2/v2;
          if (er1< 0.5 && er2<0.5)
            {
            v3  = v1/v2 - 1;
            ev3 = v3*sqrt(er1*er1 + er2*er2);
            }
          else
            {
            v3 = 0.0;
            ev3 = 0.0;
            }
          }
        else
          {
          v3 = 0.0;
          ev3 = 0.0;
          }
        R2_Q3D->SetBinContent(i1,i2,i3,v3);
        R2_Q3D->SetBinError(i1,i2,i3,ev3);
        }
      }
    }
}

// Return the average bin content of the given 1D histogram
double HistogramCollection::avgValue(TH1 * h)
{
  int nx = h->GetNbinsX();
  if (nx<1)
    {
    cout << "-ERROR- HistogramCollection::avgValue(TH1 * h) Given histogram has <1 bins." << endl;
    return 0.0;
    }
  double nnx = nx;
  double sum = 0.0;
  for (int k=1; k<=nx; k++)
    {
    sum += h->GetBinContent(k);
    }
  return sum/nnx;
}


////////////////////////////////////////////////////////////////////////
// Histogram Helper Functions
////////////////////////////////////////////////////////////////////////


/*

 void unpack_vsXY_to_vsXVsY(const TH1 * source, TH2 * target);
 void correctMerging(TH1 * h, int nEta, int nPhi, bool reverse);
 void calculateR2_H1H1H1(const TH1 * h1, const TH1 * h2, TH1 * h3, bool ijNormalization, double a1=1., double a2=1.);
 void calculateR2_H2H2H2(const TH2 * h1, const TH2 * h2, TH2 * h3, bool ijNormalization, double a1=1., double a2=1.);
 void calculateR2_H1H2H2(const TH1 * h1, const TH2 * h2, TH2 * h3, bool ijNormalization, double a1=1., double a2=1.);
 void calculateR2VsM(const TProfile * h1, const TProfile * h2, const TProfile * h12, TH1 * r2VsM, TH1 * intR2, bool sameFilter);
 void calculateBinCorr(const TProfile * h1, const TProfile * h2, TH1 * intBinCorrVsM1, bool sameFilter);
 void calculateAveragePt(const TH1 * h1, const TH1 * h2, TH1 * h3);
 void calculateAveragePtH2(const TH2 * h1, const TH2 * h2, TH2 * h3);
 void calculateAveragePtH1H2(const TH1 * h1, const TH1 * h2, TH2 * h3);
 void calculateAveragePt(const TProfile * h1, const TProfile * h2, TH1 * h3);
 void reduce_n2xEtaPhi_n2DetaDphi(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins);
 void reduce_n2xEtaPhi_n2DetaDphi(const TH2 * source, TH2 * target,int nEtaBins,int nPhiBins);
 void reduce_n2xEtaPhi_n2EtaEta(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins);
 void symmetrizeDeltaEtaDeltaPhi(TH2 * h, bool ijNormalization);
 void symmetrizeXX(TH2 * h, bool ijNormalization);

 float * getFloatArray(int size, float v);
 double * getDoubleArray(int size, double v);
 /// Allocate an array of type double with n values
 /// Initialize the array to the given value
 void resetDoubleArray(int n, double * array, double value=0.);
 */

////////////////////////////////////////////////////////////////////////
void HistogramCollection::setHistogram(TH1 * h, double v, double ev)
{
  if (h)
    {
    //set all bins of the given histogram h to contain the same given
    // value (v) and error (ev).
    int n = h->GetNbinsX();
    for (int i1=1;i1<=n;++i1)
      {
      h->SetBinContent(i1,v);
      h->SetBinError(i1,ev);
      }
    }
  else
    {
    if (reportDebug()) cout << "-E- setHistogram(TH1 * h, ...) histogram does not exist." << endl;
    }
}

////////////////////////////////////////////////////////////////////////
void HistogramCollection::setHistogram(TH2 * h, double v, double ev)
{
  if (h)
    {
    //set all bins of the given histogram h to contain the same given
    // value (v) and error (ev).
    int n_x = h->GetNbinsX();
    int n_y = h->GetNbinsY();
    for (int i1=1;i1<=n_x;++i1)
      {
      for (int i2=1;i2<=n_y;++i2)
        {
        h->SetBinContent(i1,i2,v);
        h->SetBinError(i1,i2,ev);
        }
      }
    }
  else
    {
    if (reportDebug()) cout << "-E- setHistogram(TH1 * h, ...) histogram does not exist." << endl;
    }
}

TH1 * HistogramCollection::loadH1(TFile * inputFile,
                                  const TString & histoName,
                                  bool  scale,
                                  bool  save,
                                  bool  plot,
                                  bool  print,
                                  bool  sumw2)
{
  TH1* h = (TH1*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportDebug()) cout << "Could not load histogram "<< histoName << endl;
    }
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}

///Load the given 1D histogram (name) from the given TFile
///No test is //done to verify that the file is properly opened.

TH2 * HistogramCollection::loadH2(TFile * inputFile,
                                  const TString & histoName,
                                  bool  scale,
                                  bool  save,
                                  bool  plot,
                                  bool  print,
                                  bool  sumw2)
{
  TH2* h = (TH2*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportDebug()) cout << "Could not load histogram "<< histoName << endl;
    }
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}

///Load the given 3D histogram (name) from the given TFile
///No test is //done to verify that the file is properly opened.
TH3 * HistogramCollection::loadH3(TFile * inputFile,
                                  const TString & histoName,
                                  bool  scale,
                                  bool  save,
                                  bool  plot,
                                  bool  print,
                                  bool  sumw2)
{
  TH3* h = (TH3*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportDebug()) cout << "Could not load histogram "<< histoName << endl;
    }
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}

///Load the given 3D histogram (name) from the given TFile
///No test is //done to verify that the file is properly opened.
TProfile * HistogramCollection::loadProfile(TFile * inputFile,
                                            const TString & histoName,
                                            bool  scale,
                                            bool  save,
                                            bool  plot,
                                            bool  print,
                                            bool  sumw2)
{
  TProfile * h = (TProfile*) inputFile->Get(histoName);
  if (!h)
    {
    if (reportDebug()) cout << "Could not load histogram "<< histoName << endl;
    }
  addToList(h,scale,save,plot,print,sumw2);
  return h;
}

///Clone the given histogram, and set the clone's name to the given name.
/////throws a HistogramException if the histogram does not exist (null pointer).
TH1 * HistogramCollection::cloneH1(const TH1 * h1, const TString & histoName)
{
  if (!h1)
    {
    if (reportDebug()) cout << "Could not clone histogram "<< histoName << endl;
    }
  TH1 * h = (TH1*) h1->Clone();
  if (!h)
    {
    if (reportDebug()) cout << "Could not clone histogram "<< histoName << endl;
    }
  if (histoName.Length()>0) h->SetName(histoName);
  return h;
}


TH2 * HistogramCollection::cloneH2(const TH2 * h2, const TString & histoName)
{
  if (!h2)
    {
    if (reportDebug()) cout << "Could not clone histogram "<< histoName << endl;
    }
  TH2 * h = (TH2*) h2->Clone();
  if (!h)
    {
    if (reportDebug()) cout << "Could not clone histogram "<< histoName << endl;
    }
  if (histoName.Length()>0) h->SetName(histoName);
  return h;
}


TH3 * HistogramCollection::cloneH3(const TH3 * h3, const TString & histoName)
{
  if (!h3)
    {
    if (reportDebug()) cout << "Could not clone histogram "<< histoName << endl;
    }
  TH3 * h = (TH3*) h3->Clone();
  if (!h)
    {
    if (reportDebug()) cout << "Could not clone histogram "<< histoName << endl;
    }
  if (histoName.Length()>0) h->SetName(histoName);
  return h;
}

void HistogramCollection::findMaximum(TH1 * h, int xFirstBin, int xLastBin, int & xMaxValueBin, double & xMaxValue)
{
  int    n   = h->GetNbinsX();
  if (xFirstBin<1) xFirstBin = 1;
  if (xLastBin>n)  xLastBin = n;
  for (int i=xFirstBin;i<=xLastBin;++i)
    {
    double v = h->GetBinContent(i);
    if (v>xMaxValue)
      {
      xMaxValueBin = i;
      xMaxValue = v;
      }
    }
}

void HistogramCollection::findMinimum(TH1 * h, int xFirstBin, int xLastBin, int & xMinValueBin, double  & xMinValue)
{
  int    n   = h->GetNbinsX();
  if (xFirstBin<1) xFirstBin = 1;
  if (xLastBin>n)  xLastBin = n;
  for (int i=xFirstBin;i<=xLastBin;++i)
    {
    double v = h->GetBinContent(i);
    if (v>xMinValue)
      {
      xMinValueBin = i;
      xMinValue = v;
      }
    }
}

void HistogramCollection::scaleByBinWidth1D(TH1 * h, double scale)
{
  if (reportDebug()) cout << "scaleByBinWidth(TH1 * h, double scale) 1D histogram " << h->GetName()
    << " will be scaled by a common scale of " << scale
    << " and bin width " << h->GetBinWidth(1) << endl;
  int n_x = h->GetNbinsX();
  double width_x, v, ev;

  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    v = h->GetBinContent(i_x);
    ev = h->GetBinError(i_x);
    width_x = h->GetBinWidth(i_x);
    h->SetBinContent(i_x,v*scale/width_x);
    h->SetBinError(i_x,ev*scale/width_x);
    }
}

void HistogramCollection::scaleByBinWidth2D(TH2 * h, double scale)
{
  int n_x = h->GetNbinsX();
  int n_y = h->GetNbinsY();
  double width_x,width_y, v, ev, w;

  TAxis * xAxis = h->GetXaxis();
  TAxis * yAxis = h->GetYaxis();
  if (reportDebug()) cout << "scaleByBinWidth(TH2 * h, double scale) 2D histogram " << h->GetName()
    << " will be scaled by a common scale of " << scale
    << " bin width_x " << xAxis->GetBinWidth(1) << " bin width_y " << yAxis->GetBinWidth(1) << endl;
  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    width_x = xAxis->GetBinWidth(i_x);
    for (int i_y=1; i_y<=n_y; ++i_y)
      {
      v  = h->GetBinContent(i_x,i_y);
      ev = h->GetBinError(i_x,i_y);
      width_y = yAxis->GetBinWidth(i_y);
      w = scale/(width_x*width_y);
      h->SetBinContent(i_x,i_y,v*w);
      h->SetBinError(i_x,i_y,ev*w);
      }
    }
}

void HistogramCollection::scaleByBinWidth3D(TH3 * h, double scale)
{
  int n_x = h->GetNbinsX();
  int n_y = h->GetNbinsY();
  int n_z = h->GetNbinsZ();
  double width_x,width_y, width_z, v, ev, w;

  TAxis * xAxis = h->GetXaxis();
  TAxis * yAxis = h->GetYaxis();
  TAxis * zAxis = h->GetZaxis();
  //if (reportDebug())
  cout << "scaleByBinWidth(TH3 * h, double scale) 3D histogram " << h->GetName()
  << " will be scaled by a common scale of " << scale
  << " and for bin width_x " << xAxis->GetBinWidth(1) << " bin width_y " << yAxis->GetBinWidth(1) << " bin width_y " << zAxis->GetBinWidth(1) << endl;
  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    width_x = xAxis->GetBinWidth(i_x);
    for (int i_y=1; i_y<=n_y; ++i_y)
      {
      width_y = yAxis->GetBinWidth(i_y);
      for (int i_z=1; i_z<=n_z; ++i_z)
        {
        width_z = zAxis->GetBinWidth(i_z);
        v  = h->GetBinContent(i_x, i_y, i_z);
        ev = h->GetBinError(  i_x, i_y, i_z);
        w = scale/(width_x*width_y*width_z);
        h->SetBinContent(i_x, i_y, i_z,  v*w);
        h->SetBinError(  i_x, i_y, i_z,  ev*w);
        }
      }
    }
}

void HistogramCollection::scaleByBinWidth(TH1 * h, double scale)
{
  TClass * c = h->IsA();
  if (c == TProfile::Class())
    {
    //TProfile are not to be scaled
    if (reportDebug()) cout << "1D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
    }
  else if (c == TH1F::Class() || c == TH1D::Class())
    {
    if (reportDebug()) cout << "1D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale << endl;
    scaleByBinWidth1D(h, scale);
    }
  else if (c == TH2F::Class() || c == TH2D::Class())
    {
    if (reportDebug()) cout << "2D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale  << endl;
    TH2 * h2 = (TH2*) h;
    scaleByBinWidth2D(h2, scale);
    }
  else if (c == TH3F::Class() || c == TH3D::Class())
    {
    if (reportDebug()) cout << "3D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale  << endl;
    TH3 * h3 = (TH3*) h;
    scaleByBinWidth3D(h3, scale);
    }
  else if (c == TProfile2D::Class())
    {
    if (reportDebug()) cout << "2D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
    }
  else
    {
    cout << "-ERROR- Histogram named:" << h->GetName() << " of unrecognized type will NOT be scaled." << endl;
    }
}


void HistogramCollection::scaleAllHistoByBinWidth(double scale)
{
  TH1 * h;
  for (int i=0; i<nHistograms; ++i)
    {
    h = histograms[i];
    TClass * c = h->IsA();


    if (c == TProfile::Class())
      {
      //TProfile are not to be scaled
      if (reportDebug()) cout << "1D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
      }
    else if (c == TH1F::Class() || c == TH1D::Class())
      {
      if (reportDebug()) cout << "1D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale << endl;
      scaleByBinWidth1D(h, scale);
      }
    else if (c == TH2F::Class() || c == TH2D::Class())
      {
      if (reportDebug()) cout << "2D Histo named:" << h->GetName() << " will  be scaled by binwidth and given scale " << scale  << endl;
      TH2 * h2 = (TH2*) h;
      scaleByBinWidth2D(h2, scale);
      }
    else if (c == TProfile2D::Class())
      {
      if (reportDebug()) cout << "2D Profile named:" << h->GetName() << " will NOT be scaled." << endl;
      }
    }
}


void HistogramCollection::sumw2All()
{
  TH1 * h;
  for (int i=0; i<nHistograms; ++i)
    {
    h = histograms[i];
    TClass * c = h->IsA();
    if (c==TProfile::Class() )
      {
      if (reportDebug()) cout << "Profile named:" << h->GetName() << " will not be sumw2" << endl;
      }
    else
      {
      if (reportDebug()) cout << "Histo named:" << h->GetName() << " will  be sumw2" << endl;
      if (h->GetEntries()>0)
        h->Sumw2();
      }
    }
}



void HistogramCollection::unpack_vsXY_to_vsXVsY(const TH1 * source, TH2 * target)
{
  // Unpack 1D source histogram into a 2D histogram
  int k = 1;
  double v, ev;
  int n  = source->GetNbinsX();
  int n_x = target->GetNbinsX();
  int n_y = target->GetNbinsY();
  if (n!=n_x*n_y)
    {
    if (reportDebug()) cout << "-E- unpack_vsXY_to_vsXVsY(...)  Incompatible histogram dimensions" << endl;
    if (reportDebug()) cout << "    source  n:" << n << endl;
    if (reportDebug()) cout << "    target n_x:" << n_x << endl;
    if (reportDebug()) cout << "    target n_y:" << n_y << endl;
    return;
    }

  for (int i_x=1; i_x<=n_x; ++i_x)
    {
    for (int i_y=1; i_y<=n_y; ++i_y)
      {
      v  = source->GetBinContent(k);
      ev = source->GetBinError(k);
      target->SetBinContent(i_x,i_y,v);
      target->SetBinError(i_x,i_y,v);
      k++;
      }
    }
}

void HistogramCollection::correctMerging(TH1 * h, int nEta, int nPhi, bool reverse)
{
  double v, ev;
  int iEta, iPhi, iDphi, iPhiA, iPhiB;
  int ij1, ij2;
  int nEtaPhi = nEta*nPhi;
  for (iEta=0; iEta<nEta; iEta++)
    {
    for (iPhi=0; iPhi<nPhi; iPhi++)
      {
      for (iDphi=1; iDphi<7; iDphi++)
        {
        iPhiA = iPhi+iDphi;  if (iPhiA>=nPhi) iPhiA -= nPhi; // Dphi12 = iPhi-iPhiA < 0
        iPhiB = iPhi-iDphi;  if (iPhiB< 0) iPhiB += nPhi; // Dphi12 = iPhi-iPhiB > 0
                                                          //Deta==0
        ij1 = nEtaPhi*(iEta*nPhi+iPhi) + iEta*nPhi+iPhiA;
        ij2 = nEtaPhi*(iEta*nPhi+iPhi) + iEta*nPhi+iPhiB;
        if (reverse)
          {
          v  = h->GetBinContent(ij1);
          ev = h->GetBinError(ij1);
          h->SetBinContent(ij2,v);
          h->SetBinError(ij2,ev);
          }
        else
          {
          v  = h->GetBinContent(ij2);
          ev = h->GetBinError(ij2);
          h->SetBinContent(ij1,v);
          h->SetBinError(ij1,ev);
          }
        if (iEta>0)   //Deta==1
          {
          ij1 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta-1)*nPhi+iPhiA;
          ij2 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta-1)*nPhi+iPhiB;
          if (reverse)
            {
            v  = h->GetBinContent(ij1);
            ev = h->GetBinError(ij1);
            h->SetBinContent(ij2,v);
            h->SetBinError(ij2,ev);
            }
          else
            {
            v  = h->GetBinContent(ij2);
            ev = h->GetBinError(ij2);
            h->SetBinContent(ij1,v);
            h->SetBinError(ij1,ev);
            }

          }
        if (iEta<nEta)   //Deta==1
          {
          ij1 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta+1)*nPhi+iPhiA;
          ij2 = nEtaPhi*(iEta*nPhi+iPhi) + (iEta+1)*nPhi+iPhiB;
          if (reverse)
            {
            v  = h->GetBinContent(ij1);
            ev = h->GetBinError(ij1);
            h->SetBinContent(ij2,v);
            h->SetBinError(ij2,ev);
            }
          else
            {
            v  = h->GetBinContent(ij2);
            ev = h->GetBinError(ij2);
            h->SetBinContent(ij1,v);
            h->SetBinError(ij1,ev);
            }

          }

        }
      }
    }
}

///Calculate R2 = binCorrection*N2/N1/N1 - 1
void HistogramCollection::calculateR2_H1H1H1(const TH1 * n2_12, const TH1 * n1n1_12, TH1 * r2_12, bool ijNormalization, double a1, double a2)
{
  if (!n2_12  || !n1n1_12 || !r2_12)
    {
    if (reportDebug()) cout << "-E- calculateR2_H1H1H1(...) Null pointers as arguments" << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return;
    }
  int n2_12_n_x    = n2_12->GetNbinsX();
  int n1n1_12_n_x  = n1n1_12->GetNbinsX();
  int r2_12_n_x    = r2_12->GetNbinsX();
  if (n2_12_n_x!=n1n1_12_n_x || n2_12_n_x!=r2_12_n_x )
    {
    if (reportDebug()) cout << "-E- calculateR2_H1H1H1(...) Incompatible histo dimensions" << endl;
    if (reportDebug()) cout << "H1: " << n2_12->GetName()   << " n_x:" << n2_12_n_x   << endl;
    if (reportDebug()) cout << "H2: " << n1n1_12->GetName() << " n_x:" << n1n1_12_n_x << endl;
    if (reportDebug()) cout << "H3: " << r2_12->GetName()   << " n_x:" << r2_12_n_x   << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return;
    }

  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i1=1;i1<=n2_12_n_x;++i1)
    {
    v1  = a1*n2_12->GetBinContent(i1);    ev1 = a1*n2_12->GetBinError(i1);
    v2  = a2*n1n1_12->GetBinContent(i1);  ev2 = a2*n1n1_12->GetBinError(i1);
    if (v1>0 && v2>0 && ev1/v1<0.5  && ev2/v2<0.5 )
      {
      if (ijNormalization) //account for the fact only half the pairs were counted
        {
        v   = 2*v1/v2;
        if (i1<20) if (reportDebug()) cout << "normalized v:" << v << endl;
        }
      else // all pairs counted - no need to multiply by 2
        {
        v   = v1/v2;
        if (i1<20) if (reportDebug()) cout << "NOT normalized v:" << v << endl;
        }
      re1 = ev1/v1;
      re2 = ev2/v2;
      ev  = v*sqrt(re1*re1+re2*re2);
      v   -= 1.;
      //if (i1<20) if (reportDebug()) cout << "-1:" << v << endl;

      }
    else
      {
      v = 0.;
      ev = 0;
      }
    r2_12->SetBinContent(i1,v); r2_12->SetBinError(i1,ev);
    }
}

//Calculate R2 = N2/N1/N1 - 1
void HistogramCollection::calculateR2_H2H2H2(const TH2 * n2_12, const TH2 * n1n1_12, TH2 * r2_12, bool ijNormalization, double a1, double a2)
{
  if (!n2_12  || !n1n1_12 || !r2_12)
    {
    if (reportDebug()) cout << "-E- calculateR2_H2H2H2(...) Null pointers as arguments" << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return;
    }
  int n2_12_n_x    = n2_12->GetNbinsX();
  int n2_12_n_y    = n2_12->GetNbinsY();
  int n1n1_12_n_x  = n1n1_12->GetNbinsX();
  int n1n1_12_n_y  = n1n1_12->GetNbinsY();
  int r2_12_n_x    = r2_12->GetNbinsX();
  int r2_12_n_y    = r2_12->GetNbinsY();
  if (n2_12_n_x!=n1n1_12_n_x || n2_12_n_x!=r2_12_n_x || n2_12_n_y!=n1n1_12_n_y || n2_12_n_y!=r2_12_n_y)
    {
    if (reportDebug()) cout << "-E- calculateR2_H2H2H2(...) Incompatible histo dimensions" << endl;
    if (reportDebug()) cout << "H1: " << n2_12->GetName()   << " n_x:" << n2_12_n_x   << " n_y:" << n2_12_n_y   << endl;
    if (reportDebug()) cout << "H2: " << n1n1_12->GetName() << " n_x:" << n1n1_12_n_x << " n_y:" << n1n1_12_n_y << endl;
    if (reportDebug()) cout << "H3: " << r2_12->GetName()   << " n_x:" << r2_12_n_x   << " n_y:" << r2_12_n_y   << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return;
    }

  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i_x=1;i_x<=n2_12_n_x;++i_x)
    {
    for (int i_y=1;i_y<=n2_12_n_y;++i_y)
      {
      v1  = a1*n2_12->GetBinContent(i_x,i_y);    ev1 = a1*n2_12->GetBinError(i_x,i_y);
      v2  = a2*n1n1_12->GetBinContent(i_x,i_y);  ev2 = a2*n1n1_12->GetBinError(i_x,i_y);
      if (v1>0 && v2>0) //   && ev1/v1<0.5  && ev2/v2<0.5)
        {
        if (ijNormalization) //account for the fact only half the pairs were counted
          v   = 2*v1/v2;
        else // all pairs counted - no need to multiply by 2
          v   = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev  = v*sqrt(re1*re1+re2*re2);
        v   -= 1.;
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      r2_12->SetBinContent(i_x,i_y,v); r2_12->SetBinError(i_x,i_y,ev);
      }
    }
}

//Calculate R2 = N2/N1/N1 - 1
void HistogramCollection::calculateR2_H1H2H2(const TH1 * n2_12, const TH2 * n1n1_12, TH2 * r2_12, bool ijNormalization, double a1, double a2)
{
  if (!n2_12  || !n1n1_12 || !r2_12)
    {
    if (reportDebug()) cout << "-E- calculateR2(...) Null pointers as arguments" << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return;
    }
  int n2_12_n_x    = n2_12->GetNbinsX();
  int n1n1_12_n_x  = n1n1_12->GetNbinsX();
  int n1n1_12_n_y  = n1n1_12->GetNbinsY();
  int r2_12_n_x    = r2_12->GetNbinsX();
  int r2_12_n_y    = r2_12->GetNbinsY();
  if (n2_12_n_x!=(n1n1_12_n_x*n1n1_12_n_y) || n1n1_12_n_x!=r2_12_n_x || n1n1_12_n_y!=r2_12_n_y )
    {
    if (reportDebug()) cout << "-E- calculateR2_H1H2H2(...) Incompatible histo dimensions" << endl;
    if (reportDebug()) cout << "H1: " << n2_12->GetName()   << " n_x:" << n2_12_n_x   << endl;
    if (reportDebug()) cout << "H2: " << n1n1_12->GetName() << " n_x:" << n1n1_12_n_x << " n_y:" << n1n1_12_n_y << endl;
    if (reportDebug()) cout << "H3: " << r2_12->GetName()   << " n_x:" << r2_12_n_x   << " n_y:" << r2_12_n_y   << endl;
    if (reportDebug()) cout << "ABORT!!!!!" << endl;
    return;
    }

  double v1,ev1,v2,ev2,v,ev, re1,re2;
  int i = 1;
  for (int i_x=1;i_x<=n1n1_12_n_x;++i_x)
    {
    for (int i_y=1;i_y<=n1n1_12_n_y;++i_y)
      {
      v1  = a1*n2_12->GetBinContent(i);        ev1 = a1*n2_12->GetBinError(i);
      v2  = a2*n1n1_12->GetBinContent(i_x,i_y);  ev2 = a2*n1n1_12->GetBinError(i_x,i_y);
      if (v1>0 && v2>0  && ev1/v1<0.5  && ev2/v2<0.5)
        {
        if (ijNormalization) //account for the fact only half the pairs were counted
          v   = 2*v1/v2;
        else // all pairs counted - no need to multiply by 2
          v   = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev  = v*sqrt(re1*re1+re2*re2);
        v   -= 1.;
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      r2_12->SetBinContent(i_x,i_y,v); r2_12->SetBinError(i_x,i_y,ev);
      i++;
      }
    }
}

void HistogramCollection::calculateR2VsM(const TProfile * h1, const TProfile * h2, const TProfile * h12, TH1 * r2VsM, TH1 * intR2, bool sameFilter)
{
  if (!h1)
    {
    if (reportDebug()) cout << "-E- calculateR2VsM( ...) h1 is a null pointer." << endl; return;
    }
  if (!h2)
    {
    if (reportDebug()) cout << "-E- calculateR2VsM( ...) h2 is a null pointer." << endl; return;
    }
  if (!h12)
    {
    if (reportDebug()) cout << "-E- calculateR2VsM( ...) h12 is a null pointer." << endl; return;
    }
  if (!r2VsM)
    {
    if (reportDebug()) cout << "-E- calculateR2VsM( ...) r2VsM is a null pointer." << endl; return;
    }
  if (!intR2)
    {
    if (reportDebug()) cout << "-E- calculateR2VsM( ...) intR2 is a null pointer." << endl; return;
    }

  int n1, n2, n3, n4, n5;
  n1   = h1->GetNbinsX();
  if (sameFilter) n2 = n1; else n2   = h2->GetNbinsX();
  n3   = h12->GetNbinsX();
  n4   = r2VsM->GetNbinsX();
  n5   = intR2->GetNbinsX();
  if (n1!=n2 || n1!=n3 || n1!=n4 || n5!=1)
    {
    if (reportDebug()) cout << "-E- calculateR2VsM(...) Incompatible histogram dimensions" << endl;
    return;
    }
  double n, nSum, vSum, v1,ev1,v2,ev2,v12,ev12, r, er;
  nSum = vSum = 0;
  for (int i=1;i<=n1;++i)
    {
    n    = h1->GetBinEntries(i);
    v1   = h1->GetBinContent(i);  ev1  = h1->GetBinError(i);
    if (sameFilter)
      {
      v2   = v1;  ev2  = ev1;
      }
    else
      {
      v2   = h2->GetBinContent(i);  ev2  = h2->GetBinError(i);
      }
    v12  = h12->GetBinContent(i); ev12 = h12->GetBinError(i);
    if (v1>0 && v2>0 && v12>0)
      {
      r = v12/(v1*v2);
      er = r*ev12/v12; // under estimate errors.
      nSum += n;
      vSum += n*r;
      }
    else
      {
      r  = 1;
      er = 0;
      }
    //if (reportDebug()) cout << "calculateR2VsM: :" << i << " v1:" << v1 << " v2:" << v2 << " v12:" << v12 << " r:" << r << " nSum:" << nSum << " vSum:" << vSum << endl;
    r2VsM->SetBinContent(i,r);
    r2VsM->SetBinError(i,er);
    }
  if (nSum>0)
    {
    intR2->SetBinContent(1,vSum/nSum);
    intR2->SetBinError(1,0.);
    }
  else
    {
    intR2->SetBinContent(1,0.);
    intR2->SetBinError(1,0.);
    }
}

void HistogramCollection::calculateBinCorr(const TProfile * h1, const TProfile * h2, TH1 * intBinCorrVsM1, bool sameFilter)
{
  int n1, n2, n3;
  n1   = h1->GetNbinsX();
  if (sameFilter) n2 = n1; else n2   = h2->GetNbinsX();
  n3   = intBinCorrVsM1->GetNbinsX();
  if (n1!=n2 || n3!=1)
    {
    if (reportDebug()) cout << "-E- calculateBinCorr(...) Incompatible histogram dimensions" << endl;
    return;
    }
  double n, nSum, sum1, sum2, sum12, v1,v2;
  sum1 = sum2 = sum12 = nSum = 0;
  for (int i=1;i<=n1;++i)
    {
    n    = h1->GetBinEntries(i);
    v1   = h1->GetBinContent(i);
    if (sameFilter) v2 = v1; else v2 = h2->GetBinContent(i);
    //if (reportDebug()) cout << "calculateBinCorr: i:" << i << " v1:" << v1 << " v2:" << v2 << " n:" << n << endl;
    if (n>0)
      {
      nSum += n;
      sum1 += n*v1;
      sum2 += n*v2;
      sum12 += n*v1*v2;
      }
    }
  if (nSum>0 && sum1>0 && sum2>0)
    {
    sum1 /= nSum;
    sum2 /= nSum;
    sum12 /= nSum;
    intBinCorrVsM1->SetBinContent(1,sum12/(sum1*sum2));
    intBinCorrVsM1->SetBinError(1,0.);
    }
  else
    {
    intBinCorrVsM1->SetBinContent(1,1.); //no stat - so set correction factor to unity
    intBinCorrVsM1->SetBinError(1,0.);
    }

}


void HistogramCollection::calculateAveragePt(const TH1 * h1, const TH1 * h2, TH1 * h3)
{
  //Calculate average pt by dividing sPt contained in h1 by n1 contained in h2. Store in h3.

  int n1   = h1->GetNbinsX();
  int n2   = h2->GetNbinsX();
  int n3   = h3->GetNbinsX();
  if (n1!=n2 || n2!=n3)
    {
    if (reportDebug()) cout << "-E- calculateAveragePt(...) Incompatible histogram dimensions" << endl;
    if (reportDebug()) cout << "Histo:" << h1->GetName() << " n_x:" << n1 << endl;
    if (reportDebug()) cout << "Histo:" << h2->GetName() << " n_x:" << n2 << endl;
    if (reportDebug()) cout << "Histo:" << h3->GetName() << " n_x:" << n3 << endl;
    return;
    }
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i1=1;i1<=n1;++i1)
    {
    v1  = h1->GetBinContent(i1);  ev1 = h1->GetBinError(i1);
    v2  = h2->GetBinContent(i1);  ev2 = h2->GetBinError(i1);
    if (v1>0 && v2>0)
      {
      v = v1/v2;
      re1 = ev1/v1;
      re2 = ev2/v2;
      ev = v*sqrt(re1*re1+re2*re2);
      }
    else
      {
      v = 0.;
      ev = 0;
      }
    h3->SetBinContent(i1,v); h3->SetBinError(i1,ev);
    }
}

void HistogramCollection::calculateAveragePtH2(const TH2 * h1, const TH2 * h2, TH2 * h3)
{
  //Calculate average pt by dividing sPt contained in h1 by n1 contained in h2. Store in h3.

  int n1x   = h1->GetNbinsX();int n1y   = h1->GetNbinsY();
  int n2x   = h2->GetNbinsX();int n2y   = h2->GetNbinsY();
  int n3x   = h3->GetNbinsX();int n3y   = h3->GetNbinsY();
  if (n1x!=n2x || n2x!=n3x || n1y!=n2y || n1y!=n3y)
    {
    if (reportDebug()) cout << "-E- calculateAveragePtH2(...) Incompatible histogram dimensions" << endl;
    if (reportDebug()) cout << "Histo:" << h1->GetName() << " n_x:" << n1x << endl;
    if (reportDebug()) cout << "Histo:" << h2->GetName() << " n_x:" << n2x << endl;
    if (reportDebug()) cout << "Histo:" << h3->GetName() << " n_x:" << n3x << endl;
    return;
    }
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i_x=1;i_x<=n1x;++i_x)
    {
    for (int i_y=1;i_y<=n1y;++i_y)
      {
      v1  = h1->GetBinContent(i_x,i_y);  ev1 = h1->GetBinError(i_x,i_y);
      v2  = h2->GetBinContent(i_x,i_y);  ev2 = h2->GetBinError(i_x,i_y);
      if (v1>0 && v2>0)
        {
        v = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev = v*sqrt(re1*re1+re2*re2);
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      h3->SetBinContent(i_x,i_y,v); h3->SetBinError(i_x,i_y,ev);
      }
    }
}

void HistogramCollection::calculateAveragePtH1H2(const TH1 * h1, const TH1 * h2, TH2 * h3)
{
  //Calculate average pt by dividing sPt contained in h1 by n1 contained in h2. Store in h3.

  int n1   = h1->GetNbinsX();
  int n2   = h2->GetNbinsX();
  int n3x  = h3->GetNbinsX();
  int n3y  = h3->GetNbinsY();
  int n3   = n3x*n3y;
  if (n1!=n2 || n2!=n3)
    {
    if (reportDebug()) cout << "-E- calculateAveragePtH1H2(...) Incompatible histogram dimensions" << endl;
    if (reportDebug()) cout << "Histo:" << h1->GetName() << " n_x:" << n1 << endl;
    if (reportDebug()) cout << "Histo:" << h2->GetName() << " n_x:" << n2 << endl;
    if (reportDebug()) cout << "Histo:" << h3->GetName() << " n_x:" << n3x << endl;
    if (reportDebug()) cout << "Histo:" << h3->GetName() << " n_y:" << n3y << endl;
    return;
    }
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  int i = 1;
  for (int i_x=1;i_x<=n3x;++i_x)
    {
    for (int i_y=1;i_y<=n3y;++i_y)
      {
      v1  = h1->GetBinContent(i);  ev1 = h1->GetBinError(i);
      v2  = h2->GetBinContent(i);  ev2 = h2->GetBinError(i);
      if (v1>0 && v2>0)
        {
        v = v1/v2;
        re1 = ev1/v1;
        re2 = ev2/v2;
        ev = v*sqrt(re1*re1+re2*re2);
        }
      else
        {
        v = 0.;
        ev = 0;
        }
      h3->SetBinContent(i_x,i_y,v); h3->SetBinError(i_x,i_y,ev);
      i++;
      }
    }
}

void HistogramCollection::calculateAveragePt(const TProfile * h1, const TProfile * h2, TH1 * h3)
{
  //Calculate average pt by dividing sPt contained in h1 by n1 contained in h2. Store in h3.

  int n1   = h1->GetNbinsX();
  int n2   = h2->GetNbinsX();
  int n3   = h3->GetNbinsX();
  if (n1!=n2 || n2!=n3)
    {
    if (reportDebug()) cout << "-E- calculateAveragePt(...) Incompatible histogram dimensions" << endl;
    return;
    }
  double v1,ev1,v2,ev2,v,ev, re1,re2;
  for (int i1=1;i1<=n1;++i1)
    {
    v1  = h1->GetBinContent(i1);  ev1 = h1->GetBinError(i1);
    v2  = h2->GetBinContent(i1);  ev2 = h2->GetBinError(i1);
    if (v2>0)
      {
      v = v1/v2;
      re1 = ev1/v1;
      re2 = ev2/v2;
      ev = v*sqrt(re1*re1+re2*re2);
      }
    else
      {
      v = 0.;
      ev = 0;
      }
    h3->SetBinContent(i1,v);  h3->SetBinError(i1,ev);
    }
}

void HistogramCollection::symmetrize3D(TH3* h)
{
  int n = h->GetNbinsX();
  int nc = n/2;
  int nf = nc + 1;
  cout << "-INFO- symmetrize3D(TH3* h) n  = " << n << endl;
  cout << "-INFO- symmetrize3D(TH3* h) nc = " << nc << endl;
  cout << "-INFO- symmetrize3D(TH3* h) nf = " << nf << endl;


  double v1, v2, v3, v4, v5, v6, v7, v8;
  double ev1, ev2, ev3, ev4, ev5, ev6, ev7, ev8;
  double v, ev;

  for (int i1=1; i1<=nc; i1++)
    {
    for (int i2=1; i2<=nc; i2++)
      {
      for (int i3=1; i3<=nc; i3++)
        {
        v1 = h->GetBinContent(nc+i1,nc+i2,nc+i3);  ev1 = h->GetBinError(nc+i1,nc+i2,nc+i3);
        v2 = h->GetBinContent(nc+i1,nc+i2,nf-i3);  ev2 = h->GetBinError(nc+i1,nc+i2,nf-i3);
        v3 = h->GetBinContent(nc+i1,nf-i2,nc+i3);  ev3 = h->GetBinError(nc+i1,nf-i2,nc+i3);
        v4 = h->GetBinContent(nc+i1,nf-i2,nf-i3);  ev4 = h->GetBinError(nc+i1,nf-i2,nf-i3);
        v5 = h->GetBinContent(nf-i1,nc+i2,nc+i3);  ev5 = h->GetBinError(nf-i1,nc+i2,nc+i3);
        v6 = h->GetBinContent(nf-i1,nc+i2,nf-i3);  ev6 = h->GetBinError(nf-i1,nc+i2,nf-i3);
        v7 = h->GetBinContent(nf-i1,nf-i2,nc+i3);  ev7 = h->GetBinError(nf-i1,nf-i2,nc+i3);
        v8 = h->GetBinContent(nf-i1,nf-i2,nf-i3);  ev8 = h->GetBinError(nf-i1,nf-i2,nf-i3);

        v = (v1+v2+v3+v4+v5+v6+v7+v8)/8.0;
        ev = sqrt(ev1*ev1+ev2*ev2+ev3*ev3+ev4*ev4+ev5*ev5+ev6*ev6+ev7*ev7+ev8*ev8)/8.0;

        h->SetBinContent(nc+i1,nc+i2,nc+i3,v);  h->SetBinError(nc+i1,nc+i2,nc+i3,ev);
        h->SetBinContent(nc+i1,nc+i2,nf-i3,v);  h->SetBinError(nc+i1,nc+i2,nf-i3,ev);
        h->SetBinContent(nc+i1,nf-i2,nc+i3,v);  h->SetBinError(nc+i1,nf-i2,nc+i3,ev);
        h->SetBinContent(nc+i1,nf-i2,nf-i3,v);  h->SetBinError(nc+i1,nf-i2,nf-i3,ev);
        h->SetBinContent(nf-i1,nc+i2,nc+i3,v);  h->SetBinError(nf-i1,nc+i2,nc+i3,ev);
        h->SetBinContent(nf-i1,nc+i2,nf-i3,v);  h->SetBinError(nf-i1,nc+i2,nf-i3,ev);
        h->SetBinContent(nf-i1,nf-i2,nc+i3,v);  h->SetBinError(nf-i1,nf-i2,nc+i3,ev);
        h->SetBinContent(nf-i1,nf-i2,nf-i3,v);  h->SetBinError(nf-i1,nf-i2,nf-i3,ev);

        }
      }
    }
}



// h must be writtable otherwise this does not work...
// x axis is DeltaEta. It must have an odd number of bins
// y axis is DeltaPhi. It must have an even number of bins
void HistogramCollection::symmetrizeDeltaEtaDeltaPhi(TH2 * h, bool ijNormalization)
{
  double v1, v2, v3, v4;
  double ev1, ev2, ev3, ev4;
  double sv, esv;
  int nEta = h->GetNbinsX(); //DeltaEta
  int nPhi = h->GetNbinsY(); //DeltaPhi
                             //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Will symmetrize histo:" << h->GetName() << endl;
                             //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) nEta:" << nEta << endl;
                             //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) nPhi:" << nPhi << endl;
  int nEtaHalf = (nEta-1)/2;
  int nPhiHalf = (nPhi-2)/2;
  int iEta, iPhi, iPhi1, iEta1;
  double * v = new double[nEta*nPhi];
  double * ev = new double[nEta*nPhi];
  //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Arrays created" << endl;
  for (int iPhi=0;iPhi<nPhi;iPhi++)
    {
    for (int iEta=0;iEta<nEta;iEta++)
      {
      iPhi1 = iPhi+1;
      iEta1 = iEta+1;
      v[ iEta+iPhi*nEta]  = h->GetBinContent(iEta1,iPhi1);
      ev[iEta+iPhi*nEta]  = h->GetBinError(  iEta1,iPhi1);
      }
    }
  //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Arrays copied" << endl;
  for (iEta=0;iEta<nEtaHalf;iEta++)
    {
    iEta1 = iEta+1;
    for (iPhi=0; iPhi<nPhiHalf;iPhi++)
      {
      iPhi1 = iPhi+1;
      v1 = v[  nEta-iEta1+(nPhi-iPhi1)*nEta];
      v2 = v[  nEta-iEta1+(     iPhi1)*nEta];
      v3 = v[        iEta+(nPhi-iPhi1)*nEta];
      v4 = v[        iEta+(     iPhi1)*nEta];
      ev1 = ev[nEta-iEta1+(nPhi-iPhi1)*nEta];
      ev2 = ev[nEta-iEta1+(     iPhi1)*nEta];
      ev3 = ev[      iEta+(nPhi-iPhi1)*nEta];
      ev4 = ev[      iEta+(     iPhi1)*nEta];
      if (ijNormalization)
        {
        sv = (v1+v2+v3+v4)/2.;
        esv = sqrt(ev1*ev1+ev2*ev2+ev3*ev3+ev4*ev4)/2.;
        }
      else
        {
        sv = (v1+v2+v3+v4)/4.;
        esv = sqrt(ev1*ev1+ev2*ev2+ev3*ev3+ev4*ev4)/4.;
        }
      h->SetBinContent( nEta-iEta, nPhi-iPhi, sv);
      h->SetBinContent( nEta-iEta,   iPhi1+1, sv);
      h->SetBinContent(     iEta1, nPhi-iPhi, sv);
      h->SetBinContent(     iEta1,   iPhi1+1, sv);
      h->SetBinError(   nEta-iEta, nPhi-iPhi, esv);
      h->SetBinError(   nEta-iEta,   iPhi1+1, esv);
      h->SetBinError(       iEta1, nPhi-iPhi, esv);
      h->SetBinError(       iEta1,   iPhi1+1, esv);
      }
    }
  //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Part 1 Done" << endl;
  iEta  = nEtaHalf;
  iEta1 = iEta+1;
  for (iPhi=0; iPhi<nPhiHalf;iPhi++) // iEta center bin
    {
    iPhi1 = iPhi+1;
    v3 = v[        iEta+(nPhi-iPhi1)*nEta];
    v4 = v[        iEta+(     iPhi1)*nEta];
    ev3 = ev[      iEta+(nPhi-iPhi1)*nEta];
    ev4 = ev[      iEta+(     iPhi1)*nEta];
    if (ijNormalization)
      {
      sv = (v3+v4);
      esv = sqrt(ev3*ev3+ev4*ev4);
      }
    else
      {
      sv = (v3+v4)/2.;
      esv = sqrt(ev3*ev3+ev4*ev4)/2.;
      }
    h->SetBinContent(     iEta1, nPhi-iPhi, sv);
    h->SetBinContent(     iEta1,   iPhi1+1, sv);
    h->SetBinError(       iEta1, nPhi-iPhi, esv);
    h->SetBinError(       iEta1,   iPhi1+1, esv);
    }
  //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Part 2 Done" << endl;

  iPhi = 0;
  iPhi1 = iPhi+1;
  for (iEta=0;iEta<nEtaHalf;iEta++)
    {
    iEta1 = iEta+1;
    v1 = v[  nEta-iEta1];
    v3 = v[        iEta];
    ev1 = ev[nEta-iEta1];
    ev3 = ev[      iEta];
    if (ijNormalization)
      {
      sv = (v1+v3);
      esv = sqrt(ev1*ev1+ev3*ev3);
      }
    else
      {
      sv = (v1+v3)/2.;
      esv = sqrt(ev1*ev1+ev3*ev3)/2.;
      }
    h->SetBinContent( nEta-iEta, 1, sv);
    h->SetBinContent(     iEta1, 1, sv);
    h->SetBinError(   nEta-iEta, 1, esv);
    h->SetBinError(       iEta1, 1, esv);

    iPhi  = nPhi/2;
    iPhi1 = iPhi+1;
    v1 = v[  nEta-iEta1 + iPhi*nEta];
    v3 = v[        iEta + iPhi*nEta];
    ev1 = ev[nEta-iEta1 + iPhi*nEta];
    ev3 = ev[      iEta + iPhi*nEta];
    if (ijNormalization)
      {
      sv = (v1+v3);
      esv = sqrt(ev1*ev1+ev3*ev3);
      }
    else
      {
      sv = (v1+v3)/2.;
      esv = sqrt(ev1*ev1+ev3*ev3)/2.;
      }
    h->SetBinContent( nEta-iEta, iPhi1, sv);
    h->SetBinContent(     iEta1, iPhi1, sv);
    h->SetBinError(   nEta-iEta, iPhi1, esv);
    h->SetBinError(       iEta1, iPhi1, esv);

    }
  //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) All Done now delete" << endl;
  delete[] v;
  //if (reportDebug()) cout << "symmetrizeDeltaEtaDeltaPhi(TH2 * h) Really All Done" << endl;

}

void HistogramCollection::symmetrizeXX(TH2 * h, bool ijNormalization)
{
  //if (reportDebug()) cout << "symmetrizeXX(TH2 * h) Will symmetrize histo:" << h->GetName() << endl;
  double v1, v2;
  double ev1, ev2;
  double sv, esv;
  int n_x = h->GetNbinsX(); //DeltaEta
  int n_y = h->GetNbinsY(); //DeltaPhi
  int i_x, i_y, i_x1, i_y1;
  double * v  = new double[n_x*n_y];
  double * ev = new double[n_x*n_y];
  for (i_x=0; i_x<n_x; i_x++)
    {
    i_x1 = i_x+1;
    for (i_y=0; i_y<n_y; i_y++)
      {
      i_y1 = i_y+1;
      v[ i_x+i_y*n_x]  = h->GetBinContent(i_x1,i_y1);
      ev[i_x+i_y*n_x]  = h->GetBinError(  i_x1,i_y1);
      }
    }
  for (i_x=0; i_x<n_x; i_x++)
    {
    i_x1 = i_x+1;
    for (i_y=0; i_y<n_y; i_y++)
      {
      i_y1 = i_y+1;
      v1 = v[  i_x+i_y*n_x ];
      v2 = v[  i_y+i_x*n_x ];
      ev1 = ev[ i_x+i_y*n_x ];
      ev2 = ev[ i_y+i_x*n_x];
      if (ijNormalization)
        {
        sv = (v1+v2);
        esv = sqrt(ev1*ev1+ev2*ev2);
        }
      else
        {
        sv = (v1+v2)/2.;
        esv = sqrt(ev1*ev1+ev2*ev2)/2.;
        }
      h->SetBinContent( i_x1, i_y1, sv);
      h->SetBinContent( i_y1, i_x1, sv);
      h->SetBinError(   i_x1, i_y1, esv);
      h->SetBinError(   i_y1, i_x1, esv);
      }
    }
  delete[] v;
}

//  void reduce_n2xEtaPhi_n2DetaDphi(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins)
//  {
//  //if (reportDebug()) cout << "reduce_n2xEtaPhi_n2DetaDphi() +++++++++++++ New Version  from TH1" << endl;
//  double v1,v2,ev1;
//  int dPhi,dEta, iPhi,iEta,jPhi,jEta, i, j;
//  int nBins = nEtaBins*nPhiBins;
//  int nWrk  = nPhiBins*(2*nEtaBins-1);
//  int index;
//  double * numerator    = new double[nWrk];
//  double * numeratorErr = new double[nWrk];
//  double * denominator  = new double[nWrk];
//  for (int k=0;k<nWrk;++k)
//    {
//    numerator[k]    = 0;
//    numeratorErr[k] = 0;
//    denominator[k]  = 0;
//    }
//
//  i=0;
//  for (iEta=0;iEta<nEtaBins; ++iEta)
//    {
//    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
//      {
//      j=0;
//      for (jEta=0;jEta<nEtaBins; ++jEta)
//        {
//        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
//          {
//          dPhi = iPhi-jPhi; if (dPhi<0) dPhi += nPhiBins; dPhi+=1;
//          dEta = iEta-jEta + nEtaBins;
//          v1   = source->GetBinContent(i*nBins+j+1);
//          ev1  = source->GetBinError(i*nBins+j+1);
//          //if (v1>-0.999999)
//          //{
//          index = (dEta-1)*nPhiBins + dPhi-1;
//          numerator[index]    += v1;
//          numeratorErr[index] += ev1*ev1;
//          denominator[index]  += 1.;
//          //}
//          ++j;
//          }
//        }
//      ++i;
//      }
//    }
//  for (dEta=0;dEta<2*nEtaBins-1;++dEta)
//    {
//    for (dPhi=0;dPhi<nPhiBins;++dPhi)
//      {
//      v1   = target->GetBinContent(dEta+1,dPhi+1);
//      ev1  = target->GetBinError(dEta+1,dPhi+1);
//      index = dEta*nPhiBins + dPhi;
//      v1    = numerator[index];
//      ev1   = numeratorErr[index];
//      v2    = denominator[index];
//      if (v2<=0) if (reportDebug()) cout << "miserable idiot!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//      target->SetBinContent(dEta+1,dPhi+1,v1/v2);
//      target->SetBinError(  dEta+1,dPhi+1,sqrt(ev1)/v2);
//      }
//    }
//  delete [] numerator;
//  delete [] numeratorErr;
//  delete [] denominator;
//  }

void HistogramCollection::reduce_n2xEtaPhi_n2DetaDphi(const TH2 * source, TH2 * target,int nEtaBins,int nPhiBins,Option_t *opt)
{
  //if (reportDebug()) cout << "reduce_n2xEtaPhi_n2DetaDphi() ==============  New Version From TH2" << endl;
  double v1,v2,ev1;
  int dPhi,dEta, iPhi,iEta,jPhi,jEta, i, j;
  int nBins = nEtaBins*nPhiBins;
  int nWrk  = nPhiBins*(2*nEtaBins-1);
  int index;
  double * numerator    = new double[nWrk];
  double * numeratorErr = new double[nWrk];
  double * denominator  = new double[nWrk];
  for (int k=0;k<nWrk;++k)
    {
    numerator[k]    = 0;
    numeratorErr[k] = 0;
    denominator[k]  = 0;
    }

  TString name = target->GetName();

  i=1;
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
      {
      j=1;
      for (jEta=0;jEta<nEtaBins; ++jEta)
        {
        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
          {
          dPhi = iPhi-jPhi; if (dPhi<0) dPhi += nPhiBins; dPhi+=1;
          dEta = iEta-jEta + nEtaBins;
          v1   = source->GetBinContent(i, j);
          ev1  = source->GetBinError(  i, j);
          index = (dEta-1)*nPhiBins + dPhi-1;
          numerator[index]    += v1;
          numeratorErr[index] += ev1*ev1;
          denominator[index]  += 1.;
          //cout << " " << name << "  iEta:" << iEta << " iPhi:" << iPhi << " jEta:" << jEta << " jPhi:" << jPhi << " v1:" << v1 << " ev1:" << ev1 << endl;
          ++j;
          }
        }
      ++i;
      }
    }

  for (dEta=0;dEta<2*nEtaBins-1;++dEta)
    {
    for (dPhi=0;dPhi<nPhiBins;++dPhi)
      {
      //v1   = target->GetBinContent(dEta+1,dPhi+1);
      //ev1  = target->GetBinError(dEta+1,dPhi+1);
      index = dEta*nPhiBins + dPhi;
      v1    = numerator[index];
      ev1   = numeratorErr[index];
      v2    = denominator[index];
      if (v2<=0) if (reportDebug()) cout << "HistogramCollection::reduce_n2xEtaPhi_n2DetaDphi() Elements of denominator are negative." << endl;
      target->SetBinContent(dEta+1,dPhi+1,v1/v2);
      target->SetBinError(  dEta+1,dPhi+1,sqrt(ev1)/v2);
      }
    }

  /* let's prepare the scale in case the integral needs to be preserved */
  if (TString(opt).Contains("int")) {
    double factor = source->Integral() / target->Integral();
    target->Scale(factor);
    ::Error("HistogramCollection::reduce_n2xEtaPhi_n2DetaDphi","Down scaling histogram %s by a factor %f", target->GetName(), factor);
  }



  // double a1m = target->GetBinContent(nEtaBins-1,1);
  // double a1  = target->GetBinContent(nEtaBins,1);
  // double a1p = target->GetBinContent(nEtaBins+1,1);
  // if (reportDebug()) cout << " a1m:" << a1m << endl;
  // if (reportDebug()) cout << " a1 :" << a1 << endl;
  // if (reportDebug()) cout << " a1p:" << a1p << endl;
  // target->SetBinContent(nEtaBins,1,0.5*(a1m+a1p));


  delete [] numerator;
  delete [] numeratorErr;
  delete [] denominator;
}

/* Projects the source in eta1,phi1,eta2,phi2 into the target in deta,dphi without any averaging */
void HistogramCollection::projectEtaPhiEtaPhiToDetaDphi(const TH2 * source, TH2 * target,int nEtaBins,int nPhiBins)
{
  double v1,v2,ev1;
  int dPhi,dEta, iPhi,iEta,jPhi,jEta, i, j;
  int nBins = nEtaBins*nPhiBins;
  int nWrk  = nPhiBins*(2*nEtaBins-1);
  int index;
  double * numerator    = new double[nWrk];
  double * numeratorErr = new double[nWrk];
  for (int k=0;k<nWrk;++k)
    {
    numerator[k]    = 0;
    numeratorErr[k] = 0;
    }

  TString name = target->GetName();

  i=1;
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
      {
      j=1;
      for (jEta=0;jEta<nEtaBins; ++jEta)
        {
        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
          {
          dPhi = iPhi-jPhi; if (dPhi<0) dPhi += nPhiBins; dPhi+=1;
          dEta = iEta-jEta + nEtaBins;
          v1   = source->GetBinContent(i, j);
          ev1  = source->GetBinError(  i, j);
          index = (dEta-1)*nPhiBins + dPhi-1;
          numerator[index]    += v1;
          numeratorErr[index] += ev1*ev1;
          ++j;
          }
        }
      ++i;
      }
    }

  for (dEta=0;dEta<2*nEtaBins-1;++dEta)
    {
    for (dPhi=0;dPhi<nPhiBins;++dPhi)
      {
      index = dEta*nPhiBins + dPhi;
      v1    = numerator[index];
      ev1   = numeratorErr[index];
      target->SetBinContent(dEta+1,dPhi+1,v1);
      target->SetBinError(  dEta+1,dPhi+1,sqrt(ev1));
      }
    }
  target->SetEntries(source->GetEntries());
  delete [] numerator;
  delete [] numeratorErr;
}
 


void HistogramCollection::reduce_n2xEtaPhi_n2EtaEta(const TH1 * source, TH2 * target,int nEtaBins,int nPhiBins)
{
  double v1,v2,ev1,ev2,v,ev;
  int iPhi,iEta,jPhi,jEta, i, j;
  int nBins = nEtaBins*nPhiBins;
  int nEtaEta = nEtaBins*nEtaBins;
  double * work = new double[nEtaEta];
  for (int k=0;k<nEtaEta;++k) work[k]=0;
  i=0;
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (iPhi=0;iPhi<nPhiBins; ++iPhi)
      {
      j=0;
      for (jEta=0;jEta<nEtaBins; ++jEta)
        {
        for (jPhi=0;jPhi<nPhiBins; ++jPhi)
          {
          v1   = source->GetBinContent(i*nBins+j+1);
          ev1  = source->GetBinError(i*nBins+j+1);
          if (v1> -0.9999999)
            {
            v2   = target->GetBinContent(iEta+1,jEta+1);
            ev2  = target->GetBinError(iEta+1,jEta+1);
            v = v1+v2;
            ev = sqrt(ev1*ev1+ev2*ev2);
            target->SetBinContent(iEta+1,jEta+1,v);
            target->SetBinError(iEta+1,jEta+1,ev);
            work[ iEta*nEtaBins+jEta] += 1;
            }
          ++j;
          }
        }
      ++i;
      }
    }
  for (iEta=0;iEta<nEtaBins; ++iEta)
    {
    for (jEta=0;jEta<nEtaBins; ++jEta)
      {
      v1   = target->GetBinContent(iEta+1,jEta+1);
      ev1  = target->GetBinError(iEta+1,jEta+1);
      v2   = work[ iEta*nEtaBins+jEta ];
      if (v2<=0)
        if (reportError()) cout << "Serious BUG !!!!!!!!!!!!!!!!!!!!!!!" << endl;
      target->SetBinContent(iEta+1,jEta+1,v1/v2);
      target->SetBinError(iEta+1,jEta+1,ev1/v2);
      }
    }
  delete [] work;
}

void HistogramCollection::project_n2XYXY_n2XX(const TH2 * source, TH2 * target,int nXBins,int nYBins)
{
  double v1,v2,ev1,ev2,v,ev;

  /* sanity checks */
  if ((source->GetNbinsX() != nXBins*nYBins)
      or (source->GetNbinsY() != nXBins*nYBins)
      or (target->GetNbinsX() != nXBins)
      or (target->GetNbinsY() != nXBins)) {
        cout << "-Fatal- HistogramCollection::project_n2XYXY_n2XX: " << "Inconsistent indexes for histogram " << source->GetName() << endl;
        return;
      }

  target->Reset();

  int i=0;
  for (int iX=0;iX<nXBins; ++iX)
  {
    for (int iY=0;iY<nYBins; ++iY)
    {
      int j=0;
      for (int jX=0;jX<nXBins; ++jX)
      {
        for (int jY=0;jY<nYBins; ++jY)
        {
          v1   = source->GetBinContent(i+1,j+1);
          ev1  = source->GetBinError(i+1,j+1);
          v2   = target->GetBinContent(iX+1,jX+1);
          ev2  = target->GetBinError(iX+1,jX+1);
          v = v1+v2;
          ev = sqrt(ev1*ev1+ev2*ev2);
          target->SetBinContent(iX+1,jX+1,v);
          target->SetBinError(iX+1,jX+1,ev);
          ++j;
        }
      }
      ++i;
    }
  }
  target->SetEntries(source->GetEntries());
}

void HistogramCollection::project_n2XYXY_n2YY(const TH2 * source, TH2 * target,int nXBins,int nYBins)
{
  double v1,v2,ev1,ev2,v,ev;

  /* sanity checks */
  if ((source->GetNbinsX() != nXBins*nYBins)
      or (source->GetNbinsY() != nXBins*nYBins)
      or (target->GetNbinsX() != nYBins)
      or (target->GetNbinsY() != nYBins)) {
        cout << "-Fatal- HistogramCollection::project_n2XYXY_n2YY: " << "Inconsistent indexes for histogram " << source->GetName() << endl;
        return;
      }

  target->Reset();

  int i=0;
  for (int iX=0;iX<nXBins; ++iX)
  {
    for (int iY=0;iY<nYBins; ++iY)
    {
      int j=0;
      for (int jX=0;jX<nXBins; ++jX)
      {
        for (int jY=0;jY<nYBins; ++jY)
        {
          v1   = source->GetBinContent(i+1,j+1);
          ev1  = source->GetBinError(i+1,j+1);
          v2   = target->GetBinContent(iY+1,jY+1);
          ev2  = target->GetBinError(iY+1,jY+1);
          v = v1+v2;
          ev = sqrt(ev1*ev1+ev2*ev2);
          target->SetBinContent(iY+1,jY+1,v);
          target->SetBinError(iY+1,jY+1,ev);
          ++j;
        }
      }
      ++i;
    }
  }
  target->SetEntries(source->GetEntries());
}


TH2* HistogramCollection::symmetrize(TH2* h)
{
  TH2* c = (TH2*) h->Clone();
  double v[31][72];
  for (int iPhi=1;iPhi<=72;iPhi++)
    {
    for (int iEta=1;iEta<=31;iEta++)
      {
      v[iEta-1][iPhi-1] = h->GetBinContent(iEta,iPhi);
      }
    }
  for (int iEta=1;iEta<=15;iEta++)
    {
    for (int iPhi=1;iPhi<=11;iPhi++)
      {
      double v1 = v[15-iEta][11-iPhi];
      double v2 = v[15-iEta][11+iPhi];
      double v3 = v[15+iEta][11-iPhi];
      double v4 = v[15+iEta][11+iPhi];
      c->SetBinContent(16-iEta,12-iPhi,    (v1+v2+v3+v4)/4. );
      c->SetBinContent(16-iEta,12+iPhi,    (v1+v2+v3+v4)/4. );
      c->SetBinContent(16+iEta,12-iPhi,    (v1+v2+v3+v4)/4. );
      c->SetBinContent(16+iEta,12+iPhi,    (v1+v2+v3+v4)/4. );
      // c->SetBinContent(32-iEta,iPhi,    (v1+v2+v3+v4)/4. );
      // c->SetBinContent(32-iEta,25-iPhi, (v1+v2+v3+v4)/4. );
      }
    for (int iPhi=1;iPhi<=24;iPhi++)
      {
      double v1 = v[15-iEta][47-iPhi];
      double v2 = v[15-iEta][47+iPhi];
      double v3 = v[15+iEta][47-iPhi];
      double v4 = v[15+iEta][47+iPhi];
      c->SetBinContent(16-iEta,48-iPhi,    (v1+v2+v3+v4)/4. );
      c->SetBinContent(16-iEta,48+iPhi,    (v1+v2+v3+v4)/4. );
      c->SetBinContent(16+iEta,48-iPhi,    (v1+v2+v3+v4)/4. );
      c->SetBinContent(16+iEta,48+iPhi,    (v1+v2+v3+v4)/4. );
      }

    }
  return c;

}

TH2* HistogramCollection::transpose(const TH2* h)
{
  TH2* ht = new TH2F(TString::Format("%s_T",h->GetName()),h->GetTitle(),
                                     h->GetNbinsY(),h->GetYaxis()->GetBinLowEdge(1),h->GetYaxis()->GetBinUpEdge(h->GetNbinsY()),
                                     h->GetNbinsX(),h->GetXaxis()->GetBinLowEdge(1),h->GetXaxis()->GetBinUpEdge(h->GetNbinsX()));

  for (int ix = 0; ix < h->GetNbinsX(); ++ix) {
    for (int iy = 0; iy < h->GetNbinsY(); ++iy) {
      ht->SetBinContent(iy+1,ix+1,h->GetBinContent(ix+1,iy+1));
      ht->SetBinError(iy+1,ix+1,h->GetBinError(ix+1,iy+1));
    }
  }
  ht->SetEntries(h->GetEntries());

  return ht;
}

///shift the given source to the target vertically by nbins
void HistogramCollection::shiftY(const TH2 & source, TH2 & target, int nbins)
{
  int i_x, i_y;
  int n_x = source.GetNbinsX();
  int n_y = source.GetNbinsY();

  //shift the 1st area
  for (i_x=1;i_x<=n_x;++i_x)
    {
    for (i_y=1;i_y<=n_y-nbins;++i_y)
      {
      double v  = source.GetBinContent(i_x,i_y);
      double ev = source.GetBinError(i_x,i_y);
      target.SetBinContent(i_x,  i_y+nbins, v);
      target.SetBinError(i_x,    i_y+nbins, ev);
      }
    for (i_y=n_y-nbins+1;i_y<=n_y;++i_y)
      {
      double v  = source.GetBinContent(i_x,i_y);
      double ev = source.GetBinError(i_x,i_y);
      target.SetBinContent(i_x, i_y-(n_y-nbins),v);
      target.SetBinError(i_x,   i_y-(n_y-nbins),ev);
      }
    }
}

////////////////////////////////////////////////////////////////////////
float * HistogramCollection::getFloatArray(int size, float v)
{
  /// Allocate an array of type float with n values
  /// Initialize the array to the given value
  float * array = new float [size];
  for (int i=0;i<size;++i) array[i]=v;
  return array;
}


////////////////////////////////////////////////////////////////////////
double * HistogramCollection::getDoubleArray(int size, double v)
{
  /// Allocate an array of type double with n values
  /// Initialize the array to the given value
  double * array = new double [size];
  for (int i=0;i<size;++i) array[i]=v;
  return array;
}

////////////////////////////////////////////////////////////////////////
void HistogramCollection::resetDoubleArray(int n, double * array, double value)
{
  /// Set all n values of the given array to the given value
  double * ptr;
  double * ptrEnd = array+n;
  for (ptr=array; ptr<ptrEnd; ++ptr)
    {
    *ptr = value;
    }
}


int HistogramCollection::index2(int i1, int i2)
{
  int v;
  switch (i1)
    {
      case 0:
      switch (i2)
        {
          case 0: v = 0; break;
          case 1: v = 1; break;
          case 2: v = 2; break;
          case 3: v = 3; break;
          default: v = -1; break;
        }
      break;
      case 1:
      switch (i2)
        {
          case 1: v = 4; break;
          case 2: v = 5; break;
          case 3: v = 6; break;
          default: v = -1; break;
        }
      break;
      case 2:
      switch (i2)
        {
          case 2: v = 7; break;
          case 3: v = 8; break;
          default: v = -1; break;
        }
      break;
      case 3:
      switch (i2)
        {
          case 3: v = 9; break;
          default: v = -1; break;
        }
      break;
      default: v = -1; break;
    }
  if (v<0) cout << "<E> HistogramCollection::index2(int i1, int i2) Combination i1:" << i1 << " i2:" << i2 << " is not allowed." << endl;
  return v;
}

int HistogramCollection::index3(int i1, int i2, int i3)
{
  int v;
  switch (i1)
    {
      case 0:
      switch (i2)
        {
          case 0:
          switch (i3)
            {
              case 0: v = 0; break;
              case 1: v = 1; break;
              case 2: v = 2; break;
              case 3: v = 3; break;
              default: v = -1; break;
            }
          break;
          case 1:
          switch (i3)
            {
              case 1: v = 4; break;
              case 2: v = 5; break;
              case 3: v = 6; break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2: v = 7; break;
              case 3: v = 8; break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3: v = 9; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 1:
      switch (i2)
        {
          case 1:
          switch (i3)
            {
              case 1: v = 10; break;
              case 2: v = 11; break;
              case 3: v = 12; break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2: v = 13; break;
              case 3: v = 14; break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3: v = 15; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 2:
      switch (i2)
        {
          case 2:
          switch (i3)
            {
              case 2: v = 16; break;
              case 3: v = 17; break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3: v = 18; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 3:
      switch (i2)
        {
          case 3:
          switch (i3)
            {
              case 3: v = 19; break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      default: v = -1; break;
    }
  if (v<0) cout << "<E> HistogramCollection::index3(int i1, int i2, int i3) Combination i1:" << i1 << " i2:" << i2 <<  " i3:" << i3 << " is not allowed." << endl;
  return v;
}

int HistogramCollection::index4(int i1, int i2, int i3, int i4)
{
  int v;
  switch (i1)
    {
      case 0:
      switch (i2)
        {
          case 0:
          switch (i3)
            {
              case 0:
              switch (i4)
                {
                  case 0: v = 0; break;
                  case 1: v = 1; break;
                  case 2: v = 2; break;
                  case 3: v = 3; break;
                  default: v = -1; break;
                }
              break;
              case 1:
              switch (i4)
                {
                  case 1: v = 4; break;
                  case 2: v = 5; break;
                  case 3: v = 6; break;
                  default: v = -1; break;
                }
              break;
              case 2:
              switch (i4)
                {
                  case 2: v = 7; break;
                  case 3: v = 8; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 9; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 1:
          switch (i3)
            {
              case 1:
              switch (i4)
                {
                  case 1: v = 10; break;
                  case 2: v = 11; break;
                  case 3: v = 12; break;
                  default: v = -1; break;
                }
              break;
              case 2:
              switch (i4)
                {
                  case 2: v = 13; break;
                  case 3: v = 14; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 15; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2:
              switch (i4)
                {
                  case 2: v = 16; break;
                  case 3: v = 17; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 18; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 19; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 1:
      switch (i2)
        {
          case 1:
          switch (i3)
            {
              case 1:
              switch (i4)
                {
                  case 1: v = 20; break;
                  case 2: v = 21; break;
                  case 3: v = 22; break;
                  default: v = -1; break;
                }
              break;
              case 2:
              switch (i4)
                {
                  case 2: v = 23; break;
                  case 3: v = 24; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 25; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 2:
          switch (i3)
            {
              case 2:
              switch (i4)
                {
                  case 2: v = 26; break;
                  case 3: v = 27; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 28; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 29; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 2:
      switch (i2)
        {
          case 2:
          switch (i3)
            {
              case 2:
              switch (i4)
                {
                  case 2: v = 30; break;
                  case 3: v = 31; break;
                  default: v = -1; break;
                }
              break;
              case 3:
              switch (i4)
                {
                  case 3: v = 32; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 33; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      case 3:
      switch (i2)
        {
          case 3:
          switch (i3)
            {
              case 3:
              switch (i4)
                {
                  case 3: v = 34; break;
                  default: v = -1; break;
                }
              break;
              default: v = -1; break;
            }
          break;
          default: v = -1; break;
        }
      break;
      default: v = -1; break;
    }
  if (v<0) cout << "<E> HistogramCollection::index3(int i1, int i2, int i3, int i4) Combination i1:" << i1 << " i2:" << i2 <<  " i3:" << i3 <<  " i4:" << i4 << " is not allowed." << endl;
  return v;
}

void HistogramCollection::calculateF2R2(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f2_12, TH1* h_F2_12, TH1* h_R2_12)
{
  double f1_1, ef1_1, f1_2, ef1_2;
  double f2_12, ef2_12;
  double F2_12, eF2_12, R2_12, eR2_12;
  int nBins = h_f1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    f1_1   = h_f1_1->GetBinContent(iBin);   ef1_1 =   h_f1_1->GetBinError(iBin);
    f1_2   = h_f1_2->GetBinContent(iBin);   ef1_2  =  h_f1_2->GetBinError(iBin);
    f2_12  = h_f2_12->GetBinContent(iBin);  ef2_12 =  h_f2_12->GetBinError(iBin);
    calculateF2R2(f1_1,ef1_1,f1_2,ef1_2,f2_12,ef2_12, F2_12,eF2_12,R2_12,eR2_12);
    h_F2_12->SetBinContent(iBin, F2_12);
    h_F2_12->SetBinError(iBin,   eF2_12);
    h_R2_12->SetBinContent(iBin, R2_12);
    h_R2_12->SetBinError(iBin,   eR2_12);
    }
}

void HistogramCollection::calculateNudyn(TH1* h_R2_11, TH1* h_R2_12, TH1* h_R2_22, TH1* h_nudyn_12)
{
  double R2_11, eR2_11, R2_12, eR2_12, R2_22, eR2_22, nudyn, enudyn;
  int nBins = h_R2_11->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    R2_11   = h_R2_11->GetBinContent(iBin);  eR2_11  =  h_R2_11->GetBinError(iBin);
    R2_12   = h_R2_12->GetBinContent(iBin);  eR2_12  =  h_R2_12->GetBinError(iBin);
    R2_22   = h_R2_22->GetBinContent(iBin);  eR2_22  =  h_R2_22->GetBinError(iBin);
    calculateNudyn(R2_11, eR2_11, R2_12, eR2_12, R2_22, eR2_22, nudyn, enudyn);
    h_nudyn_12->SetBinContent(iBin, nudyn);
    h_nudyn_12->SetBinError(iBin,   enudyn);
    }
}


void HistogramCollection::calculateF3R3(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f1_3,
                                        TH1* h_f2_12, TH1* h_f2_13, TH1* h_f2_23,
                                        TH1* h_f3_123,
                                        TH1* h_F3_123, TH1* h_R3_123)
{
  double f1_1, ef1_1, f1_2, ef1_2, f1_3, ef1_3;
  double f2_12, ef2_12, f2_13, ef2_13, f2_23, ef2_23;
  double f3_123, ef3_123;
  double F3_123, eF3_123, R3_123, eR3_123;
  int nBins = h_f1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    f1_1   = h_f1_1->GetBinContent(iBin);    ef1_1 =   h_f1_1->GetBinError(iBin);
    f1_2   = h_f1_2->GetBinContent(iBin);    ef1_2  =  h_f1_2->GetBinError(iBin);
    f1_3   = h_f1_3->GetBinContent(iBin);    ef1_3  =  h_f1_2->GetBinError(iBin);
    f2_12  = h_f2_12->GetBinContent(iBin);   ef2_12 =  h_f2_12->GetBinError(iBin);
    f2_13  = h_f2_13->GetBinContent(iBin);   ef2_13 =  h_f2_13->GetBinError(iBin);
    f2_23  = h_f2_23->GetBinContent(iBin);   ef2_23 =  h_f2_23->GetBinError(iBin);
    f3_123 = h_f3_123->GetBinContent(iBin);  ef3_123 =  h_f3_123->GetBinError(iBin);
    calculateF3R3(f1_1,ef1_1,f1_2,ef1_2,f1_3,ef1_3,
                  f2_12,ef2_12,f2_13,ef2_13,f2_23,ef2_23,
                  f3_123,ef3_123,
                  F3_123,eF3_123,R3_123,eR3_123);
    h_F3_123->SetBinContent(iBin, F3_123);
    h_F3_123->SetBinError(iBin,   eF3_123);
    h_R3_123->SetBinContent(iBin, R3_123);
    h_R3_123->SetBinError(iBin,   eR3_123);
    }
}

void HistogramCollection::calculateF4R4(TH1* h_f1_1, TH1* h_f1_2, TH1* h_f1_3, TH1* h_f1_4,
                                        TH1* h_f2_12, TH1* h_f2_13, TH1* h_f2_14,
                                        TH1* h_f2_23, TH1* h_f2_24, TH1* h_f2_34,
                                        TH1* h_f3_123, TH1* h_f3_124, TH1* h_f3_134, TH1* h_f3_234,
                                        TH1* h_f4_1234,
                                        TH1* h_F4_1234, TH1* h_R4_1234)
{
  double f1_1, ef1_1, f1_2, ef1_2, f1_3, ef1_3, f1_4, ef1_4;
  double f2_12, ef2_12, f2_13, ef2_13, f2_14, ef2_14, f2_23, ef2_23, f2_24, ef2_24, f2_34, ef2_34;
  double f3_123, ef3_123, f3_124, ef3_124, f3_134, ef3_134, f3_234, ef3_234;
  double f4_1234, ef4_1234;
  double F4_1234, eF4_1234, R4_1234, eR4_1234;
  int nBins = h_f1_1->GetNbinsX();
  for (int iBin = 1; iBin <=nBins; iBin++)
    {
    f1_1    = h_f1_1->GetBinContent(iBin);   ef1_1   =  h_f1_1->GetBinError(iBin);
    f1_2    = h_f1_2->GetBinContent(iBin);   ef1_2   =  h_f1_2->GetBinError(iBin);
    f1_3    = h_f1_3->GetBinContent(iBin);   ef1_3   =  h_f1_3->GetBinError(iBin);
    f1_4    = h_f1_4->GetBinContent(iBin);   ef1_4   =  h_f1_4->GetBinError(iBin);
    f2_12   = h_f2_12->GetBinContent(iBin);  ef2_12  =  h_f2_12->GetBinError(iBin);
    f2_13   = h_f2_13->GetBinContent(iBin);  ef2_13  =  h_f2_13->GetBinError(iBin);
    f2_14   = h_f2_14->GetBinContent(iBin);  ef2_14  =  h_f2_14->GetBinError(iBin);
    f2_23   = h_f2_23->GetBinContent(iBin);  ef2_23  =  h_f2_23->GetBinError(iBin);
    f2_24   = h_f2_24->GetBinContent(iBin);  ef2_24  =  h_f2_24->GetBinError(iBin);
    f2_34   = h_f2_34->GetBinContent(iBin);  ef2_34  =  h_f2_34->GetBinError(iBin);
    f3_123  = h_f3_123->GetBinContent(iBin); ef3_123 =  h_f3_123->GetBinError(iBin);
    f3_124  = h_f3_124->GetBinContent(iBin); ef3_124 =  h_f3_124->GetBinError(iBin);
    f3_134  = h_f3_134->GetBinContent(iBin); ef3_134 =  h_f3_134->GetBinError(iBin);
    f3_234  = h_f3_234->GetBinContent(iBin); ef3_234 =  h_f3_234->GetBinError(iBin);
    f4_1234 = h_f4_1234->GetBinContent(iBin);ef4_1234 =  h_f4_1234->GetBinError(iBin);
    calculateF4R4(f1_1,ef1_1,f1_2,ef1_2,f1_3,ef1_3,f1_4,ef1_4,
                  f2_12,ef2_12, f2_13,ef2_13, f2_14,ef2_14,
                  f2_23,ef2_23, f2_24,ef2_24, f2_34,ef2_34,
                  f3_123,ef3_123, f3_124,ef3_124, f3_134,ef3_134, f3_234,ef3_234,
                  f4_1234,ef4_1234,
                  F4_1234,eF4_1234,R4_1234,eR4_1234);
    h_F4_1234->SetBinContent(iBin, F4_1234);
    h_F4_1234->SetBinError(iBin,   eF4_1234);
    h_R4_1234->SetBinContent(iBin, R4_1234);
    h_R4_1234->SetBinError(iBin,   eR4_1234);
    }
}


void HistogramCollection::calculateF2R2(double f1_1,double ef1_1,double f1_2,double ef1_2, double f2_12,double ef2_12,
                                        double & F2_12,double & eF2_12, double &  R2_12,double & eR2_12)
{
  // calculate  F2_12 = f2_12 - f1_1*f1_2  and R2 = f2/f1_1/f1_2 - 1
  // errors are calculated assuming there are no correlations

  double ref1_1, ref1_2, ref2_12;
  if (f1_1<1E-20 || f1_2<1E-20)
    {
    F2_12  = 0.0;
    eF2_12 = 0.0;
    R2_12  = 0.0;
    eR2_12 = 0.0;
    }
  else
    {
    F2_12     = f2_12 - f1_1*f1_2;
    ref1_1    = ef1_1/f1_1;
    ref1_2    = ef1_2/f1_2;
    ref2_12   = ef2_12/f2_12;
    eF2_12    = sqrt(f1_1*f1_2*f1_1*f1_2*(ref1_1*ref1_1 + ref1_2*ref1_2) + eF2_12*eF2_12);
    R2_12     = F2_12/(f1_1*f1_2);
    eR2_12    = R2_12*sqrt(ref1_1*ref1_1 + ref1_2*ref1_2 + ref2_12*ref2_12);
    }
}


void HistogramCollection::calculateF3R3(double f1_1,double ef1_1,double f1_2,double ef1_2, double f1_3,double ef1_3,
                                        double f2_12,double ef2_12,double f2_13,double ef2_13,double f2_23,double ef2_23,
                                        double f3_123, double ef3_123,
                                        double & F3_123,double & eF3_123, double &  R3_123,double & eR3_123)
{
  // calculate  F2_12 = f2_12 - f1_1*f1_2  and R2 = f2/f1_1/f1_2 - 1
  // errors are calculated assuming there are no correlations

  double ref1_1, ref1_2, ref1_3, ref2_12, ref2_13, ref2_23, reF3_123;
  if (f1_1<1E-20 || f1_2<1E-20 || f1_3<1E-20)
    {
    F3_123   = 0.0;
    eF3_123  = 0.0;
    R3_123   = 0.0;
    eR3_123  = 0.0;
    }
  else
    {
    F3_123    = f3_123 - f2_12*f1_3 - f2_13*f1_2 - f2_23*f1_1 + 2.0*f1_1*f1_2*f1_3;
    ref1_1    = ef1_1/f1_1;
    ref1_2    = ef1_2/f1_2;
    ref1_3    = ef1_3/f1_3;
    ref2_12   = ef2_12/f2_12;
    ref2_13   = ef2_13/f2_13;
    ref2_23   = ef2_23/f2_23;
    eF3_123   = ef3_123*ef3_123;
    eF3_123   += f2_12*f2_12*f1_3*f1_3*(ref2_12*ref2_12 + ref1_3*ref1_3);
    eF3_123   += f2_13*f2_13*f1_2*f1_2*(ref2_13*ref2_13 + ref1_2*ref1_2);
    eF3_123   += f2_23*f2_23*f1_1*f1_1*(ref2_23*ref2_23 + ref1_1*ref1_1);
    eF3_123   += 4.0*f1_1*f1_1*f1_2*f1_2*f1_3*f1_3*sqrt(ref1_1*ref1_1+ref1_2*ref1_2+ref1_3*ref1_3);
    eF3_123   = sqrt(eF3_123);
    reF3_123   = eF3_123/F3_123;
    R3_123     = F3_123/(f1_1*f1_2*f1_3);
    eR3_123    = R3_123*reF3_123;
    }
}

void HistogramCollection::calculateF4R4(double f1_1,double ef1_1,double f1_2,double ef1_2, double f1_3,double ef1_3, double f1_4,double ef1_4,
                                        double f2_12,double ef2_12,double f2_13,double ef2_13,double f2_14,double ef2_14,double f2_23,double ef2_23,
                                        double f2_24,double ef2_24,double f2_34,double ef2_34,
                                        double f3_123, double ef3_123,double f3_124, double ef3_124, double f3_134, double ef3_134,double f3_234, double ef3_234,
                                        double f4_1234, double ef4_1234,
                                        double &F4_1234,double &eF4_1234, double &  R4_1234,double & eR4_1234)
{
  // calculate  F2_12 = f2_12 - f1_1*f1_2  and R2 = f2/f1_1/f1_2 - 1
  // errors are calculated assuming there are no correlations

  double ref1_1,  ref1_2,  ref1_3,  ref1_4;
  double ref2_12, ref2_13, ref2_14, ref2_23, ref2_24, ref2_34;
  double ref3_123, ref3_124, ref3_134, ref3_234;
  double ref4_1234;
  double reF4_1234;

  if (f1_1<1E-20 || f1_2<1E-20 || f1_3<1E-20 || f1_4<1E-20)
    {
    F4_1234   = 0.0;
    eF4_1234  = 0.0;
    R4_1234   = 0.0;
    eR4_1234  = 0.0;
    }
  else
    {

    F4_1234   = f4_1234 - f3_123*f1_4 - f3_124*f1_3 - f3_134*f1_2 - f3_234*f1_1
    - f2_12*f2_34 - f2_13*f2_24 - f2_14*f2_23
    + 4.0*(f2_12*f1_3*f1_4 + f2_13*f1_2*f1_4 + f2_14*f1_2*f1_3)
    - 6.0*f1_1*f1_2*f1_3*f1_4;
    ref1_1    = ef1_1/f1_1;
    ref1_2    = ef1_2/f1_2;
    ref1_3    = ef1_3/f1_3;
    ref1_4    = ef1_3/f1_4;
    ref2_12   = ef2_12/f2_12;
    ref2_13   = ef2_13/f2_13;
    ref2_14   = ef2_14/f2_14;
    ref2_23   = ef2_23/f2_23;
    ref2_24   = ef2_24/f2_24;
    ref2_34   = ef2_34/f2_34;
    ref3_123  = ef3_123/f3_123;
    ref3_124  = ef3_124/f3_124;
    ref3_134  = ef3_134/f3_134;
    ref3_234  = ef3_234/f3_234;
    ref4_1234 = ef4_1234/f4_1234;

    eF4_1234   = ef4_1234*ef4_1234
    + f3_123*f3_123*f1_4*f1_4*(ref3_123*ref3_123 + ef1_4*ef1_4)
    + f3_124*f3_124*f1_3*f1_3*(ref3_124*ref3_124 + ref1_3*ref1_3)
    + f3_134*f3_134*f1_2*f1_2*(ref3_134*ref3_134 + ref1_2*ref1_2)
    + f3_234*f3_234*f1_1*f1_1*(ref3_234*ref3_234 + ref1_1*ref1_1)
    + 16.0*f2_12*f2_12*f2_34*f2_34*(ref2_12*ref2_12 + ef2_34*ef2_34)
    + 16.0*f2_13*f2_13*f2_24*f2_24*(ref2_13*ref2_13 + ef2_24*ef2_24)
    + 16.0*f2_14*f2_14*f2_23*f2_23*(ref2_14*ref2_14 + ef2_23*ef2_23);
    // neglect singles term for now...
    eF4_1234   = sqrt(eF4_1234);
    reF4_1234  = 0.0; //eF4_1234/F4_1234;
    R4_1234    = F4_1234/(f1_1*f1_2*f1_3*f1_4);
    eR4_1234   = 0.0; //R4_1234*reF4_1234;
    }
}

void HistogramCollection::calculateNudyn(double r2_11,double er2_11,double r2_12,double er2_12,double r2_22,double er2_22,double & nudyn,double & enudyn)
{
  nudyn = r2_11 + r2_22 - 2.0*r2_12;
  enudyn = sqrt(er2_11*er2_11 + er2_22*er2_22 + 4.0*er2_12*er2_12);
}

