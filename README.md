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

# Commands

This app uses `ParticleGun` to create primary events. You then have access to the `/gun/` folder of commands.

In supplement to native Geant4 commands, this app also define other commands :
- /target/addLayer material size(um)
- /input/setFileName filename
- /input/setParticle particle
- /output/setFileName filename
- /output/setLowEnergyLimit number unit



# Known problems

- Infinite statistical weight when the number of events is less than number of particles
