// Author: Claude Pruneau   09/25/2019

/*************************************************************************
 * Copyright (C) 2019, Claude Pruneau.                                   *
 * All rights reserved.                                                  *
 * Based on the ROOT package and environment                             *
 *                                                                       *
 * For the licensing terms see LICENSE.                                  *
 *************************************************************************/
/**
 \class EccentricityVsCent
 \ingroup WAC

 Class defining EccentricityVsCent
 */

#include "EccentricityVsCent.hpp"

ClassImp(EccentricityVsCent);

///////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////////////////////
EccentricityVsCent::EccentricityVsCent(const TString & _systemName,
                                       int nX, double minX, double maxX,
                                       int nY, double minY, double maxY,
                                       int nB, double minB, double maxB)
:
systemName(_systemName)
{
  cout << "-INFO- EccentricityVsCent::EccentricityVsCent() ctor Started" << endl;

  TString baseName = systemName;
  baseName += "_";
  // 1D histogram

  h_b           = new TH1D(baseName+"b", baseName+"b",nB,minB,maxB);
  h_var_x_vsB   = new TH1D(baseName+"var_x_vsB", baseName+"var_x_vsB",nB,minB,maxB);
  h_var_y_vsB   = new TH1D(baseName+"var_y_vsB", baseName+"var_y_vsB",nB,minB,maxB);
  h_var_xy_vsB  = new TH1D(baseName+"var_xy_vsB",baseName+"var_xy_vsB",nB,minB,maxB);
  h_eps_x_vsB   = new TH1D(baseName+"eps_x_vsB", baseName+"eps_x_vsB",nB,minB,maxB);
  h_eps_y_vsB   = new TH1D(baseName+"eps_y_vsB", baseName+"eps_y_vsB",nB,minB,maxB);
  h_eps_mod_vsB = new TH1D(baseName+"eps_mod_vsB",baseName+"eps_mod_vsB",nB,minB,maxB);

  h_eps22_vsB = new TH1D(baseName+"eps22_vsB",baseName+"eps22_vsB",nB,minB,maxB);
  h_eps24_vsB = new TH1D(baseName+"eps24_vsB",baseName+"eps24_vsB",nB,minB,maxB);
  h_eps26_vsB = new TH1D(baseName+"eps26_vsB",baseName+"eps26_vsB",nB,minB,maxB);
  h_eps28_vsB = new TH1D(baseName+"eps28_vsB",baseName+"eps28_vsB",nB,minB,maxB);

  h_psi2_vsB  = new TH1D(baseName+"psi2_vsB",baseName+"psi2_vsB",nB,minB,maxB);

  // 2D histogram
  baseName = systemName;
  baseName += "_2d_";

  h2_eps_x_vsB   = new TH2D(baseName+"eps_x_vsB",baseName+"eps_x",nB,minB,maxB, 40, -1.0, 1.0);
  h2_eps_y_vsB   = new TH2D(baseName+"eps_y_vsB",baseName+"eps_y",nB,minB,maxB, 40, -1.0, 1.0);
  h2_eps_mod_vsB = new TH2D(baseName+"eps_mod_vsB",baseName+"eps_mod",nB,minB,maxB, 20, 0.0, 1.0);
  h2_psi2_vsB    = new TH2D(baseName+"psi2_vsB",baseName+"psi2_vsB",nB,minB,maxB,40,-TMath::Pi(), TMath::Pi());

  //
//  h2_eps22_vsB   = new TH2D(baseName+"eps22",baseName+"eps22",nB,minB,maxB, 20, -1.0, 1.0);
//  h2_eps24_vsB   = new TH2D(baseName+"eps24",baseName+"eps24",nB,minB,maxB, 20, -1.0, 1.0);
//  h2_eps26_vsB   = new TH2D(baseName+"eps26",baseName+"eps26",nB,minB,maxB, 20, -1.0, 1.0);
//  h2_eps28_vsB   = new TH2D(baseName+"eps28",baseName+"eps28",nB,minB,maxB, 20, -1.0, 1.0);

  cout << "-INFO- EccentricityVsCent::EccentricityVsCent() Completed" << endl;
}

void EccentricityVsCent::fill(double b, EccentricityCalc * eccCalc)
{
  if (eccCalc->counts>0)
    {
    h_b  ->Fill(b);
    h_var_x_vsB   ->Fill(b,eccCalc->var_x);
    h_var_y_vsB   ->Fill(b,eccCalc->var_y);
    h_var_xy_vsB  ->Fill(b,eccCalc->var_xy);

    h_eps_x_vsB   ->Fill(b,eccCalc->eps_x);
    h_eps_y_vsB   ->Fill(b,eccCalc->eps_y);
    h_eps_mod_vsB ->Fill(b,eccCalc->eps_mod);

    h_eps22_vsB  ->Fill(b,eccCalc->eps_22);
    h_eps24_vsB  ->Fill(b,eccCalc->eps_24);
    h_eps26_vsB  ->Fill(b,eccCalc->eps_26);
    h_eps28_vsB  ->Fill(b,eccCalc->eps_28);


    // 2D histogram
    h2_eps_x_vsB   ->Fill(b,eccCalc->eps_x);
    h2_eps_y_vsB   ->Fill(b,eccCalc->eps_y);
    h2_eps_mod_vsB ->Fill(b,eccCalc->eps_mod);

    h_psi2_vsB   ->Fill(b,eccCalc->psi2);
    h2_psi2_vsB  ->Fill(b,eccCalc->psi2);

    //  h2_eps22_vsB  ->Fill(b,eccCalc->eps22);
    //  h2_eps24_vsB  ->Fill(b,eccCalc->eps24);
    //  h2_eps26_vsB  ->Fill(b,eccCalc->eps26);
    }


}

void EccentricityVsCent::fill(int iB, EccentricityCalc * eccCalc)
{
  h_b           ->SetBinContent(iB,1.0);
  h_var_x_vsB   ->SetBinContent(iB,eccCalc->var_x);
  h_var_y_vsB   ->SetBinContent(iB,eccCalc->var_y);
  h_var_xy_vsB  ->SetBinContent(iB,eccCalc->var_xy);

  h_eps_x_vsB   ->SetBinContent(iB,eccCalc->eps_x);
  h_eps_y_vsB   ->SetBinContent(iB,eccCalc->eps_y);
  h_eps_mod_vsB ->SetBinContent(iB,eccCalc->eps_mod);

  h_eps22_vsB  ->SetBinContent(iB,eccCalc->eps_22);
  h_eps24_vsB  ->SetBinContent(iB,eccCalc->eps_24);
  h_eps26_vsB  ->SetBinContent(iB,eccCalc->eps_26);
  h_eps28_vsB  ->SetBinContent(iB,eccCalc->eps_28);

  h_psi2_vsB   ->SetBinContent(iB,eccCalc->psi2);

  h_var_x_vsB   ->SetBinError(iB,0.0);
  h_var_y_vsB   ->SetBinError(iB,0.0);
  h_var_xy_vsB  ->SetBinError(iB,0.0);

  h_eps_x_vsB   ->SetBinError(iB,0.0);
  h_eps_y_vsB   ->SetBinError(iB,0.0);
  h_eps_mod_vsB ->SetBinError(iB,0.0);

  h_eps22_vsB  ->SetBinError(iB,0.0);
  h_eps24_vsB  ->SetBinError(iB,0.0);
  h_eps26_vsB  ->SetBinError(iB,0.0);
  h_eps28_vsB  ->SetBinError(iB,0.0);
  h_psi2_vsB   ->SetBinError(iB,0.0);


  // 2D histogram
//  h2_eps_x_vsB   ->Fill(iB,eccCalc->eps_x);
//  h2_eps_y_vsB   ->Fill(iB,eccCalc->eps_y);
//  h2_eps_mod_vsB ->Fill(iB,eccCalc->eps_mod);

//  h2_eps22_vsB  ->Fill(b,eccCalc->eps22);
//  h2_eps24_vsB  ->Fill(b,eccCalc->eps24);
//  h2_eps26_vsB  ->Fill(b,eccCalc->eps26);
}

void EccentricityVsCent::normalize()
{
  h_var_x_vsB   ->Divide(h_b);
  h_var_y_vsB   ->Divide(h_b);
  h_var_xy_vsB  ->Divide(h_b);

  h_eps_x_vsB   ->Divide(h_b);
  h_eps_y_vsB   ->Divide(h_b);
  h_eps_mod_vsB ->Divide(h_b);

  h_eps22_vsB  ->Divide(h_b);
  h_eps24_vsB  ->Divide(h_b);
  h_eps26_vsB  ->Divide(h_b);
  h_eps28_vsB  ->Divide(h_b);

  h_psi2_vsB  ->Divide(h_b);

  int nB = h_eps22_vsB->GetNbinsX();
  for (int iB=1; iB<=nB; iB++)
    {
    double v1, v2, ev1, ev2, er1, er2;

    v1  = h_eps22_vsB->GetBinContent(iB);
    ev1 = h_eps22_vsB->GetBinError(iB);

    if (v1>0)
      {
      v2 = sqrt(v1);
       ev2 = 0.5*ev1/sqrt(v1);
       h_eps22_vsB->SetBinContent(iB, v2);
       h_eps22_vsB->SetBinError(iB, ev2);

       v1  = h_eps24_vsB->GetBinContent(iB);
       ev1 = h_eps24_vsB->GetBinError(iB);
       v2 = TMath::Power(v1,0.25);
       ev2 = 0.25*ev1/TMath::Power(v1,0.75);
       h_eps24_vsB->SetBinContent(iB, v2);
       h_eps24_vsB->SetBinError(iB, ev2);

       v1  = h_eps26_vsB->GetBinContent(iB);
       ev1 = h_eps26_vsB->GetBinError(iB);
       v2 = TMath::Power(v1,1.0/6.0);
       ev2 = (1.0/6.0)*ev1/TMath::Power(v1,5.0/6.0);
       h_eps26_vsB->SetBinContent(iB, v2);
       h_eps26_vsB->SetBinError(iB, ev2);

       v1  = h_eps28_vsB->GetBinContent(iB);
       ev1 = h_eps28_vsB->GetBinError(iB);
       v2 = TMath::Power(v1,1.0/8.0);
       ev2 = (1.0/8.0)*ev1/TMath::Power(v1,7.0/8.0);
       h_eps28_vsB->SetBinContent(iB, v2);
       h_eps28_vsB->SetBinError(iB, ev2);
      }
    else
      {
      v2   = 0;
      ev2  = 0;
      h_eps22_vsB->SetBinContent(iB, v2);
      h_eps22_vsB->SetBinError(iB,   ev2);
      h_eps24_vsB->SetBinContent(iB, v2);
      h_eps24_vsB->SetBinError(iB,   ev2);
      h_eps26_vsB->SetBinContent(iB, v2);
      h_eps26_vsB->SetBinError(iB,   ev2);
      h_eps28_vsB->SetBinContent(iB, v2);
      h_eps28_vsB->SetBinError(iB,   ev2);
      }

    }

}


/// Clear container to fresh start
void EccentricityVsCent::clear()
{
  h_b           ->Reset();
  h_var_x_vsB   ->Reset();
  h_var_y_vsB   ->Reset();
  h_var_xy_vsB  ->Reset();

  h_eps_x_vsB   ->Reset();
  h_eps_y_vsB   ->Reset();
  h_eps_mod_vsB ->Reset();

  h_eps22_vsB  ->Reset();
  h_eps24_vsB  ->Reset();
  h_eps26_vsB  ->Reset();
  h_eps28_vsB  ->Reset();

  // 2D histogram
  h2_eps_x_vsB   ->Reset();
  h2_eps_y_vsB   ->Reset();
  h2_eps_mod_vsB ->Reset();

  h_psi2_vsB ->Reset();
  h2_psi2_vsB ->Reset();

//  h2_eps22_vsB  ->Reset();
//  h2_eps24_vsB  ->Reset();
//  h2_eps26_vsB  ->Reset();
}

/// Save histograms
void EccentricityVsCent::save()
{
  h_b           ->Write();
  h_var_x_vsB   ->Write();
  h_var_y_vsB   ->Write();
  h_var_xy_vsB  ->Write();

  h_eps_x_vsB   ->Write();
  h_eps_y_vsB   ->Write();
  h_eps_mod_vsB ->Write();

  h_eps22_vsB  ->Write();
  h_eps24_vsB  ->Write();
  h_eps26_vsB  ->Write();
  h_eps28_vsB  ->Write();

  // 2D histogram
  h2_eps_x_vsB   ->Write();
  h2_eps_y_vsB   ->Write();
  h2_eps_mod_vsB ->Write();

  h_psi2_vsB     ->Write();
  h2_psi2_vsB    ->Write();

//
//  h2_eps22_vsB  ->Write();
//  h2_eps24_vsB  ->Write();
//  h2_eps26_vsB  ->Write();
}

