#coding:utf8

bench_path="../"
import sys
if bench_path not in sys.path:sys.path.append(bench_path)
import benchmark

# Import results and ref input file
sim = benchmark.Data(specie="e-")
ref = benchmark.Data(specie="e-")
sim.extract_G4("test_nt_electron_t*.csv",nthreads=2)
ref.extract_txt("input.dat")

# Artificially creates an error if needed
# sim.w +=1.0
# sim.py +=1.0

# Instanciate an Test object with a maximum display of 3 errors per test
test = benchmark.Test(nerrors=3,rtol=1e-12)

# Print initialization
test.print_init(id="00")

# Define number of events
nevents = 1000

# Test if number of output entries = number of primaries
test.value('Number of events',len(sim.w),nevents)

# Test total number of particles
test.value('Total number of particles',sum(sim.w),sum(ref.w))

# Test weight normalization
wnorm=float(len(ref.w))/nevents
test.vector('Weight normalization',sim.w/wnorm,ref.w)

# Test if output momentum are present in the input
test.vector('px',sim.px,ref.px)
test.vector('py',sim.py,ref.py)
test.vector('pz',sim.pz,ref.pz)

# Define diag x coordinate (um)
Dx = 50.

# Test if x is in diag axis
test.vector('Diagnostic position',sim.x,[Dx])

# Calculate particle origin in y,z from momentum
Dy = Dx*sim.py/sim.px
Dz = Dx*sim.pz/sim.px
# Test if it fit with input value
test.vector('Origin of y',sim.y-Dy,ref.y)

# Test if propagation time is correct
Dt = sim.d()/sim.v()
test.atol=1e-2 # precision of 1e-2 fs
test.vector('Origin of t',sim.t-Dt,ref.t)

# Print errors
test.print_errors()