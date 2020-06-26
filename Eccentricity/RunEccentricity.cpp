#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "AnalysisConfiguration.hpp"
#include "EccentricityCalc.hpp"
#include "GeometryGenerator.hpp"
#include "OverlapFunction.hpp"
#include "EccentricityVsCent.hpp"

int main()
{
  //double radius = 6.38;
  double radius = 6.5;

  int    nX     = 500;
  double xMax   = 10.0;
  int    nB     = 60;
  double bMax   = 20.0;// *radius;


  double crossSection = 3.1415927*radius*radius;
  long   nEvents = 4000000;
  TString geometryName;
  //int     geometryType = 1;
  double  parA;
  double  parB;
  double  parC;
  int     geometryType;

  double nucleusSize   = 197.0;
  double nucleusRadius = radius;
  double nnCrossSection = crossSection;
  double nucleonRadius  = 1.0;


  for (int i=6;i<8;i++)
    {
    switch (i)
      {
        // ////////////////////////////////////////////
        // nucleon scattering...
        // ////////////////////////////////////////////
          case 0: geometryName = "HardSphere";
        geometryType = 0;
        radius = 0.89;
        parA = radius*radius*radius;
        parB = 0.0;
        parC = 0.0;
        break;
        case 1: geometryName = "WoodsSaxonSoft";
            geometryType = 1;
        radius = 0.56;
        parA = radius;
        parB = 0.112;
        parC = 0.0;
        break;
        case 2: geometryName = "WoodsSaxonHard";
            geometryType = 1;
        radius = 0.89;
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

        // ////////////////////////////////////////////
        // au scattering...
        // ////////////////////////////////////////////
        case 6: geometryName = "AuHardSphere";
        geometryType = 0;
        radius = 6.65;
        nucleusSize   = 197.0;
        nucleusRadius = radius;
        nnCrossSection = crossSection;
        nucleonRadius  = 1.0;
        parA = radius*radius*radius;
        parB = 0.0;
        parC = 0.0;
        break;
        case 7:
        geometryName = "AuWoodsSaxon";
        geometryType = 1;
        radius = 6.5;
        nucleusSize   = 197.0;
        nucleusRadius = radius;
        nnCrossSection = crossSection;
        nucleonRadius  = 1.0;
        parA = radius;
        parB = 0.535;
        parC = 0.0;
        break;
      }

    cout << "Eccentricity Study" << endl;
    cout << "   geometryType : " << geometryType << endl;
    cout << "    geometryName : " << geometryName << endl;
    cout << "         nEvents : " << nEvents << endl;
    cout << "          radius : " << radius << endl;
    cout << "              nX : " << nX << endl;
    cout << "            xMax : " << xMax << endl;
    cout << "              nB : " << nB << endl;
    cout << "            bMax : " << bMax << endl;
    cout << "    crossSection : " << crossSection << endl;
    cout << "     nucleusSize : " << nucleusSize << endl;
    cout << "   nucleusRadius : " << nucleusRadius << endl;
    cout << "    volume (hard): " << 4.0*3.1415927*nucleusRadius*nucleusRadius*nucleusRadius/3.0 << endl;
    cout << "rms radius (hard): " << sqrt(3.0*nucleusRadius*nucleusRadius/5.0) << endl;
    cout << "  nnCrossSection : " << nnCrossSection << endl;
    cout << "   nucleonRadius : " << nucleonRadius << endl;


    TFile * outputFile = new TFile(geometryName+".root","RECREATE");


    GeometryGenerator * geometryGenerator = new GeometryGenerator(geometryName,
                                                                  nX, xMax,
                                                                  nX, xMax,
                                                                  geometryType, parA, parB, parC,
                                                                  nucleusSize,
                                                                  nucleusRadius,
                                                                  nnCrossSection,
                                                                  nucleonRadius,
                                                                  nEvents, crossSection);
    geometryGenerator->generate();
    geometryGenerator->generateThicknessFunction();
    geometryGenerator->save();
    EccentricityCalc   * eccCalc = new EccentricityCalc(geometryName,nX,-xMax,xMax, nX, -xMax,xMax, 50, -1.0, 1.0, false);
    EccentricityVsCent * eccVsB  = new EccentricityVsCent(geometryName,nX,-xMax,xMax,nX, -xMax,xMax,nB, 0.0, 1.5);

    OverlapFunction *  overlapFunction = new  OverlapFunction(geometryName,nB,bMax,nX,xMax,geometryGenerator->rmsRadius,eccCalc,eccVsB,true);
    overlapFunction->calculateOverlap(geometryGenerator->xyThickness,geometryGenerator->xyThickness);
    overlapFunction->calculateProbability(1.0);
    overlapFunction->save();
    eccCalc->save();
    eccVsB->save();
    outputFile->Close();
    }


  return 0;
}

