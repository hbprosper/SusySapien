
void fatal(string message)
{
  cout << message << endl;
  exit(0);
}

void getbranches()
{
  gSystem->Load("libFWCoreFWLite");
  gROOT->ProcessLine(".L getlist.cpp+");

  TFile* file = new TFile("zee_pat.root");

  TTree* tree = (TTree*)file->Get("Events");
  if ( ! tree ) 
	fatal("EventStream - unable to get tree ");

  // -----------------------------------------------------------------
  // Get all branch names ending in ".obj"
  // -----------------------------------------------------------------

  TObjArray* array = tree->GetListOfBranches();
  if ( ! array ) exit(0);

  ofstream out("branches.txt");
  int nitems = array->GetEntries();
  for (int i = 0; i < nitems; i++)
	{
	  TBranch* b = (TBranch*)((*array)[i]);
      out << b->GetName() << endl;
      getlist(out, b, 1);
    }
  out.close();
  file->Close();
  exit(0);
}


