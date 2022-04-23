#include "Riostream.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"
#include "TError.h"

extern void UrQMD2TTree(const char* filename, Int_t arrno);

// int main(const char *username, const char *datafilename, Int_t arrayno) {
int main(int argc, char* argv[])
{

  if (argc != 3)
    Fatal("UrQMDParser", "Wrong number of parameters %d", argc);

  int arrayno = -1;
  sscanf(argv[2], "%d", &arrayno);

  //  printf("first par: %s\n",argv[1]);
  //  printf("second par: %d\n",arrayno);

  UrQMD2TTree(argv[1], arrayno);
  return 0;
}
