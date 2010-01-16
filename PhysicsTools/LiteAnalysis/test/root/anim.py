#!/usr/bin/env python
#----------------------------------------------------------------------------
# File:        anim.py
# Description: Example of animation using Root and the Signal/Slot
#              mechanism
# Created:     January 2002 Harrison B. Prosper
#----------------------------------------------------------------------------
import sys
from sys  import argv, exit
from ROOT import *
from math import *
from time import time, ctime
from PhysicsTools.LiteAnalysis import *
#----------------------------------------------------------------------------
# Constants
#----------------------------------------------------------------------------
T0    = 0
PHI   = 30
COUNT = 100
DELAY = 100
#----------------------------------------------------------------------------
class Plot:

    def __init__(self, name, t, phi, delay):        

        self.t   = t
        self.phi = phi
        self.n   = 0
        self.timer = TTimer(delay)
        
        # Create a connection between timer and method animate of Plot
        
        self.conn = Connection(self.timer, "Timeout()", self, "animate")
        
        self.c = TCanvas("c","Test of animation", 200, 50, 500, 500)
        self.c.SetFillColor(0)

        self.f = TF2("f2","[0]*cos(2*x)*sin(2*y)",0,pi,0,pi)
        self.f.SetParameter(0,1)
        self.f.SetNpx(15)
        self.f.SetNpy(15)
        self.f.SetMaximum(1.0)
        self.f.SetMinimum(-1.0)
        self.f.Draw("surf1")
        
    def animate(self):
        self.n   += 1
        self.t   += 0.05*pi
        self.phi += 2
        
        self.f.SetParameter(0, cos(self.t))
        self.c.SetPhi(self.phi)
        self.c.Modified()
        self.c.Update()
        
        if self.n >= COUNT: self.timer.Stop()

    def start(self):
        self.timer.Start()
        gApplication.Run()
#----------------------------------------------------------------------------
def main():
    plot = Plot("Animated Plot", T0, PHI, DELAY)
    plot.start()
#----------------------------------------------------------------------------
main()
