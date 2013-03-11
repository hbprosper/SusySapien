{
  cout << "hello from rootlogon\n";
  gROOT->ProcessLine(".L bfunction.C+");
}
