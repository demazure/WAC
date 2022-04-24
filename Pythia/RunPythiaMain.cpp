#include "TSystem.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include "ParticleHistosMPA.hpp"
#include "ParticlePairHistosMPA.hpp"
#include "ParticleTripletHistosMPA.hpp"
#include "ParticleQuadHistosMPA.hpp"
#include "ParticleHistosMPADerived.hpp"
//#include "ParticlePairHistosMPADerived.hpp"
//#include "ParticleTripletHistosMPADerived.hpp"
//#include "ParticleQuadHistosMPADerived.hpp"
#include "AnalysisConfiguration.hpp"
#include "TPythia8.h"

int main()
{
  Int_t nev = 100;
  Int_t ndeb = 0;

  // Load libraries
  gSystem->Load("libEG");
  gSystem->Load("libEGPythia8");

  int nMax = 5000;
  // Array of particles
  TClonesArray* particles = new TClonesArray("TParticle", nMax);
  // Create pythia8 object
  TPythia8* pythia8 = new TPythia8();

#if PYTHIA_VERSION_INTEGER == 8235
  // Pythia 8.235 is known to cause crashes:
  printf("ABORTING PYTHIA8 TUTORIAL!\n");
  printf("The version of Pythia you use is known to cause crashes due to memory errors.\n");
  printf("They have been reported to the authors; the Pythia versions 8.1... are known to work.\n");
  return;
#endif

  // Configure
  pythia8->ReadString("HardQCD:all = on");
  pythia8->ReadString("Random:setSeed = on");
  // use a reproducible seed: always the same results for the tutorial.
  pythia8->ReadString("Random:seed = 42");
  // Initialize
  pythia8->Initialize(2212 /* p */, 2212 /* p */, 14000. /* TeV */);

  AnalysisConfiguration* ac = new AnalysisConfiguration();
  ac->outputPath = "../PythiaOutput/";
  ac->baseName = "Pythia";
  ac->configFileName = "Pythia.txt";
  ac->rootOutputFileName = "Pythia.root";
  cout << "================================================================" << endl;
  cout << "" << endl;
  cout << "         running for = " << ac->baseName << endl;
  cout << "" << endl;
  cout << "================================================================" << endl;

  ac->nBins_pt = 4;
  ac->min_pt = 0.2;
  ac->max_pt = 2.2;
  ac->nBins_eta = 10;
  ac->min_eta = -2.0;
  ac->max_eta = 2.0;
  ac->nBins_phi = 6;
  ac->min_phi = 0.0;
  ac->max_phi = 2.0 * 3.1415927;

  //  ac->nBins_DeltaPlong = 10;
  //  ac->min_DeltaPlong   = -1.0;
  //  ac->max_DeltaPlong   =  1.0;
  //  ac->nBins_DeltaPside = 10;
  //  ac->min_DeltaPside   = -1.0;
  //  ac->max_DeltaPside   =  1.0;
  //  ac->nBins_DeltaPout  = 10;
  //  ac->min_DeltaPout    = -1.0;
  //  ac->max_DeltaPout    =  1.0;

  //  ac->fillPairs        = true;
  //  ac->fill3D           = false;
  //  ac->fill6D           = false;
  //  ac->fillQ3D          = true;
  //  ac->fillY            = true;

  TFile* outputFile = new TFile(ac->rootOutputFileName, "RECREATE");

  ParticleHistosMPA* p = new ParticleHistosMPA("P", ac, "P", 1);
  ParticleHistosMPA* n = new ParticleHistosMPA("N", ac, "N", 1);
  ParticlePairHistosMPA* nn = new ParticlePairHistosMPA("NN", ac, "NN", 1);
  ParticlePairHistosMPA* pp = new ParticlePairHistosMPA("PP", ac, "PP", 1);
  ParticlePairHistosMPA* pn = new ParticlePairHistosMPA("PN", ac, "PN", 1);
  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX 3a XXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  ParticleTripletHistosMPA* ppp = new ParticleTripletHistosMPA("PPP", ac, "PPP", 0);
  ParticleTripletHistosMPA* ppn = new ParticleTripletHistosMPA("PPN", ac, "PPN", 0);
  ParticleTripletHistosMPA* pnn = new ParticleTripletHistosMPA("PNN", ac, "PNN", 0);
  ParticleTripletHistosMPA* nnn = new ParticleTripletHistosMPA("NNN", ac, "NNN", 0);
  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX 4 XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  ParticleQuadHistosMPA* pppp = new ParticleQuadHistosMPA("PPPP", ac, "PPPP", 1);
  ParticleQuadHistosMPA* pppn = new ParticleQuadHistosMPA("PPPN", ac, "PPPN", 0);
  ParticleQuadHistosMPA* ppnn = new ParticleQuadHistosMPA("PPNN", ac, "PPNN", 0);
  ParticleQuadHistosMPA* pnnn = new ParticleQuadHistosMPA("PNNN", ac, "PNNN", 0);
  ParticleQuadHistosMPA* nnnn = new ParticleQuadHistosMPA("NNNN", ac, "NNNN", 0);

  cout << " XXXXXXXXXXXXXXXXXXXXXXX 5 XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  // Event loop
  for (Int_t iev = 0; iev < nev; iev++) {
    pythia8->GenerateEvent();
    if (iev < ndeb)
      pythia8->EventListing();
    pythia8->ImportParticles(particles, "Final");
    Int_t np = particles->GetEntriesFast();
    // Particle loop

    cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
    cout << " XXXXXXXXXXXXXXXXXXXXX ANALYZE XXXXXXXXXXXXXXXXXXXXXXXX " << endl;
    Int_t ist1, ist2, ist3, ist4, pdg1, pdg2, pdg3, pdg4;
    Float_t charge1, charge2, charge3, charge4;
    TParticle* part1;
    TParticle* part2;
    TParticle* part3;
    TParticle* part4;
    // Positive codes are final particles.

    if (np > nMax) {
      cout << " ARRAY TOO SMALL - STOP -- np:" << np << " nMax:" << nMax << endl;
      exit(0);
    }

    for (Int_t iPart1 = 0; iPart1 < np; iPart1++) {
      TParticle& part1 = *(TParticle*)particles->At(iPart1);
      ist1 = part1.GetStatusCode();
      if (ist1 <= 0)
        continue;
      pdg1 = part1.GetPdgCode();
      charge1 = TDatabasePDG::Instance()->GetParticle(pdg1)->Charge();
      if (charge1 == 0.)
        continue;

      cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
      cout << " XXXXXXXXXXXXXXXXXXXXX Fill 1  XXXXXXXXXXXXXXXXXXXXXXXX " << endl;

      // fill singles
      if (charge1 > 0)
        p->fill(part1, 1.0);
      else
        n->fill(part1, 1.0);

      cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
      cout << " XXXXXXXXXXXXXXXXXXXXX Fill 2  XXXXXXXXXXXXXXXXXXXXXXXX " << endl;

      for (Int_t iPart2 = 0; iPart2 < iPart1; iPart2++) {
        // if (iPart2==iPart1) continue;
        TParticle& part2 = *(TParticle*)particles->At(iPart2);
        ist2 = part2.GetStatusCode();
        if (ist2 <= 0)
          continue;
        pdg2 = part2.GetPdgCode();
        charge2 = TDatabasePDG::Instance()->GetParticle(pdg2)->Charge();
        if (charge2 == 0.)
          continue;

        // fill pairs
        if (charge1 > 0) // px
        {
          if (charge2 > 0) // pp
          {
            pp->fill(part1, part2, 1.0);
          } else // pn
          {
            pn->fill(part1, part2, 1.0);
          }
        } else // nx
        {
          if (charge2 > 0) // np
          {
            pn->fill(part2, part1, 1.0);
          } else // nn
          {
            nn->fill(part1, part2, 1.0);
          }
        }
        // continue;
        // cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
        // cout << " XXXXXXXXXXXXXXXXXXXXX Fill 3  XXXXXXXXXXXXXXXXXXXXXXXX " << endl;

        for (Int_t iPart3 = 0; iPart3 < iPart2; iPart3++) {
          // if (iPart3==iPart1) continue;
          // if (iPart3==iPart2) continue;
          TParticle& part3 = *(TParticle*)particles->At(iPart3);
          ist3 = part3.GetStatusCode();
          if (ist3 <= 0)
            continue;
          pdg3 = part3.GetPdgCode();
          charge3 = TDatabasePDG::Instance()->GetParticle(pdg3)->Charge();
          if (charge3 == 0.)
            continue;

          // fill triplets
          if (charge1 > 0) // pxx
          {
            if (charge2 > 0) // ppx
            {
              if (charge3 > 0) // ppp
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  ppp->fill(part1,part2,part3,1.0)   ======" << endl;
                ppp->fill(part1, part2, part3, 1.0);
              } else // ppn
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  ppn->fill(part1,part2,part3,1.0)   ======" << endl;
                ppn->fill(part1, part2, part3, 1.0);
              }
            } else // pnx
            {
              if (charge3 > 0) // pnp
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  ppn->fill(part1,part3,part2,1.0);   ======" << endl;
                ppn->fill(part1, part3, part2, 1.0);
              } else // pnn
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  pnn->fill(part1,part2,part3,1.0);   ======" << endl;
                pnn->fill(part1, part2, part3, 1.0);
              }
            }
          } else // nxx
          {
            if (charge2 > 0) // npx
            {
              if (charge3 > 0) // npp
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  ppn->fill(part2,part3,part1,1.0)   ======" << endl;

                ppn->fill(part2, part3, part1, 1.0);
              } else // npn
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  pnn->fill(part2,part1,part3,1.0);  ======" << endl;
                pnn->fill(part2, part1, part3, 1.0);
              }
            } else // nnx
            {
              if (charge3 > 0) // nnp
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  pnn->fill(part3,part1,part2,1.0);  ======" << endl;
                pnn->fill(part3, part1, part2, 1.0);
              } else // nnn
              {
                // cout << " ================================================= " << endl;
                // cout << " ======  nnn->fill(part1,part2,part3,1.0);  ======" << endl;
                nnn->fill(part1, part2, part3, 1.0);
              }
            }
          }

          //
          //          // cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
          //          // cout << " XXXXXXXXXXXXXXXXXXXXX Fill 4  XXXXXXXXXXXXXXXXXXXXXXXX " << endl;
          //
          //          for (Int_t iPart4 = 0; iPart4 < iPart3; iPart4++)
          //            {
          //            //if (iPart4==iPart1) continue;
          //            //if (iPart4==iPart2) continue;
          //            //if (iPart4==iPart3) continue;
          //            TParticle & part4 = *(TParticle*) particles->At(iPart4);
          //            ist4 = part4.GetStatusCode();
          //            if (ist4 <= 0) continue;
          //            pdg4 = part4.GetPdgCode();
          //            charge4 = TDatabasePDG::Instance()->GetParticle(pdg4)->Charge();
          //            if (charge4 == 0.) continue;
          //
          //            // fill quads
          //            if (charge1>0)   // pxxx
          //              {
          //              if (charge2>0)  // ppxx
          //                {
          //                if (charge3>0)  // pppx
          //                  {
          //                  if (charge4>0) // pppp
          //                    {
          //                    pppp->fill(part1,part2,part3,part4,1.0);
          //                    }
          //                  else // pppn
          //                    {
          //                    pppn->fill(part1,part2,part3,part4,1.0);
          //                    }
          //                  }
          //                else // ppnx
          //                  {
          //                  if (charge4>0)  // ppnp
          //                    {
          //                    pppn->fill(part1,part2,part4,part3,1.0);
          //                    }
          //                  else  // ppnn
          //                    {
          //                    ppnn->fill(part1,part2,part3,part4,1.0);
          //                    }
          //                  }
          //                }
          //              else  // pnxx
          //                {
          //                if (charge3>0)  // pnpx
          //                  {
          //                  if (charge4>0) // pnpp
          //                    {
          //                    pppn->fill(part1,part4,part3,part2,1.0);
          //                    }
          //                  else // pnpn
          //                    {
          //                    ppnn->fill(part1,part3,part2,part4,1.0);
          //                    }
          //                  }
          //                else // pnnx
          //                  {
          //                  if (charge4>0) // pnnp
          //                    {
          //                    ppnn->fill(part1,part4,part2,part3,1.0);
          //                    }
          //                  else  // pnnn
          //                    {
          //                    pnnn->fill(part1,part2,part3,part4,1.0);
          //                    }
          //                  }
          //                }
          //              }
          //            else  // nxxx
          //              {
          //              if (charge2>0)  // npxx
          //                {
          //                if (charge3>0) // nppx
          //                  {
          //                  if (charge4>0)  // nppp
          //                    {
          //                    pppn->fill(part2,part3,part4,part1,1.0);
          //                    }
          //                  else  // nppn
          //                    {
          //                    ppnn->fill(part2,part3,part1,part4,1.0);
          //                    }
          //                  }
          //                else // if (charge3<0)
          //                  {
          //                  if (charge4>0)
          //                    {
          //                    ppnn->fill(part2,part4,part1,part3,1.0);
          //                    }
          //                  else // if (charge4<0)
          //                    {
          //                    pnnn->fill(part2,part1,part3,part4,1.0);
          //                    }
          //                  }
          //                }
          //              else  // nnxx
          //                {
          //                if (charge3>0)  // nnpx
          //                  {
          //                  if (charge4>0)  // nnpp
          //                    {
          //                    ppnn->fill(part3,part4,part1,part2,1.0);
          //                    }
          //                  else  // nnpn
          //                    {
          //                    pnnn->fill(part3,part1,part2,part4,1.0);
          //                    }
          //                  }
          //                else // nnnx
          //                  {
          //                  if (charge4>0)  // nnnp
          //                    {
          //                    pnnn->fill(part2,part3,part4,part1,1.0);
          //                    }
          //                  else  // nnnn
          //                    {
          //                    nnnn->fill(part1,part2,part3,part4,1.0);
          //                    }
          //                  }
          //                }
          //              }
          //
          //            } // iPart4
        } // iPart3
      }   // iPart2
    }     // iPart1
  }       // event loop

  pythia8->PrintStatistics();

  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX 1 XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  double scale = 1.0 / double(nev);
  p->scale(scale, true);
  n->scale(scale, true);

  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX 2 pp XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  pp->scale(scale, true);

  cout << " XXXXXXXXXXXXXXXXXXXXXXX 2 pn XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  pn->scale(scale, true);

  cout << " XXXXXXXXXXXXXXXXXXXXXXX 2 nn XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  nn->scale(scale, true);

  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX 3 XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  ppp->scale(scale, true);
  ppn->scale(scale, true);
  pnn->scale(scale, true);
  nnn->scale(scale, true);

  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX 4 XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  //  pppp->scale(scale,true);
  //  pppn->scale(scale,true);
  //  ppnn->scale(scale,true);
  //  pnnn->scale(scale,true);
  //  nnnn->scale(scale,true);

  cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;
  cout << " XXXXXXXXXXXXXXXXXXXXXXX save XXXXXXXXXXXXXXXXXXXXXXXXXXXX " << endl;

  //  ParticleHistosMPADerived * pd = new ParticleHistosMPADerived("P",ac,"P",1);
  //  ParticleHistosMPADerived * nd = new ParticleHistosMPADerived("N",ac,"N",1);
  //  pd->calculateFrom(p);
  //  nd->calculateFrom(n);
  //
  //
  //  ParticlePairHistosMPADerived * nnd = new ParticlePairHistosMPADerived("NN",ac,"NN",1);
  //  ParticlePairHistosMPADerived * ppd = new ParticlePairHistosMPADerived("PP",ac,"PP",1);
  //  ParticlePairHistosMPADerived * pnd = new ParticlePairHistosMPADerived("PN",ac,"PN",1);
  //
  //  nnd->calculateDerivedHistograms(nd,nd,nn);
  //  ppd->calculateDerivedHistograms(pd,pd,pp);
  //  pnd->calculateDerivedHistograms(pd,nd,pn);

  p->saveHistograms(outputFile, true);
  n->saveHistograms(outputFile, true);
  pp->saveHistograms(outputFile, true);
  pn->saveHistograms(outputFile, true);
  nn->saveHistograms(outputFile, true);
  //  pd->saveHistograms(outputFile,true);
  //  nd->saveHistograms(outputFile,true);
  //  ppd->saveHistograms(outputFile,true);
  //  pnd->saveHistograms(outputFile,true);
  //  nnd->saveHistograms(outputFile,true);
  ppp->saveHistograms(outputFile, true);
  ppn->saveHistograms(outputFile, true);
  pnn->saveHistograms(outputFile, true);
  nnn->saveHistograms(outputFile, true);
  //  pppp->saveHistograms(outputFile,true);
  //  pppn->saveHistograms(outputFile,true);
  //  ppnn->saveHistograms(outputFile,true);
  //  pnnn->saveHistograms(outputFile,true);
  //  nnnn->saveHistograms(outputFile,true);

  outputFile->Close();

  return 0;
}
