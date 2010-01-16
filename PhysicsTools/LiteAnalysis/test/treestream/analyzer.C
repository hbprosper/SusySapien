#include "src/DataFormats/FWLite/interface/Handle.h"

void analyzer()
{
  cout << endl << "\t\t<< analyzer >>" << endl << endl;

  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  gSystem->Load("libDataFormatsFWLite.so");

  TFile* file = new TFile("zmumu_pat.root");
  fwlite::Event ev(file);

  cout << endl << "\t\t<< enter event loop >>" << endl << endl;

  int event = 0;
  for (ev.toBegin(); !ev.atEnd(); ++ev) 
    {
      fwlite::Handle<vector<pat::Muon> > muons;
      muons.getByLabel(ev, "cleanLayer1Muons");
      //cout << event << "\tnmuon: " << muons.ptr()->size() << endl; 

      event++;
     }
}
