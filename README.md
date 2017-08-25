# Game-Engine Assisted Research framework for Scientific computing (GEARS) in Virtual Reality

<div align="center">
  <img src="https://magics.usc.edu/wp-content/uploads/2017/06/GEARS-Logo2.jpg"><br><br>
</div>

-----------------

## Summary
The recent widespread availability of commodity head-mounted displays (HMDs) has fueled a virtual reality (VR) renaissance in several entertainment and media industries. This resurgence has produced a number of enabling technologies, both hardware and software, that reduce the barrier to entry for new applications of VR. In an effort to leverage these advantages for scientific computing, in this report, we provide a workflow for use with game engines to adapt visualization and simulation techniques for VR. This framework accommodates multiple programming languages and game engines. These game engines provide unique advantages over other tools, like automatic control mappings, that make it more approachable for non-specialists. As part of this framework, we present a hardware-agnostic platform, based on LAMMPS, that allows researchers to directly port existing codebases to VR using familiar programming languages. Additionally, we present a number of software tools that take advantage of VR’s enhanced functionality for scientific computing. To demonstrate the effectiveness of these systems, we review several use cases that range from visualizing the results of simulations carried out on high-performance computing clusters (HPCCs) to running scientific computing code from within the visualization. Building off our previous work on the subject, we first implement a real-time VR simulation of biological electron transfer (ET) in cytochrome proteins. We then outline the inclusion and optimization of LAMMPS for use in VR. These optimizations, which include the use of multiple cores through OpenMP, allow for real-time simulation sizes reaching 500,000 particles. Lastly, we demonstrate the unique power of immersive visualization by develop new VR-specific tools to enhance these simulations, including a shader that acts as a virtual confocal microscope. We apply this virtual scope to exam a molecular model of a desalination membrane.

Here we provide two flavors of GEARS, one utilizing the Unity game engine and another utilizing the Unreal Engine. Although both engines share many common features, they each have their strengths and weaknesses. Therefore, the platform on which one should build their VR visualzation tool depends on their nature of their research. Through the use cases below, we hope to provide a good overview of what these engines are capable of and how it could benefit the scientific computing community. For more information on each use of GEARS, including the system requirements, refer to the READMEs in the engines' respective directories.

## Unity GEARS
### Sample Use Cases
1. VR Post-process Data Viewing Interface

   Here we provide a simple example of viewing pre-computed data from a simulation in VR. By converting our simulation data into a 3D simplicial complex, and integrating Leap Motion technology, one can grab hold of their data with their own hands. They can translate, rotate, and scale their data mesh using the unconstrained, intuitive movement of their own hands. Think Minority Report.

<p align="center">
   <img src="./UnityGEARS/images/InteractiveViewer.png" width=50%/>
</p>

2. Virtual Confocal Microscopy

   While visualizing data in VR alone does provide a more immersive viewing experience, it does not necessarily tap into the unique tools only accessible in VR. To display the powerful immersiveness that VR provides, we developed a graphics shader based confocal microscopy system that allows users to scan through different viewing planes of large complex structures. Users can isolate sequestered regions in their data using head movement alone, a feature only possible with VR.

<div align="center">
     <img src="./UnityGEARS/images/VCMIllustrationOff.png" width=40% float="left"/> <img src="./UnityGEARS/images/VCMIllustrationOn.png" width=40%/>
</div><br>

3. Real-time Simulator in Virtual Reality

   This demo consists of two parts, both hilighting the engines' capabilities of not only viewing pre-computed data, but actually running simulations in real time. By translating existing code into the native language of Unity, C#, we provide the user real-time examples of a kinetic Monte Carlo and molecular dynamics based simulations. 

## Unreal GEARS
### Sample Use Cases (include blurbs on each)
1. LAMMPS Integrated Simulator
2. LAMMPS Integrated Playback Engine

## License

## How to cite