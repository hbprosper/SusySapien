//-----------------------------------------------------------------------------
// File:        analyzer.cc
// Description: Analyzer for ntuples created by Mkntuple
// Created:     Wed Mar 10 08:10:43 2010 by mkntanalyzer.py
// $Revision:$
//-----------------------------------------------------------------------------

// -- System

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

// -- CMSSW

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/LiteAnalysis/interface/treestream.hpp"
#include "PhysicsTools/LiteAnalysis/interface/kit.h"

// -- Root

#include "TApplication.h"
#include "TCanvas.h"
#include "TH1F.h"
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------

// -- Procedures and functions

void
error(string message)
{
  cout << "** error ** " << message << endl;
  exit(0);
}

// Read ntuple filenames from file list

vector<string>
getFilenames(int argc, char** argv)
{
  string filelist("filelist.txt");
  if ( argc > 1 ) filelist = string(argv[1]);

  // Open file list
  
  ifstream stream(filelist.c_str());
  if ( !stream.good() ) error("unable to open file: " + filelist);

  // Get list of ntuple files to be processed
  
  vector<string> v;
  string filename;
  while ( stream >> filename )
    if ( kit::strip(filename) != "" ) v.push_back(filename);
  return v;
}

//=============================================================================

int main(int argc, char** argv)
{
  cout << "enabling autoloader...";
  AutoLibraryLoader::enable();
  cout << "done!" << endl;
  
  // The root application is needed to make canvases visible during
  // program execution. If this is not needed, just comment out the following
  // line

  TApplication app("analyzer", &argc, argv);
  
  // Get names of ntuple files to be processed and open chain of ntuples
  
  vector<string> filenames = getFilenames(argc, argv);
  itreestream stream(filenames, "Events");
  if ( !stream.good() ) error("unable to open ntuple file(s)");
  
  // Get number of events to be read
  
  int nevents = stream.size();
  cout << "Number of events: " << nevents << endl;
  
  //---------------------------------------------------------------------------
  // Define variables to be read
  //---------------------------------------------------------------------------
  double	beamspot_x0;
  double	beamspot_y0;
  double	beamspot_z0;
  std::vector<int>	genmother_charge(1000,0);
  std::vector<double>	genmother_eta(1000,0);
  std::vector<double>	genmother_mass(1000,0);
  std::vector<int>	genmother_pdgId(1000,0);
  std::vector<double>	genmother_phi(1000,0);
  std::vector<double>	genmother_pt(1000,0);
  std::vector<int>	genmother_status(1000,0);
  std::vector<int>	genparticle_charge(1000,0);
  std::vector<double>	genparticle_eta(1000,0);
  std::vector<double>	genparticle_mass(1000,0);
  std::vector<int>	genparticle_pdgId(1000,0);
  std::vector<double>	genparticle_phi(1000,0);
  std::vector<double>	genparticle_pt(1000,0);
  std::vector<int>	genparticle_status(1000,0);
  double	genruninfoproduct_externalXSecLO_value;
  double	genruninfoproduct_filterEfficiency;
  double	genruninfoproduct_internalXSec_value;
  std::vector<float>	muon_caloIso(500,0);
  std::vector<int>	muon_charge(500,0);
  std::vector<float>	muon_ecalIso(500,0);
  std::vector<double>	muon_ecalIsoDeposit_candEnergy(500,0);
  std::vector<double>	muon_energy(500,0);
  std::vector<double>	muon_et(500,0);
  std::vector<double>	muon_eta(500,0);
  std::vector<float>	muon_hcalIso(500,0);
  std::vector<int>	muon_isCaloMuon(500,0);
  std::vector<int>	muon_isGlobalMuon(500,0);
  std::vector<int>	muon_isStandAloneMuon(500,0);
  std::vector<int>	muon_isTrackerMuon(500,0);
  std::vector<double>	muon_p(500,0);
  std::vector<double>	muon_phi(500,0);
  std::vector<double>	muon_pt(500,0);
  std::vector<float>	muon_trackIso(500,0);
  int	ngenmother;
  int	ngenparticle;
  int	nmuon;
  int	triggerbits_HLT_Jet30;
  int	triggerbits_HLT_Jet50;
  int	triggerbits_HLT_Jet80;
  int	triggerbits_value_HLT_L1Jet15;

  stream.select("recoBeamSpot_offlineBeamSpot.x0", beamspot_x0);
  stream.select("recoBeamSpot_offlineBeamSpot.y0", beamspot_y0);
  stream.select("recoBeamSpot_offlineBeamSpot.z0", beamspot_z0);
  stream.select("GenMother_genParticles.charge", genmother_charge);
  stream.select("GenMother_genParticles.eta", genmother_eta);
  stream.select("GenMother_genParticles.mass", genmother_mass);
  stream.select("GenMother_genParticles.pdgId", genmother_pdgId);
  stream.select("GenMother_genParticles.phi", genmother_phi);
  stream.select("GenMother_genParticles.pt", genmother_pt);
  stream.select("GenMother_genParticles.status", genmother_status);
  stream.select("recoGenParticle_genParticles.charge", genparticle_charge);
  stream.select("recoGenParticle_genParticles.eta", genparticle_eta);
  stream.select("recoGenParticle_genParticles.mass", genparticle_mass);
  stream.select("recoGenParticle_genParticles.pdgId", genparticle_pdgId);
  stream.select("recoGenParticle_genParticles.phi", genparticle_phi);
  stream.select("recoGenParticle_genParticles.pt", genparticle_pt);
  stream.select("recoGenParticle_genParticles.status", genparticle_status);
  stream.select("GenRunInfoProduct_generator.externalXSecLO_value", genruninfoproduct_externalXSecLO_value);
  stream.select("GenRunInfoProduct_generator.filterEfficiency", genruninfoproduct_filterEfficiency);
  stream.select("GenRunInfoProduct_generator.internalXSec_value", genruninfoproduct_internalXSec_value);
  stream.select("patMuon_cleanLayer1Muons.caloIso", muon_caloIso);
  stream.select("patMuon_cleanLayer1Muons.charge", muon_charge);
  stream.select("patMuon_cleanLayer1Muons.ecalIso", muon_ecalIso);
  stream.select("patMuon_cleanLayer1Muons.ecalIsoDeposit_candEnergy", muon_ecalIsoDeposit_candEnergy);
  stream.select("patMuon_cleanLayer1Muons.energy", muon_energy);
  stream.select("patMuon_cleanLayer1Muons.et", muon_et);
  stream.select("patMuon_cleanLayer1Muons.eta", muon_eta);
  stream.select("patMuon_cleanLayer1Muons.hcalIso", muon_hcalIso);
  stream.select("patMuon_cleanLayer1Muons.isCaloMuon", muon_isCaloMuon);
  stream.select("patMuon_cleanLayer1Muons.isGlobalMuon", muon_isGlobalMuon);
  stream.select("patMuon_cleanLayer1Muons.isStandAloneMuon", muon_isStandAloneMuon);
  stream.select("patMuon_cleanLayer1Muons.isTrackerMuon", muon_isTrackerMuon);
  stream.select("patMuon_cleanLayer1Muons.p", muon_p);
  stream.select("patMuon_cleanLayer1Muons.phi", muon_phi);
  stream.select("patMuon_cleanLayer1Muons.pt", muon_pt);
  stream.select("patMuon_cleanLayer1Muons.trackIso", muon_trackIso);
  stream.select("nGenMother_genParticles", ngenmother);
  stream.select("nrecoGenParticle_genParticles", ngenparticle);
  stream.select("npatMuon_cleanLayer1Muons", nmuon);
  stream.select("triggerBits_TriggerResults.HLT_Jet30", triggerbits_HLT_Jet30);
  stream.select("triggerBits_TriggerResults.HLT_Jet50", triggerbits_HLT_Jet50);
  stream.select("triggerBits_TriggerResults.HLT_Jet80", triggerbits_HLT_Jet80);
  stream.select("triggerBits_TriggerResults.value_HLT_L1Jet15", triggerbits_value_HLT_L1Jet15);


  //---------------------------------------------------------------------------
  // Book histograms etc.
  //---------------------------------------------------------------------------
  kit::setStyle();

  //---------------------------------------------------------------------------
  // Loop over events
  //---------------------------------------------------------------------------

  for(int entry=0; entry < nevents; entry++)
    {
      stream.read(entry);

	  // Find SUSY...
	  
    }
  stream.close();
  return 0;
}
