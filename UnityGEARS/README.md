# GEARS in Unity 5

## System Requirements
* [Windows 10 OS](https://www.microsoft.com/en-us/windows/) (64-bit Version Recommended)
* [Unity Engine](https://unity3d.com/) (Version 5.6.1f1 Recommended)
* Virtual Reality Head Mounted Display (only need one)
  1. [HTC Vive](https://www.vive.com/)
  2. [Oculus Rift](https://www.oculus.com/)
  * *Note: Respective runtimes and supporting software for the HMDs are also required*
* For Interactive Demos: [Leap Motion](https://www.leapmotion.com/) attached to front of HMD

## Installation
The default installation only includes two of the four demos available. The two non-default demos are the Interactive Viewer and the Virtual Confocal Microscopy. Although these demos provide considerable insight on the strengths of Unity GEARS in VR, they utilize large object files to represent our data meshes. Therefore their installation steps may take extra time depending on the speed of your computer. If one chooses not to include them in their installation, then the large object files will simply be left in their respective .zip files.

### Setup via Python
Make sure Python 2 is installed on your Windows 10 machine. From the Windows command prompt run:
```
python.exe setup.py
```
If you wish to install the interactive and virtual confocal microscopy demos, include the appropriate flags with the above command like so:
```
python.exe setup.py --interactive --vcm
```
### Manual Setup
If you wish to install the interactive and virtual confocal microscopy demos, unzip the following folders:
```
GEARS\UnityGEARS\Editor\Assets\Demo1-DataViewer\GeometryData.zip
GEARS\UnityGEARS\Editor\Assets\Demo2-VirtualConfocalMicroscopy\GeometryData.zip
GEARS\UnityGEARS\Editor\Assets\Leap_Motion_CoreAssets_4.2.1.zip
```
Open up Unity.exe. Fill out any necessary login credentials. Select "Open" project. Navigate to the GEARS\UnrealGEARS\Editor\ directory and select it. Unity will the generate all the necessary project files.

## How to use
 Press the play button on the top of the screen. Switch between demos using the number pad.
<ol start="0">
 <li> Empty Environment </li>
 <li> Demo1 - Interactive Viewer with Leap Motion control </li>
 <li> Demo2 - Virtual Confocal Microscopy with Leap Motion Control </li>
 <li> Demo3a - Lennard Jones Molecular Dynamics Simulation in real time </li>
 <li> Demo3b - Kinetic Monte Carlo simulation of electron transfer in cytochrome proteins (also real time) </li>
</ol>

## Demo Descriptions
### Demo 1 - Interactive Data Viewer for Post-Processing
### Demo 2 - Virtual Confocal Microscopy Tool
### Demo 3 - Real Time Simulator
    A. Molecular Dynamics
    B. Kinetic Monte Carlo

## Customization
What good are our demos gonna do for your research? If you really want to have fun with GEARS, you gotta adapt it to your schtuff. Here's how you can drag and customize the simulation data.

Here's how you start simulating and visualizing things in the engine. For more detail oon that, refer to our publication.