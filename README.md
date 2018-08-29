# Geant4 Particle Phase-space Propagation in a Multi-layer Material

gp3m2 is a Geant4 application created to simulate the particle phase-space propagation in a multi-layer material.

It gets a phase space as input, and export particle phase space when a particle cross any layer boundary.

The phase space contains following informations :
- Particle statistical weight
- Particle position
- Particle momentum
- Time when the particle has cross the boundary

**Notes :**
- This G4 app was made for my personal use, to simulate propagation of particles created by laser-plasma interaction (simulation done with a Particle-In-Cell code)
- This app is experimental, no warranty about the physical results you may find

## Install

You first need Geant4 installed with multi-threading mode,
you can then download the gp3m2 code from github and type the following commands

```bash
cd /path/to/gp3m2/
mkdir build
cd build
cmake ../
make
```

It is possible to use the `make` command with multi-thread using `make -jN`,
`N` being the number of threads you want to use.

The bash script `compile` can also be used to compile the source (when `cmake` command was done).

## Commands

This app uses the `G4ParticleGun` class to create primary events.
You then have access to the `/gun/` folder of commands.

In addition to native Geant4 commands, this app also define other commands :
- /target/addLayer material size(um)
- /input/setFileName filename
- /input/setParticle particle
- /output/setFileName filename
- /output/setLowEnergyLimit number unit

## Benchmarks

I made some benchmarks in order to validate the method.
To launch the benchmark `N` from the root dir, you can type

```bash
./benchmark N
```

More informations can be found at `benchmarks/README.md`

## Documentation

You can generate a html documentation with doxygen (class and methods are documented).
In the folder `doc/`, type

```bash
doxygen doxyfile
```

or use doxywizard.

## Known problems

- Infinite statistical weight when the number of events is less than number of particles
