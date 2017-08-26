# GEARS in Unity 5

<div align="center">
     <img src="./images/InteractiveViewer.png"/> 
</div><br>

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
 Press the play button on the top of the screen. Switch between demos using the number pad (0-4). Number pad and demo scene correspondences are as follows:
<ol start="0">
 <li> Empty Environment </li>
 <li> Demo1 - Interactive Viewer with Leap Motion control </li>
 <li> Demo2 - Virtual Confocal Microscopy with Leap Motion Control </li>
 <li> Demo3a - Lennard Jones Molecular Dynamics Simulation in real time </li>
 <li> Demo3b - Kinetic Monte Carlo simulation of electron transfer in cytochrome proteins (also real time) </li>
</ol>

## Demo Descriptions
### Demo 1 - Interactive Data Viewer for Post-Processing
  Here we display a quick method for viewing precomputed simulation data. This method highlights a minimal number steps to vewing your data in VR via game engines. The data in this scene was computed via simulation on a high performace computing cluster (HPCC), then output to an .xyz file. This .xyz file was loaded up in VMD, then rendered into the Wavefront format (.obj and .mtl). Once rendered, we imported the wavefront file into our Unity project and were able to view it immediately in VR (due to the engine's native VR support).

  Leap Motion controls are also implemented for interaction. Using the Leap, once can grab, reorient, and scale their simulation data with their own hands (no extra controller necessary). This allows for the user to intuitively modify how they view their data, facilitating accelerated data acquisition. The directions for Leap Motion use are as follows:
  * Make sure that the Leap Motion device is attached to the front of your HMD
  * Keep your palms wide open about 1 foot or more away from your face
  * To grab an object, pinch using your index finger and thumb. You can use one or both hands to then move, rotate, or scale the data object. Note, this grab will always register when the hands in the scene are pinching, so be sure to keep your palms wide open when you don't intend to grab.
  (The Leap Motion controls are powered by the LeapRTS.cs script, provided by Leap Motion)
  * One-handed and Two-handed half/full rotations can be toggled using the GUI interface on the computer monitor (not in the HMD).
  * To scale the size of the data up, pinch with both hands then pull your arms apart as if you were stretching a rubber band. To scale further, unpinch your fingers, bring your arms close together, then repeat the process again. 

### Demo 2 - Virtual Confocal Microscopy Tool

  The previous demo is intended to embolden researchers to adapt existing scientific programs for use in VR, but, for our purposes, it is not enough to simply provide interoperability. To fully realize the promise of immersive scientific computing, we need to develop new tools to enhance these simulations and visualizations. Current data visualization tools, like VMD and ParaView, limit researchers to interaction via the mouse and keyboard; however, with rich programming options, visual scripting capabilities, and an abundance of pre-integrated control schemes, GEARS yields opportunities for immersive, interactive data analysis. An example of one of these tools is our ongoing development of a rendering method called *virtual confocal microscopy*. This method mimics the noninvasive optical sectioning possible in confocal microscopy. Utilizing Unity’s surface shader capabilities, we control how each vertex on the structure is rendered to highlight certain areas or planes of the material in the simulation. Our solution seeks to generate a viewing plane that sits in front of the user’s head, follows their head movement, and always maintains a set distance from the user. This distance, as well as the thickness of the highlighted viewing plane can be specified and changed  by the user, however, for simplicity, this demo excludes the viewing thickness to two modes: on or off. 

  Controls:
    * Toggle the confocal plane on and off by pinching your index fingers and thumb together then releasing.
    * When the confocal plane is turned on, you should only see a slice out of the data object. Every other part should be transparent. Move the confocal plane through the structure by using your head movements. Don't be afraid to take a few steps backward or forward to scan through the entire data set.
    
### Demo 3 - Real Time Simulator
  To enhance the dynamic aspects of GEARS, in these demos we go beyond recreations of previously computed results to generate immersive and interactive VR simulations. These simulations run in real time on the engine, allowing the user to put on their head set and jump right in. Although simulations follow many different paradigms, we provide examples in molecular dynamics and kinetic Monte Carlo, both of which can utilize our methodologies for real time simulation rendering in VR: Run-and-Render and Render-when-Ready (discussed further in our publication tbd).
  
    A. Molecular Dynamics (MD)
      This demo consists of a system of particles under a Lennard Jones potential. The code was translated to C# directly from Aiichiro Nakano's sequential [MD code](http://cacs.usc.edu/education/cs596/src/md/) written in C. This particular simulation utilizes the Run-and-Render method where we calculate a time step on the Update() function (called once per frame in Unity). The game waits for this calculation to finish then immediately update our paticle positions
    B. Kinetic Monte Carlo
      This demo consists of a system of 20 heme sites on a cytochrome protein undergoing electron transfer. The code was translated to C# from Hye Suk Byun's simulation from [insert citation]. This particular simulation utilizes our Render-when-Ready method where we offload the time step calculation onto a separate thread. The game thread then continues, only updating the heme occupation states when user-dictated time has passed. The speed at which the position updates occur is controlled by the slider floating in front of the protein. The user can slow down and speed up the simulation using the left and right arrow keys. When the slider is all the way on the left, the simulation is paused.

## Customization
What good are our demos gonna do for your research? If you really want to have fun with GEARS, you gotta adapt it to your schtuff. Here's how you can drag and customize the simulation data.

### Demo 1
Using the demos as a template, you can just replace the obj and mtl files with your wavefront file that you wish to view. If you do this, you can just play the demo scene and you can interact with your data the same way.
Alternatively you can setup a new scene from scratch. Here is how:

<ol>
  <li> Create a new Unity Scene </li>
  <li> Import your data into the engine editor (drag and drop or use the file explorer </li>
  <li> Drag your newly imported object from the Unity "Project" window </li>
  <li> Adjust its position and rotation via the inspector or scene window </li>
  For Leap Motion Controls
  <li> Make sure that the Leap Motion Library is imported </li>
  <li> LMHeadMountedRig prefab</li>
  <li> New PinchDetectors, attach script </li>
  <li> Attach LeapRTS </li>
  <li> etc. </li>
</ol>

### Demo 3a and 3b
Here's how you start simulating and visualizing things in the engine. For more detail oon that, refer to our publication.
1. Run and Render
2. Render When Ready