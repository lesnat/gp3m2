#coding:utf8

bench_path="../"
import sys
if bench_path not in sys.path:sys.path.append(bench_path)
import benchmark

import numpy as np
import matplotlib.pyplot as plt

"""
# Import results and input file
bench = benchmark.Benchmark(
        path        = ".",
        ref_file    = "input.dat",
        nthreads    = 2,
        nevents     = 1000,
        dpos        = 50.
        )




"""











"""
# Import results and input file
w,x,y,z,px,py,pz=benchmark.extract("test_nt_electron_t*.csv",nthreads=1)
W,X,Y,Z,Px,Py,Pz,T=np.loadtxt("input.dat").T

print("Launching analysis script ...")

# Instanciate an Error object with a maximum of 10 errors
error = benchmark.Error(10)

# Define number of events
nevents = 1000

# Test if number of output entries = number of primaries
error.test_value('len(w)',len(w),nevents)


# Set relative tolerance
error.rtol = 1e-2
# Test weight normalization via integrate
error.test_value('trapz(w)',np.trapz(w),np.trapz(W))

###
# if
# Test if the histogram of w is on same order of magnitude than W
error.atol = 0.5
# Number of primaries per entry. Each input row is used in average nopbe times. (here int)
nopbe = nevents/len(W)
# Make histos with weights
hw,bw=np.histogram(w,bins=100)
hW,bW=np.histogram(W,bins=100)
# Calculate total weight per bin
nw=hw*bw[1:]
nW=hW*bW[1:]
error.test_vector('nw/nW',(nw/nW)[nW!=0],[1])
###

# Set tolerance
error.rtol = 1e-5

# Test if output momentum are present in the input
error.test_vector('px',px,Px)
error.test_vector('py',py,Py)
error.test_vector('pz',pz,Pz)

# Test if all the input momentum are represented in the output
error.test_vector('Px',Px,px)
error.test_vector('Py',Py,py)
error.test_vector('Pz',Pz,pz)


# Set absolute tolerance to 0.01 (um)
error.atol = 0.01

# Define diag x coordinate (um)
Dx = 50.
# Test if x is in diag axis
error.test_vector('x',x,[Dx])

# Calculate particle origin in y,z from momentum
Dy = Dx*py/px
Dz = Dx*pz/px
# Test if it fit with input value
error.test_vector('y-Dy',y-Dy,Y)
error.test_vector('z-Dz',z-Dz,Z)

# Print errors
error.print_results()
"""
