// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class GeometryGenerator
 \ingroup WAC

 Class defining GeometryGenerator
 */

#include "GeometryGenerator.hpp"


ClassImp(GeometryGenerator);

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////
GeometryGenerator::GeometryGenerator(const TString & _generatorBaseName,
                                     int _nR, double _maxR,
                                     int _nX, double _maxX,
                                     int _type, double _parA, double _parB, double _parC,
                                     double _nucleusSize,
                                     double _nucleusRadius,
                                     double _nnCrossSection,
                                     double _nucleonRadius,
                                     long _nEvents, double _crossSection)
:
generatorBaseName(_generatorBaseName),
generatorName(_generatorBaseName),
nR(_nR),
maxR(_maxR),
nX(_nX),
maxX(_maxX),
type(_type),
parA(_parA),
parB(_parB),
parC(_parC),
rmsRadius(0.0),
nucleusSize(_nucleusSize),
nucleonnucleonCrossSection(_nnCrossSection), // in fm^2
nucleonRadius(_nucleonRadius),
nucleusRadius(_nucleusRadius),
rho0(0),
volume(0),
avgR(0),
rmsR(0),
rDensity(0),
rProfile(0),
rProfileGen(0),
xyThickness(0),
thicknessVsR(0),
nEvents(_nEvents),
crossSection(_crossSection),
points(0)
{
  createHistograms();
}


GeometryGenerator::GeometryGenerator(const TString & _generatorBaseName,
                                     int _nR, double _maxR,
                                     int _nX, double _maxX,
                                     int _type, double _parA, double _parB, double _parC,
                                     double _nucleusSize,
                                     double _nucleusRadius,
                                     double _nnCrossSection,
                                     double _nucleonRadius,
                                     int _generationMode)
:
generatorBaseName(_generatorBaseName),
generatorName(_generatorBaseName),
nR(_nR),
maxR(_maxR),
nX(_nX),
maxX(_maxX),
type(_type),
parA(_parA),
parB(_parB),
parC(_parC),
rmsRadius(0.0),
nucleusSize(_nucleusSize),
nucleonnucleonCrossSection(_nnCrossSection), // in fm^2
nucleonRadius(_nucleonRadius),
nucleusRadius(_nucleusRadius),
maxDistanceSq(_nnCrossSection/3.1415927),
volume(0),
avgR(0),
rmsR(0),
rDensity(0),
rProfile(0),
rProfileGen(0),
xyThickness(0),
nEvents(0),
crossSection(0.0),
generationMode(_generationMode),
nParticles(0),
points(0)
{
  createHistograms();
  if (generationMode<1000)
    {
    nParticles = generationMode*nucleusSize;
    }
  else // exclude nucleon in close proximity
    {
    nParticles = (generationMode-1000)*nucleusSize;
    }

  points = new GeometryPoint*[nParticles];
  long generated = 0;
  while (generated<nParticles)
    {
    points[generated] = new GeometryPoint();
    }
}

void GeometryGenerator::createHistograms()
{
  double r, r2, dr, density, profile,sum;
  double sumProfile;
  double sumProfileR1;
  double sumProfileR2;

  int  nSteps = 10000;
  TString name;
  name = generatorBaseName + "_rDensity";
  rDensity = new TH1D(name,name,nSteps,0.0, maxR);
  name = generatorBaseName + "_rProfile";
  rProfile = new TH1D(name,name,nSteps,0.0, maxR);
  name = generatorBaseName + "_rProfileGen";
  rProfileGen = new TH1D(name,name,nR, 0.0, maxR);
  name = generatorBaseName + "_xyThickness";
  xyThickness = new TH2D(name,name,nX,-maxX, maxX, nX,-maxX, maxX);
  name = generatorBaseName + "_thicknessVsR";
  thicknessVsR = new TH1D(name,name,nR,0.0, maxR);

  dr = maxR/double(nSteps);
  r  = dr/2.0;
  sum          = 0.0;
  sumProfile   = 0.0;
  sumProfileR1 = 0.0;
  sumProfileR2 = 0.0;

  for (int iR=0; iR<nSteps; iR++)
    {
    r2 = r*r;
    switch (type)
      {
        case 0: // uniform hard sphere
        if (r*r*r<parA)
          density = 1.0;
        else
          density = 0.0;
        break;
        case 1: // Woods-Saxon/Fermi
        density = 1.0/(1.0+exp((r-parA)/parB) );
        break;
        case 2: // exponential
        density = exp(-r/parA);
        break;
        case 3: // gaussian
        density = exp(-r2/2.0/parA/parA);
        break;
        case 4: //double-gaussian
        density =  (1.0-parC)*exp(-r2/parA/parA)/parA/parA/parA;
        density += parC*exp(-r2/parB/parB)/parB/parB/parB;
        break;
      }
    profile = r2*density;
    sum     += dr*profile;
    sumProfile   += profile;
    sumProfileR1 += profile*r;
    sumProfileR2 += profile*r*r;
    rDensity->SetBinContent(iR+1,density);
    rDensity->SetBinError(iR+1,0.0);
      //cout << "ir:"<< iR << " r:" << r << " density:" << rDensity->GetBinContent(iR+1) << endl;

    rProfile->SetBinContent(iR+1, profile);
    rProfile->SetBinError(iR+1,0.0);
    r += dr;
    }
  double norm = 4.0*3.1415927*sum;
  rDensity->Scale(nucleusSize/norm);
  rho0   = nucleusSize/norm;
  avgR   = sumProfileR1/sumProfile;
  rmsR   = sqrt(sumProfileR2/sumProfile);
  cout << " nucleusSize=" << nucleusSize << endl;
  cout << "        rho0=" << rho0 << endl;
  cout << "     Counts =" << nSteps << endl;
  cout << "         dr =" << dr << endl;
  cout << "        sum =" << sum << endl;
  cout << "    4pi*sum =" << norm << endl;
  cout << " avg Radius =" << avgR << endl;
  cout << " RMS Radius =" << rmsR << endl;

}


/// Generate point
void GeometryGenerator::generatePoint(GeometryPoint & point)
{
  double r;
  double cosTheta;
  double phi;
  cosTheta = -1 + 2.0*gRandom->Rndm();
  phi      = 2.0*3.1415927*gRandom->Rndm();
  r = rProfile->GetRandom();
  point.setRCosThetaPhi(r,cosTheta,phi);
  rProfileGen->Fill(r);
  xyThickness->Fill(point.x,point.y);
}

void GeometryGenerator::generateThicknessFunction()
{
  double b, z;
  double r = 0;
  double sum;

  int nZbins = rDensity->GetNbinsX();
  double zMaximum = rDensity->GetXaxis()->GetXmax();
  double dz = zMaximum/nZbins;

  int nBbins = thicknessVsR->GetNbinsX();
  double bMaximum = thicknessVsR->GetXaxis()->GetXmax();
  double db=bMaximum/double(nBbins);

  for (int iB=0; iB<nBbins; iB++)
    {
    b = double(iB)*db;
    sum = 0.0;
    for (int iZ=0; iZ<nZbins; iZ++)
      {
      z = double(iZ)*dz;
      r = sqrt(b*b+z*z);
      int iR = rDensity->GetXaxis()->FindFixBin(r);
      if (iR<1 || iR>nZbins) continue; // fct is zero here...
      double density = rDensity->GetBinContent(iR);
      //cout << "iB:" << iB << " b:" << b << " iZ:" << iZ << " z:" << z << " iR:" << iR  << " r:" << r << " density:" << density << endl;
      sum += 2.0*dz*density;  // include the positive and negative sides 
      }
    thicknessVsR->SetBinContent(iB,sum);
    thicknessVsR->SetBinError(iB,0.0);
    }
}

void GeometryGenerator::calculateMoments()
{
//  geometryMoments.reset();
//  int    nSteps    = rDensity->GetNbinsX();
//  double maxRadius = rDensity->GetXaxis()->GetXmax();
//
//  double dr=maxRadius/double(nSteps);
//  double r = 0.0;
//  double sum = 0.0;
//  double sumR1 = 0.0;
//  double sumR2 = 0.0;
//  double sumR3 = 0.0;
//  double sumR4 = 0.0;

//  for (int iR = 0; iR < nSteps; iR++)
//    {
//    r = double(iR)*dr;
//    double w = rDensity->GetBinContent(iR);
//    sum   += dr*w;
//    sumR1 += dr*r*w;
//    sumR2 += dr*r*r*w;
//    sumR3 += dr*r*r*r*w;
//    sumR4 += dr*r*r*r*r*w;
//    }
//  avgDensity =
//  volume = 4.0*3.1415927*sum;
//  avgR   = sumR1/sum;
//  rmsR   = sqrt(sumR2/sum);
//  cout << "     Counts =" << nR << endl;
//  cout << "     Volume =" << volume << endl;
//  cout << " avg Radius =" << avgR << endl;
//  cout << " RMS Radius =" << rmsR << endl;
}

void GeometryGenerator::generate()
{
  GeometryPoint point;
  //double dr=maxR/double(nR);

  for (long iEvent = 0; iEvent < nEvents; iEvent++)
    {
    generatePoint(point);
    //int iR = rDensity->GetXaxis()->FindFixBin(point.r);
    //if (iR<1 || iR>nR) continue; // fct is zero here...
    //double w = rDensity->GetBinContent(iR);
//    geometryMoments.fill(point.r,w);
    }

  double scaleR = 1.0/double(nEvents)/(maxR/double(nR));
  rProfileGen->Scale(scaleR);
  //double scaleT = 1.0/double(nEvents)/(2.0*maxX/double(nX))/(2.0*maxX/double(nX));
  double scaleT = double(nX)*double(nX)/double(nEvents);
  xyThickness->Scale(scaleT);
}

void GeometryGenerator::generateNucleus()
{
  int    generated = 0;

  while (generated<nParticles)
    {
    points[generated] = new GeometryPoint();
    generatePoint(*points[generated]);
    geometryMoments.fill(points[generated]->r,1.0);
    generated++;
    //cout << " generated: " << generated << " " << xx << " " << yy << " " << zz << endl;
    }
  cout << " Max radius =" << geometryMoments.rMax << endl;
  cout << "     Counts =" << geometryMoments.counts << endl;
  cout << "     Volume =" << geometryMoments.volume << endl;
  cout << " RMS Radius =" << geometryMoments.rmsR << endl;
}

void GeometryGenerator::save()
{

  rDensity->Write();
  rProfile->Write();
  rProfileGen->Write();
  thicknessVsR->Write();
  xyThickness->Write();
}
