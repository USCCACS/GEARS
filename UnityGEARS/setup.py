#!/usr/bin/env python

import optparse
import zipfile
import os
import os.path
import subprocess
import sys

# Unzip Geometry Data Files
assetsPath = './Editor/Assets/'
geometryDir = 'GeometryData/'
geometryMeta = 'GeometryData.meta'
geometryZip = 'GeometryData.zip'

demo1Dir = assetsPath + 'Demo1-DataViewer/'
demo2Dir = assetsPath + 'Demo2-VirtualConfocalMicroscopy/'
demo1GeometryZipPath = demo1Dir + geometryZip
demo2GeometryZipPath = demo2Dir + geometryZip

if not os.path.isdir(demo1Dir + geometryDir) and not os.path.isfile(demo1Dir + geometryMeta):
    demo1Zip = zipfile.ZipFile(demo1GeometryZipPath, 'r')
    demo1Zip.extractall(demo1Dir)
    demo1Zip.close()
else:
    print('Demo 1 Geometry Data already exists. We recommend remove folder and retry.')

if not os.path.isdir(demo2Dir + geometryDir) and not os.path.isfile(demo2Dir + geometryMeta):
    demo2Zip = zipfile.ZipFile(demo2GeometryZipPath, 'r')
    demo2Zip.extractall(demo2Dir)
    demo2Zip.close()
else:
    print('Demo 2 Geometry Data already exists. We recommend remove folder and retry.')


# Open Unity
#pid = os.fork()
#if pid > 0:
#    sys.exit()
#else:
subprocess.call('C:/Program Files/Unity/Editor/Unity.exe')

print('Setup Complete')
