#coding:utf8

bench_path="../"
import sys
if bench_path not in sys.path:sys.path.append(bench_path)
import benchmark

import numpy as np
import matplotlib.pyplot as plt


# Import results and input file
sim = benchmark.Data()
ref = benchmark.Data()
sim.extract_G4("test_nt_electron_t*.csv",nthreads=2)
ref.extract_txt("input.dat")

print("Launching analysis script ...")

# Instanciate an Error object with a maximum of 10 errors
test = benchmark.Test(10)

# Define number of events
nevents = 1000

# Test if number of output entries = number of primaries
test.value('Number of events',len(sim.w),nevents)


# Set relative tolerance
test.rtol = 1e-2
# Test total number of particles
test.value('Total number of particles',np.trapz(sim.w),np.trapz(ref.w))
# Test weight normalization
test.vector('Weight normalization : sim in ref',sim.w*nevents,ref.w)
test.vector('Weight normalization : ref in sim',ref.w,sim.w*nevents)
"""
# if
# Test if the histogram of w is on same order of magnitude than W
test.atol = 0.5
# Number of primaries per entry. Each input row is used in average nopbe times. (here int)
nopbe = nevents/len(W)
# Make histos with weights
hw,bw=np.histogram(w,bins=100)
hW,bW=np.histogram(W,bins=100)
# Calculate total weight per bin
nw=hw*bw[1:]
nW=hW*bW[1:]
test.vector('nw/nW',(nw/nW)[nW!=0],[1])
"""

# Set tolerance
test.rtol = 1e-5

# Test if output momentum are present in the input
test.vector('px : sim in ref',sim.px,ref.px)
test.vector('py : sim in ref',sim.py,ref.py)
test.vector('pz : sim in ref',sim.pz,ref.pz)

# Test if all the input momentum are represented in the output
test.vector('px : ref in sim',ref.px,sim.px)
test.vector('py : ref in sim',ref.py,sim.py)
test.vector('pz : ref in sim',ref.pz,sim.pz)


# Set absolute tolerance to 0.01 (um)
test.atol = 0.01

# Define diag x coordinate (um)
Dx = 50.
# Test if x is in diag axis
test.vector('x',sim.x,[Dx])
# Test if the only x are at diag pos or 0
test.vector('x',[Dx,0],sim.x)

# Calculate particle origin in y,z from momentum
Dy = Dx*sim.py/sim.px
Dz = Dx*sim.pz/sim.px
# Test if it fit with input value
test.vector('y-Dy',sim.y-Dy,ref.y)
test.vector('z-Dz',sim.z-Dz,ref.z)

# Print errors
test.print_results()
