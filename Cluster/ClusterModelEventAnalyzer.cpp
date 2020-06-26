// Author: Claude Pruneau   09/25/2019

/***********************************************************************
 * Copyright (C) 2019, Claude Pruneau.
 * All rights reserved.
 * Based on the ROOT package and environment
 *
 * For the licensing terms see LICENSE.
 **********************************************************************/
/**
 \class Task
 \ingroup WAC

 Class defining Task
 */

#include "ClusterModelEventAnalyzer.hpp"
ClassImp(ClusterModelEventAnalyzer);


ClusterModelEventAnalyzer::ClusterModelEventAnalyzer(const TString &  name,
                                                     TaskConfiguration * configuration,
                                                     ClusterModelEvent * selectedEvent,
                                                     EventFilter       * eventFilter,
                                                     ParticleFilter    * pf)
:
Task(name,configuration, selectedEvent),
cmEvent(selectedEvent),
cmEventFilter(eventFilter),
particleFilter(pf)
{
  if (reportDebug())
    cout << "ClusterModelEventAnalyzer::ClusterModelEventAnalyzer(..) Started." << endl;
  if (!cmEvent && reportError())
    cout << "ClusterModelEventAnalyzer::ClusterModelEventAnalyzer(..) selectedEvent is a null pointer." << endl;
  if (reportDebug())
    cout << "ClusterModelEventAnalyzer::ClusterModelEventAnalyzer(..) Completed." << endl;
}


ClusterModelEventAnalyzer::~ClusterModelEventAnalyzer()
{
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::DTOR() No ops." << endl;
}


//////////////////////////////////////////////////////////////
// Initialize this analyzer
//////////////////////////////////////////////////////////////
void ClusterModelEventAnalyzer::createHistograms()
{
  if (reportDebug()) cout << "ClusterModelEventAnalyzer::createHistograms() Starting." << endl;
  AnalysisConfiguration * configuration = (AnalysisConfiguration * ) getTaskConfiguration();
  //event_Histos  = new ClusterModelEventHistos("Cluster Model Event Histos", configuration,getDebugLevel());
  cellHistos    = new CellHistos(   "Cells Histos",configuration,getReportLevel());
  clusterHistos = new ClusterHistos("Clusters Histos",configuration,getReportLevel());
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::createHistograms() Completed." << endl;
}

//////////////////////////////////////////////////////////////
// Load histograms from the given file
//////////////////////////////////////////////////////////////
void ClusterModelEventAnalyzer::loadHistograms(TFile * inputFile)
{
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::loadHistograms() Starting." << endl;
  AnalysisConfiguration * configuration = (AnalysisConfiguration * ) getTaskConfiguration();
  //this->ClusterModelEventAnalyzer::loadHistograms(inputFile);
  cellHistos    = new CellHistos(   inputFile,"CellsHistos",   configuration,getReportLevel());
  clusterHistos = new ClusterHistos(inputFile,"ClustersHistos",configuration,getReportLevel());
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::loadHistograms() Completed." << endl;
}

//////////////////////////////////////////////////////////////
// Save histograms to the given file
//////////////////////////////////////////////////////////////
void ClusterModelEventAnalyzer::saveHistograms(TFile * outputFile)
{
  if (reportDebug()) cout << "ClusterModelEventAnalyzer::saveHistograms(...) Saving Event histograms to given output file." << endl;
  outputFile->cd();
  cellHistos   ->saveHistograms(outputFile);
  clusterHistos->saveHistograms(outputFile);
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::saveHistograms(...) Completed." << endl;
}

//////////////////////////////////////////////////////////////
// Save histograms to the given file
//////////////////////////////////////////////////////////////

void ClusterModelEventAnalyzer::execute()
{
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::analyze(...) Starting" << endl;
  if (!cmEvent && reportError())
    {
    cout << "ClusterModelEventAnalyzer::execute() Pointer to Event is null." << endl;
    return;
    }
  if (!cmEventFilter->accept(*cmEvent)) return;
//  cmEvent->printProperties(cout);
  int nCells = cmEvent->nCells;
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::analyze(...) nCells: " << nCells << endl;
  for (int iCell=0; iCell<nCells; iCell++)
    {
    Cell * cell = cmEvent->getCellAt(iCell);
    cellHistos->fill(cell);
    for (int iCluster=0; iCluster<cell->nClusters; iCluster++)
      {
      clusterHistos->fill( cmEvent->getClusterAt(iCluster) );
      }
    }
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::analyze(...) Completed." << endl;
}

//////////////////////////////////////////////////////////////
// Scale all filled histograms by the given factor
// Derived histograms are *NOT* scaled.
//////////////////////////////////////////////////////////////
void ClusterModelEventAnalyzer::scale(double factor)
{
  if (reportDebug()) cout << "ClusterModelEventAnalyzer::scale(..) Scale all primary histograms by " << factor << endl;
  cellHistos    ->scale(factor);
  clusterHistos ->scale(factor);
  if (reportDebug())  cout << "ClusterModelEventAnalyzer::scale(..) Completed"  << endl;
}

  ClusterModelEvent * ClusterModelEventAnalyzer::getClusterModelEvent()
  {
  return cmEvent;
  }
