//
//  statUncertain.C
//  CodeV3
//
//  Created by Claude Pruneau on 8/22/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//

#include <stdio.h>

//
//  cm_figure9_6.c
//  MyMC
//
//  Created by Claude Pruneau on 4/10/17.
//  Copyright © 2017 Claude Pruneau. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <TError.h>
#include <TObjArray.h>
#include <TTimeStamp.h>
#include <TList.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TMath.h>
#include "TRandom.h"
#include "../Base/AnalysisConfiguration.hpp"
#include "../Base/TwoPartCorrelationAnalyzer.hpp"

const Int_t npairs = 5;
const char *pname[npairs] = {"CD","CI","MM","PM","PP"};
const Int_t ncorrf = 4;
const char *cfname[ncorrf] = {"P2","R2","G2","BF"};
const Int_t naux = 10;
const char *auxname[naux] = {
    "h2_n1_P",
    "h2_n1_M",
    "h2_Pt_P",
    "h2_Pt_M",
    "h2_n2_PP",
    "h2_PtPt_PP",
    "h2_n2_MM",
    "h2_PtPt_MM",
    "h2_n2_PM",
    "h2_PtPt_PM"
};

std::vector<std::string>centfname = {
  "00-05", "05-10","10-20","20-30","30-40","40-50", "50-60", "60-70", "70-80"
};
int ncent = centfname.size();

std::vector<std::string>samplesfnames = {
  "20200721_222906_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub0.root",
  "20200721_222906_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub1.root",
  "20200723_200552_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub0.root",
  "20200723_200552_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub1.root",
  "20200724_091749_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub0.root", 
  "20200724_091749_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub1.root", 
  "20200725_101223_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub0.root",
  "20200725_101223_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub1.root",
  "20200725_172248_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub0.root",
  "20200725_172248_samesamples/output/Centrality%s.in/scanout/Histograms%s_Sub1.root"
};
int nsamples = samplesfnames.size();

TFile *getSampleFile(int icent,int isample) {
  TFile *f = nullptr;

  std::string filename = TString::Format(samplesfnames[isample].c_str(),centfname[icent].c_str(),centfname[icent].c_str()).Data();
  f = new TFile(filename.c_str());
  if (f == nullptr) {
    Error("statUncertain::getSampleFile","File %s not found. ABORTING!!!", filename.c_str());
  }
  return f;
}


TH2 *extractHistoMeanAndStDevFromSubSets(const TObjArray &listsarray, Int_t ih, const TString &name) {

  TH2 *h2 = dynamic_cast<TH2*>(((TList*) listsarray[0])->At(ih));

  if (h2 != NULL) {
    /* extract the name of the result histogram */
    TString title = h2->GetTitle();
    TString xtitle = h2->GetXaxis()->GetTitle();
    TString ytitle = h2->GetYaxis()->GetTitle();
    TString ztitle = h2->GetZaxis()->GetTitle();

    /* let's first create the profile we use as support */
    TProfile2D *p = new TProfile2D("auxprofile","auxprofile",
        h2->GetNbinsX(), h2->GetXaxis()->GetXmin(), h2->GetXaxis()->GetXmax(),
        h2->GetNbinsY(), h2->GetYaxis()->GetXmin(), h2->GetYaxis()->GetXmax());

    for (Int_t ix = 0; ix < h2->GetNbinsX(); ix++) {
      Double_t x = h2->GetXaxis()->GetBinCenter(ix+1);
      for (Int_t iy = 0; iy < h2->GetNbinsY(); iy++) {
        Double_t y = h2->GetYaxis()->GetBinCenter(iy+1);
        for (Int_t iset = 0; iset < listsarray.GetEntries(); iset++) {
          p->Fill(x,y,((TH2*)(((TList*) listsarray[iset])->At(ih)))->GetBinContent(ix+1,iy+1));
        }
      }
    }
    h2 = p->ProjectionXY(name.Data());
    h2->SetTitle(title.Data());
    h2->SetXTitle(xtitle.Data());
    h2->SetYTitle(ytitle.Data());
    h2->SetZTitle(ztitle.Data());

    /* set the proper number of entries */
    Double_t nentries = 0;
    for (Int_t iset = 0; iset < listsarray.GetEntries(); iset++) {
      nentries += ((TH2*)(((TList*) listsarray[iset])->At(ih)))->GetEntries();
    }
    h2->SetEntries(nentries);

    delete p;
    return h2;
  }
  else {
    Error("extractHistoMeanAndStDevFromSubSets", "Wrong histogram type");
    return NULL;
  }
}

TList *extractMeanAndStDevFromSubSets(const TObjArray &listsarray, const TString &pattern, const char **name) {
  /* basically we receive an array of histograms lists */
  /* each array item corresponds to a results subset   */
  /* we extract an equivalent list of histograms which */
  /* corresponds to the mean of the subsets histograms */
  /* and have as errors the standard deviation from    */
  /* the mean on each bin                              */

  /* first, some consistency checks */
  Int_t nhistos = ((TList*) listsarray[0])->GetEntries();
  if (nhistos != ncorrf && nhistos != naux)
    Error("extractMeanAndStDevFromSubSets", "Inconsistent number of histograms to average");
  for (Int_t iset = 0; iset < listsarray.GetEntries(); iset++) {
    if (nhistos != ((TList*) listsarray[iset])->GetEntries()) {
      Error("extractMeanAndStDevFromSubSets", "Inconsistent array of lists");
      return NULL;
    }
  }

  TList *list = new TList();
  list->SetOwner(kTRUE);
  for (Int_t ih = 0; ih < nhistos; ih++) {
    list->Add(extractHistoMeanAndStDevFromSubSets(listsarray,ih,TString::Format(pattern.Data(),name[ih])));
  }

  return list;
}

TList *extractSampleResults(AnalysisConfiguration *ac, int icent,int isample) {

  /* we will control what goes to the directory tree */
  Bool_t oldstatus = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  ParticleFilter *particleFilter1  = new ParticleFilter(ParticleFilter::Hadron,
                                                        ParticleFilter::Positive,
                                                        ac->min_pt,ac->max_pt,
                                                        ac->min_eta,ac->max_eta, 
                                                        ac->min_y,ac->max_y); 
  ParticleFilter *particleFilter2  = new ParticleFilter(ParticleFilter::Hadron,
                                                        ParticleFilter::Negative,
                                                        ac->min_pt,ac->max_pt,
                                                        ac->min_eta,ac->max_eta, 
                                                        ac->min_y,ac->max_y);

  EventFilter *eventFilter = new EventFilter(EventFilter::MinBias,0,100);

  Particle::factorySize = 15000;
  Event *event = Event::getEvent();

  TwoPartCorrelationAnalyzer *eventanalyzer = new TwoPartCorrelationAnalyzer("DukeEventGen",
                                                                             ac,
                                                                             event,
                                                                             eventFilter,
                                                                             particleFilter1,
                                                                             particleFilter2);
                                                  
  eventanalyzer->setReportLevel(MessageLogger::Error);

  TFile *myfile = getSampleFile(icent,isample);
  if (myfile == nullptr) return nullptr;

  eventanalyzer->loadBaseHistograms(myfile);
  eventanalyzer->finalize();

  TList *list = new TList();
  list->SetOwner(kFALSE);

  for (Int_t ipair = 0; ipair < npairs; ipair++) {
    TList *plist = new TList();
    plist->SetOwner(kTRUE);

    for (Int_t icf = 0; icf < ncorrf; icf++) {
      TH2 *h2 = NULL;

      switch (ipair) {
      case 0: /* CD */
        switch (icf) {
        case 0: /* P2 */
          h2 = eventanalyzer->pair12_CDHistos->h_P2_DetaDphi_shft;
          break;
        case 1: /* R2 */
          h2 = eventanalyzer->pair12_CDHistos->h_R2_DetaDphi_shft;
          break;
        case 2: /* G2 */
          h2 = eventanalyzer->pair12_CDHistos->h_G2_DetaDphi_shft;
          break;
        case 3: /* BF */
          h2 = eventanalyzer->pair12_CDHistos->h_BF_DetaDphi_shft;
          break;
        default:
          Fatal("extractSampleResults","Wrong correlator index");
        }
        break;
      case 1: /* CI */
        switch (icf) {
        case 0: /* P2 */
          h2 = eventanalyzer->pair12_CIHistos->h_P2_DetaDphi_shft;
          break;
        case 1: /* R2 */
          h2 = eventanalyzer->pair12_CIHistos->h_R2_DetaDphi_shft;
          break;
        case 2: /* G2 */
          h2 = eventanalyzer->pair12_CIHistos->h_G2_DetaDphi_shft;
          break;
        case 3: /* BF */
          h2 = eventanalyzer->pair12_CIHistos->h_BF_DetaDphi_shft;
          break;
        default:
          Fatal("extractSampleResults","Wrong correlator index");
        }
        break;
      case 2: /* MM */
        switch (icf) {
        case 0: /* P2 */
          h2 = eventanalyzer->pair22_DerivedHistos->h_P2_DetaDphi_shft;
          break;
        case 1: /* R2 */
          h2 = eventanalyzer->pair22_DerivedHistos->h_R2_DetaDphi_shft;
          break;
        case 2: /* G2 */
          h2 = eventanalyzer->pair22_DerivedHistos->h_G2_DetaDphi_shft;
          break;
        case 3: /* BF */
          h2 = eventanalyzer->pair22_DerivedHistos->h_bf12_DetaDphi_shft;
          break;
        default:
          Fatal("extractSampleResults","Wrong correlator index");
        }
        break;
      case 3: /* PM */
        switch (icf) {
        case 0: /* P2 */
          h2 = eventanalyzer->pair12_DerivedHistos->h_P2_DetaDphi_shft;
          break;
        case 1: /* R2 */
          h2 = eventanalyzer->pair12_DerivedHistos->h_R2_DetaDphi_shft;
          break;
        case 2: /* G2 */
          h2 = eventanalyzer->pair12_DerivedHistos->h_G2_DetaDphi_shft;
          break;
        case 3: /* BF */
          h2 = eventanalyzer->pair12_DerivedHistos->h_bf12_DetaDphi_shft;
          break;
        default:
          Fatal("extractSampleResults","Wrong correlator index");
        }
        break;
      case 4: /* PP */
        switch (icf) {
        case 0: /* P2 */
          h2 = eventanalyzer->pair11_DerivedHistos->h_P2_DetaDphi_shft;
          break;
        case 1: /* R2 */
          h2 = eventanalyzer->pair11_DerivedHistos->h_R2_DetaDphi_shft;
          break;
        case 2: /* G2 */
          h2 = eventanalyzer->pair11_DerivedHistos->h_G2_DetaDphi_shft;
          break;
        case 3: /* BF */
          h2 = eventanalyzer->pair11_DerivedHistos->h_bf12_DetaDphi_shft;
          break;
        default:
          Fatal("extractSampleResults","Wrong correlator index");
        }
        break;
      default:
        Fatal("extractSampleResults","Wrong pair combination index");
      }
      plist->Add(h2->Clone(TString::Format("%s_Sub%02d",h2->GetName(),isample)));
    }
    list->Add(plist);
  }

  myfile->Close();

  delete myfile;
  delete eventanalyzer;
  delete eventFilter;
  delete particleFilter1;
  delete particleFilter2;

  /* back to normal */
  TH1::AddDirectory(oldstatus);

  return list;
}


/////////////////////////////////////////////////////////////////////////////////////////
// produce results with statistic uncertainties out of a set of sub-samples
/////////////////////////////////////////////////////////////////////////////////////////
void statUncertain(Option_t *opt)
{

  TTimeStamp now = TTimeStamp();
  if (!TString(opt).Contains("verb"))
    gErrorIgnoreLevel = kWarning;

  /* Cummulate errors by default */
  TH1::SetDefaultSumw2(kTRUE);

  TFile *outputfile = new TFile(TString::Format("Histograms%02dSub_%d_%d.root", nsamples,now.GetDate(),now.GetTime()),"RECREATE");

  for (int icent = 0; icent < ncent; icent++) {

    Info("statUncertain","Setup configuration");

    AnalysisConfiguration *ac = new AnalysisConfiguration("Jupyter","OSG-hydro","1.0");

    ac->outputPath               = "./";
    ac->configurationFileName    = "Config_DUKE.txt";
    ac->rootOuputFileName        = TString::Format("Histograms%02dSub",nsamples).Data();


    cout<<"================================================================"<<endl;
    cout<<""<<endl;
    cout<<"         running for = "<<ac->getName()<<endl;
    cout<<""<<endl;
    cout<<"================================================================"<<endl;

    ac->nBins_pt    = 18;
    ac->min_pt      = 0.2;
    ac->max_pt      = 2.0;
    ac->nBins_eta   = 20;
    ac->min_eta     = -1.0;
    ac->max_eta     = 1.0;
    ac->nBins_y     = 20;
    ac->min_y       = -1.0;
    ac->max_y       = 1.0;
    ac->nBins_phi   = 72;
    ac->min_phi     = 0.0;
    ac->max_phi     = TMath::TwoPi();

    ac->fillPairs        = true;
    ac->fill3D           = false;
    ac->fill6D           = false;
    ac->fillQ3D          = false;
    ac->fillY            = false;

    ac->scaleHistograms                = true;
    ac->saveHistograms                 = false;
    ac->forceHistogramsRewrite         = false;
    ac->calculateDerivedHistograms     = true;

    std::vector<TObjArray>pairslists(npairs,TObjArray(nsamples));
    for (Int_t ipair = 0; ipair < npairs; ipair++) {
      pairslists[ipair].SetOwner(kTRUE);
    }

    TList *auxlist;
    for (Int_t isamp = 0; isamp < nsamples; isamp++) {
      Warning("statUncertain","Processing sample %d for centrality %s",isamp,centfname[icent].c_str());
      TList *list = extractSampleResults(ac, icent, isamp);

      for (Int_t ipair = 0; ipair < npairs; ipair++) {
        pairslists[ipair][isamp] = list->At(ipair);
      }
      delete list;
    }

    /* we will control what goes to the directory tree */
    Bool_t oldstatus = TH1::AddDirectoryStatus();
    TH1::AddDirectory(kFALSE);

    outputfile->cd();
    for (Int_t ipair = 0; ipair < npairs; ipair++) {
      TString pattern = TString::Format("Urqmd_%s%%s_DetaDphi_shft_%s", centfname[icent].c_str(), pname[ipair]);
      TList *meanhlist = extractMeanAndStDevFromSubSets(pairslists[ipair], pattern,cfname);
      for (Int_t ixh = 0; ixh < meanhlist->GetEntries(); ixh++) {
        meanhlist->At(ixh)->Write();
      }
      delete meanhlist;
    }

    /* back to normal */
    TH1::AddDirectory(oldstatus);
  }
  outputfile->Close();
  delete outputfile;
}
