#!/usr/bin/env python

import argparse
import zipfile
import os
import os.path
import subprocess
import sys

# Parse Arguments
parser = argparse.ArgumentParser()
parser.add_argument("--interactive", help="Adds Interactive Data Viewer demo to the build", action="store_true")
parser.add_argument("--vcm", help="Adds the Virtual Confocal Microscopy demo to the build", action="store_true")

args = parser.parse_args();

# Unzip Geometry Data Files
assetsPath = './Editor/Assets/'
geometryDir = 'GeometryData/'
geometryMeta = 'GeometryData.meta'
geometryZip = 'GeometryData.zip'

demo1Dir = assetsPath + 'Demo1-DataViewer/'
demo2Dir = assetsPath + 'Demo2-VirtualConfocalMicroscopy/'
demo1GeometryZipPath = demo1Dir + geometryZip
demo2GeometryZipPath = demo2Dir + geometryZip

if args.interactive:
    if not os.path.isdir(demo1Dir + geometryDir) and not os.path.isfile(demo1Dir + geometryMeta):
        print('Unzipping Interactive geometry data')
        demo1Zip = zipfile.ZipFile(demo1GeometryZipPath, 'r')
        demo1Zip.extractall(demo1Dir)
        demo1Zip.close()
    else:
        print('Demo 1 Geometry Data already exists. If prefab errors occur, remove folder and meta files, then retry.')

if args.vcm:
    if not os.path.isdir(demo2Dir + geometryDir) and not os.path.isfile(demo2Dir + geometryMeta):
        print('Unzipping Virtual Confocal Microscopy geometry data')
        demo2Zip = zipfile.ZipFile(demo2GeometryZipPath, 'r')
        demo2Zip.extractall(demo2Dir)
        demo2Zip.close()
    else:
        print('Demo 2 Geometry Data already exists. If prefab errors occur, remove folder and meta files, then retry.')


# Open Unity
#subprocess.call('C:/Program Files/Unity/Editor/Unity.exe')

print('Setup Complete')
