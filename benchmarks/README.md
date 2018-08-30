# Benchmarking

## 0 - Input/Ouput correspondance in vacuum

Check input and output reading and writing, and the correspondance between
input and output for particle propagation in vacuum.

Commands :
- /target/addLayer material position(um)
- /input/setFileName filename
- /input/setParticle particle
- /output/setFileName filename

Tests :
- number of events
- convergence on total number of particles (precision = 10 %)
- weight normalization
- momentum conservation in each direction
- diagnostic position
- origin of y,z,t

## 1 - High statistics and multi-threading

Check the convergence on total number of particles (in vacuum),
and check the command to select number of threads.

Commands:
- /run/numberOfThreads N
- /run/printProgress N

Tests:
- convergence of total particle number with high statistics (precision = 1%)

## 2

Physics ...
