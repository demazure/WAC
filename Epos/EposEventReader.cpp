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
#include "EposEventReader.hpp"
ClassImp(EposEventReader);

EposEventReader::EposEventReader(const TString & name,
                                 TaskConfiguration * configuration,
                                 Event * event,
                                 EventFilter * ef,
                                 ParticleFilter * pf)
:
Task(name, configuration, event),
eventFilter(ef),
particleFilter(pf)
{
  if (reportDebug()) cout << "EposEventReader::EposEventReader(...) No ops" << endl;
}

EposEventReader::~EposEventReader()
{
if (reportDebug()) cout << "EposEventReader::~EposEventReader(...) No ops" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EposEventReader::initialize()
{
  if (reportDebug()) cout << "EposEventReader::initialize() Started" << endl;

  TaskConfiguration * genConfig = getTaskConfiguration();
   if (!genConfig)
     {
     if (reportFatal()) cout << "EposEventReader::initialize() genConfig is a null pointer" << endl;
     postTaskFatal();
     return;
     }
   TChain *chain = new TChain(genConfig->dataInputTreeName," ");
   if (!chain)
      {
      if (reportFatal()) cout << "EposEventReader::initialize() chain is a null pointer" << endl;
      postTaskFatal();
      return;
      }

   TString dataInputFileName = genConfig->dataInputPath;
   dataInputFileName += "/";
   dataInputFileName += genConfig->dataInputFileName;

   for(int ifl=genConfig->dataInputFileMinIndex; ifl<genConfig->dataInputFileMaxIndex; ifl++)
     {
     //chain->Add(Form("/local/victor/PROJECTS/EPOSWSU/DATA/epos_pbpb_urqmd_on_%i.root",ifl+1));
     //chain->Add(Form("/Users/sumit/Desktop/macros_epos/root_maker/epos_pbpb_urqmd_on_%i.root",ifl+1));
     cout << "EposEventReader::initialize() Form:" << Form(dataInputFileName,ifl) << endl;
     chain->Add(Form(dataInputFileName,ifl));
     }
  Init(chain);
  jentry = 0;

  nentries = fChain->GetEntries();
  if (reportInfo()) cout << "EposEventReader::initialize() nEntries: " << nentries << endl;
  if (nentries < 1)
    {
    if (reportError()) cout << "EposEventReader::initialize() no data found. Abort." << endl;
    postTaskFatal();
    return;
    }
  nbytes = 0;
  nb = 0;
  if (reportDebug()) cout << "EposEventReader::initialize() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset and Initialize the generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EposEventReader::reset()
{
  if (reportDebug()) cout << "EposEventReader::reset() Started" << endl;
  event->reset();
  Particle::getFactory()->reset();
  if (reportDebug()) cout << "EposEventReader::reset() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read an ampt event from file
// Copy the event into Event for convenience...
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EposEventReader::execute()
{
  if (reportDebug()) cout << "EposEventReader::execute() Started" << endl;

  if (!fChain)
    {
    if (reportFatal()) cout << " EposEventReader::execute() no TChain available" << endl;
    postTaskFatal();
    return;
    }

  Factory<Particle> * particleFactory = Particle::getFactory();

  bool seekingEvent = true;
  while (seekingEvent)
    {
    // load another event from the root file/TTree
    Long64_t ientry = LoadTree(jentry++);

    // returning a null point is an indication that
    // there are no more events in the file or stack of files.
    if (ientry < 0)
      {
      postTaskEod(); // end of data
      return;
      }
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //cout<<"sumit : "<<Mult<<endl;
    // check if the event is acceptable.
    event->impactParameter = impact;
    if (eventFilter->accept(*event)) seekingEvent = false;
    }

  if (mult > arraySize)
    {
    if (reportError()) cout<< "EposEventReader::execute() n particles is " << mult << " and exceeds capacity " << arraySize << endl;
    postTaskError();
    return;
    }
  if (reportDebug())  cout<< "EposEventReader::execute() Impact par: "<<impact<<"  "<<mult<<endl;

  int thePid;
  double charge, p_x, p_y, p_z, p_e, mass;
  Particle * particle;
  int particleAccepted = 0;

  //------------------- Randomizing the particle phi --------------Starts
  double eventAngle= TMath::TwoPi() * gRandom->Rndm();
  double cosPhi = cos(eventAngle);
  double sinPhi = sin(eventAngle);

  // load particles from TTree and copy those that are selected into
  // event
  int iParticle=0;
  bool readingEvent = true;
  while (readingEvent)
    {
    particle = particleFactory->getNextObject();
    bool seekParticle = true;
    while (seekParticle)
      {
      thePid = pid[iParticle];
      if ( thePid == 211 || thePid == 321  || thePid ==2212 )
        charge = 1;
      else if (thePid ==-211 || thePid ==-321 || thePid==-2212)
        charge = -1;
      else
        charge = 0;
      // only accept charged particles
      if (charge==0) continue;
      p_x  = cosPhi*px[iParticle] - sinPhi*py[iParticle];
      p_y  = sinPhi*px[iParticle] + cosPhi*py[iParticle];
      p_z  = pz[iParticle];
      p_e  = e[iParticle];;
      particle->setPidPxPyPzE(thePid, charge, p_x,p_y,p_z,p_e);
      if (particleFilter->accept(*particle)) seekParticle = false;
      if (reportDebug()) particle->printProperties(cout);
      particleAccepted++;
      iParticle++;
      if (iParticle>=mult)
        {
        seekParticle = false;
        readingEvent = false;
        }
      }
    }
  event->nParticles = particleAccepted;
  if (reportDebug()) cout << "EposEventReader::execute() No of accepted Particles : "<< particleAccepted<<endl;
  if (reportDebug()) cout << "EposEventReader::execute() event completed!" << endl;
}


Int_t EposEventReader::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t EposEventReader::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void EposEventReader::Init(TTree *tree)
 {
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Events", &events, &b_Events);
   fChain->SetBranchAddress("Mult", &mult, &b_Mult);
   fChain->SetBranchAddress("Impact", &impact, &b_Impact);
   fChain->SetBranchAddress("PID", pid, &b_PID);
   fChain->SetBranchAddress("Px", px, &b_Px);
   fChain->SetBranchAddress("Py", py, &b_Py);
   fChain->SetBranchAddress("Pz", pz, &b_Pz);
   fChain->SetBranchAddress("E", e, &b_E);
   Notify();


   nentries = fChain->GetEntriesFast();
   nbytes = 0;
   nb = 0;

 }


Bool_t EposEventReader::Notify()
{
  return kTRUE;
}

void EposEventReader::Show(Long64_t entry)
{
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t EposEventReader::Cut(Long64_t entry)
{
  return 1;
}


