{
string cmsbase = ".include " + string(gSystem->Getenv("CMSSW_RELEASE_BASE"));
gROOT->ProcessLine(cmsbase.c_str());
gROOT->ProcessLine(".include");
}
