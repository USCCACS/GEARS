#!/usr/bin/env python

import argparse
import zipfile
import os
import os.path
import subprocess
import sys

#Acquire the current working directory
currentDir = os.path.dirname(os.path.realpath(__file__))

# Parse Arguments
parser = argparse.ArgumentParser()
parser.add_argument("--vcm", help="Adds the Virtual Confocal Microscopy demo to the build", action="store_true")

args = parser.parse_args();

# Unzip LeapMotion and Geometry Data Files
projectDir = './Editor/'
fullProjectPath = currentDir + projectDir
assetsPath = projectDir + 'Assets/'

geometryDir = 'GeometryData/'
geometryMeta = 'GeometryData.meta'
geometryZip = 'GeometryData.zip'

leapZipPath =  assetsPath + 'Leap_Motion_CoreAssets_4.2.1.zip'
leapDir = assetsPath + 'LeapMotion/'
leapMeta = assetsPath + 'LeapMotion.meta'
demo2Dir = assetsPath + 'Demo2-VirtualConfocalMicroscopy/'
demo2GeometryZipPath = demo2Dir + geometryZip

if not os.path.isdir(leapDir) and not os.path.isfile(leapMeta):
	print('Unzipping Leap Motion Package')
	leapZip = zipfile.ZipFile(leapZipPath, 'r')
	leapZip.extractall(assetsPath)
	leapZip.close()

if args.vcm:
    if not os.path.isdir(demo2Dir + geometryDir) and not os.path.isfile(demo2Dir + geometryMeta):
        print('Unzipping Virtual Confocal Microscopy geometry data')
        demo2Zip = zipfile.ZipFile(demo2GeometryZipPath, 'r')
        demo2Zip.extractall(demo2Dir)
        demo2Zip.close()
    else:
        print('Demo 2 Geometry Data already exists. If prefab errors occur, remove folder and meta files, then retry.')


# Open Unity
print('Startin Up Unity')
subprocess.call(['C:/Program Files/Unity/Editor/Unity.exe', '-projectPath', fullProjectPath])

print('Setup Complete')
