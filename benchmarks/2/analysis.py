#coding:utf8

import numpy as np
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

# Get experimental results
# Based on the paper B. A. Faddegon, C. K. Ross, and D. W. O. Rogers, Medical Physics 18, 727 (1991).
E_exp, S_0, err_0, S_10, err_10, S_30, err_30, S_60, err_60, S_90, err_90 = np.loadtxt("Faddegon_1991_Al.dat",unpack=True)

# Define experimental parameters
E0 = 0.145 # MeV
A0 = 0.4 # deg
# angle = [0,10,30,60,90]
angle = [0,10,30]

# Get simulation results
gps = p2sat.datasets.PhaseSpace(specie="gamma", unit_system="UHI")
gps.load.gp3m2_csv("Al")
# gps.edit.round_axis("x")

# Plot spectra (number/MeV/sr/incident e-) for each angle
for a in angle:
    # Selecting all the particle on a ring with theta between a-A0 and a+A0
    omega_min = 2*np.pi * (1 - np.cos(np.radians(max(a-A0,0)))) # the minimum theta angle is 0
    omega_max = 2*np.pi * (1 - np.cos(np.radians(a+A0)))
    domega=omega_max-omega_min
    select=dict(omegax=[omega_min,omega_max],x=36000)

    # Get the desired spectra
    E_sim, S_sim = p2sat.hist.hist1d(gps, 'ekin',bwidth=.1,select=select)

    # Assuming an isotropical beam on phi, we can rescale the selection on a ring to a "square" of edge A0 modeling the detector
    if a>0: # This operation is basically useless but remain here for clarity
        S_sim *= np.radians(2*A0)/(2*np.pi) # Rescale spectrum on phi angle
        domega *= np.radians(2*A0)/(2*np.pi)# Rescale domega on phi angle

    # Plot simulation results
    plt.step(E_sim[:-1], S_sim/domega, where='post',color=colors[a], label="%i deg"%a)

    # Plot experimental results
    S_exp = eval("S_%s"%a)
    err_exp = eval("err_%s"%a)
    plt.errorbar(E_exp,S_exp,err_exp, marker='o',linestyle='None',color=colors[a])

    # Format the plot
    plt.yscale("log")
    plt.ylabel(r"$d^2 N_{\gamma}/d E_{\gamma} d \Omega$  [$(MeV \ sr \ N_{e^-})^{-1}$]")
    plt.xlabel(r"$E_\gamma$ [$MeV$]")
    plt.pause(.001)

# Format the legend
plt.gca().add_artist(plt.legend(loc=3,ncol=2))

from matplotlib.lines import Line2D
plt.gca().add_artist(plt.legend(
            (Line2D([0],[0],marker="o",ls="None",c='k'),
            Line2D([0],[0],marker="None",ls="-",c='k')),
            ("Faddegon 1991","gp3m2"),
            loc=1))

plt.pause(.001)

input("Waiting ...")
