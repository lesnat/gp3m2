#coding:utf8

bench_path="../"
import sys
if bench_path not in sys.path:sys.path.append(bench_path)
import benchmark

# Import results and reference input file
sim = benchmark.Data(specie="e-")
ref = benchmark.Data(specie="e-")
sim.extract_G4("test_nt_electron_t*.csv",nthreads=2)
ref.extract_txt("input.dat")

# Define number of events
nevents = 1000

# Also test if all the input are in the output (OK for high statistics)
both=True

# Artificially creates an error if needed
# sim.w +=1.0
# sim.py +=1.0

# Instanciate an Test object with a maximum display of 3 errors per test
test = benchmark.Test(nerrors=3,rtol=1e-5)

# Print initialization
test.print_init(id="0")

# Test if number of output entries = number of primaries
test.value('Number of events',len(sim.w),nevents)

# Test total number of particles (low rtol because of the low statistics)
test.rtol=1e-1
test.value('Total number of particles',sum(sim.w),sum(ref.w))

# Test weight normalization
test.rtol=1e-5
wnorm=float(len(ref.w))/nevents
test.vector('Weight normalization',sim.w/wnorm,ref.w,both=both)

# Test if output momentum are present in the input
test.vector('px',sim.px,ref.px,both=both)
test.vector('py',sim.py,ref.py,both=both)
test.vector('pz',sim.pz,ref.pz,both=both)

# Define diag x coordinate (um)
Dx = 50.
x0 = 1.5

# Test if x is in diag axis
test.vector('Diagnostic position',sim.x,[Dx],both=both)

# Calculate particle origin in y,z from momentum
Dy = (Dx-x0)*sim.py/sim.px
Dz = (Dx-x0)*sim.pz/sim.px
# Test if it fit with input value
test.vector('Origin of y',sim.y-Dy,ref.y,both=both)
test.vector('Origin of z',sim.z-Dz,ref.z,both=both)

# Test if propagation time is correct
test.rtol=1e-4
Dt = sim.d(x0=x0,y0=sim.y-Dy,z0=sim.z-Dz)/sim.v()
test.vector('Origin of t',sim.t-Dt,ref.t,both=both)

# Print errors
test.print_errors()
