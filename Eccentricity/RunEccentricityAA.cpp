// /////////////////////////////////////////////////
//
// Eccentricity Model for AA collisions.
//
// C. Pruneau
// May 15, 2020.
// /////////////////////////////////////////////////
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "AnalysisConfiguration.hpp"
#include "EccentricityCalc.hpp"
#include "GeometryGenerator.hpp"
#include "OverlapFunction.hpp"
#include "EccentricityVsCent.hpp"
#include "TRandom.h"

int main()
{
  long   nEvents = 100000;
  double nucleusSize = 208.0;
  double nucleusRadius = 1.2*TMath::Power(nucleusSize,0.3333333);
  double nnCrossSection = 4.5; // in fm^2
  double nucleonRadius = sqrt(nnCrossSection/3.1415927);
  int    generationMode = 3;
  double radius = nucleusRadius;

  int    nX     = 500;
  double xMax   = 1.2*nucleusRadius;
  int    nB     = 40;
  double bMax   = 20.0;
  double dB     = bMax/double(nB);

  int nEcc = 100;
  double minEcc = -1.0;
  double maxEcc = 1.0;

  TString geometryName;
  int     geometryType;

  double  parA;
  double  parB;
  double  parC;

  for (int i=1;i<2;i++)
    {
    switch (i)
      {
        case 0: geometryName = "HardSphere";
        geometryType = 0;
        parA = radius*radius*radius;
        parB = 0.0;
        parC = 0.0;
        break;
        case 1: geometryName = "WoodsSaxon";
            geometryType = 1;
        parA = 6.5;//radius;
        parB = 0.535;
        parC = 0.0;
        break;
        case 2: geometryName = "WoodsSaxonHard";
            geometryType = 1;
        parA = radius;
        parB = 0.02;
        parC = 0.0;
        break;
        case 3: geometryName = "Exponential";
               geometryType = 2;
         radius = 0.20;
        parA = radius;
        parB = 0.0;
        parC = 0.0;
        break;
        case 4: geometryName = "Gaussian";
               geometryType = 3;
         radius = 0.5;
        parA = sqrt(radius);
        parB = 0.0;
        parC = 0.0;
        break;
        case 5: geometryName = "DoubleGaussian";
               geometryType = 4;
         radius = 0.5;
        parA = sqrt(0.1*radius);
        parB = sqrt(radius);
        parC = 0.2;
        break;
      }

    cout << "Eccentricity Study" << endl;
    cout << "generationMode : " << generationMode << endl;
    cout << "  geometryType : " << geometryType << endl;
    cout << "  geometryName : " << geometryName << endl;
    cout << "        radius : " << radius << endl;
    cout << "            nX : " << nX << endl;
    cout << "          xMax : " << xMax << endl;
    cout << "            nB : " << nB << endl;
    cout << "          bMax : " << bMax << endl;


    TFile * outputFile = new TFile(geometryName+".root","RECREATE");


    GeometryGenerator * geometryGenerator1 = new GeometryGenerator(geometryName+"1",
                                                                  nX, xMax,
                                                                  nX, xMax,
                                                                  geometryType, parA, parB, parC,
                                                                  nucleusSize,
                                                                  nucleusRadius,
                                                                  nnCrossSection,
                                                                  nucleonRadius,
                                                                  generationMode);
    GeometryGenerator * geometryGenerator2 = new GeometryGenerator(geometryName+"2",
                                                                   nX, xMax,
                                                                   nX, xMax,
                                                                   geometryType, parA, parB, parC,
                                                                   nucleusSize,
                                                                   nucleusRadius,
                                                                   nnCrossSection,
                                                                   nucleonRadius,
                                                                   generationMode);

    EccentricityCalc   ** eccCalc = new EccentricityCalc*[nB];
    EccentricityVsCent * eccVsB  = new EccentricityVsCent(geometryName,nX,-xMax,xMax,nX, -xMax,xMax,nB, 0.0, bMax);
    double b;

    // generate equiprobable b distribution for best statistics at all b
    for (int iB=0; iB<nB; iB++)
      {
      TString name = geometryName;
      name += "_b";
      name += iB;
      eccCalc[iB] = new EccentricityCalc(name,nX,-xMax,xMax, nX, -xMax,xMax, nEcc,minEcc, maxEcc, true);

      for (long iEvent=0; iEvent<nEvents; iEvent++)
        {
        b = double(iB)*dB + dB*gRandom->Rndm();
        geometryGenerator1->generateNucleus();
        geometryGenerator2->generateNucleus();
        eccCalc[iB]->fillWithNuclei(b, geometryGenerator1, geometryGenerator2);
        eccCalc[iB]->calculateAverages();
        eccCalc[iB]->fillAvg();
        eccVsB->fill(b,eccCalc[iB]);
        eccCalc[iB]->reset();
        }
      eccCalc[iB]->save();
      }
    geometryGenerator1->save();
    eccVsB->normalize();
    eccVsB->save();
    outputFile->Close();
    }


  return 0;
}

