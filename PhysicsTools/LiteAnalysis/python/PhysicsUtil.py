#-----------------------------------------------------------------------------
#$Revision: 1.2 $
import sys
from ROOT import *
#-----------------------------------------------------------------------------
def fatal(message):
	print "** error ** %s" % message
	sys.exit(0)
#-----------------------------------------------------------------------------
def enableAutoLoader():
	if not (gSystem.Load( "libFWCoreFWLite" ) == 0):
		fatal("unable to load FWCoreFWLite")        
	AutoLibraryLoader.enable()
#-----------------------------------------------------------------------------
enableAutoLoader()

vint         = vector("int")
vdouble      = vector("double")
vstring      = vector("string")
vvdouble     = vector("vector<double>")

vGenParticle = vector("reco::GenParticle")
vMatchedPair = vector("kit::MatchedPair")
vPtThing     = vector("kit::PtThing")
vTVector3    = vector("TVector3")
vTLorentzVector = vector("TLorentzVector")



