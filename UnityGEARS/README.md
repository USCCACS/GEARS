# GEARS in Unity 5

<div align="center">
     <img src="./images/InteractiveViewer.png"/> 
</div><br>

## System Requirements
* [Windows 10 OS](https://www.microsoft.com/en-us/windows/) (64-bit Version Recommended)
* [Unity Engine](https://unity3d.com/) (Version 5.6.1f1 Recommended)
  * *Note: Be sure to also have Unity account created and logged in to the engine*
* Virtual Reality Head Mounted Display (only need one)
  1. [HTC Vive](https://www.vive.com/)
  2. [Oculus Rift](https://www.oculus.com/)
  * *Note: Respective runtimes and supporting software for the HMDs are also required*
* For Interactive Demos: [Leap Motion](https://www.leapmotion.com/) attached to front of HMD

## Installation
The default installation only includes two of the four demos available. The two non-default demos are the Interactive Viewer and the Virtual Confocal Microscopy. Although these demos provide considerable insight on the strengths of Unity GEARS in VR, they utilize large object files to represent our data meshes. Therefore their installation steps may take extra time depending on the speed of your computer. If one chooses not to include them in their installation, then the large object files will simply be left in their respective .zip files, and the corresponding scenes will be empty.

### Setup via Python
Make sure Python 2 is installed on your Windows 10 machine. From the Windows command prompt run:
```
python.exe setup.py
```
If you wish to install the interactive and virtual confocal microscopy demos, include the appropriate flags with the above command like so:
```
python.exe setup.py --interactive --vcm
```
* *Note: This script assumes that your version of Unity is stored at "C:\Program Files\Unity\Editor\Unity.exe"*

### Manual Setup
* If you wish to install the interactive and virtual confocal microscopy demos, unzip the following folders:
  ```
  GEARS\UnityGEARS\Editor\Assets\Demo1-DataViewer\GeometryData.zip
  GEARS\UnityGEARS\Editor\Assets\Demo2-VirtualConfocalMicroscopy\GeometryData.zip
  ```

* Unzip the following file regardless of whether you choose to install the interactive and confocal microscopy demos:
  ```
  GEARS\UnityGEARS\Editor\Assets\Leap_Motion_CoreAssets_4.2.1.zip
  ```

* Open up Unity.exe and fill out any necessary login credentials.
* Select "Open" to open a new project, and navigate to the GEARS\UnrealGEARS\Editor\ folder and select it. Unity will the generate all the necessary project files.

## How to use
 Press the play button on the top of the editor window. Switch between demos using the number pad (0-4). Number pad and demo scene correspondences are as follows:
<ol start="0">
 <li> Empty Environment </li>
 <li> Demo1 - Interactive Viewer with Leap Motion control </li>
 <li> Demo2 - Virtual Confocal Microscopy with Leap Motion Control </li>
 <li> Demo3a - Lennard Jones Molecular Dynamics Simulation in real time </li>
 <li> Demo3b - Kinetic Monte Carlo simulation of electron transfer in cytochrome proteins (also real time) </li>
</ol>

## Demo Descriptions
### Demo 1 - Interactive Data Viewer for Post-Processing
  Here we display a quick method for viewing precomputed simulation data. This method highlights a minimal number steps to viewing your data in VR via game engines. The data in this scene was computed via simulation on a high performace computing cluster (HPCC), then immediately converted into a 3D mesh that we could import into our scene.

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
What good are our demos gonna do for your research? If you really want to have fun with GEARS, you need to adapt it to your own stuff. Here's how you can drag and customize the simulation data.
I'll be refering to several "windows" within the Unity editor in this section. If you are unfamiliar with the nomenclature, then we suggest reading through [this section](https://docs.unity3d.com/Manual/UsingTheEditor.html) in the Unity manual first.

### Demo 1

#### Getting your data into a Unity scene
1. Mesh-ify your data
    In order for Unity to recognize your simulations data, you'll need to turn it into a 3D mesh. Unity supports several [3D formats](https://docs.unity3d.com/Manual/class-Mesh.html), but for our demo we only use obj files. We ran an MD simulation on an HPCC, then output the particle coordinates from a single frame to an .xyz file. This .xyz file was loaded into [Visual Molecular Dynamics (VMD)](http://www.ks.uiuc.edu/Research/vmd/), and then rendered into the Wavefront format (.obj and .mtl). Once rendered, we imported the wavefront file by simply dragging and dropping it into the Unity editor [Project window](https://docs.unity3d.com/Manual/ProjectView.html) from the File Explorer.
    
2. Create a new Unity [scene](https://docs.unity3d.com/Manual/CreatingScenes.html) or work off of ours (located in "\Editor\Assets\Demo1-DataViewer\InteractiveViewer.unity")
3. Drag & Drop your data .obj file into the scene from the Project window to the [Hierarchy window](https://docs.unity3d.com/Manual/Hierarchy.html).
4. Adjust your data's transform to a comfortable position, rotation, and scale relative to the camera in the scene. Do this via the [Inspector](https://docs.unity3d.com/Manual/UsingTheInspector.html) window after selecting your data in the Hierarchy window.

#### Leap Motion Controls
  Doing the above steps will get your data into the scene and ready to view in VR. However, if you would like to have some control over your data while you're viewing it, a control scheme will be necessary. Unity supports many types of controllers and [input methods](https://docs.unity3d.com/Manual/class-InputManager.html), however, for our demos we chose to use the Leap Motion for its gesture control library and well supported [prefabs](https://docs.unity3d.com/Manual/Prefabs.html) which can be brought into Unity scenes without any extra programming. Assuming the Leap Motion library folder was properly unzipped during the installation step described above, one can implement Leap Motion controls into our project template as follows:
  
  As stated earlier, Unity will automatically display the in-game camera’s output to the user’s HMD when VR support is enabled. To use the Leap Motion controller, though, the in-game camera must be replaced with the prefab provided with Leap Motion library, [LMHeadMountedRig](https://developer-archive.leapmotion.com/documentation/v2/unity/unity/Unity.Prefab.LMHeadMountedRig.html). This prefab contains a replacement camera as well as game objects and scripts to handle processing input from the controller. To acquire this prefab, we first downloaded the Unity Core Assets 4.2.1 from the company’s [developer website](https://developer.leapmotion.com/unity#116). Other versions of the Unity Core Assets may be available, but these have not been tested with provided demo and may or may not be compatible with them. The Leap Motion controller was mounted to the front of our VR HMD. The company provides a mounting kit for this purpose. 
  
  The LMHeadMountedRig generates a set of virtual hands that appear in the engine editor and game. From there, the user is free to program the controls how they see fit using the Detection Utilities found in the Unity Core Assets 4.2.1 library. For our demo we also use the Detection Examples 1.0.4 package, provided by Leap Motion, to allow for simple interaction with our precomputed simulation data. Specifically, we make use of the LeapRTS.cs script to allow us to grab, rotate, and scale our simulation data.
  
  To utilize the LeapRTS.cs script, pinch detectors must be placed in the scene. This can be done by attaching the PinchDetector.cs scripts to the empty child GameObjects of CapsuleHand_L and CapsuleHand_R. In the Inspector windows, drag the appropriate CapsuleHand to its corresponding PinchDetector’s “Hand Model” variable. When the simulation data has been converted into an object file and dragged into the scene Hierarchy, attach the LeapRTS.cs script to that object. Assign the appropriate left and right pinch detectors created earlier to LeapRTS.cs’s PinchDetector A and B fields.

  <div align="center">
     <img src="./images/LeapControls.png"/> 
  </div><br>

  Above is an example of what the Hierarchy window should look like when the LMHeadMountedRig and appropriate pinch detectors are successfully palced in the scene. Once setup, the user should be able to grab and manipulate their simulation data in VR. It should be noted that the Detection Examples 1.0.4 package has been deprecated by the company at the time of this publication. Leap Motion offers a new Interaction library to replace it. This is an excellent example of how quickly these technologies are developing and evolving, providing a rich and ever expanding plethora of software and hardware tools for use with data visualizaiton. The LeapRTS.cs script is provided with the demo files.

### Demo 3a and 3b
Here we discuss how you can begin simulating then visualizing data in the engine on the fly. For more detail on that, you can refer to our publication. In Section 3.2 and 3.3, we described two methods for running and rendering real-time simulations in GEARS. We call these methods Run-and-Render and Render-when-Ready. The following will describe how both can be implemented the Unity 5 and Unreal 4.16 game engines. 

Both methods are driven by the use of two common purpose functions within Unity and Unreal: Start() and Update() for Unity, and BeginPlay() and Tick() for Unreal. For the sake of simplicity, we will use Unreal Engine 4.16 terminology when referring to game objects and these common purpose functions. At the beginning of any engine scene, the BeginPlay() member function is called on any instance of a GameObject or Actor constructed in the scene. 

The first method will compute a timestep, then immediately render the results, all on a single frame of the game thread. The second method incorporates the first performance optimization from Section 2.4 and offload the timestep calculation onto separate thread from the main game thread. The main game thread then continues viewing results from the last timestep until it is ready to update based on the new timestep information.

The first approach, which we denote as Run-and-Render, can be implemented in a few steps using the Start() and Update() methods of the GameObject or class responsible for handling the simulation. We start by creating a member variable, DeltaTime, to store the length of a time step in the simulation. Then, in the Start() method (which is placed by default in all Unity classes), all the positions of the objects that represent participants in the simulation are initialized. In our case, we cloned a sphere prefab and stored references to its clones in an array. Next, in the Update() method, we assign the DeltaTime variable to the current Unity-based time step (Time.deltaTime). This value can then be scaled by an arbitrary tuning factor to account for realistic observation capabilities of a human observer. Otherwise, since Update() gets called once per frame, the simulation may animate much more quickly than the user can beneficially observe. We also choose the assignment of DeltaTime such that the simulation executes proportionally to the real time that has passed for the user. Next, we add a call to the single time step method of the simulation inside the function, Update(). After the single time step, we iterate through each participant and update their status. 

The second approach, Render-when-Ready, utilizes the multi-threading optimization mentioned in section 2.4 and provides the user more flexibility within the simulation. For the case of KMC within Unity GEARS, we begin by defining a new class, called Event, to encapsulate all the changes that occur within the system and associate them with each time step. This class can be anything from a collection of coordinates to the occupancy of certain states. Then, we create a member variable queue in either the class responsible for simulating or the one for rendering. The class this queue belongs to does not affect the output as long as both the simulation and rendering class have access to it. Next, in the simulation class, we create a method that will push to the event queue. Subsequently, in the rendering class, we create a method that will pop a single event off the queue. Finally, we spawn a new thread and pass in the simulation class’ queue-pushing method. Because this method is independent of the Update function and stores data independently of the visualization simulation, it is better suited to future applications involving more extreme, calculation intensive time steps.

The other is to have the simulation populate an Event Queue on each time step. Another class, responsible for rendering, can then pop events off this queue at an appropriate speed, designated by the user.

Using control schemes similar to those outlined in the Methods section, the simulation can be manipulated or varied in real-time. The RTSLeap script can handle user calls to move, scale, and rotate the object of interest. As mentioned previously, however, Leap Motion provides a large library of hand-detectors to add more functionality to user interaction with the simulation. 
