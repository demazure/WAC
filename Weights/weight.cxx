#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TRandom2.h>
#include "fortranc.h"
#include "weight.h"

// --- Prototype of the function used in the weight calculator 
//     (in FsiWeightLedinicky.F)
#define fsiini F77_NAME(fsiini,FSIINI)
extern "C" {void type_of_call F77_NAME(fsiini,FSIINI)(const int &itest,const int &ll,const int &ns,const int &ich, const int &iqs, const int &isi,const int &i3c);}
#define fsinucl F77_NAME(fsinucl,FSINUCL)
extern "C" {void type_of_call  F77_NAME(fsinucl,FSINUCL)(const double &mn,const double &cn);}
#define fsimomentum F77_NAME(fsimomentum,FSIMOMENTUM)
extern "C" {void type_of_call F77_NAME(fsimomentum,FSIMOMENTUM)(double &p1,double &p2);}
#define fsiposition F77_NAME(fsiposition,FSIPOSITION)
extern "C" {void type_of_call F77_NAME(fsiposition,FSIPOSITION)(double &x1,double &x2);}
#define fsiw F77_NAME(fsiw,FSIW)
extern "C" {void type_of_call F77_NAME(fsiw,FSIW)(const int &i,double &weif,
						  double &wei,double &wein);}
#define ltran12 F77_NAME(ltran12,LTRAN12)
extern "C" {void type_of_call ltran12_();}

// --- Additional prototyping of some CERN functions (in FsiTool.F)
typedef float   REAL;
typedef struct { REAL re; REAL im; } COMPLEX;
#define cgamma F77_NAME(cgamma,CGAMMA)
extern "C" {COMPLEX type_of_call cgamma_(COMPLEX*);}


int dico(int id1, int id2)
{
bool s;
int m;
int M;
s = (id1*id2>0);
int a1=id1;
if (a1<0) a1*=-1;
int a2=id2;
if (a2<0) a2*=-1;
if (a1<a2){
  m=a1;
  M=a2;}
else {
  M=a1;
  m=a2;}

int LL=0;
if      ( (m==2212) && (M==2212) &&  s ) LL=2;
else if ( (m==211 ) && (M==211 ) && !s ) LL=5;
else if ( (m==211 ) && (M==211 ) &&  s ) LL=7;
else if ( (m==211 ) && (M==321 ) && !s ) LL=10;
else if ( (m==211 ) && (M==321 ) &&  s ) LL=11;
else if ( (m==211 ) && (M==2212) &&  s ) LL=12;
else if ( (m==211 ) && (M==2212) && !s ) LL=13;
else if ( (m==321 ) && (M==321 ) && !s ) LL=14;
else if ( (m==321 ) && (M==321 ) &&  s ) LL=15;
else if ( (m==321 ) && (M==2212) &&  s ) LL=16;
else if ( (m==321 ) && (M==2212) && !s ) LL=17;
else if ( (m==2212) && (M==3122) &&true) LL=27;
else if ( (m==3122) && (M==3122) &&true) LL=29;
else if ( (m==2212) && (M==2212) && !s ) LL=30;

return LL;
}


double weight(int id1, int id2, double px1, double py1, double pz1, double e1, double px2, double py2, double pz2, double e2, double x1, double y1, double z1, double t1, double x2, double y2, double z2, double t2)
{
  int LL;
  LL = dico(id1, id2);
  if (LL==0) return 1.0;
  fsiini(0,LL,0,0,0,0,0);
  fsinucl(1.,0.);
  
  double *mom1 = (double *) malloc(sizeof(double) * 4);
  double *mom2 = (double *) malloc(sizeof(double) * 4);
  double *pos1 = (double *) malloc(sizeof(double) * 4);
  double *pos2 = (double *) malloc(sizeof(double) * 4);
  mom1[0]=px1;
  mom1[1]=py1;
  mom1[2]=pz1;
  mom1[3]=e1;
  mom2[0]=px2;
  mom2[1]=py2;
  mom2[2]=pz2;
  mom2[3]=e2;
  pos1[0]=x1;
  pos1[1]=y1;
  pos1[2]=z1;
  pos1[3]=t1;
  pos2[0]=x2;
  pos2[1]=y2;
  pos2[2]=z2;
  pos2[3]=t2;

  fsimomentum(*mom1,*mom2);
  fsiposition(*pos1,*pos2);
  ltran12();
  double Wei;
  double Wein;
  double Weif;
  fsiw(1,Weif,Wei,Wein);
  return Wei;
}