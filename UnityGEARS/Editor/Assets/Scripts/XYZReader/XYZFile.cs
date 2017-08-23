using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;

/* 
 * This class encapsulates the data stored in an .xyz file (and by extension .pdb). 
 * It's purpose is to make visualization of systems described by an .xyz file easier 
 * in Unity, which only uses scripts written in  C# and Javascript.
 */
public class XYZFile {//: MonoBehaviour {
  private string _filePath;
  private List<string> _fileLines;
  public List<Frame> Frames { get; set; }

  public XYZFile(string filePath) {
    Frames = new List<Frame>();
    _filePath = filePath;
    parseFileToFrames();
  }

  /* Uses the data stored in _FILELINES to construct/build up FRAMES with instances of  animation frames */
  private void parseFileToFrames() {
    _fileLines = new List<string>(File.ReadAllLines(_filePath));
    _fileLines.RemoveAll(isCommentLine);

    while (_fileLines.Any()) {
      //Create a new Animation frame
      Frame newFrame = new Frame();

      //Add collect data from each atom in the frame
      string numberOfAtoms = _fileLines.PopFront();
      for (int i=0; i< int.Parse(numberOfAtoms); ++i) {
        string[] line = _fileLines.PopFront().Split(new[] { " ", "\t" }, System.StringSplitOptions.None);
        Vector3 position = new Vector3(float.Parse(line[1]), float.Parse(line[2]), float.Parse(line[3]));
        newFrame.Add(line[0], position);
      }

      //add the new frame to the list of frames
      Frames.Add(newFrame);
    }
  }

  /* 
   * Search predicate returns true if string is a .xyz comment line. Used in parseFileToFrames() to remove
   * all comments from _FILELINES 
   */
  private static bool isCommentLine(string s) {
    return s.StartsWith("#");
  }

  /*
   * Private container class for each frame in the XYZ file. Inteded use is to make animation of the .xyz file easier.
   * Programs that utilize this XYZFile class can just retrieve the animation frame by frame
   */
  public class Frame
  {
    public List<KeyValuePair<string, Vector3>> Positions { get; set; }
    public int Count { get { return Positions.Count; } }

    //Constructor
    public Frame() {
      Positions = new List<KeyValuePair<string, Vector3>>();
    }

    public void Add(string moleculeType, Vector3 position) {
      KeyValuePair<string, Vector3> newObject = new KeyValuePair<string, Vector3>(moleculeType, position);
      Positions.Add(newObject);
    }

    public Vector3 getPositionOf(int index) {
      return Positions[index].Value;
    }
    public string getMoleculeTypeOf(int index) {
      return Positions[index].Key;
    }
  }
}

/* Extensions for List<> class */
static class ListExtension
{
  public static T PopFront<T>(this List<T> list) {
    T r = list[0];
    list.RemoveAt(0);
    return r;
  }
}
