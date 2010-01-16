void cloneme()
{
  TFile* file = new TFile("simpletree.root");
  TTree* tree = (TTree*)file->Get("Analysis");

  TFile* fileout = new TFile("newtree.root", "recreate");
  TTree* newtree = (TTree*)tree->CloneTree(0); // Just structure
  newtree->SetDirectory(fileout);

  for(int i=0; i < 10; i++)
    {
      tree->GetEntry(i);
      newtree->Fill();
    }
  fileout->Write();
}
