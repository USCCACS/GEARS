#FAQS:
1. *Do you support platforms like Linux or Mac?*
   Sadly, GEARS is not fully supported for Linux or OSX due to the lack of native VR support on those platforms. You may be able to use OSX for the Unity Engine related projects, however, our mechanism for linking LAMMPS at runtime in Unreal Engine 4.16 only supports Windows Dlls. One may be able 
2. *Can I use other VR head mounted displays besides the Oculus and HTC Vive?*
   You can use any VR HMD that the engines support. However, we have only tested GEARS using the an HTC Vive, Oculus DK2, and Oculus CV1
3. *Can I utilize MPI in the LAMMPS Dll?*
   Sadly, we do not have support for any MPI related calls in LAMMPS. However, we do provide a LAMMPS dll with OpenMP compiled into it. Making OpenMP commands in GEARS, however, is mostly untested, therefore you may encounter bugs that we have not seen should you use it. 
4. *How good of a computer do I need for this?*
   Any "VR ready" machine should perform fine. Refer to the [HTC guide](https://www.vive.com/us/ready/) for more details on if your PC is VR ready. You can also refer to our previous publication for details on our machine's specs. (insert link to VizBET paper).
5. *How much graphics programming do I need to know for this?*
   To just use GEARS, graphics programming is not a necessity. That's the great thing about GEARS! The VR support is native to the game engines that we built our visualization tools on. If you'd like to program for GEARS, it may help to know some topics such as the rendering pipeline, instancing, and shaders. A basic understanding is sufficient for optimizing rendering performance. No knowledge of OpenGL, Vulkan, DirectX or what-have-you is needed :)