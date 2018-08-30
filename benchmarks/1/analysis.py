#coding:utf8

bench_path="../"
import sys
if bench_path not in sys.path:sys.path.append(bench_path)
import benchmark

# Import results and reference input file
sim = benchmark.Data(specie="e-")
ref = benchmark.Data(specie="e-")
sim.extract_G4("test_nt_electron_t*.csv",nthreads=4)
ref.extract_txt("input.dat")

# Define number of events
nevents = 1e5

# Instanciate an Test object with a maximum display of 3 errors per test
test = benchmark.Test(nerrors=3,rtol=1e-2)

# Print initialization
test.print_init(id="1")

# Test if number of output entries = number of primaries
test.value('Number of events',len(sim.w),nevents)

# Test total number of particles
# (1% error max for 1e5 particles (20 configurations => 5000 e-/configuration))
test.value('Total number of particles',sum(sim.w),sum(ref.w))

# Print errors
test.print_errors()
