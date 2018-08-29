# Benchmarking

## 0

Check if output data is in the input for a G4_Galactic target material.
The x size is also changed for checking if this value is well exported.

Commands:
- /target/addLayer G4_Galactic 50
- /input/setFileName input.dat
- /input/setParticle e-
- /output/setFileName test


Verify :
- length of vectors corresponds to number of entries
- output px,py,pz are in input px,py,pz
- all the input px,py,pz are present in the output (check the statistics)
- x fit with diag coordinate
- output y,z fit with input y,z (with transformation because of geometry)

## 1

Same as 0, but with a longer and more realistic input file.

## 2

- /output/setLowEnergyLimit 10 keV
