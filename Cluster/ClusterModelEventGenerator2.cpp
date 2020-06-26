#include "ClusterModelEventGenerator2.hpp"
ClassImp(ClusterModelEventGenerator2);

//extern Factory<Particle> * getParticleFactory();
//extern Factory<Cluster> * getClusterFactory();
//extern Factory<Cell> * getCellFactory();



ClusterModelEventGenerator2::ClusterModelEventGenerator2(const TString &  name,
                                                         TaskConfiguration * configuration,
                                                         ClusterModelEvent * event)
:
ClusterModelEventGenerator(name,configuration,event)
{
  if (reportDebug())  cout << "ClusterModelEventGenerator2::ClusterModelEventGenerator2(...) No ops." << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DTOR
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
ClusterModelEventGenerator2::~ClusterModelEventGenerator2()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generate an event
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClusterModelEventGenerator2::execute()
{
  if (reportDebug()) cout << "ClusterModelEventGenerator2::execute() Started" << endl;

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

  //ClusterModelConfiguration * configuration = (ClusterModelConfiguration * ) generatorConfiguration;

  //ClusterModelEvent * cmEvent = (ClusterModelEvent *) event;
  ////cmEvent->reset();





//  if (reportDebug())  cout << "ClusterModelEventGenerator2::execute() extracting configuration" << endl;
//  ClusterModelConfiguration * configuration = (ClusterModelConfiguration * ) getTaskConfiguration();
//  if (!configuration)
//    {
//    if (reportFatal())  cout << "ClusterModelEventGenerator2::execute() extracting configuration" << endl;
//    return;
//    }
  // --------------------------------------------------------------
  // Generate cells
  // --------------------------------------------------------------
  int nCells = cellMultiplicityGenerator->generate();
  if (reportDebug())  cout << "ClusterModelEventGenerator::execute() nCells : " << nCells << endl;
  if (nCells>ClusterModelEvent::nCellsMax)
    {
    nCells = ClusterModelEvent::nCellsMax;
    if (reportError())  cout << "ClusterModelEventGenerator::execute() nCells exceeds maximum. Was set to " << nCells << endl;
    }

  for (int kCell=0; kCell<nCells; kCell++)
    {
    if (reportDebug()) cout << "ClusterModelEventGenerator2::execute() generate kCell:" << kCell << endl;
    Cell * cell = Cell::getFactory()->getNextObject();
    if (!cell)
      {
      if (reportError()) cout << "ClusterModelEventGenerator2::execute() cell is a null pointer." << endl;
      return;
      }
    cell->reset();
    cell->beta       = cellBetaGenerator->generate();
    cell->phi        = cellPhiGenerator->generate();
//    if (reportInfo())
//      {
//      cout
//      << "ClusterModelEventGenerator2::execute() cell->beta: " << cell->beta << endl
//      << "ClusterModelEventGenerator2::execute()  cell->phi: " << cell->phi << endl;
//      cell->printProperties(cout);
//      }
    // --------------------------------------------------------------
    // Generate clusters
    // --------------------------------------------------------------
    int nClusters = clusterMultiplicityGenerator->generate();
    if (reportDebug()) cout << "CellGenerator2::execute() nClusters: " << nClusters << endl;
    if (nClusters > Cell::nClustersMax)
      {
      if (reportWarning()) cout << "CellGenerator2::execute() nClusters exceeds maximum: " << Cell::nClustersMax << " will be set to that value." << endl;
      nClusters = Cell::nClustersMax;
      }

    for (int kCluster=0; kCluster<nClusters; kCluster++)
      {
      if (reportDebug()) cout << "CellGenerator2::execute() kCluster: " << kCluster << endl;
      Cluster * cluster = Cluster::getFactory()->getNextObject();
      if (!cluster)
        {
        if (reportError()) cout << "CellGenerator2::execute() cluster pointer is null." << endl;
        return;
        }
      cluster->reset();

      int nParticles   = particleMultiplicityGenerator->generate();
      if (reportDebug()) cout << "ClusterGenerator::execute() nParticles: " << nParticles << endl;
      for (int iPart=0; iPart<nParticles; ++iPart)
        {
          Particle * particle = Particle::getFactory()->getNextObject();
        particleDirectionGenerator->generate(particle_theta,
                                             particle_phi,
                                             particle_ax,
                                             particle_ay,
                                             particle_az);
        particle_p = particleMomenumGenerator->generate();
        particle_e    = sqrt(particle_p*particle_p + massesSq[0]);
        particle_px   = particle_p * particle_ax;
        particle_py   = particle_p * particle_ay;
        particle_pz   = particle_p * particle_az;
        if (reportDebug())
          {
          cout << "ClusterGenerator2::execute()  " << endl
          << "         particle mass:" << sqrt(massesSq[0]) << endl
          << "         particle    p:" << particle_p << endl
          << "         particle   px:" << particle_px << endl
          << "         particle   py:" << particle_py << endl
          << "         particle   pz:" << particle_pz << endl
          << "         particle    e:" << particle_e << endl
          << "         particle beta:" << particle_p/particle_e << endl;
          }
        
        if (nParticles>2)
          particle->charge =  iPart%3 - 1;
        else
          particle->charge =  2*(iPart%2) - 1;
        particle->setPxPyPzE( particle_px, particle_py, particle_pz, particle_e);
        cluster->add(particle);
        }

      //----------------------------------------------------------------------------------------------------
      // Boost the cluster isotropically with some p distribution (local temp boost)
      //----------------------------------------------------------------------------------------------------
      cluster_p  = 0.001 + clusterMomentumGenerator->generate();
      if (cluster_p>0.0)
        {
        clusterDirectionGenerator->generate(cluster_theta, cluster_phi, cluster_ax, cluster_ay, cluster_az);
        cluster_mass = 0.5;
        cluster->p   = TLorentzVector(0.0,0.0,0.0,cluster_mass);
        cluster_e    = sqrt(cluster_mass*cluster_mass + cluster_p*cluster_p);
        cluster_beta = cluster_p/cluster_e;

        if (reportDebug())
          {
          cluster_px = cluster_p*cluster_ax;
          cluster_py = cluster_p*cluster_ay;
          cluster_pz = cluster_p*cluster_az;
          cluster_pt = sqrt(cluster_px*cluster_px     + cluster_py*cluster_py);
          cout << "ClusterGenerator2::execute()  " << endl
          << "     Cluster  nParticles:" << nParticles << endl
          << "                mass:" << cluster_mass << endl
          << "                   p:" << cluster_p << endl
          << "                  px:" << cluster_px << endl
          << "                  py:" << cluster_py << endl
          << "                  pz:" << cluster_pz << endl
          << "                   e:" << cluster_e << endl
          << "        cluster_beta:" << cluster_beta << endl;
          }
        // Temperature boost within cell
        cluster->boost(cluster_ax*cluster_beta, cluster_ay*cluster_beta, cluster_az*cluster_beta);
        }
      else
        {
        if (reportError()) cout << "ClusterGenerator::execute() invalid cluster_p." << cluster_p << endl;
        return;
        }

      //----------------------------------------------------------------------------------------------------
      // Boost the cluster radially to simulate radial flow.
      //----------------------------------------------------------------------------------------------------
//      if (cell->beta>0.0)
//        {
//        cluster->boost(cell->beta * cos(cell->phi), cell->beta * sin(cell->phi), 0.0);
//        }

      //----------------------------------------------------------------------------------------------------
      // Boost the cluster longitundinally
      //----------------------------------------------------------------------------------------------------
      cluster_boost = clusterRapidityGenerator->generate();
      if (cluster_boost != 0.0)
        {
        // rapidity boost within cell
        cluster->boostZ(cluster_boost);
        }
      cell->add(cluster);
      if (reportDebug()) cout << "ClusterGenerator::execute() Cluster completed!" << endl;
      }
    cmEvent->add(cell);
    if (reportDebug()) cout << "ClusterGenerator::execute() Cell completed!" << endl;
    }
  //if (reportDebug()) cmEvent->printProperties(cout);
  if (reportDebug()) cout << "ClusterModelEventGenerator2::execute() Event completed!" << endl;
}
