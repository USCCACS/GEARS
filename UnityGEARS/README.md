# GEARS in Unity 5

## System Requirements
* [Windows 10 OS](https://www.microsoft.com/en-us/windows/) (64-bit Version Recommended)
* [Unity Engine](https://unity3d.com/) (Version 5.6.1f1 Recommended)
* Virtual Reality Head Mounted Display (only need one)
  1. [HTC Vive](https://www.vive.com/)
  2. [Oculus Rift](https://www.oculus.com/)
  * *Note: Respective runtimes and supporting software for the HMDs are also required*
* For Interactive Demos: [Leap Motion](https://www.leapmotion.com/) attached to front of HMD

## Installation/Setup

From the Windows command prompt run:
```
python.exe setup.py
```
If you wish to install the interactive and virtual confocal microscopy demos, include the appropriate flags with the above command like so:
```
python.exe setup.py --interactive --vcm
```

The interactive viewing and virtual confocal microscopy demos use considerable large object files to represent our data meshes, therefore their installation steps may take extra time depending on the speed of your computer.

## How to use
 Press the play button on the top of the screen. Switch between demos using the number pad.
 0. Empty Environment
 1. Demo1 - Interactive Viewer with Leap Motion control
 2. Demo2 - Virtual Confocal Microscopy with Leap Motion Control
 3. Demo3a - Lennard Jones Molecular Dynamics Simulation in real time
 4. Demo3b - Kinetic Monte Carlo simulation of electron transfer in cytochrome proteins (also real time)

## Demo Descriptions
### Demo 1 - Interactive Data Viewer for Post-Processing
### Demo 2 - Virtual Confocal Microscopy Tool
### Demo 3 - Real Time Simulator
    A. Molecular Dynamics
    B. Kinetic Monte Carlo

## Customization
What good are our demos gonna do for your research? If you really want to have fun with GEARS, you gotta adapt it to your schtuff. Here's how you can drag and customize the simulation data.

Here's how you start simulating and visualizing things in the engine. For more detail oon that, refer to our publication.