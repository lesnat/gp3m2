# Geant4 Particle Phase-space Propagation in a Multi-layer Material

gp3m2 is a Geant4 application created to simulate the particle phase-space propagation in a multi-layer material. It gets a phase space as input, and export particle phase space when a particle cross any layer boundary.

## Principle

The phase space contains following informations :

- Particle statistical weight
- Particle position
- Particle momentum
- Time when the particle has cross the boundary

**Notes :**
- This G4 app was made for my personal use, to simulate propagation of particles created by laser-plasma interaction (simulation done with a Particle-In-Cell code)
- This app is experimental, no warranty about the physical results you may find

## Installation

### Geant4 build

You first need Geant4 installed with data and multi-threading mode. Build options for the `cmake` command are:

```bash
CMAKE_INSTALL_PREFIX=/path/to/your/install
GEANT4_INSTALL_DATA=ON
GEANT4_BUILD_MULTITHREADED=ON
GEANT4_USE_QT=ON # This is optional, but needed for visualisation
```

If the `GEANT4_USE_QT` tag is set, it requires qt5 and openGL librairies (`qt5-default` package on Ubuntu).

```bash
source /path/to/your/install/bin/geant4.sh
```

### gp3m2 build

You can then download the gp3m2 code from github and type the following commands

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

### Benchmarks

I made some benchmarks in order to validate the method.
To launch the benchmark `N` from the root dir, you can type

```bash
./benchmark [N] [v]
```

To see the Geant4 output in the terminal, use the keyword `v` as a second argument.

By default the benchmark 0 is launched with no verbose mode.

More informations can be found at `benchmarks/README.md`.



## Features and usage

### Units

...

```

```



### Geometry

The geometry is

image

It can be managed with macro commands

```
/target/
```

### InputReader



### Diagnostics



### Other macro commands

This app uses the `G4ParticleGun` class to create primary events.
You then have access to the `/gun/` folder of commands.

In addition to native Geant4 commands, this app also define other commands :
- /target/addLayer material size
- /input/setFileName filename
- /input/setParticle particle
- /output/setFileName filename
- /output/setLowEnergyLimit number unit

## Documentation
### Geant4 documentation

If you're not familiar with the Geant4 toolkit, you can find documentation here:



### gp3m2 documentation

You can generate a html documentation with doxygen (class and methods are documented).
In the folder `doc/`, type

```bash
doxygen doxyfile
```

or use doxywizard.

The code structure is:

![](/mnt/local/esnault/gp3m2/code_structure.png)

## Known problems/Remarks

- Infinite statistical weight in diagnostics when the number of events is less than number of lines of the input file (number of configurations)
- Precision of 6 digits in diagnostics (forced by the analysis manager ?)
