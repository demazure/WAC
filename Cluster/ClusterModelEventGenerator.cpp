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

 Class defining ClusterModelEventGenerator
 */

#include "ClusterModelEventGenerator.hpp"
ClassImp(ClusterModelEventGenerator);

//extern Factory<Particle> * particleFactory;
//extern Factory<Cluster> *  clusterFactory;
//extern Factory<Cell> * cellFactory;

ClusterModelEventGenerator::ClusterModelEventGenerator(const TString &  name,
                                                       TaskConfiguration * configuration,
                                                       ClusterModelEvent * event)
:
Task(name, configuration,event),
cmEvent(event)
{
  if (reportDebug())  cout << "ClusterModelEventGenerator::ClusterModelEventGenerator(...)" << endl;
}

ClusterModelEventGenerator::~ClusterModelEventGenerator()
{
   if (reportDebug())  cout << "ClusterModelEventGenerator::~ClusterModelEventGenerator(...)" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClusterModelEventGenerator::initialize()
{
  pionMass =0.139;
  if (reportDebug())  cout << "ClusterModelEventGenerator::initialize() Started" << endl;

  for (int k=0; k<100; k++)
    {
    masses[k]   = pionMass;
    massesSq[k] = pionMass*pionMass;
    }

  ClusterModelConfiguration * configuration = (ClusterModelConfiguration * ) getTaskConfiguration();
  TRandom * randomGenerator = getRandomGenerator();

  if (!randomGenerator)
    {
    if (reportFatal())  cout << "ClusterModelEventGenerator::initialize() randomGenerator is a null pointer " << endl;
    return;
    }


  // ---------------------------------------------
  // Cell Multiplicity Generator
  // ---------------------------------------------
  switch (configuration->cellMultiplicity_Random)
    {
      default:
      case 0: // fixed velocity
      cellMultiplicityGenerator = new FixedScalarIntRandomGenerator(configuration->cellMultiplicity_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellMultiplicityGenerator set to FixedScalarIntRandomGenerator " << endl;
      break;
      case 1: // flat profile
      cellMultiplicityGenerator = new ScalarIntRandomGenerator(configuration->cellMultiplicity_Minimum,
                                                               configuration->cellMultiplicity_Maximum,
                                                               randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellMultiplicityGenerator set to ScalarIntRandomGenerator " << endl;
      break;
      case 2: // exp profile
      cellMultiplicityGenerator = new ExpScalarIntRandomGenerator(configuration->cellMultiplicity_Minimum,
                                                                  configuration->cellMultiplicity_Slope,
                                                                  randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellMultiplicityGenerator set to ExpScalarIntRandomGenerator " << endl;
      break;
    }

  // ---------------------------------------------
  // Cell Radial Velocity Generator
  // ---------------------------------------------
  switch (configuration->cellBeta_Random)
    {
      default:
      case 0: // fixed velocity
      cellBetaGenerator = new FixedScalarDoubleRandomGenerator(configuration->cellBeta_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellBetaGenerator set to FixedScalarDoubleRandomGenerator " << endl;
      break;
      case 1: // flat profile
      cellBetaGenerator = new ScalarDoubleRandomGenerator(configuration->cellBeta_Minimum,
                                                          configuration->cellBeta_Maximum,
                                                          randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellBetaGenerator set to ScalarDoubleRandomGenerator " << endl;

      break;
      case 2: // linear profile
      cellBetaGenerator = new LinearScalarDoubleRandomGenerator(configuration->cellBeta_Minimum,
                                                                configuration->cellBeta_Maximum,
                                                                randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellBetaGenerator set to LinearScalarDoubleRandomGenerator " << endl;
      break;
      //case 3: // histogram profile //
      //cellBetaGenerator = new HistoScalarDoubleRandomGenerator(configuration->histogram);
      //break;
    }

  switch (configuration->cellPhi_Random)
    {
      default:
      case 0: // fixed phi
      cellPhiGenerator = new FixedScalarDoubleRandomGenerator(configuration->cellPhi_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellPhiGenerator set to FixedScalarDoubleRandomGenerator " << endl;
      break;
      case 1: // flat phi profile
      cellPhiGenerator = new ScalarDoubleRandomGenerator(configuration->cellPhi_Minimum,
                                                         configuration->cellPhi_Maximum,
                                                         randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() cellPhiGenerator set to ScalarDoubleRandomGenerator " << endl;
      break;
      //case 2: // histogram profile
      //cellPhiGenerator = new HistoScalarDoubleRandomGenerator(configuration->histogram);
      //break;
    }

  // select cluster multiplicity generator
  switch (configuration->clusterMultiplicity_Random)
    {
      case 0: // fixed
      clusterMultiplicityGenerator = new FixedScalarIntRandomGenerator(configuration->clusterMultiplicity_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterMultiplicityGenerator set to FixedScalarIntRandomGenerator " << endl;
      break;
      case 1: // random flat
      clusterMultiplicityGenerator = new ScalarIntRandomGenerator(configuration->clusterMultiplicity_Minimum,
                                                                  configuration->clusterMultiplicity_Maximum,
                                                                  randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterMultiplicityGenerator set to ScalarIntRandomGenerator " << endl;
      break;
      case 2: // random exp
      clusterMultiplicityGenerator = new ExpScalarIntRandomGenerator(configuration->clusterMultiplicity_Minimum,
                                                                     configuration->clusterMultiplicity_Slope,
                                                                     randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterMultiplicityGenerator set to ExpScalarIntRandomGenerator " << endl;
      break;
    }


  // select rapidity generator
  switch (configuration->clusterRapidity_Random)
    {
      case 0: // fixed
      clusterRapidityGenerator = new FixedScalarDoubleRandomGenerator(configuration->clusterRapidity_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterRapidityGenerator set to FixedScalarDoubleRandomGenerator " << endl;
      break;
      case 1: // uniform rapidity
      clusterRapidityGenerator = new ScalarDoubleRandomGenerator(configuration->clusterRapidity_Minimum,
                                                                 configuration->clusterRapidity_Maximum,
                                                                 randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterRapidityGenerator set to ScalarDoubleRandomGenerator " << endl;
      break;
      case 2: // exponential
      clusterRapidityGenerator = new ExpScalarDoubleRandomGenerator(configuration->clusterRapidity_Minimum,
                                                                    configuration->clusterRapidity_Slope,
                                                                    randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterRapidityGenerator set to ExpScalarDoubleRandomGenerator " << endl;
      break;
    }

  // select mass generator
  switch (configuration->clusterMass_Random)
    {
      case 0: // fixed
       clusterMassGenerator = new FixedScalarDoubleRandomGenerator(configuration->clusterMass_Fixed);
      if (reportInfo()) cout << "  configuration->clusterMass_Fixed: " << configuration->clusterMass_Fixed  << endl;
      break;
      case 1: // uniform rapidity
      clusterMassGenerator = new ScalarDoubleRandomGenerator(configuration->clusterMass_Minimum,
                                                             configuration->clusterMass_Maximum,
                                                             randomGenerator);
      if (reportInfo())
        {
        cout << "  configuration->clusterMass_Minimum: " << configuration->clusterMass_Minimum  << endl;
        cout << "  configuration->clusterMass_Maximum: " << configuration->clusterMass_Maximum  << endl;
        }
      break;
      case 2: // exponential
      if (reportInfo())
        {
        cout << "  configuration->clusterMass_Minimum: " << configuration->clusterMass_Minimum  << endl;
        cout << "    configuration->clusterMass_Slope: " << configuration->clusterMass_Slope  << endl;
        }
      clusterMassGenerator = new ExpScalarDoubleRandomGenerator(configuration->clusterMass_Minimum,
                                                                configuration->clusterMass_Slope,
                                                                randomGenerator);
      break;
    }

  // select cluster momentum generator
  switch (configuration->clusterMomentum_Random)
    {
      case 0: // fixed
      clusterMomentumGenerator = new FixedScalarDoubleRandomGenerator(configuration->clusterMomentum_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterMomentumGenerator set to FixedScalarDoubleRandomGenerator " << endl;
      break;
      case 1: // uniform rapidity
      clusterMomentumGenerator = new ScalarDoubleRandomGenerator(configuration->clusterMomentum_Minimum,
                                                                 configuration->clusterMomentum_Maximum,
                                                                 randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterMomentumGenerator set to ScalarDoubleRandomGenerator " << endl;
      break;
      case 2: // exponential
      clusterMomentumGenerator = new ExpScalarDoubleRandomGenerator(configuration->clusterMomentum_Minimum,
                                                                    configuration->clusterMomentum_Slope,
                                                                    randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() clusterMomentumGenerator set to ExpScalarDoubleRandomGenerator " << endl;
      break;
    }

  // select particle multiplicity generator
  switch (configuration->particleMultiplicity_Random)
    {
      case 0: // fixed
      particleMultiplicityGenerator = new FixedScalarIntRandomGenerator(configuration->particleMultiplicity_Fixed);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() particleMultiplicityGenerator set to FixedScalarIntRandomGenerator " << endl;
      break;
      case 1: // random flat
      particleMultiplicityGenerator = new ScalarIntRandomGenerator(configuration->particleMultiplicity_Minimum,
                                                                   configuration->particleMultiplicity_Maximum,
                                                                   randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() particleMultiplicityGenerator set to ScalarIntRandomGenerator " << endl;
      break;
      case 2: // random exp
      particleMultiplicityGenerator = new ExpScalarIntRandomGenerator(configuration->particleMultiplicity_Minimum,
                                                                      configuration->particleMultiplicity_Slope,
                                                                      randomGenerator);
      if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() particleMultiplicityGenerator set to ExpScalarIntRandomGenerator " << endl;
      break;
    }

  switch (configuration->particleMomenum_Random)
    {
      case 0: // fixed
        particleMomenumGenerator = new FixedScalarDoubleRandomGenerator(configuration->particleMomenum_Fixed);
        if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() particleMomenumGenerator set to FixedScalarDoubleRandomGenerator " << endl;
        break;
      case 1: // random flat
        particleMomenumGenerator = new ScalarDoubleRandomGenerator(configuration->particleMomenum_Minimum,
                                                                     configuration->particleMomenum_Maximum,
                                                                     randomGenerator);
        if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() particleMomenumGenerator set to ScalarDoubleRandomGenerator " << endl;
        break;
      case 2: // random exp
        particleMomenumGenerator = new ExpScalarDoubleRandomGenerator(configuration->particleMomenum_Minimum,
                                                                        configuration->particleMomenum_Slope,
                                                                        randomGenerator);
        if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() particleMomenumGenerator set to ExpScalarDoubleRandomGenerator " << endl;
        break;
    }

  if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() setting clusterDirectionGenerator to  VectorRandomGenerator" << endl;

  clusterDirectionGenerator  = new VectorRandomGenerator(randomGenerator);
  if (reportInfo())  cout << "ClusterModelEventGenerator::initialize() setting particleDirectionGenerator to  VectorRandomGenerator" << endl;
  particleDirectionGenerator = new VectorRandomGenerator(randomGenerator);
  if (!cmEvent)
    {
    if (reportFatal()) cout << "ClusterModelEventGenerator::initialize() cmEvent pointer is null" << endl;
    }
  cmEvent->reset();
  if (reportDebug())  cout << "ClusterModelEventGenerator::initialize() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset and Initialize the generator
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClusterModelEventGenerator::reset()
{
  if (reportDebug())  cout << "ClusterModelEventGenerator::reset() Started" << endl;
  cmEvent->reset();
  if (reportDebug())  cout << "ClusterModelEventGenerator::reset() Completed" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate an event
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClusterModelEventGenerator::execute()
{
  if (reportDebug())  cout << "ClusterModelEventGenerator::execute() Started" << endl;

  double cluster_theta;
  double cluster_phi;
  double cluster_ax;
  double cluster_ay;
  double cluster_az;
  double cluster_p;
  double cluster_pt;
  double cluster_px;
  double cluster_py;
  double cluster_pz;
  double cluster_e;
  double cluster_beta;
  double cluster_mass;
  double cluster_rapidity;
  double cluster_boost;

  double particle_theta;
  double particle_phi;
  double particle_ax;
  double particle_ay;
  double particle_az;
  double particle_p;
  double particle_pt;
  double particle_px;
  double particle_py;
  double particle_pz;
  double particle_e;
  double particle_mass;
  double particle_rapidity;

  cmEvent->reset();

  Cell     * cell = NULL;
  Cluster  * cluster = NULL;
  Particle * particle = NULL;
  Particle * particle1 = NULL;
  Particle * particle2 = NULL;

  int nCells     = 0;
  int nClusters  = 0;
  int nParticles = 0;

  if (reportDebug())  cout << "ClusterModelEventGenerator::execute() extracting configuration" << endl;

  ClusterModelConfiguration * configuration = (ClusterModelConfiguration * ) getTaskConfiguration();

  if (!configuration)
    {
    if (reportFatal())  cout << "ClusterModelEventGenerator::execute() extracting configuration" << endl;
    return;
    }
  if (reportDebug())  cout << "ClusterModelEventGenerator::execute() generate the number of cells" << endl;

  // --------------------------------------------------------------
  // Generate cells
  // --------------------------------------------------------------
  nCells = cellMultiplicityGenerator->generate();
  if (reportDebug())  cout << "ClusterModelEventGenerator::execute() nCells : " << nCells << endl;
  if (nCells>20)
    {
    nCells = 20;
    if (reportError())  cout << "ClusterModelEventGenerator::execute() nCells set to 20, was " << nCells << endl;
    }
  for (int kCell=0; kCell<nCells; kCell++)
    {
    if (reportDebug()) cout << "ClusterModelEventGenerator::generate() generate kCell:" << kCell << endl;
    cell = Cell::getFactory()->getNextObject();
    if (!cell) return;
    cell->reset();
    cell->beta       = cellBetaGenerator->generate();
    cell->phi        = cellPhiGenerator->generate();
    if (reportDebug())
      cout
      << "ClusterModelEventGenerator::generate() cell->beta: " << cell->beta << endl
      << "ClusterModelEventGenerator::generate()  cell->phi: " << cell->phi << endl;

    // --------------------------------------------------------------
    // Generate clusters
    // --------------------------------------------------------------
    nClusters = clusterMultiplicityGenerator->generate();
    if (reportDebug()) cout << "CellGenerator::generate() nClusters: " << nClusters << endl;
    if (nClusters>20) nClusters = 20;


    for (int kCluster=0; kCluster<nClusters; kCluster++)
      {
      if (reportDebug()) cout << "CellGenerator::generate() kCluster: " << kCluster << endl;
      cluster = Cluster::getFactory()->getNextObject();
      if (!cluster) return;
      cluster->reset();
      nParticles   = particleMultiplicityGenerator->generate();
      if (configuration->clusterMass_Random == 0)
        cluster_mass = clusterMassGenerator->generate();
      else
        cluster_mass = 0.001 + double(nParticles)*pionMass + clusterMassGenerator->generate();
      cluster->p.SetPxPyPzE(0.0, 0.0, 0.0, cluster_mass);
      if (reportDebug())
        {
        cout << "ClusterGenerator::generate()   nParticles: " << nParticles << endl;
        cout << "ClusterGenerator::generate() cluster_mass: " << cluster_mass << endl;
        }

      if (nParticles == 2)
        {
        if (reportDebug()) cout << "ClusterGenerator::generate() initiate 2-body decay: " << endl;
        // two body decay is simplest... The pair is at rest in the frame

        particle1 = Particle::getFactory()->getNextObject();
        particle2 = Particle::getFactory()->getNextObject();
        cluster->add(particle1);
        cluster->add(particle2);

        particleDirectionGenerator->generate(particle_theta,
                                             particle_phi,
                                             particle_ax,
                                             particle_ay,
                                             particle_az);


        particle_e    = (cluster_mass*cluster_mass - massesSq[1] + massesSq[0])/2/cluster_mass;
        particle_p    = sqrt(particle_e*particle_e - massesSq[0]);
        particle_px   = particle_p * particle_ax;
        particle_py   = particle_p * particle_ay;
        particle_pz   = particle_p * particle_az;

        //        particle1->charge =  1;
        //        particle1->p.SetPxPyPzE( particle_px, particle_py, particle_pz, particle_e);
        //        particle2->charge = -1;
        //        particle2->p.SetPxPyPzE(-particle_px,-particle_py,-particle_pz, sqrt(particle_p*particle_p + massesSq[1] ));
        //

        if (reportDebug())
          {
          cout << "Cluster Mass:" << cluster_mass << endl;
          particle1->printProperties(cout);
          particle2->printProperties(cout);
          }

        // notion of long, out, side are not workable in the rest frame of the cluster
        // without some externally defined direction - so just set them as px, py, pz.
        cluster->Qlong = 2.0 * particle_pz;
        cluster->Qout  = 2.0 * particle_px;
        cluster->Qside = 2.0 * particle_py;

        }
      else if (nParticles > 2)
        {
        // Q does not make sense for more than 2 particles so plugin non-sensical values.
        cluster->Qlong = -1000.0;
        cluster->Qout  = -1000.0;
        cluster->Qside = -1000.0;


        // three or more body decay -- use TGenPhaseSpace ...
        // useGenBod to produce particle within cluster with uniform probability (and p,E conservation)
        // assign charge sign on a cyclical basis -, 0, +
        TGenPhaseSpace * gbEvent = new TGenPhaseSpace();
        gbEvent->SetDecay(cluster->p, nParticles, masses);
        gbEvent->Generate();

        Particle * particle;
        for (int k=0; k<nParticles; k++)
          {
          p1       = gbEvent->GetDecay(k);
          particle = Particle::getFactory()->getNextObject();
          cluster->add(particle1);
          //          particle->p = *p1;
          //          particle->charge = k%3 - 1;
          }
        delete  gbEvent;
        }
      else
        {
        cout << "-ERROR- generate() nParticles<=1  -- ABORT" << endl;
        return;
        }

      //----------------------------------------------------------------------------------------------------
      // Boost the cluster isotropically with some p distribution (local temp boost)
      //----------------------------------------------------------------------------------------------------
      cluster_p  = clusterMomentumGenerator->generate();
      if (cluster_p>0)
        {
        clusterDirectionGenerator->generate(cluster_theta,
                                            cluster_phi,
                                            cluster_ax,
                                            cluster_ay,
                                            cluster_az);
        cluster_e    = sqrt(cluster_mass*cluster_mass + cluster_p*cluster_p);
        cluster_beta = cluster_p/cluster_e;

        if (reportDebug())
          {
          cluster_px = cluster_p*cluster_ax;
          cluster_py = cluster_p*cluster_ay;
          cluster_pz = cluster_p*cluster_az;
          cluster_pt = sqrt(cluster_px*cluster_px     + cluster_py*cluster_py);
          cout << "ClusterGenerator::generate()  " << endl
          << "     Cluster  nParticles:" << nParticles << endl
          << "                mass:" << cluster_mass << endl
          << "                  px:" << cluster_px << endl
          << "                  py:" << cluster_py << endl
          << "                  pz:" << cluster_pz << endl
          << "                   e:" << cluster_e << endl
          << "        cluster_beta:" << cluster_beta << endl;
          }
        // Temperature boost within cell
        cluster->boost(cluster_ax*cluster_beta, cluster_ay*cluster_beta, cluster_az*cluster_beta);

        }

      //----------------------------------------------------------------------------------------------------
      // Boost the cluster radially to simulate radial flow.
      //----------------------------------------------------------------------------------------------------
      if (cell->beta>0.0)
        {
        cluster->boost(cell->beta * cos(cell->phi), cell->beta * sin(cell->phi), 0.0);
        }

      //----------------------------------------------------------------------------------------------------
      // Boost the cluster longitundinally
      //----------------------------------------------------------------------------------------------------
      cluster_boost = clusterRapidityGenerator->generate();
      //cout << "ClusterGenerator::generate() cluster_boost: " << cluster_boost << endl;
      if (cluster_boost != 0.0)
        {
        // rapidity boost within cell
        cluster->boostZ(cluster_boost);
        }


      // The following code is useful to check the quality of the numerical calculations.
      // It seems to work at the 1 part per mil level but there are discrepencies - seemingly
      // because of the TLorentzVector class.
      /*
       if (nParticles == 2)
       {
       // test q reconstruction.
       TLorentzVector & p1 = particle1->p;
       TLorentzVector & p2 = particle2->p;
       TLorentzVector pSum = p1 + p2;
       TLorentzVector p1p(p1);
       TLorentzVector p2p(p2);

       double m  = pSum.M();
       double Px = pSum.Px();
       double Py = pSum.Py();
       double Pz = pSum.Pz();
       double Pt = pSum.Pt();
       double P  = pSum.P();
       double P0 = pSum.E();

       /// boost in the pair frame

       double beta = P/P0;
       //double betaX = -beta*Px/P;
       //double betaY = -beta*Py/P;
       //double betaZ = -beta*Pz/P;
       double betaX = -Px/P0;
       double betaY = -Py/P0;
       double betaZ = -Pz/P0;

       p1p.Boost(betaX,betaY,betaZ);
       p2p.Boost(betaX,betaY,betaZ);



       double p1xp = p1p.Px();
       double p1yp = p1p.Py();
       double p1zp = p1p.Pz();
       double e1p  = p1p.E();

       double p2xp = p2p.Px();
       double p2yp = p2p.Py();
       double p2zp = p2p.Pz();
       double e2p  = p2p.E();

       double qx = p1xp - p2xp;
       double qy = p1yp - p2yp;
       double qz = p1zp - p2zp;

       double rap1  = p1.Rapidity();
       double rap2  = p2.Rapidity();
       // if (fabs(rap1-rap2)<0.005)
       //  {
       cout << "===============================================" << endl;
       cout << " Pair m: " << m << " P:" << P << " E:" << P0 << " Px:" << Px << " Py:" << Py << " Pz:" << Pz << " Pt:" << Pt << endl;
       cout << " part 1 LAB: px :" << p1.Px() << " py :" << p1.Py()  << " pz :" << p1.Pz() <<  " e :" << p1.E() << " m :" << p1.M() << " rap1: " << rap1 << endl;
       cout << " part 2 LAB: px :" << p2.Px() << " py :" << p2.Py()  << " pz :" << p2.Pz() <<  " e :" << p2.E() << " m :" << p2.M() << " rap2: " << rap1 << endl;
       cout << " part 1 PAIR: px :" << p1p.Px() << " py :" << p1p.Py()  << " pz :" << p1p.Pz() <<  " e :" << p1p.E() << " m :" << p1p.M() << " rap1: " << p1p.Rapidity() <<endl;
       cout << " part 2 PAIR: px :" << p2p.Px() << " py :" << p2p.Py()  << " pz :" << p2p.Pz() <<  " e :" << p2p.E() << " m :" << p2p.M() << " rap2: " << p2p.Rapidity() <<endl;
       cout << " rap2: " << rap2 << " rap diff:" << fabs(rap1-rap2) << endl;
       cout << " rec: qx:" << qx << "  qy:" << qy << "  qz:" << qz << endl;
       cout << " gen: qx:" << 2.0*particle_px << "  qy:" << 2.0*particle_py << "  qz:" << 2.0*particle_pz << endl;
       // }
       }
       */

      //----------------------------------------------------------------------------------------------------
      // Done w/ cluster. Add it to cell.
      //----------------------------------------------------------------------------------------------------
      cell->add(cluster);
      if (reportDebug()) cout << "ClusterGenerator::generate() Cluster completed!" << endl;
      }
    if (reportDebug()) cout << "ClusterGenerator::generate() All clusters generated. Cell completed!" << endl;
    cmEvent->add(cell);
    }
  if (reportDebug()) cout << "ClusterModelEventGenerator::generate() event completed!" << endl;
}


Event * ClusterModelEventGenerator::getClusterModelEvent()
 {
 return (ClusterModelEvent*) event;
 }
