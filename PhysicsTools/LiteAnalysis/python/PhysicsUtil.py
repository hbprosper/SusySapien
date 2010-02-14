#-----------------------------------------------------------------------------
#$Revision: 1.3 $
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
vfloat       = vector("float")
vdouble      = vector("double")
vstring      = vector("string")
vvdouble     = vector("vector<double>")

vGenParticle = vector("reco::GenParticle")
vMatchedPair = vector("kit::MatchedPair")
vPtThing     = vector("kit::PtThing")
vTVector3    = vector("TVector3")
vTLorentzVector = vector("TLorentzVector")



