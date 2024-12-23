#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) 2024 C.R. Drisko. All rights reserved.
# Licensed under the MIT License. See the LICENSE file in the project root for more information.
#
# Name: harmonic.py - Version 1.0.0
# Author: crdrisko
# Date: 12/20/2024-05:37:39
# Description:

import numpy as np
import matplotlib.pyplot as plt

def harmonic_pos(Amplitude, omega, time, phase):
    return Amplitude * np.cos(omega * time - phase)

def harmonic_vel(Amplitude, omega, time, phase):
    return -Amplitude * omega * np.sin(omega * time - phase)

def calc_force(spring_constant, displacement):
    return -spring_constant * displacement

x_0       = 1
v_0       = 1
mass      = 1
k         = 1
damping   = 1
omega     = np.sqrt(k / mass)
phase     = np.arctan(v_0 / omega / x_0)
amplitude = np.sqrt(x_0**2 + (v_0 / omega)**2)

# t        = np.linspace(0, 2 * np.pi / omega, 100)
t        = np.arange(0, 100, np.pi / 3 / omega)
x        = np.zeros(len(t))
v        = np.zeros(len(t))
x_euler  = np.zeros(len(t))
v_euler  = np.zeros(len(t))
x_verlet = np.zeros(len(t))
v_verlet = np.zeros(len(t))

dt   = t[1] - t[0]
dt_2 = 0.5 * dt

print(dt)

x[0]        = x_0
v[0]        = v_0
x_euler[0]  = x_0
v_euler[0]  = v_0
x_verlet[0] = x_0
v_verlet[0] = v_0

for i in range(1, len(t)):
    x[i] = harmonic_pos(amplitude, omega, t[i - 1], phase)
    v[i] = harmonic_vel(amplitude, omega, t[i - 1], phase)

    x_euler[i] = x_euler[i-1] + v_euler[i-1] * dt
    v_euler[i] = v_euler[i-1] + dt * calc_force(k, x_euler[i]) / mass
    v_euler[i] *= damping**dt

    v_halfstep = v_verlet[i - 1] + dt_2 * calc_force(k, x_verlet[i - 1]) / mass
    x_verlet[i] = x_verlet[i - 1] + dt * v_halfstep
    v_verlet[i] = v_halfstep + dt_2 * calc_force(k, x_verlet[i]) / mass

plt.plot(x, v, label='Analytical')
plt.plot(x_euler, v_euler, label='Euler')
plt.plot(x_verlet, v_verlet, label='Verlet')
plt.legend()
plt.show()
