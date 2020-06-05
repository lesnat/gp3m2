#coding:utf8

"""

Theoretical paper (equation (7))
D. J. S. Findlay, Nuclear Instruments and Methods in Physics Research Section A: Accelerators, Spectrometers, Detectors and Associated Equipment 276, 598 (1989).

Experimental paper
B. A. Faddegon, C. K. Ross, and D. W. O. Rogers, Medical Physics 18, 727 (1991).

"""

import numpy as np
from scipy.integrate import quad
import matplotlib.pyplot as plt
plt.ion()

colors = {
    0  : '#348ABD',
    10 : '#A60628',
    30 : '#7A68A6',
    60 : '#467821',
    90 : '#D55E00'
}

try:
    import p2sat
except ImportError:
    import sys
    sys.path.append("/mnt/local/esnault/Modules/p2sat/")
    import p2sat

# material = "Al"
material = "Pb"

# Get simulation results
gps = p2sat.datasets.PhaseSpace(specie="gamma", unit_system="UHI")
gps.load.gp3m2_csv(material)
if material == "Al": gps.edit.filter(select=dict(x=[35999, 36001]))
if material == "Pb": gps.edit.filter(select=dict(x=[7999, 8001]))

# Define experimental parameters
x           = 9.13                                                  # target witdth, in g/cm^2
r           = 17.95                                                 # target radius, in g/cm^2
rho         = 11.35                                                 # target material density in g/cm^3
E0          = 15.                                                   # incident electron energy, in MeV

NA          = 6.02e23                                               # Avogadro number
A           = 207.2                                                 # atomic weight of the target material
Z           = 82                                                    # atomic number of the target material
dE_over_dx  = 1.25                                                  # electron stopping power in the target material, in g/cm^2
mu          = 0.05                                                  # mass attenuation coefficient, in cm^2/g
X0          = 6.37                                                  # Pb radiation length, in g/cm^2

# Detectors
Emin        = 0.145                                                 # minimum energy of the detectors, in MeV
dtheta      = 0.4                                                   # angular acceptance of the detectors, in deg
angles      = [0, 10, 30, 60, 90]                                           # list of angles of the detectors, in deg

# Define theoretical parameters and functions
a           = 11.*1e-27                                             # parameter of the Seltzer and Berger cross section, equation (2), in mb
b           = 0.83                                                  # parameter of the Seltzer and Berger cross section, equation (2)
bbar        = np.sqrt(2)                                            # parameter of the divergence evolution, equation (3)
thetab      = 0.511/(E0 + 0.511)                                    # parameter of the divergence evolution, equation (3)
Ee          = 21.                                                   # parameter of the Rossi and Greisen formula, equation (5)
thetae2bar  = lambda xp: (Ee/E0 * np.sqrt(x/X0))**2                 # square of the rms multiple scattering angle of the Rossi and Greisen formula, equation (5)
ap          = np.exp(-0.05*x)                                       # parameter of the electron-photon transmission function, equation (6)
bp          = ap * (2.2/E0)                                         # parameter of the electron-photon transmission function, equation (6), in cm^2/g
upperbound  = lambda k:min([x, (E0 - k )/dE_over_dx, ap/bp])                                # upper bound of the integral in equation (7)
line1       = lambda xp, k, theta: NA/A * Z**2/k * a                                        # line 1 of equation (7)
line2       = lambda xp, k, theta: ap - bp * xp                                             # line 2 of equation (7)
line3       = lambda xp, k, theta: 1 - b * (k/(E0 - xp * dE_over_dx))                       # line 3 of equation (7)
line4       = lambda xp, k, theta: (np.pi * ((bbar * thetab)**2 + thetae2bar(xp)))**(-1)    # line 4 of equation (7)
line5       = lambda xp, k, theta: np.exp(-theta**2/((bbar * thetab)**2 + thetae2bar(xp)))  # line 5 of equation (7)
I_th        = lambda xp, k, theta: line1(xp, k, theta) * line2(xp, k, theta) * line3(xp, k, theta) * line4(xp, k, theta) * line5(xp, k, theta)


# Plot spectra (number/MeV/sr/incident e-) for each angle
for angle in angles:
    print(angle)
    ## Plot simulation results
    # Retrieve simulation data
    """
    if angle==0:
        # If the detector is along propagation axis, the solid angle is fully defined with the upper bound on theta
        E_sim, S_sim = p2sat.hist.hist1d(gps, 'ekin', bwidth=.1, select=dict(thetax=[0, np.radians(dtheta)])) # Retrieve energy distribution
        domega = 2 * np.pi * (1 - np.cos(np.radians(angle))) # Get corresponding solid angle
        S_sim /= domega # Normalize number of photons on solid angle
    else:
        # Otherwise, we select a ring with theta between a-A0 and a+A0 and we assume an isotropical beam on phi to rescale the solid angle, in order to get a better statistics
        E_sim, S_sim = p2sat.hist.hist1d(gps, 'ekin', bwidth=.1, select=dict(thetax=[np.radians(angle-dtheta), np.radians(angle+dtheta)])) # Retrieve energy distribution
        domega = (2 * np.pi * (1 - np.cos(np.radians(angle+dtheta))) - 2 * np.pi * (1 - np.cos(np.radians(angle-dtheta)))) * np.radians(2 * dtheta)/(2 * np.pi) # Get corresponding solid angle
        S_sim *= np.radians(2 * dtheta)/(2 * np.pi) # Rescale number of photons on phi angle
        S_sim /= domega # Normalize number of photons on solid angle
    """

    omega_min = 2 * (1 - np.cos(np.radians(max(angle-dtheta,0)))) # the minimum theta angle is 0
    omega_max = 2 * (1 - np.cos(np.radians(angle+dtheta)))
    domega=np.pi*(omega_max-omega_min)
    select=dict(omegax=[omega_min,omega_max])
    if angle==0:
        # If the detector is along propagation axis, the solid angle is fully defined with the upper bound on theta
        E_sim, S_sim = p2sat.hist.hist1d(gps, 'ekin', bwidth=.1, select=select) # Retrieve energy distribution
        # domega = 2 * np.pi * (1 - np.cos(np.radians(angle))) # Get corresponding solid angle
        S_sim /= domega # Normalize number of photons on solid angle
    else:
        # Otherwise, we select a ring with theta between a-A0 and a+A0 and we assume an isotropical beam on phi to rescale the solid angle, in order to get a better statistics
        E_sim, S_sim = p2sat.hist.hist1d(gps, 'ekin', bwidth=.1, select=select) # Retrieve energy distribution
        # domega = (2 * np.pi * (1 - np.cos(np.radians(angle+dtheta))) - 2 * np.pi * (1 - np.cos(np.radians(angle-dtheta)))) * np.radians(2 * dtheta)/(2 * np.pi) # Get corresponding solid angle
        #S_sim *= np.radians(2 * dtheta)/(2 * np.pi) # Rescale number of photons on phi angle
        S_sim /= domega # Normalize number of photons on solid angle
    # Plot simulation data
    plt.step(E_sim[:-1], S_sim, '-', where='post',color=colors[angle], label="%i deg"%angle)

    ## Plot experimental results
    # Retrieve experimental data
    E_exp, S_0, err_0, S_10, err_10, S_30, err_30, S_60, err_60, S_90, err_90 = np.loadtxt("Faddegon_1991_%s.dat"%material, unpack=True)
    # Plot numerical data
    plt.errorbar(E_exp, eval("S_%s"%angle), eval("err_%s"%angle), marker='o',linestyle='None',color=colors[angle])

    ## Plot theoretical results
    if material == "Pb":
        # Compute theoretical data
        E_th = np.linspace(1e-3, 15, 1000)
        S_th = []
        S_th = np.array([quad(I_th, 0, upperbound(e), args=(e, np.radians(angle)))[0] for e in E_th])
        # Compute and plot theoretical data
        plt.plot(E_th, S_th, '--', color=colors[angle], label="%i deg"%angle)

    plt.pause(.001)

# Format the plot
plt.xlim(0, 16)
plt.ylim(1e-5, 1e1)
plt.yscale("log")
plt.ylabel(r"$d^2 N_{\gamma}/d E_{\gamma} d \Omega$  [$(MeV \ sr \ N_{e^-})^{-1}$]")
plt.xlabel(r"$E_\gamma$ [$MeV$]")

# Format the legend
plt.gca().add_artist(plt.legend(loc=3,ncol=2))

from matplotlib.lines import Line2D
plt.gca().add_artist(plt.legend(
            (Line2D([0],[0],marker="o",ls="None",c='k'),
            Line2D([0],[0],marker="None",ls="-",c='k')),
            ("Faddegon 1991","gp3m2"),
            loc=1))

plt.pause(.001)


#input("Waiting ...")
